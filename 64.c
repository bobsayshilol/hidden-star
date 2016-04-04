// gcc -o 64 64.c -O2 -lSDL2_image `sdl2-config --cflags --libs`

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "SDL.h" 
#include "SDL_image.h"

#define MASK0 0
#define MASK1 1
#define MASK2 2
#define NOFLIP 0
#define FLIPV 1
#define FLIPH 2
#define FLIPHV 3
#define ROT270 4
#define ROT90 5

int scale=4;
int resX=64;
int resY=64;
int player_move1=0;
int player_move2=0;

SDL_Event event;
SDL_Surface *screen;
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *ship1;
SDL_Texture *baddy1;
SDL_Texture *timerbar;
SDL_Texture *combat_hud;
SDL_Texture *combat_movetext;
SDL_Texture *combat_firetext;
SDL_Texture *combat_moveselect;
SDL_Texture *combat_fireselect;
SDL_Texture *combat_diag;
SDL_Texture *combat_card;
SDL_Texture *combat_mid;

SDL_Texture* Load_img(char *filename){
	SDL_Texture* texture = IMG_LoadTexture(renderer, filename);
    if (!texture){
	    printf("Load image error: %s, %s \n",filename, IMG_GetError());
		exit(0);
    }
    return texture;
}

void blit(SDL_Texture *tex, int x, int y, int mask, int mode){
	int w, h;
	SDL_RendererFlip flip;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	SDL_Rect rect;
	rect.x=x*scale;
	rect.y=y*scale;
	rect.w=w*scale;
	rect.h=h*scale;
	switch(mask){
		case MASK0:
			//no change
			break;
		case MASK1:
			SDL_SetTextureColorMod(tex, 74, 142, 255);
			break;
		case MASK2:
			SDL_SetTextureColorMod(tex, 255, 0, 0);
			break;
	}
	switch(mode){
		case NOFLIP:
		    SDL_RenderCopy(renderer, tex, NULL, &rect);
			break;
		case FLIPV:
			flip = SDL_FLIP_VERTICAL;
		    SDL_RenderCopyEx(renderer, tex, NULL, &rect, 0, 0, flip);
			break;
		case FLIPH:
			flip = SDL_FLIP_HORIZONTAL;
		    SDL_RenderCopyEx(renderer, tex, NULL, &rect, 0, 0, flip);
			break;
		case FLIPHV:
			flip = SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL;
		    SDL_RenderCopyEx(renderer, tex, NULL, &rect, 0, 0, flip);
			break;
		case ROT270:
		    SDL_RenderCopyEx(renderer, tex, NULL, &rect, 270, 0, 0);
			break;
		case ROT90:
		    SDL_RenderCopyEx(renderer, tex, NULL, &rect, 90, 0, 0);
			break;
	}	
}

void draw_action_button(int direction, int x, int y, int mask){
	switch(direction){
		case 1:
			blit(combat_diag, x, y, mask, FLIPV);
			break;
		case 2:
			blit(combat_card, x, y, mask, FLIPV);
			break;
		case 3:
			blit(combat_diag, x, y, mask, FLIPHV);
			break;
		case 4:
			blit(combat_card, x, y, mask, ROT270);
			break;
		case 5:
			blit(combat_mid, x, y, mask, NOFLIP);
			break;
		case 6:
			blit(combat_card, x, y, mask, ROT90);
			break;
		case 7:
			blit(combat_diag, x, y, mask, NOFLIP);
			break;
		case 8:
			blit(combat_card, x, y, mask, NOFLIP);
			break;
		case 9:
			blit(combat_diag, x, y, mask, FLIPH);
			break;
	}
}

void draw_action_buttons(int action){
	int offset=0;
	int mask=MASK1;
	if(action==2){
		offset=21;
		mask=MASK2;
	}
	draw_action_button(7, 13+offset, 29-offset, mask);
	draw_action_button(8, 21+offset, 29-offset, mask);
	draw_action_button(9, 29+offset, 29-offset, mask);
	draw_action_button(4, 13+offset, 37-offset, mask);
	draw_action_button(5, 21+offset, 37-offset, mask);
	draw_action_button(6, 29+offset, 37-offset, mask);
	draw_action_button(1, 13+offset, 45-offset, mask);
	draw_action_button(2, 21+offset, 45-offset, mask);
	draw_action_button(3, 29+offset, 45-offset, mask);
}

void draw_combat(int time_pos){
	SDL_RenderClear(renderer);
	blit(timerbar, time_pos, 0, MASK0, NOFLIP);
	blit(ship1, 16, 32, MASK0, NOFLIP);
	blit(baddy1, 37, 11, MASK0, NOFLIP);
	blit(combat_hud, 0, 55, MASK0, NOFLIP);
	blit(combat_movetext, 4, 57, MASK0, NOFLIP);
	blit(combat_firetext, 44, 57, MASK0, NOFLIP);
	if(player_move1>0){
		if(player_move1>9){
			draw_action_button(player_move1-9, 28, 57, MASK2);
		}else{
			draw_action_button(player_move1, 28, 57, MASK1);
		}
	}
	if(player_move2>0){
		if(player_move2>9){
			draw_action_button(player_move2-9, 34, 57, MASK2);
		}else{
			draw_action_button(player_move2, 34, 57, MASK1);
		}
	}
}

