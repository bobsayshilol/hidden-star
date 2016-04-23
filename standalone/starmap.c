//gcc -std=c99 -g util/vector.c standalone/starmap.c `sdl2-config --cflags`  `sdl2-config --libs`

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "../util/vector.h"

SDL_Window * window;
SDL_Renderer * renderer;

typedef struct Node NODE;

typedef struct Node
{
	int depth;
	int x;
	int y;
	int f;
	bool connected;
	NODE *connections[4];
} Node;

void set_pixel(SDL_Renderer *rend, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_SetRenderDrawColor(rend, r,g,b,a);
	SDL_RenderDrawPoint(rend, x, y);
}

void make_child_nodes(Vector *node_list, Node *n, int max_depth, int spread, int jitter, int merge_dist, int bounds, int direction)
{
	Node *nn = malloc(sizeof(Node));
	nn->depth = n->depth + 1;
	nn->x = (rand() % jitter) + spread;
	nn->y = (rand() % jitter) + spread;
	nn->connected = false;
	for (int i = 0; i < sizeof(nn->connections) / sizeof(Node *); ++i)
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
		Node *on = (Node *)vector_get(node_list, i);
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

	vector_add(node_list, nn);
}

//yoinked from http://stackoverflow.com/a/14795484
int get_line_intersection(Node *p0, Node *p1, Node *p2, Node *p3)
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
bool check_connection(Vector *node_list, Node *na1, Node *na2)
{
	Node *nb1;
	Node *nb2;

	for (int i = 0; i < vector_get_size(node_list); ++i)
	{
		nb1 = vector_get(node_list, i);

		//Loop through each of this node's connections
		for (int i = 0; i < sizeof(nb1->connections) / sizeof(Node *); ++i)
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

bool add_connection(Node *localNode, Node *remoteNode)
{
	bool connection_success = false;

	//Set up the local side of the node connection
	for (int i = 0; i < sizeof(localNode->connections) / sizeof(Node *); ++i)
	{
		if (localNode->connections[i] == NULL)
		{
			localNode->connections[i] = remoteNode;
			connection_success = true;
			break;
		}
	}

	if(connection_success)
	{
		//Set up the remote side of the node connection
		for (int i = 0; i < sizeof(remoteNode->connections) / sizeof(Node *); ++i)
		{
			if (remoteNode->connections[i] == NULL)
			{
				remoteNode->connections[i] = localNode;
				break;
			}
		}
	}

	return connection_success;
}

void makeConnections(Vector *node_list, Node *n, int max_connection_dist)
{
	if (!n->connected)
	{
		int c[4] = {0};
		Node *closeNodes[4] = {NULL};

		//Work out the four closest nodes that have connections available
		for (int i = 0; i < vector_get_size(node_list); ++i)
		{
			Node *on = (Node *)vector_get(node_list, i);
			bool available_connection = false;
			for (int i = 0; i < sizeof(on->connections) / sizeof(Node *); ++i)
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

						closeNodes[3] = closeNodes[2];
						closeNodes[2] = closeNodes[1];
						closeNodes[1] = closeNodes[0];
						closeNodes[0] = on;
					}
				}
				else if ((c[1] == 0 || d < c[1]) && d < max_connection_dist * max_connection_dist)
				{
					if (check_connection(node_list, n, on))
					{
						c[3] = c[2];
						c[2] = c[1];
						c[1] = d;

						closeNodes[3] = closeNodes[2];
						closeNodes[2] = closeNodes[1];
						closeNodes[1] = on;
					}
				}
				else if ((c[2] == 0 || d < c[2]) && d < max_connection_dist * max_connection_dist)
				{
					if (check_connection(node_list, n, on))
					{
						c[3] = c[2];
						c[2] = d;

						closeNodes[3] = closeNodes[2];
						closeNodes[2] = on;
					}
				}
				else if ((c[3] == 0 || d < c[3]) && d < max_connection_dist * max_connection_dist)
				{
					if (check_connection(node_list, n, on))
					{
						c[3] = d;

						closeNodes[3] = on;
					}
				}
			}
		}
		if (closeNodes[0] != NULL)
		{
			if (!add_connection(n, closeNodes[0]))
			{
				//We're full up. There's no more we can do here.
				n->connected = true;
				return;
			}
			else if (closeNodes[1] != NULL)
			{
				if (!add_connection(n, closeNodes[1]))
				{
					//We're full up. There's no more we can do here.
					n->connected = true;
					return;
				}
				else if (closeNodes[2] != NULL)
				{
					if (!add_connection(n, closeNodes[2]))
					{
						//We're full up. There's no more we can do here.
						n->connected = true;
						return;
					}
					else if (closeNodes[3] != NULL)
					{
						if (!add_connection(n, closeNodes[3]))
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

	for (int i = 0; i < sizeof(n->connections) / sizeof(Node *); ++i)
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

int main(int argc, char* argv[])
{
	SDL_CreateWindowAndRenderer(640, 640, 0, &window, &renderer);
	//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest"); // To get nice 'pixely' upscaling
	//SDL_RenderSetLogicalSize(renderer, 64, 64);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	int ticks = SDL_GetTicks();
	bool running = true;
	srand(4);


	//TREE CREATION CODE START
	Vector node_list;
	int merge_dist = 40;
	int max_depth = 3;
	int spread =  20;
	int jitter = 60;
	int bounds = 640;
	int max_connection_dist = 150; //something around merge_dist + spread + jitter

	vector_init(&node_list, 5);

	Node root;
	root.x = 320;
	root.y = 320;
	root.f = 0;
	root.depth = 0;
	root.connected = false;
	for (int i = 0; i < sizeof(root.connections) / sizeof(Node *); ++i)
	{
		root.connections[i] = NULL;
	}

	vector_add(&node_list, &root);
	make_child_nodes(&node_list, &root, max_depth, spread, jitter, merge_dist, bounds, 0);
	make_child_nodes(&node_list, &root, max_depth, spread, jitter, merge_dist, bounds, 1);
	make_child_nodes(&node_list, &root, max_depth, spread, jitter, merge_dist, bounds, 2);
	make_child_nodes(&node_list, &root, max_depth, spread, jitter, merge_dist, bounds, 3);

	//TODO: There are better breadth-first search strategies
	for (int i = 0; i <= max_depth; ++i)
	{
		for (int j = 0; j < vector_get_size(&node_list); ++j)
		{
			Node *n = (Node *)vector_get(&node_list, j);
			if (n->depth == i)
			{
				makeConnections(&node_list, n, max_connection_dist);
			}
		}
	}

	for (int i = 0; i < vector_get_size(&node_list); ++i)
	{
		Node *n = (Node *)vector_get(&node_list, i);
		if (!n->connected)
		{
			int connection_count = 0;
			for (int i = 0; i < sizeof(n->connections) / sizeof(Node *); ++i)
			{
				if (n->connections[i] != NULL)
				{
					connection_count ++;
				}
			}

			if (connection_count == 0)
			{
				vector_remove(&node_list, i);
			}
		}
	}
	//TREE CREATION CODE END


	printf("Generated %d nodes.\n", vector_get_size(&node_list));

	while (running)
	{
		//Poll for events to prevent Gnome from thinking we've locked up
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				running = false;
				break;
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		for (int i = 0; i < vector_get_size(&node_list); ++i)
		{
			//Render connections before nodes so that nodes can be on top
			Node *n = (Node *)vector_get(&node_list, i);
			if (n->depth <= max_depth)
			{
				Node *cn;
				
				for (int i = 0; i < sizeof(n->connections) / sizeof(Node *); ++i)
				{
					if (n->connections[i] != NULL)
					{
						SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
						cn = n->connections[i];
						SDL_RenderDrawLine(renderer, n->x, n->y, cn->x, cn->y);
					}
				}
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
			}
		}
		for (int i = 0; i < vector_get_size(&node_list); ++i)
		{
			Node *n = (Node *)vector_get(&node_list, i);
			int c = 255 - n->depth * (255 / max_depth);
			set_pixel(renderer,  n->x-1, n->y, 255, c, 0, 255);
			set_pixel(renderer,  n->x, n->y-1, 255, c, 0, 255);
			set_pixel(renderer,  n->x-1, n->y-1, 255, c, 0, 255);
			set_pixel(renderer,  n->x+1, n->y, 255, c, 0, 255);
			set_pixel(renderer,  n->x, n->y+1, 255, c, 0, 255);
			set_pixel(renderer,  n->x+1, n->y+1, 255, c, 0, 255);
			set_pixel(renderer,  n->x, n->y, 255, c, 0, 255);
			set_pixel(renderer,  n->x-1, n->y+1, 255, c, 0, 255);
			set_pixel(renderer,  n->x+1, n->y-1, 255, c, 0, 255);
		}

		set_pixel(renderer,  320, 322, 255, 255, 255, 255);
		set_pixel(renderer,  320, 318, 255, 255, 255, 255);
		set_pixel(renderer,  320, 323, 255, 255, 255, 255);
		set_pixel(renderer,  320, 317, 255, 255, 255, 255);
		set_pixel(renderer,  322, 320, 255, 255, 255, 255);
		set_pixel(renderer,  318, 320, 255, 255, 255, 255);
		set_pixel(renderer,  323, 320, 255, 255, 255, 255);
		set_pixel(renderer,  317, 320, 255, 255, 255, 255);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

