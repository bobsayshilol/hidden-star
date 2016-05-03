// gcc -std=c99 -o tunnel tunnel.c -O2 -lSDL2_image `sdl2-config --cflags --libs` -lm

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include "SDL.h" 
#include "SDL_image.h"

SDL_Event event;
SDL_Surface *screen;
SDL_Window *window;
SDL_Renderer *renderer;

int scale=8;
int resX=64;
int resY=64;
int ox=32;
int oy=32;

typedef struct Grid{
	float lx1;
	float ly1;
	float lx2;
	float ly2;
	float lx3;
	float ly3;
	float lx4;
	float ly4;
	float lm;
	int lc;
} Grid;

#define G_MAX 9

Grid g[G_MAX];
int gindex=0;

void draw_tunnel(){
	int a=0;
	int x1a, x1b, y1a, y1b=0;
	int x2a, x2b, y2a, y2b=0;
	int x3a, x3b, y3a, y3b=0;
	int x4a, x4b, y4a, y4b=0;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	SDL_RenderClear(renderer);
	SDL_Texture* newtexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, resX, resY);
	SDL_SetRenderTarget(renderer, newtexture);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	for(int i=0;i<=gindex;i++){
		a=((g[i].ly1-32)*4)-8;
		if(a<0){a=0;}
		SDL_SetRenderDrawColor(renderer, 0, a, 0, 255);
		if(i>0 && g[i].ly1<g[i-1].ly1){
			x1a=g[i-1].lx1; y1a=g[i-1].ly1; x1b=g[i].lx1; y1b=g[i].ly1;
			x2a=g[i-1].lx2; y2a=g[i-1].ly2; x2b=g[i].lx2; y2b=g[i].ly2;
			x3a=g[i-1].lx3; y3a=g[i-1].ly3; x3b=g[i].lx3; y3b=g[i].ly3;
			x4a=g[i-1].lx4; y4a=g[i-1].ly4; x4b=g[i].lx4; y4b=g[i].ly4;
			SDL_RenderDrawLine(renderer, x1a, y1a, x1b, y1b);
			SDL_RenderDrawLine(renderer, x2a, y2a, x2b, y2b);
			SDL_RenderDrawLine(renderer, x3a, y3a, x3b, y3b);
			SDL_RenderDrawLine(renderer, x4a, y4a, x4b, y4b);
			x1a=g[i-1].lx1+(((64-g[i-1].lx1)-g[i-1].lx1)/3); y1a=g[i-1].ly1; 
				x1b=g[i].lx1+(((64-g[i].lx1)-g[i].lx1)/3);   y1b=g[i].ly1;
			x2a=g[i-1].lx2+(((64-g[i-1].lx2)-g[i-1].lx2)/3); y2a=g[i-1].ly2; 
				x2b=g[i].lx2+(((64-g[i].lx2)-g[i].lx2)/3);   y2b=g[i].ly2;
			x3a=g[i-1].lx3+(((64-g[i-1].lx3)-g[i-1].lx3)/3); y1a=g[i-1].ly1; 
				x3b=g[i].lx3+(((64-g[i].lx3)-g[i].lx3)/3);   y1b=g[i].ly1;
			x4a=g[i-1].lx4+(((64-g[i-1].lx4)-g[i-1].lx4)/3); y1a=g[i-1].ly1; 
				x4b=g[i].lx4+(((64-g[i].lx4)-g[i].lx4)/3);   y1b=g[i].ly1;
			SDL_RenderDrawLine(renderer, x1a, y1a, x1b, y1b);
			SDL_RenderDrawLine(renderer, x2a, y2a, x2b, y2b);
			SDL_RenderDrawLine(renderer, x3a, y3a, x3b, y3b);
			SDL_RenderDrawLine(renderer, x4a, y4a, x4b, y4b);
			x1a=g[i-1].lx1; y1a=g[i-1].ly1+(((64-g[i-1].ly1)-g[i-1].ly1)/3); 
				x1b=g[i].lx1;   y1b=g[i].ly1+(((64-g[i].ly1)-g[i].ly1)/3);
			x2a=g[i-1].lx2; y2a=g[i-1].ly2+(((64-g[i-1].ly2)-g[i-1].ly2)/3); 
				x2b=g[i].lx2;   y2b=g[i].ly2+(((64-g[i].ly2)-g[i].ly2)/3);
			x3a=g[i-1].lx3; y3a=g[i-1].ly3+(((64-g[i-1].ly3)-g[i-1].ly3)/3); 
				x3b=g[i].lx3;   y3b=g[i].ly3+(((64-g[i].ly3)-g[i].ly3)/3);
			x4a=g[i-1].lx4; y4a=g[i-1].ly4+(((64-g[i-1].ly4)-g[i-1].ly4)/3); 
				x4b=g[i].lx4;   y4b=g[i].ly4+(((64-g[i].ly4)-g[i].ly4)/3);
			SDL_RenderDrawLine(renderer, x1a, y1a, x1b, y1b);
			SDL_RenderDrawLine(renderer, x2a, y2a, x2b, y2b);
			SDL_RenderDrawLine(renderer, x3a, y3a, x3b, y3b);
			SDL_RenderDrawLine(renderer, x4a, y4a, x4b, y4b);
		}
		if(i==gindex && g[i].ly1>g[0].ly1){
			x1a=g[0].lx1; y1a=g[0].ly1; x1b=g[i].lx1; y1b=g[i].ly1;
			x2a=g[0].lx2; y2a=g[0].ly2; x2b=g[i].lx2; y2b=g[i].ly2;
			x3a=g[0].lx3; y3a=g[0].ly3; x3b=g[i].lx3; y3b=g[i].ly3;
			x4a=g[0].lx4; y4a=g[0].ly4; x4b=g[i].lx4; y4b=g[i].ly4;
			SDL_RenderDrawLine(renderer, x1a, y1a, x1b, y1b);
			SDL_RenderDrawLine(renderer, x2a, y2a, x2b, y2b);
			SDL_RenderDrawLine(renderer, x3a, y3a, x3b, y3b);
			SDL_RenderDrawLine(renderer, x4a, y4a, x4b, y4b);
			x1a=g[0].lx1+(((64-g[0].lx1)-g[0].lx1)/3); y1a=g[0].ly1; 
				x1b=g[i].lx1+(((64-g[i].lx1)-g[i].lx1)/3);   y1b=g[i].ly1;
			x2a=g[0].lx2+(((64-g[0].lx2)-g[0].lx2)/3); y2a=g[0].ly2; 
				x2b=g[i].lx2+(((64-g[i].lx2)-g[i].lx2)/3);   y2b=g[i].ly2;
			x3a=g[0].lx3+(((64-g[0].lx3)-g[0].lx3)/3); y1a=g[0].ly1; 
				x3b=g[i].lx3+(((64-g[i].lx3)-g[i].lx3)/3);   y1b=g[i].ly1;
			x4a=g[0].lx4+(((64-g[0].lx4)-g[0].lx4)/3); y1a=g[0].ly1; 
				x4b=g[i].lx4+(((64-g[i].lx4)-g[i].lx4)/3);   y1b=g[i].ly1;
			SDL_RenderDrawLine(renderer, x1a, y1a, x1b, y1b);
			SDL_RenderDrawLine(renderer, x2a, y2a, x2b, y2b);
			SDL_RenderDrawLine(renderer, x3a, y3a, x3b, y3b);
			SDL_RenderDrawLine(renderer, x4a, y4a, x4b, y4b);
			x1a=g[0].lx1; y1a=g[0].ly1+(((64-g[0].ly1)-g[0].ly1)/3); 
				x1b=g[i].lx1;   y1b=g[i].ly1+(((64-g[i].ly1)-g[i].ly1)/3);
			x2a=g[0].lx2; y2a=g[0].ly2+(((64-g[0].ly2)-g[0].ly2)/3); 
				x2b=g[i].lx2;   y2b=g[i].ly2+(((64-g[i].ly2)-g[i].ly2)/3);
			x3a=g[0].lx3; y3a=g[0].ly3+(((64-g[0].ly3)-g[0].ly3)/3); 
				x3b=g[i].lx3;   y3b=g[i].ly3+(((64-g[i].ly3)-g[i].ly3)/3);
			x4a=g[0].lx4; y4a=g[0].ly4+(((64-g[0].ly4)-g[0].ly4)/3); 
				x4b=g[i].lx4;   y4b=g[i].ly4+(((64-g[i].ly4)-g[i].ly4)/3);
			SDL_RenderDrawLine(renderer, x1a, y1a, x1b, y1b);
			SDL_RenderDrawLine(renderer, x2a, y2a, x2b, y2b);
			SDL_RenderDrawLine(renderer, x3a, y3a, x3b, y3b);
			SDL_RenderDrawLine(renderer, x4a, y4a, x4b, y4b);
		}
		SDL_RenderDrawLine(renderer, g[i].lx1, g[i].ly1, g[i].lx2, g[i].ly2);
		SDL_RenderDrawLine(renderer, g[i].lx2, g[i].ly2, g[i].lx3, g[i].ly3);
		SDL_RenderDrawLine(renderer, g[i].lx3, g[i].ly3, g[i].lx4, g[i].ly4);
		SDL_RenderDrawLine(renderer, g[i].lx4, g[i].ly4, g[i].lx1, g[i].ly1);
	}
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, newtexture, NULL, NULL);
	SDL_RenderPresent(renderer);

	SDL_DestroyTexture(newtexture);
}

