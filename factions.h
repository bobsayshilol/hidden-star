#ifndef HS_FACTIONS
#define HS_FACTIONS

#include "main.h"

#define FACTION_COLOR_NONE (SDL_Color){255, 255, 255, 255}
#define FACTION_COLOR_SNEEB (SDL_Color){220, 75, 175, 255}
#define FACTION_COLOR_KRULL (SDL_Color){75, 185, 60, 255}
#define FACTION_COLOR_PLINK (SDL_Color){135, 120, 245, 255}

#define FACTION_NONE 0
#define FACTION_SNEEB 1
#define FACTION_KRULL 2
#define FACTION_PLINK 3

#define FACTION_THRESHOLD_HATE -50
#define FACTION_THRESHOLD_DISLIKE -20
#define FACTION_THRESHOLD_NEUTRAL 10
#define FACTION_THRESHOLD_LIKE 20
#define FACTION_THRESHOLD_LOVE 75

int faction_disposition[4];
SDL_Color faction_colors[4];
char *faction_names[4];
char *faction_homeworlds[4];

void factions_setup();
void faction_update_disposition(int faction, int adjustment);
int faction_get_disposition(int faction);
#endif
