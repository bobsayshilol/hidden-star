#ifndef HS_MAIN
#define HS_MAIN

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include "SDL.h" 
#include "SDL_image.h"

#include "intro.h"

SDL_Event main_event;
SDL_Surface *main_screen;
SDL_Window *main_window;
SDL_Renderer *main_renderer;

#define main_resX 64
#define main_resY 64

int main_scale;
int main_scene;

SDL_Texture *font[4];

SDL_Texture* Load_tex(char *filename);
void draw_text(int x, int y, char *text, int length, int font_set);
void draw_number(int x, int y, int num);
void draw_scene(int time_pos);
int main_setup();

#endif
