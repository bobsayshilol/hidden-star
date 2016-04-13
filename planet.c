// gcc -std=c99 -o planet planet.c -O2 -lSDL2_image `sdl2-config --cflags --libs` -lm -lSDL2_gfx

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include "SDL.h" 
#include "SDL_image.h"
#include "SDL2_gfxPrimitives.h"


#define MASK0 0
#define MASK1 1

SDL_Event event;
SDL_Surface *screen;
SDL_Window *window;
SDL_Renderer *renderer;

SDL_Texture *caps[2][5];
SDL_Texture *clouds[2][5];
SDL_Texture *continents[2][5];
SDL_Texture *ocean[2][5];
SDL_Texture *planet_mask[4][5];


const int scale=8;
const int resX=64;
const int resY=64;

int px1,px2,px3,px4=0;
int psize=3;
int p_ocn, p_cap, p_con, p_cld=0;

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
	SDL_RendererFlip flip;
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
	px1+=1;
	px2+=2;
	if(px1>width){px1=0;}
	if(px2>width){px2=0;}
	SDL_RenderClear(renderer);
	blit(ocean[p_ocn][psize],px1,0, MASK0);
	blit(ocean[p_ocn][psize],px1-width,0, MASK0);
	if(p_con<2){
	blit(continents[p_con][psize],px1,0, MASK0);
	blit(continents[p_con][psize],px1-width,0, MASK0);
	}
	if(p_cap<2){
	blit(caps[p_cap][psize],px1,0, MASK0);
	blit(caps[p_cap][psize],px1-width,0, MASK0);
	}
	if(p_cld<2){
	blit(clouds[p_cld][psize],px2,0, MASK0);
	blit(clouds[p_cld][psize],px2-width,0, MASK0);
	}
	blit(planet_mask[0][psize],0,0, MASK1);
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

//	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, resX, resY);
//	screen = SDL_GetWindowSurface(window);
	SDL_SetWindowTitle(window, "SixtyFour");
    atexit(SDL_Quit);

	srand(time(NULL));

	for(int j=0;j<5;j++){
		char file[128];
		for(int i=0;i<4;i++){
			int size=(int)(pow(2,(j+3)));
			if(i<2){
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


int main(int argc, char *argv[]){
	int s=setup();
	if(s > 0){
		return s;
	}

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
			case SDL_MOUSEBUTTONDOWN:
//				psize=(rand()%5);
				p_ocn=(rand()%2);
				p_con=(rand()%3);
				p_cap=(rand()%3);
				p_cld=(rand()%3);
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
