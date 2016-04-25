#include "starmap.h"

int starmap_setup()
{
	printf("Loading travel...\n");
	main_scene = SCENE_STARMAP;
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

	update_starmap_icons();

	SDL_SetRenderDrawColor(main_renderer, 0x00, 0x00, 0x00, 255);
	return 0;
}

int starmap_move_sector(int direction){
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
	update_starmap_icons();
}

void update_starmap_icons()
{
	gui_clear();
	Travel_Node *cn = (Travel_Node *)vector_get(starmap, current_node);
	if(t_sectorY>0){
		gui_add_sprite_button(g_card_N, 30, 0, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_MENU, -1, &starmap_move_sector, 1, NOFLIP, GUI_MOVE_BUTTON_COLOR);
	}
	if(t_sectorX<448){
		gui_add_sprite_button(g_card_E, 59, 29, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_MENU, -1, &starmap_move_sector, 2, NOFLIP, GUI_MOVE_BUTTON_COLOR);
	}
	if(t_sectorY<448){
		gui_add_sprite_button(g_card_S, 29, 59, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_MENU, -1, &starmap_move_sector, 3, FLIPV, GUI_MOVE_BUTTON_COLOR);
	}
	if(t_sectorX>0){
		gui_add_sprite_button(g_card_W, 0, 30, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_MENU, -1, &starmap_move_sector, 4, FLIPH, GUI_MOVE_BUTTON_COLOR);
	}
	for (int i = 0; i < vector_get_size(starmap); i++)
	{
		Travel_Node *t = (Travel_Node *)vector_get(starmap, i);
		if(t->x >= t_sectorX && t->x < t_sectorX+64 && t->y >= t_sectorY && t->y < t_sectorY+64){
			SDL_Texture* tex = t_node;
			int state = BUTTON_STATE_DISABLED;
			if (current_node == i)
			{
				tex = t_node_current;
			}

			for (int i = 0; i < sizeof(cn->connections) / sizeof(Travel_Node *); ++i)
			{
				if (cn->connections[i] == t)
				{
					state = BUTTON_STATE_ENABLED;
				}
			}

			if (t == (Travel_Node *)vector_get(starmap, current_node))
			{
				state = BUTTON_STATE_ENABLED;
			}

			SDL_Color c = FACTION_COLOR_NONE;
			if (t->f == 1)
			{
				c = FACTION_COLOR_SNEEB;
			}
			else if (t->f == 2)
			{
				c = FACTION_COLOR_KRULL;
			}
			else if (t->f == 3)
			{
				c = FACTION_COLOR_PLINK;
			}
			gui_add_sprite_button(tex, (t->x-t_sectorX) - half_node_sprite, (t->y-t_sectorY) - half_node_sprite, -1,  state, BUTTON_STYLE_GUI, -1, &starmap_go, i, NOFLIP, c);
		}
	}
	update_button_state(current_node, BUTTON_STATE_SELECTED);
}

int starmap_go(int destination)
{
	//get selected button
	printf("Initiating travel to %d!\n", destination);
	current_node = destination;
	update_starmap_icons();

	Travel_Node *cn = (Travel_Node *)vector_get(starmap, current_node);
//	if (cn->is_inhabited)
	{
		printf("Found habited planet! %d\n", cn->f);
		comms_set_faction(cn->f);
		planet_setup();
		comms_setup();
	}
	return 0;
}



