#include "utils/time.h"
#include "globals.h"
#include "callback.h"
#include "utils/log.h"
#include "init_app.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

#include "gui.h"
#include "settings.h"
#include "process/manager.h"

Context globalContext;

bool EventLoop();

int main(int, char**)
{
    if (!fs::exists(fs::current_path() / "binaries"))
    {
        Log::Error("Run this app inside the executable directory!");
        std::exit(1);
    }
    Log::Debug("Current Path: " + fs::current_path().string());

    InitApp(globalContext);

    Log::Debug("App is running!");

    while (true)
    {
        if (!EventLoop()) break;

        Time::Update();
        ProcessManager::Update();

        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        int w, h;
        SDL_GetWindowSize(globalContext.window, &w, &h);
        ImGui::SetNextWindowSize({static_cast<float>(w), static_cast<float>(h)});
        ImGui::SetNextWindowPos({0.f, 0.f});
        ImGui::Begin("UI Window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

        ImGui::TextWrapped("Pasta de Download: %s", Settings::GetDownloadFolderPath().string().c_str());

        if (ImGui::Button("Mudar Pasta para Download"))
        {
            GUI::SendEvent(GUIEvents::CHANGE_DEFAULT_FOLDER);
        }
        ImGui::SameLine();

        if (ImGui::Button("Abrir Pasta"))
        {
            GUI::SendEvent(GUIEvents::OPEN_DEFAULT_FOLDER);
        }

        GUI::RenderTabs();

        ImGui::End();

        ImGui::Render();

        SDL_SetRenderDrawColor(globalContext.renderer, 0, 0, 0, 255);
        SDL_RenderClear(globalContext.renderer);

        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), globalContext.renderer);

        SDL_RenderPresent(globalContext.renderer);
    }

    QuitApp(globalContext);

    return 0;
}

void HandleUserEvents(SDL_Event& event)
{
    if (event.type == GUIEvents::SEND_FILES)
    {
        SDL_ShowOpenFileDialog(
            SendFilesCallback,
            nullptr,
            globalContext.window,
            nullptr, 0,
            SDL_GetUserFolder(SDL_FOLDER_HOME),
            true
        );
    }
    else if (event.type == GUIEvents::SEND_FOLDER)
    {
        SDL_ShowOpenFolderDialog(
            SendFoldersCallback,
            nullptr,
            globalContext.window,
            SDL_GetUserFolder(SDL_FOLDER_HOME),
            false
        );
    }
    else if (event.type == GUIEvents::RECEIVE_ARCHIVE)
    {
        auto ticket = *static_cast<std::string*>(event.user.data1);
        ProcessManager::ReceiveArchive(ticket);
        delete static_cast<std::string*>(event.user.data1);
        event.user.data1 = nullptr;
    }
    else if (event.type == GUIEvents::STOP_PROCESS)
    {
        const auto ticket = *static_cast<std::string*>(event.user.data1);
        ProcessManager::CloseProcess(ticket);
        delete static_cast<std::string*>(event.user.data1);
        event.user.data1 = nullptr;
    }
    else if (event.type == GUIEvents::OPEN_DEFAULT_FOLDER)
    {
        std::string uri = "file:///" + Settings::GetDownloadFolderPath().generic_string();
        SDL_OpenURL(uri.c_str());
    }
    else if (event.type == GUIEvents::CHANGE_DEFAULT_FOLDER)
    {
        SDL_ShowOpenFolderDialog(
            ChangeDownloadFolderCallback,
            nullptr,
            globalContext.window,
            Settings::GetDownloadFolderPath().string().c_str(),
            false
        );
    }
}

bool EventLoop()
{
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent))
    {
        ImGui_ImplSDL3_ProcessEvent(&sdlEvent);
        if (sdlEvent.type == SDL_EVENT_QUIT) return false;

        HandleUserEvents(sdlEvent);
    }

    return true;
}
