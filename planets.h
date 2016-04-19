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

typedef struct Planet
{
	float speed;
	int tx1,tx2;
	int x, y;
	int size;
	int ocn, cap, con, cld, msk;
	SDL_Color color[4];
	int color_state;
} Planet;

void planet_draw(Planet *p);
int planet_setup();
SDL_Color* planet_get_color(Planet *p, int index);
void planet_random_pos(Planet *p);
void planet_set_random(Planet *p);
void planet_set_default(Planet *p, int type);
//void planets_handle_input(SDL_Event event);

#endif
