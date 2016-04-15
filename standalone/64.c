// gcc -std=c99 -o 64 64.c -O2 -lSDL2_image `sdl2-config --cflags --libs` -lm
// TODO tab key, thrusters on movement

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include "SDL.h" 
#include "SDL_image.h"

#define MASK0 0
#define MASK1 1
#define MASK2 2
#define MASK3 3
#define NOFLIP 0
#define FLIPV 1
#define FLIPH 2
#define FLIPHV 3
#define TWINK1 10
#define TWINK2 11
#define TWINK3 12
#define TWINK4 13
#define TWINK5 14
#define TWINK6 15

int scale=8;
int resX=64;
int resY=64;
int player_move1=0;
int player_move2=0;
int playerHP=50;
int player_dead=0;
int shipX=16;
int shipY=32;
int shot1X=26;
int shot1Y=34;
int combat_weapon=0;
int ship_thruster=0;
int starX[6];
int starY[6];
int action=0;
int baddy1AI[8]={8,15,6,15,2,15,4,15};
int AIshipX=37;
int AIshipY=11;
int AIshot1X=40;
int AIshot1Y=20;
int baddy1HP=50;
int baddy1_dead=0;
int combat_weaponAI=0;
int baddy_thruster=0;
int ai_round=0;
int aniframe1=0;

SDL_Event event;
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *stars1;
SDL_Texture *stars2;
SDL_Texture *stars3;
SDL_Texture *stars4;
SDL_Texture *stars5;
SDL_Texture *stars6;
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
SDL_Texture *combat_card_right;
SDL_Texture *combat_mid;
SDL_Texture *shot1;
SDL_Texture *shot2;
SDL_Texture *shot3;
SDL_Texture *explosion1;
SDL_Texture *explosion2;
SDL_Texture *miss1;
SDL_Texture *font1;

SDL_Texture* Load_img(char *filename){
	SDL_Texture* texture = IMG_LoadTexture(renderer, filename);
    if (!texture){
	    printf("Load image error: %s, %s \n",filename, IMG_GetError());
		exit(0);
    }
    return texture;
}

void draw_number(SDL_Texture *tex, int x, int y, int num){
	int w, h, f;
	int c=0;
	int n=1;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
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
		SDL_RenderCopy(renderer, tex, &srect, &drect);
	}
	if(n<0){
		c+=4;
		srect.x=41;
		srect.y=0;
		srect.w=4;
		srect.h=6;
		drect.x=x-c;
		drect.y=y;
		SDL_RenderCopy(renderer, tex, &srect, &drect);
	}
}

void animate(SDL_Texture *tex, int x, int y, int frame){
	int w, h;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	SDL_Rect srect;
	SDL_Rect drect;
	drect.x=x;
	drect.y=y;
	drect.h=h;
	srect.x=(h*frame);
	srect.y=0;
	srect.w=h;
	srect.h=h;
	drect.w=h;
	SDL_RenderCopy(renderer, tex, &srect, &drect);
}

