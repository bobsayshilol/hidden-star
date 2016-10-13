#include "user_events.h"

void userevent_add(UserEventType t, void *d1, void *d2) {
    SDL_Event e;
    SDL_UserEvent ue;
    
    ue.type = SDL_USEREVENT;
    ue.code = t;
    ue.data1 = d1;
    ue.data2 = d2;

    e.type = SDL_USEREVENT;
    e.user = ue;

    SDL_PushEvent(&e);
}

void userevent_handle(SDL_Event *e) {
    if(e->type != SDL_USEREVENT) {
        return;
    }

    switch(e->user.code) {
    case USEREVENT_CALLBACK:
        return ((void (*)(void*))(e->user.data1))(e->user.data2);
    default:
        return;
    }
}
