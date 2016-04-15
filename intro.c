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

SDL_Texture *background;
SDL_Texture *font[4];

int scale=8;
int resX=64;
int resY=64;

int count1=0;
int count2=0;
int count3=1;
int count4=0;

SDL_Texture* Load_img(char *filename){
	SDL_Texture* texture = IMG_LoadTexture(renderer, filename);
    if (!texture){
	    printf("Load image error: %s, %s \n",filename, IMG_GetError());
		exit(0);
    }
    return texture;
}

void draw_text(int x, int y, char *text, int len){
	int w, h, f, index;
	SDL_QueryTexture(font[0], NULL, NULL, &w, &h);
	for(int i=0;i<len;i++){
		if(count3>0){count3++;}
		if(count3>3){count3=1;}
		index=(((int)text[i])-97);
		SDL_Rect srect;
		SDL_Rect drect;
		drect.w=4;
		drect.h=6;
		drect.x=x+(i*4);
		drect.y=y;
		if(index<15){
			srect.x=index*4;
			srect.y=6;
		}else{
			srect.x=(index-15)*4;
			srect.y=12;
		}
		srect.w=4;
		srect.h=6;
		SDL_RenderCopy(renderer, font[count3], &srect, &drect);
	}
}

void draw_number(int x, int y, int num){
	int w, h, f;
	int c=0;
	int n=1;
	SDL_QueryTexture(font[0], NULL, NULL, &w, &h);
	SDL_Rect srect;
	SDL_Rect drect;
	drect.w=4;
	drect.h=6;
	if(num<0){
		n=-1;
	}
	num*=n;
	while(num){
		c+=4;
		f=(num%10);
		num/=10;
		srect.x=(f*4)+1;
		srect.y=0;
		srect.w=4;
		srect.h=6;
		drect.x=x-c;
		drect.y=y;
		SDL_RenderCopy(renderer, font[0], &srect, &drect);
	}
	if(n<0){
		c+=4;
		srect.x=41;
		srect.y=0;
		srect.w=4;
		srect.h=6;
		drect.x=x-c;
		drect.y=y;
		SDL_RenderCopy(renderer, font[0], &srect, &drect);
	}
}

void draw_intro(){
	if(count1<6){count1++;}else{if(count2<4){count2++;}}
	if(count2==4){count4++;}
	if(count4>5){count3=0;}
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, background, NULL, NULL);
	draw_text(29-((count1*4)), 26, "hidden", count1);
	draw_text(37, 26, "star", count2);
	if(count4<=5){
		draw_number(34, 26, count1);
		draw_number(38, 26, count2);
	}
}

 /* Init and start */
int setup(){
	if (SDL_Init(SDL_INIT_VIDEO) <0){
		 SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
		return 3;
	}

	if (SDL_CreateWindowAndRenderer(resX*scale, resY*scale, SDL_WINDOW_OPENGL, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
		return 3;
    }

    SDL_RenderSetLogicalSize(renderer, resX, resY);
	SDL_SetWindowTitle(window, "Hidden Star");
    atexit(SDL_Quit);

	srand(time(NULL));

	background = Load_img("sprites/stars/stars1_256.png");
	font[0] = Load_img("sprites/font_5x3_earth.png");
	font[1] = Load_img("sprites/font_5x3_krull.png");
	font[2] = Load_img("sprites/font_5x3_plink.png");
	font[3] = Load_img("sprites/font_5x3_sneeb.png");

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 255);
	SDL_RenderCopy(renderer, background, NULL, NULL);
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
int speed=0;

while(event.type != SDL_QUIT){
	SDL_GetMouseState(&mouseX, &mouseY);
	if(SDL_PollEvent(&event)<1){SDL_Delay(10);}else{
		switch (event.type) {
			case SDL_QUIT:
				exit(0);
				break;
		}
	}

	current_time=SDL_GetTicks();
	if((current_time-last_time) > 64){
		last_time=current_time;
		if(speed<2){
			speed++;
		}else{
			draw_intro();
			speed=0;
		}
		SDL_RenderPresent(renderer);
	}
}

return 0;
}
