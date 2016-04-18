#ifndef HS_TRAVEL
#define HS_TRAVEL

#include "main.h"
#include "util/vector.h"

#define TRAVEL_STATE_MAP 0
#define TRAVEL_STATE_TRAVEL 1

#define TRAVEL_MAX_NODES 10

#define TRAVEL_FACTION_NONE 0
#define TRAVEL_FACTION_SNEEB 1
#define TRAVEL_FACTION_KRULL 2
#define TRAVEL_FACTION_PLINK 3


typedef struct Travel_Node
{
	int x;
	int y;
	int faction;
	int is_inhabited;
	int connectedNode1;
	int connectedNode2;
} Travel_Node;

SDL_Texture *t_stars1;
SDL_Texture *t_stars2;
SDL_Texture *t_stars3;
SDL_Texture *t_stars4;
SDL_Texture *t_stars5;
SDL_Texture *t_stars6;
SDL_Texture *t_node;
SDL_Texture *t_node_current;
Vector node_list;

int current_node;
int half_node_sprite;

int travel_setup();
void generate_starmap();
void update_travel_icons();
int travel_go();
void travel_draw();

#endif
