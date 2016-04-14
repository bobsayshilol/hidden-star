// gcc -std=c99 -o planet planet.c -O2 -lSDL2_image `sdl2-config --cflags --libs` -lm -lSDL2_gfx

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include "SDL.h" 
#include "SDL_image.h"
//#include "SDL2_gfxPrimitives.h"

#ifdef __MINGW64__
	#include <windows.h>
#endif


#define MASK0 0
#define MASK1 1
#define MAXP 4

SDL_Event event;
SDL_Surface *screen;
SDL_Window *window;
SDL_Renderer *renderer;

SDL_Texture *stars1;

SDL_Texture *caps[MAXP][5];
SDL_Texture *clouds[MAXP][5];
SDL_Texture *continents[MAXP][5];
SDL_Texture *ocean[MAXP][5];
SDL_Texture *planet_mask[4][5];


const int resX=64;
const int resY=64;
int scale=8;

int px1,px2,px3,px4=0;
int psize=3;
int p_ocn, p_cap, p_con, p_cld, p_msk=0;

SDL_Texture* Load_img(char *filename){
	SDL_Texture* texture = IMG_LoadTexture(renderer, filename);
    if (!texture){
	    printf("Load image error: %s, %s \n",filename, IMG_GetError());
		exit(0);
    }
    return texture;
}


void blit(SDL_Texture *tex, int x, int y, int mask){
	int w, h, r;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	SDL_Rect rect;
	rect.x=x;
	rect.y=y;
	rect.w=w;
	rect.h=h;
	switch(mask){
		case MASK0:
			SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
			break;
		case MASK1:
			SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_MOD);
			break;
	}
	SDL_RenderCopy(renderer, tex, NULL, &rect);
}

void draw_planet(int time_pos){
	int width=(int)(pow(2,(psize+3)))*2;

	SDL_Texture* auxtexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width/2, width/2);
	//Set the new texture as the render target
	SDL_SetRenderTarget(renderer, auxtexture);

	blit(ocean[p_ocn][psize],px1,0, MASK0);
	blit(ocean[p_ocn][psize],px1-width,0, MASK0);
	if(p_con<MAXP){
	blit(continents[p_con][psize],px1,0, MASK0);
	blit(continents[p_con][psize],px1-width,0, MASK0);
	}
	if(p_cap<MAXP){
	blit(caps[p_cap][psize],px1,0, MASK0);
	blit(caps[p_cap][psize],px1-width,0, MASK0);
	}
	if(p_cld<MAXP){
	blit(clouds[p_cld][psize],px2,0, MASK0);
	blit(clouds[p_cld][psize],px2-width,0, MASK0);
	}
	blit(planet_mask[p_msk][psize],0,0, MASK1);

	SDL_Surface *surf = SDL_CreateRGBSurface(0, width/2, width/2, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
	SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA8888, surf->pixels, surf->pitch);
	SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf->format,0,0,0));
	SDL_Texture* newtexture=SDL_CreateTextureFromSurface(renderer, surf);


	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderClear(renderer);
	blit(stars1,0,0, MASK0);
	blit(newtexture,16,16,MASK0);
	px1+=1;
	px2+=2;
	if(px1>width){px1=0;}
	if(px2>width){px2=0;}
}

 /* Init and start */
