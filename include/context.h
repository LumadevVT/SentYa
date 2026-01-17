#pragma once

struct SDL_Window;
struct SDL_Renderer;

struct Context {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
};