void set_default(int i){
	g[i].lx1=ox;
	g[i].ly1=oy;
	g[i].lx2=ox;
	g[i].ly2=oy;
	g[i].lx3=ox;
	g[i].ly3=oy;
	g[i].lx4=ox;
	g[i].ly4=oy;
	g[i].lm=8;
	g[i].lc=0;
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
	SDL_SetWindowTitle(window, "Travel");
    atexit(SDL_Quit);

	srand(time(NULL));
	set_default(0);

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
int pause=0;

while(event.type != SDL_QUIT){
	SDL_GetMouseState(&mouseX, &mouseY);
	if(SDL_PollEvent(&event)<1){SDL_Delay(10);}else{
		switch (event.type) {
			case SDL_QUIT:
				exit(0);
				break;
			case SDL_MOUSEBUTTONDOWN:
				if(pause==0){pause=1;}else{pause=0;}
				break;
		}
	}

	current_time=SDL_GetTicks();
	if(pause==0 && (current_time-last_time) > 64){
		for(int i=0;i<=gindex;i++){
			if(g[i].lc > g[i].lm){
				g[i].ly1++;
				g[i].lx1--;
				g[i].ly2--;
				g[i].lx2--;
				g[i].ly3--;
				g[i].lx3++;
				g[i].ly4++;
				g[i].lx4++;
				g[i].lm=(g[i].lm/(1.2));
				g[i].lc=0;
				if(i==gindex && gindex<G_MAX-1){
					gindex++;
					set_default(gindex);
				}
			}
			g[i].lc++;
			if(g[i].ly1 > 72){
				set_default(i);
			}
		}
		last_time=current_time;
		draw_tunnel();
	}
}
return 0;
}
