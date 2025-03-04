#include "../include/globals.h"
#include "callback.h"

#include <../vendor/SDL3/include/SDL3/SDL.h>
#include <../vendor/SDL3/include/SDL3/SDL_main.h>
#include <../vendor/dear_imgui/imgui.h>
#include <../vendor/dear_imgui/misc/cpp/imgui_stdlib.h>
#include <../vendor/dear_imgui/backends/imgui_impl_sdl3.h>
#include <../vendor/dear_imgui/backends/imgui_impl_sdlrenderer3.h>

#include <fstream>
#include <filesystem>
#include <iostream>
#include <ranges>

#define SDL_FLAGS (SDL_INIT_VIDEO | SDL_INIT_EVENTS)

namespace fs = std::filesystem;

Context context;

std::vector<std::string> stuffSent;

int main(int, char**) {
    if (!SDL_Init(SDL_FLAGS)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), nullptr);
        return 1;
    }

    context.window = SDL_CreateWindow("SentYa - Eu envio, você recebe!", 800, 600, SDL_WINDOW_RESIZABLE);
    if (context.window == nullptr) {
        std::cout << "Failed to create window" << std::endl;
        return 1;
    }

    context.renderer = SDL_CreateRenderer(context.window, nullptr);
    if (context.renderer == nullptr) {
        std::cout << "Failed to create renderer" << std::endl;
        return 1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;
    io.FontGlobalScale = 1.2f;
    ImGui_ImplSDL3_InitForSDLRenderer(context.window, context.renderer);
    ImGui_ImplSDLRenderer3_Init(context.renderer);

    context.app.trashPaths.push_back(context.app.downloadPath);

    std::string inputBuffer;

    float startTime = static_cast<float>(SDL_GetTicks()) / 1000.f;
    float endTime = 0.f;
    float deltaTime = 0.f;

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    running = false;
                    break;
                default:
                    break;
            }
        }
        endTime = static_cast<float>(SDL_GetTicks()) / 1000.f;
        deltaTime = endTime - startTime;
        startTime = endTime;

        for (auto &process: context.app.upload) {
            if (process.sdlProcess == nullptr || process.loading) continue;
            process.timer += deltaTime;
        }

        for (auto &process: context.app.download) {
            if (process.sdlProcess == nullptr) continue;
            process.timer += deltaTime;
        }

        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        int w, h;
        SDL_GetWindowSize(context.window, &w, &h);
        ImGui::SetNextWindowSize({static_cast<float>(w), static_cast<float>(h)});
        ImGui::SetNextWindowPos({0.f,0.f});
        ImGui::Begin("UI Window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

        ImGui::TextWrapped("Pasta de Download: %s", context.app.downloadPath.string().c_str());

        if (ImGui::Button("Mudar Pasta para Download")) {
            static std::string str;
            str = context.app.downloadPath.string();

            SDL_ShowOpenFolderDialog(
                ChangeDownloadFolderCallback,
                &context.app,
                context.window,
                str.c_str(),
                false
            );
        }
        ImGui::SameLine();
        if (ImGui::Button("Abrir Pasta")) {
            std::string uri = "file:///" + context.app.downloadPath.generic_string();
            SDL_OpenURL(uri.c_str());
        }


        ImGui::BeginTabBar("Tabs da Aplicação");

        if (ImGui::BeginTabItem("Tutorial")) {

            ImGui::Text("Olá, este aqui é o SentYa!");
            ImGui::TextWrapped("Este é um programa criado em cima do app de terminal chamado Sendme!");
            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::TextWrapped("Para enviar um arquivo, vá para a aba \"Enviar\" e aperte \"Enviar arquivos\" ou \"Enviar Pasta\".");
            ImGui::TextWrapped("Assim que processado o arquivo/pasta, clique em copiar o ticket e envie para o destinatário!");
            ImGui::TextWrapped("Lembre-se: você tem que deixar o ticket aberto para funcionar, se cancelar o ticket não funciona mais.");
            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::TextWrapped("Para receber o arquivo, copie o ticket que você recebeu do remetente e cole na aba \"Receber\".");
            ImGui::TextWrapped("Depois disso, basta aguardar que seus arquivos serão instalados na sua pasta escolhida!");
            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::TextWrapped("Aviso: o app não consegue reconhecer direito arquivos com acento. Por favor evite o uso.");

            ImGui::EndTabItem();
        }

        int i = 0;
        if (ImGui::BeginTabItem("Enviar")) {
            constexpr SDL_DialogFileFilter filters[] = {
                { "All files",   "*" }
            };
            if (ImGui::Button("Enviar Arquivos")) {
                SDL_ShowOpenFileDialog(
                    SendStuffCallback,
                    &context.app,
                    context.window,
                    filters, 1,
                    SDL_GetUserFolder(SDL_FOLDER_HOME),
                    true
                );
            }

            if (ImGui::Button("Enviar Pasta")) {
                SDL_ShowOpenFolderDialog(
                    SendStuffCallback,
                    &context.app,
                    context.window,
                    SDL_GetUserFolder(SDL_FOLDER_HOME),
                    true
                );
            }

            for (auto& process : context.app.upload) {
                if (process.sdlProcess == nullptr) continue;

                ImGui::PushID(i);
                ImGui::Spacing();

                if (process.loading) {
                    ImGui::Text("Processando arquivo...");
                    ImGui::PopID();
                    i++;
                    continue;
                }

                const char* str = fs::is_directory(process.filePath) ? "Pasta: %s" : "Arquivo: %s";

                ImGui::TextWrapped(str, fs::absolute(process.filePath).string().c_str());
                if (ImGui::Button("Copiar Ticket"))
                    SDL_SetClipboardText(process.ticket.c_str());

                if (ImGui::Button("Cancelar")) {
                    SDL_KillProcess(process.sdlProcess, false);
                    SDL_DestroyProcess(process.sdlProcess);
                    process.sdlProcess = nullptr;
                }

                ImGui::PopID();
                i++;
            }

            {
                std::string stringBuffer;
                for (auto& process : context.app.upload) {
                    if (process.sdlProcess == nullptr) continue;
                    if (process.loading) {
                        SDL_IOStream *stream = SDL_GetProcessOutput(process.sdlProcess);

                        std::string buffer;
                        buffer.resize(256);
                        size_t len = SDL_ReadIO(stream, buffer.data(), 256);
                        if (len == 0) continue;

                        stringBuffer += buffer;
                        while (true) {
                            len = SDL_ReadIO(stream, buffer.data(), 256);
                            stringBuffer += buffer;
                            if (len == 0) break;
                        }

                        // ticket will always have blob.
                        stringBuffer = stringBuffer.substr(stringBuffer.find("blob"));
                        if (stringBuffer.find_first_of('\n') != std::string::npos) {
                            stringBuffer = stringBuffer.substr(0, stringBuffer.find_first_of('\n'));
                        }

                        process.ticket = stringBuffer;
                        process.loading = false;
                    }
                }
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Receber")) {
            ImGui::Text("Insira o ticket do arquivo aqui:");
            ImGui::SameLine();
            ImGui::InputText("##TicketPaste", &inputBuffer);

            if (ImGui::Button("Confirmar")) {
                bool added = false;
                for (auto& process: context.app.download) {
                    if (process.sdlProcess != nullptr) continue;

                    static std::string str;
                    str = context.app.binaryPath.string();

                    const char* command[] = {
                        str.c_str(),
                        "receive",
                        inputBuffer.c_str(),
                        nullptr,
                    };

                    process.sdlProcess = SDL_CreateProcess(command, true);
                    if (process.sdlProcess == nullptr) {
                        std::cout << "Failed to create SDL process" << std::endl;
                        process.sdlProcess = nullptr;
                        inputBuffer.clear();
                        break;
                    }
                    inputBuffer.clear();
                    added = true;
                    break;
                }

                if (!added) {
                    std::cout << "Error: " << SDL_GetError() << std::endl;
                }
            }

            for (auto& process : context.app.download) {
                if (process.sdlProcess == nullptr) continue;

                ImGui::PushID(i);
                ImGui::Spacing();

                int exitcode = 0;
                // if finished:
                if (SDL_WaitProcess(process.sdlProcess, false, &exitcode) == true) {
                    if (exitcode == 0) {
                        stuffSent.emplace_back("Arquivos Recebidos.");
                    } else {
                        stuffSent.emplace_back("Houve um erro, verifique se copiou o ticket corretamente.");
                    }

                    SDL_DestroyProcess(process.sdlProcess);
                    process.sdlProcess = nullptr;

                    ImGui::PopID();
                    i++;

                    continue;
                }

                ImGui::TextWrapped("Recebendo arquivos (%02i:%02i)", static_cast<int>(process.timer) / 60, static_cast<int>(process.timer) % 60);
                ImGui::SameLine(0, 20);
                if (ImGui::Button("Cancelar")) {
                    SDL_KillProcess(process.sdlProcess, false);
                    SDL_DestroyProcess(process.sdlProcess);
                    process.sdlProcess = nullptr;
                }

                ImGui::PopID();
                i++;
            }

            for (auto &it : std::ranges::reverse_view(stuffSent)) {
                ImGui::PushID(i);
                ImGui::TextWrapped("%s",it.c_str());
                ImGui::PopID();
                i++;
            }

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
        ImGui::End();

        ImGui::Render();
        SDL_SetRenderDrawColor(context.renderer, 0, 0, 0, 255);
        SDL_RenderClear(context.renderer);

        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), context.renderer);
        SDL_RenderPresent(context.renderer);
    }

    for (auto &process : context.app.upload) {
        if (process.sdlProcess == nullptr) continue;

        SDL_KillProcess(process.sdlProcess, true);
        SDL_DestroyProcess(process.sdlProcess);
        process.sdlProcess = nullptr;
    }

    for (auto &process : context.app.download) {
        if (process.sdlProcess == nullptr) continue;

        SDL_KillProcess(process.sdlProcess, true);
        SDL_DestroyProcess(process.sdlProcess);
        process.sdlProcess = nullptr;
    }

    for (auto& trashPath: context.app.trashPaths) {
        for (const auto& dir : fs::directory_iterator(trashPath)) {
            if (dir.path().string().find(".sendme") != std::string::npos) {
                fs::remove_all(dir.path());
            }
        }
    }


    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(context.renderer);
    SDL_DestroyWindow(context.window);
    SDL_Quit();
    return 0;
}