int setup(){
	if (SDL_Init(SDL_INIT_VIDEO) <0){
		 SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
		return 3;
	}

	if (SDL_CreateWindowAndRenderer(resX*scale, resY*scale, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
		return 3;
    }

    SDL_RenderSetLogicalSize(renderer, resX, resY);
	SDL_SetWindowTitle(window, "Planet Generator 64");
    atexit(SDL_Quit);

	srand(time(NULL));

	stars1 = Load_img("sprites/stars/stars1_256.png");
	for(int j=0;j<5;j++){
		char file[128];
		for(int i=0;i<4;i++){
			int size=(int)(pow(2,(j+3)));
			if(i<MAXP){
				sprintf(file,"sprites/planets/caps%d_%d.png",(i+1), size);
				caps[i][j] = Load_img(file);
				sprintf(file,"sprites/planets/clouds%d_%d.png",(i+1), size);
				clouds[i][j] = Load_img(file);
				sprintf(file,"sprites/planets/continents%d_%d.png",(i+1), size);
				continents[i][j] = Load_img(file);
				sprintf(file,"sprites/planets/ocean%d_%d.png",(i+1), size);
				ocean[i][j] = Load_img(file);
			}
			sprintf(file,"sprites/planets/planet_mask%d_%d.png",(i+1), size);
			planet_mask[i][j] = Load_img(file);
		}
	}

	return 0;
}

void get_random_planet()
{
	//psize=(rand()%5);
	p_ocn=(rand()%MAXP);
	p_con=(rand()%(MAXP+1));
	p_cap=(rand()%(MAXP+1));
	p_cld=(rand()%MAXP);
	//p_msk=(rand()%4);
}

void get_default_planet(int p)
{
	p_ocn = p;
	p_con = p;
	p_cap = p;
	p_cld = p;
}

void print_planet_params()
{
	printf("Current planet:\tocn: %d,\t", p_ocn);
	if (p_con < MAXP)
	{
		printf("con: %d,\t", p_con);
	}
	else
	{
		printf("con: -,\t");
	}
	if (p_cap < MAXP)
	{
		printf("cap: %d,\t", p_cap);
	}
	else
	{
		printf("cap: -,\t");
	}
	printf("cld: %d\n", p_cld);
}

void print_usage()
{
	printf("*****************************\n");
	printf(" SUPER COOL 64PX PLANET GENERATOR\n");
	printf("*****************************\n\n");
	printf("Click or press Enter to generate a new randomised planet.\n\n");
	printf("Manual controls:\n");
	printf("\tQ: Cycle surface layer (ocean).\n");
	printf("\tW: Cycle continent layer.\n");
	printf("\tE: Cycle polar cap layer.\n");
	printf("\tR: Cycle cloud layer.\n");
	printf("\tT: Cycle light position.\n");
	printf("\tZ: Print current planet parameters.\n");
	printf("\tF2: Decrease window size.\n");
	printf("\tF3: Increase window size.\n");
	printf("\tSpace: Cycle planet size.\n");
	printf("\t1 through %d: Display default planet configuration.\n", MAXP);
	printf("\tEscape: Quit :(\n\n");
}

int main(int argc, char *argv[]){
	int s=setup();
	if(s > 0){
		return s;
	}

print_usage();

int time_pos=0;
int current_time;
int last_time = SDL_GetTicks();
int mouseX; int mouseY;

while(event.type != SDL_QUIT){
	SDL_GetMouseState(&mouseX, &mouseY);
	if(SDL_PollEvent(&event)<1){SDL_Delay(10);}else{
		switch (event.type) {
			case SDL_QUIT:
				exit(0);
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
					case SDLK_F2:
						if(scale>1){
							scale/=2;
						}
						SDL_SetWindowSize(window, resX*scale, resY*scale);
						break;
					case SDLK_F3:
						if(scale<32){
							scale*=2;
						}
						SDL_SetWindowSize(window, resX*scale, resY*scale);
						break;
					case SDLK_ESCAPE:
						{
						SDL_Event e;
						e.type = SDL_QUIT;
						SDL_PushEvent(&e);
						}
						break;
					case SDLK_q:
						p_ocn++;
						if(p_ocn>=MAXP){p_ocn=0;}
						break;
					case SDLK_w:
						p_con++;
						if(p_con>=MAXP+1){p_con=0;}
						break;
					case SDLK_e:
						p_cap++;
						if(p_cap>=MAXP+1){p_cap=0;}
						break;
					case SDLK_r:
						p_cld++;
						if(p_cld>=MAXP){p_cld=0;}
						break;
					case SDLK_t:
						p_msk++;
						if(p_msk>=MAXP){p_msk=0;}
						break;
					case SDLK_z:
						print_planet_params();
						break;
					case SDLK_1:
					case SDLK_KP_1:
						get_default_planet(0);
						break;
					case SDLK_2:
					case SDLK_KP_2:
						get_default_planet(1);
						break;
					case SDLK_3:
					case SDLK_KP_3:
						get_default_planet(2);
						break;
					case SDLK_4:
					case SDLK_KP_4:
						get_default_planet(3);
						break;
					case SDLK_5:
					case SDLK_KP_5:
						//TODO: Make a nicer way of doing this that automatically respects MAXP
						break;
					case SDLK_RETURN:
					case SDLK_KP_ENTER:
						get_random_planet();
						break;
					case SDLK_SPACE:
						psize++;
						if(psize>=5){psize=0;}
						break;
				}
			break;
			case SDL_MOUSEBUTTONDOWN:
				get_random_planet();
				break;
		}
	}

	current_time=SDL_GetTicks();
	if((current_time-last_time) > 64){
		last_time=current_time;
		time_pos--;
		if(time_pos < -63){
			time_pos=0;
		}
		draw_planet(time_pos);
		SDL_RenderPresent(renderer);
	}
}
return 0;
}

#ifdef __MINGW64__
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	char* argv[0];
	main(0, argv);
}
#endif
