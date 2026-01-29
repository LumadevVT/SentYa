#include "init_app.h"

#include "utils/log.h"

#include <SDL3/SDL.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

#include "settings.h"
#include "process/manager.h"
#include "utils/time.h"

bool InitSDL(Context& context);
bool InitImGui(const Context& context);

void InitApp(Context& context)
{
    // if (atexit(QuitAtExit) != 0) std::exit(EXIT_FAILURE);
    if (!InitSDL(context)) std::exit(EXIT_FAILURE);
    if (!InitImGui(context)) std::exit(EXIT_FAILURE);
    if (!Settings::Init()) std::exit(EXIT_FAILURE);

    ProcessManager::Init();
    Time::Init();
    Log::Init();
}

void QuitApp(const Context& context)
{
    Settings::SaveToDisk();
    ProcessManager::Quit();
    Log::Quit();

    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(context.renderer);
    SDL_DestroyWindow(context.window);
    SDL_Quit();
}

bool InitSDL(Context& context)
{
    if (!SDL_InitSubSystem(SDL_INIT_EVENTS))
    {
        Log::Fatal("Failed to initialize events subsystem in SDL library.");
        return false;
    }
    if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
    {
        Log::Fatal("Failed to initialize video subsystem in SDL library.");
        return false;
    }

    context.window = SDL_CreateWindow("SentYa - Eu envio, você recebe!", 1280, 720, SDL_WINDOW_RESIZABLE);
    if (context.window == nullptr)
    {
        Log::Fatal("Failed to create window.");
        return false;
    }

    context.renderer = SDL_CreateRenderer(context.window, nullptr);
    if (context.renderer == nullptr)
    {
        Log::Fatal("Failed to create renderer.");
        return false;
    }

    SDL_SetRenderVSync(context.renderer, 1);

    return true;
}

bool InitImGui(const Context& context)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;
    io.FontGlobalScale = 1.f;

    if (!ImGui_ImplSDL3_InitForSDLRenderer(context.window, context.renderer))
    {
        Log::Fatal("Failed to init ImGui for SDL Renderer.");
        return false;
    }

    if (!ImGui_ImplSDLRenderer3_Init(context.renderer))
    {
        Log::Fatal("Failed to init SDL Renderer for ImGui.");
        return false;
    }

    return true;
}
