#include "travel.h"

int travel_setup()
{
	printf("Loading travel...\n");
	main_scene = SCENE_TRAVEL;
	frame_skip=0;

	t_stars1 = Load_tex("sprites/stars/stars1_256.png");
	t_stars2 = Load_tex("sprites/stars/stars2_256.png");
	t_stars3 = Load_tex("sprites/stars/stars3_256.png");
	t_stars4 = Load_tex("sprites/stars/stars4_256.png");
	t_stars5 = Load_tex("sprites/stars/stars5_256.png");
	t_stars6 = Load_tex("sprites/stars/stars6_256.png");

	t_node = Load_tex("sprites/starmap/starmap_star_inactive.png");
	t_node_current = Load_tex("sprites/starmap/starmap_star_active.png");
	SDL_QueryTexture(t_node, NULL, NULL, &half_node_sprite, NULL);
	half_node_sprite /= 2;
	t_sectorX=0;
	t_sectorY=0;
	update_travel_icons();


	SDL_SetRenderDrawColor(main_renderer, 0x00, 0x00, 0x00, 255);
	return 0;
}

void generate_starmap()
{
	inhabited_planet_count = 0;

	vector_free_and_free(&node_list);
	vector_init(&node_list, TRAVEL_MAX_NODES);
	for (int i = 0; i < TRAVEL_MAX_NODES; i ++)
	{
		Travel_Node *t = malloc(sizeof(Travel_Node));
		Planet *p = malloc(sizeof(Planet));
		//todo: unclump
		//todo: panning
		t->x = (rand() % 504) + 4;
		t->y = (rand() % 504) + 4;
		t->faction = rand() % 4;
		t->is_inhabited = rand() % 2;
		t->connectedNode1 = -1;
		t->connectedNode2 = -1;
		t->p=p;
		planet_set_default(t->p, t->faction);
		planet_set_random(t->p);
		if (t->is_inhabited > 0)
		{
			inhabited_planet_count++;
		}
		vector_add(&node_list, t);
	}

	printf("Generating map of %d systems with %d inhabited planets.\n", TRAVEL_MAX_NODES, inhabited_planet_count);

	for (int i = 0; i < vector_get_size(&node_list); i++)
	{
		Vector *adjacentNodes = malloc(sizeof(Vector));
		vector_init(adjacentNodes, 2);
		Travel_Node *t = (Travel_Node *)vector_get(&node_list, i);

		for (int j = 0; j < vector_get_size(&node_list); j++)
		{
			if (j != i)
			{
				Travel_Node *n = vector_get(&node_list, j);
				if (n->connectedNode1 == -1)
				{
					n->connectedNode1 = i;
					t->connectedNode1 = j;
				}
				else if (n->connectedNode2 == -1)
				{
					n->connectedNode2 = i;
					t->connectedNode1 = j;
				}
			}
			if (t->connectedNode1 != -1)
			{
				break;
			}
		}

		for (int j = 0; j < vector_get_size(&node_list); j++)
		{
			if (j != i)
			{
				Travel_Node *n = vector_get(&node_list, j);
				if (n->connectedNode1 == -1)
				{
					n->connectedNode1 = i;
					t->connectedNode2 = j;
				}
				else if (n->connectedNode2 == -1)
				{
					n->connectedNode2 = i;
					t->connectedNode2 = j;
				}
			}
			if (t->connectedNode1 != -1)
			{
				break;
			}
		}
	}
	current_node = 0;
}

int travel_move_sector(int direction){
	switch(direction){
		case 1:
			t_sectorY-=64;
			break;
		case 2:
			t_sectorX+=64;
			break;
		case 3:
			t_sectorY+=64;
			break;
		case 4:
			t_sectorX-=64;
			break;
	}
	if(t_sectorX<0){t_sectorX=0;}
	if(t_sectorY<0){t_sectorY=0;}
	if(t_sectorX>448){t_sectorX=448;}
	if(t_sectorX>448){t_sectorY=448;}
	update_travel_icons();
}

