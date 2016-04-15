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
#include "combat.h"

#define main_resX 64
#define main_resY 64

#define P_MODE0 0
#define P_MODE1 1
#define P_MODE2 2
#define NOFLIP 3
#define FLIPV 4
#define FLIPH 5
#define FLIPHV 6
#define TWINK1 10
#define TWINK2 11
#define TWINK3 12
#define TWINK4 13
#define TWINK5 14
#define TWINK6 15

SDL_Surface *main_screen;
SDL_Window *main_window;
SDL_Renderer *main_renderer;

int main_scale;
int main_scene;
int frame_skip;

SDL_Texture *font[4];

SDL_Texture* Load_tex(char *filename);
void main_blit(SDL_Texture *tex, int x, int y, int mode, SDL_Color *color);
void draw_text(int x, int y, char *text, int length, int font_set);
void draw_number(int x, int y, int num);
void draw_scene();
void main_input(SDL_Event event);
int main_setup();

#endif