void make_child_nodes(Vector *node_list, Travel_Node *n, int max_depth, int spread, int jitter, int merge_dist, int bounds, int direction)
{
	Travel_Node *nn = malloc(sizeof(Travel_Node));
	nn->depth = n->depth + 1;
	nn->x = (rand() % jitter) + spread;
	nn->y = (rand() % jitter) + spread;
	nn->connected = false;
	nn->f = n->f;

	for (int i = 0; i < sizeof(nn->connections) / sizeof(Travel_Node *); ++i)
	{
		nn->connections[i] = NULL;
	}

	//Make sure that our position is an appropriate distance from other sibling nodes
	switch (direction)
	{
		case 0:
			nn->x = n->x + (nn->x * 2 - nn->x);
			nn->y = n->y - nn->y * 2;
			break;
		case 1:
			nn->y = n->y + (nn->y * 2 - nn->y);
			nn->x = n->x + nn->x * 2;
			break;
		case 2:
			nn->x = n->x + (nn->x *2 - nn->x);
			nn->y = n->y + nn->y * 2;
			break;
		case 3:
			nn->x = n->x - nn->x * 2;
			nn->y = n->y + (nn->y * 2 - nn->y);
			break;
	}

	//Clamp to 0 and bounds
	if (nn->x > bounds)
	{
		nn->x = bounds;
	}
	else if (nn->x < 0)
	{
		nn->x = 0;
	}
	if (nn->y > bounds)
	{
		nn->y = bounds;
	}
	else if (nn->y < 0)
	{
		nn->y = 0;
	}

	for (int i = 0; i < vector_get_size(node_list); ++i)
	{
		Travel_Node *on = (Travel_Node *)vector_get(node_list, i);
		if (on != nn)
		{
			//If this node is too close to any other nodes, destroy it.
			if ((on->x - nn->x) * (on->x - nn->x) + (on->y - nn->y) * (on->y - nn->y) < merge_dist * merge_dist)
			{
				free(nn);
				return;
			}
		}
	}

	//If we're not too deep, make more nodes
	if(nn->depth < max_depth)
	{
		switch (direction)
		{
			case 0:
				make_child_nodes(node_list, nn, max_depth, spread, jitter, merge_dist, bounds, 0);
				make_child_nodes(node_list, nn, max_depth, spread, jitter, merge_dist, bounds, 1);
				make_child_nodes(node_list, nn, max_depth, spread, jitter, merge_dist, bounds, 3);
				break;
			case 1:
				make_child_nodes(node_list, nn, max_depth, spread, jitter, merge_dist, bounds, 0);
				make_child_nodes(node_list, nn, max_depth, spread, jitter, merge_dist, bounds, 1);
				make_child_nodes(node_list, nn, max_depth, spread, jitter, merge_dist, bounds, 2);
				break;
			case 2:
				make_child_nodes(node_list, nn, max_depth, spread, jitter, merge_dist, bounds, 1);
				make_child_nodes(node_list, nn, max_depth, spread, jitter, merge_dist, bounds, 2);
				make_child_nodes(node_list, nn, max_depth, spread, jitter, merge_dist, bounds, 3);
				break;
			case 3:
				make_child_nodes(node_list, nn, max_depth, spread, jitter, merge_dist, bounds, 0);
				make_child_nodes(node_list, nn, max_depth, spread, jitter, merge_dist, bounds, 2);
				make_child_nodes(node_list, nn, max_depth, spread, jitter, merge_dist, bounds, 3);
				break;
		}
	}

	if (nn->f < 0)
	{
		nn->f = rand() % 4;
	}
	Planet *p = malloc(sizeof(Planet));
	nn->p = p;
	planet_set_default(nn->p, nn->f);
	planet_set_random(nn->p);

	vector_add(node_list, nn);
}

