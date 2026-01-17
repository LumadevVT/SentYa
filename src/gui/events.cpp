#include "gui/events.h"

namespace GUI {
    void SendEvent(GUIEvent event, void *data1, void *data2) {
        SDL_Event sdlEvent;
        SDL_zero(sdlEvent);
        sdlEvent.type = event;
        sdlEvent.user.code = 0;
        sdlEvent.user.data1 = data1;
        sdlEvent.user.data2 = data2;
        SDL_PushEvent(&sdlEvent);
    }
}