void update_travel_icons()
{
	gui_clear();
	Travel_Node *cn = (Travel_Node *)vector_get(&node_list, current_node);
	if(t_sectorY>0){
		gui_add_sprite_button(g_card_N, 30, 0, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_MENU, -1, &travel_move_sector, 1);
	}
	if(t_sectorX<448){
		gui_add_sprite_button(g_card_E, 59, 29, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_MENU, -1, &travel_move_sector, 2);
	}
	if(t_sectorY<448){
		gui_add_sprite_button(g_card_S, 29, 59, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_MENU, -1, &travel_move_sector, 3);
	}
	if(t_sectorX>0){
		gui_add_sprite_button(g_card_W, 0, 30, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_MENU, -1, &travel_move_sector, 4);
	}
	for (int i = 0; i < vector_get_size(&node_list); i++)
	{
		Travel_Node *t = (Travel_Node *)vector_get(&node_list, i);
		if(t->x >= t_sectorX && t->x < t_sectorX+64 && t->y >= t_sectorY && t->y < t_sectorY+64){
			SDL_Texture* tex = t_node;
			int state = BUTTON_STATE_DISABLED;
			if (current_node == i)
			{
				tex = t_node_current;
			}
			if (i == cn->connectedNode1 || i == cn->connectedNode2)
			{
				state = BUTTON_STATE_ENABLED;
			}
			gui_add_sprite_button(tex, (t->x-t_sectorX) - half_node_sprite, (t->y-t_sectorY) - half_node_sprite, -1,  state, BUTTON_STYLE_MENU, -1, &travel_go, i);
		}
	}
	update_button_state(current_node, BUTTON_STATE_SELECTED);
}

int travel_go(int destination)
{
	//get selected button
	printf("Initiating travel to %d!\n", destination);
	current_node = destination;
	update_travel_icons();

	Travel_Node *cn = (Travel_Node *)vector_get(&node_list, current_node);
	if (cn->is_inhabited)
	{
		printf("Found habited planet! %d\n", cn->faction);
		comms_set_faction(cn->faction);
		planet_setup();
		comms_setup();
	}
	return 0;
}

void travel_draw()
{
	SDL_RenderClear(main_renderer);
	SDL_Texture* newtexture = SDL_CreateTexture(main_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, main_resX, main_resY);
	//Set the new texture as the render target
	SDL_SetRenderTarget(main_renderer, newtexture);
	SDL_RenderClear(main_renderer);

	for (int i = 0; i < vector_get_size(&node_list); i++)
	{
		Travel_Node *t = (Travel_Node *)vector_get(&node_list, i);
		if (t->connectedNode1 > -1)
		{
			int a = 128;
			if (i == current_node || t->connectedNode1 == current_node)
			{
				a = 255;
			}
			Travel_Node *c1 = (Travel_Node *)vector_get(&node_list, t->connectedNode1);
			SDL_SetRenderDrawColor(main_renderer, a, a, a, 1);
			SDL_RenderDrawLine(main_renderer, t->x-t_sectorX, t->y-t_sectorY, c1->x-t_sectorX, c1->y-t_sectorY);
		}
		if (t->connectedNode2 > -1)
		{
			int a = 128;
			if (i == current_node || t->connectedNode2 == current_node)
			{
				a = 255;
			}
			Travel_Node *c2 = (Travel_Node *)vector_get(&node_list, t->connectedNode2);
			SDL_SetRenderDrawColor(main_renderer, a, a, a, 1);
			SDL_RenderDrawLine(main_renderer, t->x-t_sectorX, t->y-t_sectorY, c2->x-t_sectorX, c2->y-t_sectorY);
		}
		SDL_SetRenderDrawColor(main_renderer, 0, 0, 0, 255);
	}
	SDL_SetRenderTarget(main_renderer, NULL);
	SDL_RenderCopy(main_renderer, newtexture, NULL, NULL);
}