//yoinked from http://stackoverflow.com/a/14795484
int get_line_intersection(Travel_Node *p0, Travel_Node *p1, Travel_Node *p2, Travel_Node *p3)
{
	float s02_x, s02_y, s10_x, s10_y, s32_x, s32_y, s_numer, t_numer, denom, t;
	s10_x = p1->x - p0->x;
	s10_y = p1->y - p0->y;
	s32_x = p3->x - p2->x;
	s32_y = p3->y - p2->y;

	denom = s10_x * s32_y - s32_x * s10_y;
	if (denom == 0)
	{
		return false; // Collinear
	}

	bool denomPositive = denom > 0;

	s02_x = p0->x - p2->x;
	s02_y = p0->y - p2->y;
	s_numer = s10_x * s02_y - s10_y * s02_x;
	if ((s_numer < 0) == denomPositive)
	{
		return false; // No collision
	}

	t_numer = s32_x * s02_y - s32_y * s02_x;
	if ((t_numer < 0) == denomPositive)
	{
		return false; // No collision
	}

	if (((s_numer > denom) == denomPositive) || ((t_numer > denom) == denomPositive))
	{
		return false; // No collision
	}

	// Collision detected
	t = t_numer / denom;
	float i_x = p0->x + (t * s10_x);
	float i_y = p0->y + (t * s10_y);

	if ((i_x == p0->x && i_y == p0->y) || (i_x == p1->x && i_y == p1->y) || (i_x == p2->x && i_y == p2->y) || (i_x == p3->x && i_y == p3->y))
	{
		return false; //Collision is at node intersection and not relevant
	}

	return true;
}

//Return whether or not this connection is safe
bool check_connection(Vector *node_list, Travel_Node *na1, Travel_Node *na2)
{
	Travel_Node *nb1;
	Travel_Node *nb2;

	for (int i = 0; i < vector_get_size(node_list); ++i)
	{
		nb1 = vector_get(node_list, i);

		//Loop through each of this node's connections
		for (int i = 0; i < sizeof(nb1->connections) / sizeof(Travel_Node *); ++i)
		{
			if (nb1->connections[i] == NULL)
			{
				continue;
			}
			else
			{
				nb2 = nb1->connections[i];
			}
			
			if ((nb1 == nb2) || ((nb1 == na1 && nb2 == na2) || (nb1 == na2 && nb2 == na1)))
			{
				continue;
			}

			float x, y;
			if (get_line_intersection(na1, na2, nb1, nb2) == true)
			{
				return false;
			}
		}
	}
	return true;
}

bool add_connection(Travel_Node *local_node, Travel_Node *remote_node)
{
	bool connection_success = false;

	//Set up the local side of the node connection
	for (int i = 0; i < sizeof(local_node->connections) / sizeof(Travel_Node *); ++i)
	{
		if (local_node->connections[i] == NULL)
		{
			local_node->connections[i] = remote_node;
			connection_success = true;
			break;
		}
	}

	if(connection_success)
	{
		//Set up the remote side of the node connection
		for (int i = 0; i < sizeof(remote_node->connections) / sizeof(Travel_Node *); ++i)
		{
			if (remote_node->connections[i] == NULL)
			{
				remote_node->connections[i] = local_node;
				break;
			}
		}
	}

	return connection_success;
}

