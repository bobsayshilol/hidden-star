#ifndef HS_USER_EVENTS
#define HS_USER_EVENTS
#include "SDL_events.h"

typedef enum UserEventType {
    USEREVENT_CALLBACK = 0,
    USEREVENT_NUM
} UserEventType;

typedef void (*UserEventCallback)(void *);

void userevent_add(UserEventType t, void *d1, void *d2);
void userevent_handle(SDL_Event *e);
#endif
