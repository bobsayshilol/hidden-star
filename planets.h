#ifndef HS_PLANETS
#define HS_PLANETS

#include "main.h"

#define P_MAX 4


SDL_Texture *stars1;
SDL_Texture *caps[P_MAX][5];
SDL_Texture *clouds[P_MAX][5];
SDL_Texture *continents[P_MAX][5];
SDL_Texture *ocean[P_MAX][5];
SDL_Texture *planet_mask[4][5];

float p_speed;
int p_tx1,p_tx2;
int p_x, p_y;
int p_size;
int p_ocn, p_cap, p_con, p_cld, p_msk;
SDL_Color p_color[4];
int p_color_state;

void planet_draw();
int planet_setup();
SDL_Color* planet_get_color(int index);
void planet_random_pos();
void planet_get_random();
void planet_get_default(int p);
void planets_handle_input(SDL_Event event);

#endif