void configure_connections(Vector *node_list, Travel_Node *n, int max_connection_dist)
{
	if (!n->connected)
	{
		int c[4] = {0};
		Travel_Node *close_node[4] = {NULL};

		//Work out the four closest nodes that have connections available
		for (int i = 0; i < vector_get_size(node_list); ++i)
		{
			Travel_Node *on = (Travel_Node *)vector_get(node_list, i);
			bool available_connection = false;
			for (int i = 0; i < sizeof(on->connections) / sizeof(Travel_Node *); ++i)
			{
				if (on->connections[i] == NULL)
				{
					available_connection = true;
					break;
				}
			}
			if (on != n && available_connection)
			{
				int d = (on->x - n->x) * (on->x - n->x) + (on->y - n->y) * (on->y - n->y);
				if ((c[0] == 0 || d < c[0]) && d < max_connection_dist * max_connection_dist)
				{
					if (check_connection(node_list, n, on))
					{
						c[3] = c[2];
						c[2] = c[1];
						c[1] = c[0];
						c[0] = d;

						close_node[3] = close_node[2];
						close_node[2] = close_node[1];
						close_node[1] = close_node[0];
						close_node[0] = on;
					}
				}
				else if ((c[1] == 0 || d < c[1]) && d < max_connection_dist * max_connection_dist)
				{
					if (check_connection(node_list, n, on))
					{
						c[3] = c[2];
						c[2] = c[1];
						c[1] = d;

						close_node[3] = close_node[2];
						close_node[2] = close_node[1];
						close_node[1] = on;
					}
				}
				else if ((c[2] == 0 || d < c[2]) && d < max_connection_dist * max_connection_dist)
				{
					if (check_connection(node_list, n, on))
					{
						c[3] = c[2];
						c[2] = d;

						close_node[3] = close_node[2];
						close_node[2] = on;
					}
				}
				else if ((c[3] == 0 || d < c[3]) && d < max_connection_dist * max_connection_dist)
				{
					if (check_connection(node_list, n, on))
					{
						c[3] = d;

						close_node[3] = on;
					}
				}
			}
		}
		if (close_node[0] != NULL)
		{
			if (!add_connection(n, close_node[0]))
			{
				//We're full up. There's no more we can do here.
				n->connected = true;
				return;
			}
			else if (close_node[1] != NULL)
			{
				if (!add_connection(n, close_node[1]))
				{
					//We're full up. There's no more we can do here.
					n->connected = true;
					return;
				}
				else if (close_node[2] != NULL)
				{
					if (!add_connection(n, close_node[2]))
					{
						//We're full up. There's no more we can do here.
						n->connected = true;
						return;
					}
					else if (close_node[3] != NULL)
					{
						if (!add_connection(n, close_node[3]))
						{
							//We're full up. There's no more we can do here.
							n->connected = true;
							return;
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < sizeof(n->connections) / sizeof(Travel_Node *); ++i)
	{
		bool connections_full = true;
		if (n->connections[i] == NULL)
		{
			connections_full = false;
			break;
		}
		n->connected = connections_full;
	}
}

void make_tree(Vector *node_list, Travel_NodeDefs defs, int root_x, int root_y)
{
	Travel_Node *root = malloc(sizeof(Travel_Node));
	root->x = root_x;
	root->y = root_y;
	root->f = defs.faction;
	root->depth = 0;
	root->connected = false;
	for (int i = 0; i < sizeof(root->connections) / sizeof(Travel_Node *); ++i)
	{
		root->connections[i] = NULL;
	}

	vector_add(node_list, root);
	make_child_nodes(node_list, root, defs.max_depth, defs.spread, defs.jitter, defs.merge_dist, defs.bounds, 0);
	make_child_nodes(node_list, root, defs.max_depth, defs.spread, defs.jitter, defs.merge_dist, defs.bounds, 1);
	make_child_nodes(node_list, root, defs.max_depth, defs.spread, defs.jitter, defs.merge_dist, defs.bounds, 2);
	make_child_nodes(node_list, root, defs.max_depth, defs.spread, defs.jitter, defs.merge_dist, defs.bounds, 3);

	if (defs.faction < 0)
	{
		root->f = rand() % 4;
	}
	Planet *p = malloc(sizeof(Planet));
	root->p = p;
	planet_set_default(root->p, root->f);
	planet_set_random(root->p);


	//TODO: There are better breadth-first search strategies
	for (int i = 0; i <= defs.max_depth; ++i)
	{
		for (int j = 0; j < vector_get_size(node_list); ++j)
		{
			Travel_Node *n = (Travel_Node *)vector_get(node_list, j);
			if (n->depth == i)
			{
				configure_connections(node_list, n, defs.max_connection_dist);
			}
		}
	}

	for (int i = 0; i < vector_get_size(node_list); ++i)
	{
		Travel_Node *n = (Travel_Node *)vector_get(node_list, i);
		if (!n->connected)
		{
			int connection_count = 0;
			for (int i = 0; i < sizeof(n->connections) / sizeof(Travel_Node *); ++i)
			{
				if (n->connections[i] != NULL)
				{
					connection_count ++;
				}
			}

			if (connection_count == 0)
			{
				vector_remove(node_list, i);
			}
		}
	}
}

void starmap_draw(Vector *node_list)
{
	SDL_SetRenderDrawColor(main_renderer, 0, 0, 0, 255);
	SDL_RenderClear(main_renderer);
	SDL_Texture* newtexture = SDL_CreateTexture(main_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, main_resX, main_resY);
	//Set the new texture as the render target
	SDL_SetRenderTarget(main_renderer, newtexture);
	SDL_RenderClear(main_renderer);

	for (int i = 0; i < vector_get_size(node_list); ++i)
	{
		//Render connections before nodes so that nodes can be on top
		Travel_Node *n = (Travel_Node *)vector_get(node_list, i);
		Travel_Node *cn;
		
		for (int j = 0; j < sizeof(n->connections) / sizeof(Travel_Node *); ++j)
		{
			if (n->connections[j] != NULL)
			{
				SDL_SetRenderDrawColor(main_renderer, 128, 128, 128, 255);
				cn = n->connections[j];
				if (n == (Travel_Node *)vector_get(node_list, current_node) || cn == (Travel_Node *)vector_get(node_list, current_node))
				{
					SDL_SetRenderDrawColor(main_renderer, 255, 255, 255, 255);
				}
				SDL_RenderDrawLine(main_renderer, n->x-t_sectorX, n->y-t_sectorY, cn->x-t_sectorX, cn->y-t_sectorY);
			}
		}
		SDL_SetRenderDrawColor(main_renderer, 0, 0, 0, 255);
	}
	for (int i = 0; i < vector_get_size(node_list); ++i)
	{
		Travel_Node *n = (Travel_Node *)vector_get(node_list, i);

		SDL_Color c = FACTION_COLOR_NONE;
		if (n->f == 1)
		{
			c = FACTION_COLOR_SNEEB;
		}
		else if (n->f == 2)
		{
			c = FACTION_COLOR_KRULL;
		}
		else if (n->f == 3)
		{
			c = FACTION_COLOR_PLINK;
		}
		SDL_SetRenderDrawColor(main_renderer, c.r, c.g, c.b, c.a);
		int x=n->x - t_sectorX;
		int y=n->y - t_sectorY;
/*
		SDL_RenderDrawPoint(main_renderer,  x-1, y);
		SDL_RenderDrawPoint(main_renderer,  x, y-1);
		SDL_RenderDrawPoint(main_renderer,  x-1, y-1);
		SDL_RenderDrawPoint(main_renderer,  x+1, y);
		SDL_RenderDrawPoint(main_renderer,  x, y+1);
		SDL_RenderDrawPoint(main_renderer,  x+1, y+1);
		SDL_RenderDrawPoint(main_renderer,  x, y);
		SDL_RenderDrawPoint(main_renderer,  x-1, y+1);*/
		SDL_RenderDrawPoint(main_renderer,  x+3, y-3);
	}

	SDL_SetRenderTarget(main_renderer, NULL);
	SDL_RenderCopy(main_renderer, newtexture, NULL, NULL);
}

void generate_starmap(Vector *node_list)
{
	//TODO: We'll need to set this somewhere else, or maybe pass it in?
	srand(4);

	Travel_NodeDefs defs;
	defs.merge_dist = 15;
	defs.max_depth = 7;
	defs.spread =  10;
	defs.jitter = 25;
	defs.bounds = 512;
	defs.max_connection_dist = 75; //something around merge_dist + spread + jitter
	defs.faction = -1; //-1 gives nodes random factions

	int root_x = defs.bounds / 2 + 32;
	int root_y = defs.bounds / 2 + 32;

	make_tree(node_list, defs, root_x, root_y);

	current_node = 0;
	t_sectorX=round(((Travel_Node *)vector_get(starmap, current_node))->x/64)*64;
	t_sectorY=round(((Travel_Node *)vector_get(starmap, current_node))->y/64)*64;
}
