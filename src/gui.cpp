#include "gui.h"

#include "gui/tabs.h"

#include <string>

#include <imgui.h>

namespace GUI {
    void RenderTabs() {
        ImGui::BeginTabBar("Abas da Aplicação");

        if (ImGui::BeginTabItem("Tutorial")) {
            static std::string introText = R"(Olá, este aqui é o SentYa!
Este é um programa criado em cima do app de terminal chamado Sendme!

Para enviar um arquivo, vá para a aba "Enviar" e aperte "Enviar arquivos" ou "Enviar pasta".
Assim que processado o arquivo/pasta, clique em copiar o ticket e envie para o destinatário!
Lembre-se: você tem que deixar o ticket aberto para funcionar, se cancelar o ticket não funciona mais.

Para receber o arquivo, copie o ticket que você recebeu do remetente e cole na aba "Receber".
Depois disso, basta aguardar que seus arquivos serão instalados na sua pasta escolhida!


Aviso: o app não consegue reconhecer direito arquivos e pastas com acento, tome cuidado com o nome das pastas e arquivos.)";

            ImGui::TextWrapped("%s", introText.c_str());
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Enviar")) {
            RenderSendingTab();
            ImGui::EndTabItem();
            /*
            for (auto &process: globalContext.app.upload) {
                if (process.sdlProcess == nullptr) continue;

                ImGui::PushID(i);
                ImGui::Spacing();

                if (process.loading) {
                    ImGui::Text("Processando arquivo...");
                    ImGui::PopID();
                    i++;
                    continue;
                }

                const char *str = fs::is_directory(process.filePath) ? "Pasta: %s" : "Arquivo: %s";

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
                for (auto &process: globalContext.app.upload) {
                    if (process.sdlProcess == nullptr) continue;
                    static std::string stringBuffer;
                    stringBuffer = "";
                    if (process.loading) {
                        SDL_IOStream *stream = SDL_GetProcessOutput(process.sdlProcess);
                        if (stream == nullptr) continue;

                        char buffer[256];

                        while (SDL_GetIOStatus(stream) != SDL_IO_STATUS_EOF) {
                            const size_t len = SDL_ReadIO(stream, buffer, 256);
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
            */
        }

        // Aba de recebimento de ticket
        if (ImGui::BeginTabItem("Receber")) {
            RenderReceivingTab();
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
}
