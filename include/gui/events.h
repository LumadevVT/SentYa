#pragma once

#include <SDL3/SDL_events.h>

using GUIEvent = Uint32;

namespace GUIEvents {
    inline const GUIEvent SEND_FILES = SDL_RegisterEvents(1);
    inline const GUIEvent SEND_FOLDER = SDL_RegisterEvents(1);
    inline const GUIEvent RECEIVE_ARCHIVE = SDL_RegisterEvents(1);
    inline const GUIEvent STOP_PROCESS = SDL_RegisterEvents(1);
    inline const GUIEvent OPEN_DEFAULT_FOLDER = SDL_RegisterEvents(1);
    inline const GUIEvent CHANGE_DEFAULT_FOLDER = SDL_RegisterEvents(1);
}

namespace GUI {
    void SendEvent(GUIEvent event, void *data1 = nullptr, void *data2 = nullptr);
}