void show_fight(){
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
    SDL_RenderSetLogicalSize(renderer, resX*scale, resY*scale);
//	screen = SDL_GetWindowSurface(window);
	SDL_SetWindowTitle(window, "SixtyFour");
    atexit(SDL_Quit);

	ship1 = Load_img("sprites/ship1.png");
	baddy1 = Load_img("sprites/baddy1.png");
	timerbar = Load_img("sprites/timerbar.png");
	combat_hud = Load_img("sprites/combat_hud_bottom.png");
	combat_firetext = Load_img("sprites/combat_hud_fire.png");
	combat_movetext = Load_img("sprites/combat_hud_move.png");
	combat_moveselect = Load_img("sprites/combat_hud_move_highlight.png");
	combat_fireselect = Load_img("sprites/combat_hud_fire_highlight.png");
	combat_diag = Load_img("sprites/directional_diag.png");
	combat_card = Load_img("sprites/directional_card.png");
	combat_mid = Load_img("sprites/directional_mid.png");

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
	draw_combat(0);
	SDL_RenderPresent(renderer);
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
int action=0;

while(event.type != SDL_QUIT){
	SDL_GetMouseState(&mouseX, &mouseY);
	if(SDL_PollEvent(&event)<1){SDL_Delay(10);}else{
		switch (event.type) {
			case SDL_QUIT:
				exit(0);
			case SDL_MOUSEBUTTONDOWN:
				if(player_move2==0 && mouseX < (25*scale) 
								   && mouseY > (56*scale)){
					//movebutt on
					draw_combat(time_pos);
						blit(combat_moveselect, 0, 55, MASK0, NOFLIP);
						action=1;
						draw_action_buttons(action);
				}
				if(player_move2==0 && mouseX > (42*scale) 
								   && mouseY > (56*scale)){
					//firebutt on
					draw_combat(time_pos);
						blit(combat_fireselect, 40, 55, MASK0, NOFLIP);
						action=2;
						draw_action_buttons(action);
				}
				if(mouseX > (12*scale) && mouseX < (35*scale) &&
				   mouseY > (28*scale) && mouseY < (51*scale)){
					if(action==1){
						int xr=round(((mouseX/scale)-12)/8);
						int yr=round(((mouseY/scale)-28)/8);
						int direction=0;
						if(xr==0 && yr==0){direction=7;}
						if(xr==1 && yr==0){direction=8;}
						if(xr==2 && yr==0){direction=9;}
						if(xr==0 && yr==1){direction=4;}
						if(xr==1 && yr==1){direction=0;} //move nowhere?
						if(xr==2 && yr==1){direction=6;}
						if(xr==0 && yr==2){direction=1;}
						if(xr==1 && yr==2){direction=2;}
						if(xr==2 && yr==2){direction=3;}
						if(direction>0 && player_move1>0 && player_move2==0){
							player_move2=direction;
							action=0;
							draw_combat(time_pos);
						}
						if(direction>0 && player_move1==0){
							player_move1=direction;
							action=0;
							draw_combat(time_pos);
						}
					}else if(action==0 && player_move2==0){
						//movebutt on
						draw_combat(time_pos);
							blit(combat_moveselect, 0, 55, MASK0, NOFLIP);
							action=1;
							draw_action_buttons(action);
					}
				}
				if(mouseX > (33*scale) && mouseX < (56*scale) &&
				   mouseY > (7*scale) && mouseY < (30*scale)){
					if(action==2){
						int xr=round(((mouseX/scale)-33)/8);
						int yr=round(((mouseY/scale)-7)/8);
						int direction=0;
						if(xr==0 && yr==0){direction=7;}
						if(xr==1 && yr==0){direction=8;}
						if(xr==2 && yr==0){direction=9;}
						if(xr==0 && yr==1){direction=4;}
						if(xr==1 && yr==1){direction=5;}
						if(xr==2 && yr==1){direction=6;}
						if(xr==0 && yr==2){direction=1;}
						if(xr==1 && yr==2){direction=2;}
						if(xr==2 && yr==2){direction=3;}
						if(direction>0 && player_move1>0 && player_move2==0){
							player_move2=direction+9;
							action=0;
							draw_combat(time_pos);
						}
						if(direction>0 && player_move1==0){
							player_move1=direction+9;
							action=0;
							draw_combat(time_pos);
						}
					}else if(action==0 && player_move2==0){
						//firebutt on
						draw_combat(time_pos);
							blit(combat_fireselect, 40, 55, MASK0, NOFLIP);
							action=2;
							draw_action_buttons(action);
					}
			}
			
		}
	}

	//round timer
	current_time=SDL_GetTicks();
	if((current_time-last_time) > 64){
		last_time=current_time;
		time_pos--;
		if(time_pos<-63){
			time_pos=0;
			show_fight();
			action=0;
			player_move1=0;
			player_move2=0;
			draw_combat(time_pos);
		}
		blit(timerbar, time_pos, 0, MASK0, NOFLIP);
	}
	SDL_RenderPresent(renderer);
	
}
return 0;
}
