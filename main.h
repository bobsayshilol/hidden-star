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
#include "planets.h"

SDL_Surface *main_screen;
SDL_Window *main_window;
SDL_Renderer *main_renderer;

#define main_resX 64
#define main_resY 64

int main_scale;
int main_scene;
int frame_skip;

SDL_Texture *font[4];

SDL_Texture* Load_tex(char *filename);
void main_blit(SDL_Texture *tex, int x, int y, int mode, SDL_Color *color);
void draw_text(int x, int y, char *text, int length, int font_set);
void draw_number(int x, int y, int num);
void draw_scene(int time_pos);
void main_input(SDL_Event event);
int main_setup();

#endif
