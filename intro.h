#ifndef HS_INTRO
#define HS_INTRO

#include "main.h"

SDL_Texture *intro_bg;
SDL_Texture *intro_flare;

int intro_count1;
int intro_count2;
int intro_count3;
int intro_font;

int intro_setup();
void intro_skip();
void intro_draw();

#endif