void blit(SDL_Texture *tex, int x, int y, int mask, int mode){
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
			SDL_SetTextureColorMod(tex, 255, 255, 255);
			break;
		case MASK1:
			SDL_SetTextureColorMod(tex, 74, 142, 255);
			break;
		case MASK2:
			SDL_SetTextureColorMod(tex, 255, 0, 0);
			break;
		case MASK3:
			SDL_SetTextureColorMod(tex, 235, 255, 0);
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
		case TWINK1:
			if((rand()%64)==0){
				r=((rand() % 256) + 128);
			}else{
				r=((rand() % 8) + 32);
			}
			SDL_SetTextureAlphaMod(tex, r);
		    SDL_RenderCopyEx(renderer, tex, NULL, &rect, 90, 0, 0);
			break;
		case TWINK2:
			if((rand()%32)==0){
				r=((rand() % 256) + 128);
			}else{
				r=((rand() % 16) + 48);
			}
			SDL_SetTextureAlphaMod(tex, r);
		    SDL_RenderCopyEx(renderer, tex, NULL, &rect, 90, 0, 0);
			break;
		case TWINK3:
			if((rand()%32)==0){
				r=((rand() % 256) + 128);
			}else{
				r=((rand() % 24) + 64);
			}
			SDL_SetTextureAlphaMod(tex, r);
		    SDL_RenderCopyEx(renderer, tex, NULL, &rect, 90, 0, 0);
			break;
		case TWINK4:
			if((rand()%48)==0){
				r=((rand() % 256) + 128);
			}else{
				r=((rand() % 32) + 80);
			}
			SDL_SetTextureAlphaMod(tex, r);
		    SDL_RenderCopyEx(renderer, tex, NULL, &rect, 90, 0, 0);
			break;
		case TWINK5:
			if((rand()%56)==0){
				r=((rand() % 256) + 128);
			}else{
				r=((rand() % 40) + 96);
			}
			SDL_SetTextureAlphaMod(tex, r);
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
			blit(combat_card_right, x, y, mask, FLIPH);
			break;
		case 5:
			blit(combat_mid, x, y, mask, NOFLIP);
			break;
		case 6:
			blit(combat_card_right, x, y, mask, NOFLIP);
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

void draw_action_buttons(){
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

void draw_thruster(int x, int y, int thruster){
		if(thruster==7){blit(shot2, x+11, y+9, MASK3, FLIPH);}
		if(thruster==8){blit(shot1, x+6, y+8, MASK3, NOFLIP);}
		if(thruster==9){blit(shot2, x, y+9, MASK3, NOFLIP);}
		if(thruster==4){blit(shot3, x+11, y+6, MASK3, FLIPH);}
		if(thruster==6){blit(shot3, x, y+6, MASK3, NOFLIP);}
		if(thruster==1){blit(shot2, x+10, y+3, MASK3, FLIPHV);}
		if(thruster==2){blit(shot1, x+6, y+2, MASK3, FLIPV);}
		if(thruster==3){blit(shot2, x+1, y+3, MASK3, FLIPV);}
}

void draw_combat(int time_pos){
	int bob1;
	int bob2;
	SDL_Rect hprect;
	SDL_RenderClear(renderer);
	for(int x=-256;x<=256;x+=256){
		for(int y=-256;y<=256;y+=256){
			blit(stars1, starX[0]+x, starY[0]+y, MASK0, NOFLIP);
			blit(stars2, starX[1]+x, starY[1]+y, MASK0, TWINK1);
			blit(stars3, starX[2]+x, starY[2]+y, MASK0, TWINK2);
			blit(stars4, starX[3]+x, starY[3]+y, MASK0, TWINK3);
			blit(stars5, starX[4]+x, starY[4]+y, MASK0, TWINK4);
			blit(stars6, starX[5]+x, starY[5]+y, MASK0, TWINK5);
		}
	}
	if(time_pos <= -48){bob1=1;}
	if(time_pos > -48 && time_pos <= -32){bob1=0;}
	if(time_pos > -32 && time_pos <= -16){bob1=-1;}
	if(time_pos > -16){bob1=0;}
	if(time_pos <= -56){bob2=1;}
	if(time_pos > -56 && time_pos <= -48){bob2=0;}
	if(time_pos > -48 && time_pos <= -32){bob2=-1;}
	if(time_pos > -32 && time_pos <= -24){bob2=0;}
	if(time_pos > -24 && time_pos <= -16){bob2=1;}
	if(time_pos > -16 && time_pos <= -8){bob2=1;}
	if(time_pos > -8){bob2=0;}

	if(ship_thruster>0){
		draw_thruster(shipX,shipY,ship_thruster);
	}
	if(baddy_thruster>0){
		draw_thruster(AIshipX,AIshipY,baddy_thruster);
	}

	if(player_dead<3){
		blit(ship1, shipX, shipY+bob1, MASK0, NOFLIP);
	}
	if(baddy1_dead<3){
		blit(baddy1, AIshipX, AIshipY+bob2, MASK0, NOFLIP);
	}

	blit(timerbar, time_pos, 0, MASK0, NOFLIP);
	blit(combat_hud, 0, 55, MASK0, NOFLIP);
	blit(combat_movetext, 4, 57, MASK0, NOFLIP);
	blit(combat_firetext, 44, 57, MASK0, NOFLIP);

	if(player_dead==0){
		SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255);
		hprect.x = shipX;
		hprect.y = (shipY+bob1);
		hprect.w = 16;
		hprect.h = 1;
		SDL_RenderFillRect(renderer, &hprect);
		SDL_SetRenderDrawColor(renderer, 0, 224, 0, 255);
		hprect.x = shipX;
		hprect.y = (shipY+bob1);
		hprect.w = (round(playerHP*16/100));
		hprect.h = 1;
		SDL_RenderFillRect(renderer, &hprect);
	}
	
	if(baddy1_dead==0){
		SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255);
		hprect.x = AIshipX;
		hprect.y = (AIshipY+bob2);
		hprect.w = 16;
		hprect.h = 1;
		SDL_RenderFillRect(renderer, &hprect);
		SDL_SetRenderDrawColor(renderer, 224, 0, 0, 255);
		hprect.x = AIshipX;
		hprect.y = (AIshipY+bob2);
		hprect.w = (round(baddy1HP*16/100));
		hprect.h = 1;
		SDL_RenderFillRect(renderer, &hprect);
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

	if(combat_weapon==1){
		blit(shot2, shot1X, shot1Y, MASK2, NOFLIP);
	}
	if(combat_weapon==10 && baddy1_dead<2){
		animate(explosion1, shot1X-2, shot1Y-2, aniframe1);
		draw_number(font1, AIshipX-5,AIshipY+3, -10);
	}
	if(combat_weapon==20 && baddy1_dead<2){
		animate(miss1, shot1X-2, shot1Y-2, aniframe1);
		draw_number(font1, AIshipX-5,AIshipY+3, 0);
	}
	if(combat_weaponAI==1){
		blit(shot2, AIshot1X, AIshot1Y, MASK1, FLIPHV);
	}
	if(combat_weaponAI==10 && player_dead<2){
		animate(explosion1, AIshot1X-2, AIshot1Y-1, aniframe1);
		draw_number(font1, shipX+28,shipY+4, -10);
	}
	if(combat_weaponAI==20 && player_dead<2){
		animate(miss1, AIshot1X-2, AIshot1Y-1, aniframe1);
		draw_number(font1, shipX+28,shipY+4, 0);
	}

	if(player_dead > 1 && player_dead < 4){
		animate(explosion2, AIshot1X-14, AIshot1Y-14, aniframe1);
	}
	if(baddy1_dead > 1 && baddy1_dead < 4){
		animate(explosion2, shot1X-12, shot1Y-16, aniframe1);
	}

	if(player_move1>0){
		if(player_move1>10){
			draw_action_button(player_move1-10, 28, 57, MASK2);
		}else{
			draw_action_button(player_move1, 28, 57, MASK1);
		}
	}
	if(player_move2>0){
		if(player_move2>10){
			draw_action_button(player_move2-10, 34, 57, MASK2);
		}else{
			draw_action_button(player_move2, 34, 57, MASK1);
		}
	}
	if(action==1){
		blit(combat_moveselect, 0, 55, MASK0, NOFLIP);
		draw_action_buttons();
	}
	if(action==2){
		blit(combat_fireselect, 40, 55, MASK0, NOFLIP);
		draw_action_buttons();
	}
}

void move_stars(int xm, int ym, int d){
	int z;
	for(int j=0;j<6;j++){
		if(d==0){
			z=j;
		}else{
			z=d-j;
		}
		starX[j]=starX[j]+(z*xm);
		starY[j]=starY[j]+(z*ym);
	}
}

void show_fight(){
	int s=0;
	int s2=0;
	int back=0;
	int baddy_back=0;
	for(int h=0;h < 4; h++){ // 4 movements per round
		if(player_dead>0 || baddy1_dead>0){break;}
		if(h==0){
			s=player_move1;
			s2=baddy1AI[ai_round];
		}
		if(h==1){
			s=back;
			s2=baddy_back;
			SDL_Delay(100);
		}
		if(h==2){
			s=player_move2; 
			back=0;
			baddy_back=0;
			s2=baddy1AI[ai_round];
		}
		if(h==3){
			s=back;
			s2=baddy_back;
			SDL_Delay(100);
		}
		for(int i=0;i < 8; i++){ // 8 frames per movement
			if(s>0){
				switch(s){
					case 7:
						shipX--;
						shipY--;
						if(back==s){
							move_stars(1,1,6);
						}else{
							ship_thruster=7;
							back=3;
						}
						break;
					case 8:
						shipY--;
						if(back==s){
							move_stars(0,1,6);
						}else{
							ship_thruster=8;
							back=2;
						}
						break;
					case 9:
						shipX++;
						shipY--;
						if(back==s){
							move_stars(-1,1,6);
						}else{
							ship_thruster=9;
							back=1;
						}
						break;
					case 4:
						shipX--;
						if(back==s){
							move_stars(1,0,6);
						}else{
							ship_thruster=4;
							back=6;
						}
						break;
					case 6:
						shipX++;
						if(back==s){
							move_stars(-1,0,6);
						}else{
							ship_thruster=6;
							back=4;
						}
						break;
					case 1:
						shipX--;
						shipY++;
						if(back==s){
							move_stars(1,-1,6);
						}else{
							ship_thruster=1;
							back=9;
						}
						break;
					case 2:
						shipY++;
						if(back==s){
							move_stars(0,-1,6);
						}else{
							ship_thruster=2;
							back=8;
						}
						break;
					case 3:
						shipX++;
						shipY++;
						if(back==s){
							move_stars(-1,-1,6);
						}else{
							ship_thruster=3;
							back=7;
						}
						break;
					case 17:
						combat_weapon=1;
						shot1X+=1;
						shot1Y-=3;
						break;
					case 18:
						combat_weapon=1;
						shot1X+=2;
						shot1Y-=3;
						break;
					case 19:
						combat_weapon=1;
						shot1X+=3;
						shot1Y-=3;
						break;
					case 14:
						combat_weapon=1;
						shot1X+=1;
						shot1Y-=2;
						break;
					case 15:
						combat_weapon=1;
						shot1X+=2;
						shot1Y-=2;
						break;
					case 16:
						combat_weapon=1;
						shot1X+=3;
						shot1Y-=2;
						break;
					case 11:
						combat_weapon=1;
						shot1X+=1;
						shot1Y-=1;
						break;
					case 12:
						combat_weapon=1;
						shot1X+=2;
						shot1Y-=1;
						break;
					case 13:
						combat_weapon=1;
						shot1X+=3;
						shot1Y-=1;
						break;
				}
			}
			switch(s2){
				case 7:
					AIshipX--;
					AIshipY--;
					if(baddy_back==s2){
						move_stars(1,1,0);
					}else{
						baddy_thruster=7;
						baddy_back=3;
					}
					break;
				case 8:
					AIshipY--;
					if(baddy_back==s2){
						move_stars(0,1,0);
					}else{
						baddy_thruster=8;
						baddy_back=2;
					}
					break;
				case 9:
					AIshipX++;
					AIshipY--;
					if(baddy_back==s2){
						move_stars(-1,1,0);
					}else{
						baddy_thruster=9;
						baddy_back=1;
					}
					break;
				case 4:
					AIshipX--;
					if(baddy_back==s2){
						move_stars(1,0,0);
					}else{
						baddy_thruster=4;
						baddy_back=6;
					}
					break;
				case 6:
					AIshipX++;
					if(baddy_back==s2){
						move_stars(-1,0,0);
					}else{
						baddy_thruster=6;
						baddy_back=4;
					}
					break;
				case 1:
					AIshipX--;
					AIshipY++;
					if(baddy_back==s2){
						move_stars(1,-1,0);
					}else{
						baddy_thruster=1;
						baddy_back=9;
					}
					break;
				case 2:
					AIshipY++;
					if(baddy_back==s2){
						move_stars(0,-1,0);
					}else{
						baddy_thruster=2;
						baddy_back=8;
					}
					break;
				case 3:
					AIshipX++;
					AIshipY++;
					if(baddy_back==s2){
						move_stars(-1,-1,0);
					}else{
						baddy_thruster=3;
						baddy_back=7;
					}
					break;
				case 17:
					combat_weaponAI=1;
					AIshot1X-=3;
					AIshot1Y+=1;
					break;
				case 18:
					combat_weaponAI=1;
					AIshot1X-=2;
					AIshot1Y+=1;
					break;
				case 19:
					combat_weaponAI=1;
					AIshot1X-=1;
					AIshot1Y+=1;
					break;
				case 14:
					combat_weaponAI=1;
					AIshot1X-=3;
					AIshot1Y+=2;
					break;
				case 15:
					combat_weaponAI=1;
					AIshot1X-=2;
					AIshot1Y+=2;
					break;
				case 16:
					combat_weaponAI=1;
					AIshot1X-=1;
					AIshot1Y+=2;
					break;
				case 11:
					combat_weaponAI=1;
					AIshot1X-=3;
					AIshot1Y+=3;
					break;
				case 12:
					combat_weaponAI=1;
					AIshot1X-=2;
					AIshot1Y+=3;
					break;
				case 13:
					combat_weaponAI=1;
					AIshot1X-=1;
					AIshot1Y+=3;
					break;
			}
			for(int j=0;j<6;j++){
					if(starX[j] < -256){starX[j]=256;}
					if(starX[j] > 256){starX[j]=-256;}
					if(starY[j] < -256){starY[j]=256;}
					if(starY[j] > 256){starY[j]=-256;}
			}
			if(i==3 || i==5 || i==7){ship_thruster=0;}
			if(i==2 || i==4 || i==7){baddy_thruster=0;}

			draw_combat(0);
			SDL_RenderPresent(renderer);
			SDL_Delay(32);
		}
		if(h==0 || h==2){
			if(s==baddy1AI[ai_round]+10 || 
			  (s==15 && baddy1AI[ai_round] > 10)){
				combat_weapon=10;
				baddy1HP-=10;
				if(baddy1HP < 1 ){baddy1_dead=1;}
			}else{
				if(combat_weapon==1){
					combat_weapon=20;
				}else{
					combat_weapon=0;
				}
			}
			if(s+10==baddy1AI[ai_round] || 
			  (s>10 && baddy1AI[ai_round]==15)){
				combat_weaponAI=10;
				playerHP-=10;
				if(playerHP < 1 ){player_dead=1;}
			}else{
				if(combat_weaponAI==1){
					combat_weaponAI=20;
				}else{
					combat_weaponAI=0;
				}
			}
			// extra render for hit explosion
			if(combat_weapon>=10 || combat_weaponAI>=10){
				for(int i=0;i<6;i++){
					draw_combat(0);
					SDL_RenderPresent(renderer);	
					SDL_Delay(32);
					aniframe1++;
				}
				aniframe1=0;
			}
			//extra extra render for death explosion
			if(player_dead>0 || baddy1_dead>0){
				if(player_dead==1){player_dead=2;}
				if(baddy1_dead==1){baddy1_dead=2;}
				for(int i=0;i<6;i++){
					if(player_dead==2 && i==2){player_dead=3;}
					if(baddy1_dead==2 && i==2){baddy1_dead=3;}
					draw_combat(0);
					SDL_RenderPresent(renderer);	
					SDL_Delay(32);
					aniframe1++;
				}
				aniframe1=0;
				if(player_dead==3){player_dead=4;}
				if(baddy1_dead==3){baddy1_dead=4;}
			}
			ai_round++;
			if(ai_round>7){ai_round=0;}
		}
		combat_weapon=0;
		combat_weaponAI=0;
		ship_thruster=0;
		baddy_thruster=0;
		AIshot1X=40;
		AIshot1Y=20;
		shot1X=26;
		shot1Y=34;
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

//	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, resX, resY);
	SDL_SetWindowTitle(window, "SixtyFour");
    atexit(SDL_Quit);

	srand(time(NULL));

	stars1 = Load_img("sprites/stars/stars1_256.png");
	stars2 = Load_img("sprites/stars/stars2_256.png");
	stars3 = Load_img("sprites/stars/stars3_256.png");
	stars4 = Load_img("sprites/stars/stars4_256.png");
	stars5 = Load_img("sprites/stars/stars5_256.png");
	stars6 = Load_img("sprites/stars/stars6_256.png");
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
	combat_card_right = Load_img("sprites/directional_card_right.png");
	combat_mid = Load_img("sprites/directional_mid.png");
	shot1 = Load_img("sprites/projectile1.png");
	shot2 = Load_img("sprites/projectile1_diag.png");
	shot3 = Load_img("sprites/projectile1_right.png");
	explosion1 = Load_img("sprites/explosion1.png");
	explosion2 = Load_img("sprites/explosion1_big.png");
	font1= Load_img("sprites/font_5x3_earth.png");
	miss1= Load_img("sprites/miss1.png");

	for(int i=0;i<6;i++){
			starX[i]=-64;
			starY[i]=-64;
	}
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 255);
	draw_combat(0);
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
int ani_time = SDL_GetTicks();
int mouseX; int mouseY;
int direction=0;

while(event.type != SDL_QUIT){
	draw_combat(time_pos);
	SDL_GetMouseState(&mouseX, &mouseY);
	if(SDL_PollEvent(&event)<1){SDL_Delay(10);}else{
		switch (event.type) {
			case SDL_QUIT:
				exit(0);
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
					case SDLK_KP_0:
					case SDLK_LSHIFT:
						action=1;
						break;
					case SDLK_KP_ENTER:
					case SDLK_SPACE:
						action=2;
						break;
					case SDLK_KP_7:
					case SDLK_q:
						direction=7;
						break;
					case SDLK_KP_8:
					case SDLK_w:
						direction=8;
						break;
					case SDLK_KP_9:
					case SDLK_e:
						direction=9;
						break;
					case SDLK_KP_4:
					case SDLK_a:
						direction=4;
						break;
					case SDLK_KP_5:
					case SDLK_s:
						direction=5;
						break;
					case SDLK_KP_6:
					case SDLK_d:
						direction=6;
						break;
					case SDLK_KP_1:
					case SDLK_z:
						direction=1;
						break;
					case SDLK_KP_2:
					case SDLK_x:
						direction=2;
						break;
					case SDLK_KP_3:
					case SDLK_c:
						direction=3;
						break;
				}
				if(direction>0 && action==2){direction+=10;}
				if(direction>0){action=0;}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if(player_move2==0 && mouseX < (25*scale) 
								   && mouseY > (56*scale)){
					//movebutt on
						action=1;
				}
				if(player_move2==0 && mouseX > (42*scale) 
								   && mouseY > (56*scale)){
					//firebutt on
						action=2;
				}
				if(mouseX > (12*scale) && mouseX < (35*scale) &&
				   mouseY > (28*scale) && mouseY < (51*scale)){
					if(action==1){
						int xr=round(((mouseX/scale)-12)/8);
						int yr=round(((mouseY/scale)-28)/8);
						if(xr==0 && yr==0){direction=7;}
						if(xr==1 && yr==0){direction=8;}
						if(xr==2 && yr==0){direction=9;}
						if(xr==0 && yr==1){direction=4;}
						if(xr==1 && yr==1){direction=5;}
						if(xr==2 && yr==1){direction=6;}
						if(xr==0 && yr==2){direction=1;}
						if(xr==1 && yr==2){direction=2;}
						if(xr==2 && yr==2){direction=3;}
					}else if(action!=1 && player_move2==0){
						//movebutt on
							action=1;
					}
				}
				if(mouseX > (33*scale) && mouseX < (56*scale) &&
				   mouseY > (7*scale) && mouseY < (30*scale)){
					if(action==2){
						int xr=round(((mouseX/scale)-33)/8);
						int yr=round(((mouseY/scale)-7)/8);
						if(xr==0 && yr==0){direction=17;}
						if(xr==1 && yr==0){direction=18;}
						if(xr==2 && yr==0){direction=19;}
						if(xr==0 && yr==1){direction=14;}
						if(xr==1 && yr==1){direction=15;}
						if(xr==2 && yr==1){direction=16;}
						if(xr==0 && yr==2){direction=11;}
						if(xr==1 && yr==2){direction=12;}
						if(xr==2 && yr==2){direction=13;}
					}else if(action!=2 && player_move2==0){
						//firebutt on
							action=2;
					}
				break;
			}
		}
		if(player_dead==0 && baddy1_dead ==0){
			if(direction>0 && player_move1>0 && player_move2==0){
				player_move2=direction;
				action=0;
			}
			if(direction>0 && player_move1==0){
				player_move1=direction;
				action=0;
			}
		}else{
			action=0;
		}
		direction=0;	
	}

	//round timer
	current_time=SDL_GetTicks();
	if((current_time-last_time) > 64){
		last_time=current_time;
		time_pos--;
		if(time_pos < -63){
			if(player_move1==0){player_move1=5;}
			if(player_move2==0){player_move2=5;}
			time_pos=0;
			show_fight();
			action=0;
			player_move1=0;
			player_move2=0;
			draw_combat(time_pos);
		}
		blit(timerbar, time_pos, 0, MASK0, NOFLIP);
	}
	current_time=SDL_GetTicks();
	if((current_time-ani_time) >= 60){
		ani_time=current_time;
		SDL_RenderPresent(renderer);
	}
}
return 0;
}
