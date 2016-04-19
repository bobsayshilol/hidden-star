// TODO tab key
#include "combat.h"
#include "travel.h"

void combat_animate(SDL_Texture *tex, int x, int y, int frame){
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
	SDL_RenderCopy(main_renderer, tex, &srect, &drect);
}

void combat_set_faction(int f)
{
	if (f == TRAVEL_FACTION_SNEEB)
	{
		c_baddy1 = Load_tex("sprites/ships/tri2_front.png");
	}
	else if (f == TRAVEL_FACTION_KRULL)
	{
		c_baddy1 = Load_tex("sprites/ships/tri3_front.png");
	}
	else if (f == TRAVEL_FACTION_PLINK)
	{
		c_baddy1 = Load_tex("sprites/ships/ring1_front.png");
	}
}

void combat_draw_action_button(int direction, int x, int y, int mask){
	switch(direction){
		case 1:
			main_blit(c_diag, x, y, FLIPV, combat_get_color(mask));
			break;
		case 2:
			main_blit(c_card, x, y, FLIPV, combat_get_color(mask));
			break;
		case 3:
			main_blit(c_diag, x, y, FLIPHV, combat_get_color(mask));
			break;
		case 4:
			main_blit(c_card_right, x, y, FLIPH, combat_get_color(mask));
			break;
		case 5:
			main_blit(c_mid, x, y, NOFLIP, combat_get_color(mask));
			break;
		case 6:
			main_blit(c_card_right, x, y, NOFLIP, combat_get_color(mask));
			break;
		case 7:
			main_blit(c_diag, x, y, NOFLIP, combat_get_color(mask));
			break;
		case 8:
			main_blit(c_card, x, y, NOFLIP, combat_get_color(mask));
			break;
		case 9:
			main_blit(c_diag, x, y, FLIPH, combat_get_color(mask));
			break;
	}
}

void combat_draw_action_buttons(){
	int offset=0;
	int mask=1;
	if(c_action==2){
		offset=21;
		mask=2;
	}
	combat_draw_action_button(7, 13+offset, 29-offset, mask);
	combat_draw_action_button(8, 21+offset, 29-offset, mask);
	combat_draw_action_button(9, 29+offset, 29-offset, mask);
	combat_draw_action_button(4, 13+offset, 37-offset, mask);
	combat_draw_action_button(5, 21+offset, 37-offset, mask);
	combat_draw_action_button(6, 29+offset, 37-offset, mask);
	combat_draw_action_button(1, 13+offset, 45-offset, mask);
	combat_draw_action_button(2, 21+offset, 45-offset, mask);
	combat_draw_action_button(3, 29+offset, 45-offset, mask);
}

void combat_draw_thruster(int x, int y, int thruster){
		if(thruster==7){main_blit(c_shot2, x+11, y+9, FLIPH, combat_get_color(3));}
		if(thruster==8){main_blit(c_shot1, x+6, y+8, NOFLIP, combat_get_color(3));}
		if(thruster==9){main_blit(c_shot2, x, y+9, NOFLIP, combat_get_color(3));}
		if(thruster==4){main_blit(c_shot3, x+11, y+6, FLIPH, combat_get_color(3));}
		if(thruster==6){main_blit(c_shot3, x, y+6, NOFLIP, combat_get_color(3));}
		if(thruster==1){main_blit(c_shot2, x+10, y+3, FLIPHV, combat_get_color(3));}
		if(thruster==2){main_blit(c_shot1, x+6, y+2, FLIPV, combat_get_color(3));}
		if(thruster==3){main_blit(c_shot2, x+1, y+3, FLIPV, combat_get_color(3));}
}

SDL_Color* combat_get_color(int index){
	switch(index){
		case 0:
			return NULL;
			break;
		case 1: 
			c_color.r=74; c_color.g=142; c_color.b=255;
			return &c_color;
			break;
		case 2:
			c_color.r=255; c_color.g=0; c_color.b=0;
			return &c_color;
			break;
		case 3:
			c_color.r=235; c_color.g=255; c_color.b=0;
			return &c_color;
			break;
	}
	return NULL;
}

void combat_draw(){
	c_time_pos--;
	if(c_time_pos < -63){
		if(c_player_move1==0){c_player_move1=5;}
		if(c_player_move2==0){c_player_move2=5;}
		c_time_pos=0;
		combat_show_fight();
		c_action=0;
		c_player_move1=0;
		c_player_move2=0;
	}
	combat_show(c_time_pos);
	if (c_AI_dead>0)
	{
		//todo: handle reputation implications of the fight
		travel_setup();
	}
}

void combat_show(int time_pos){
	int bob1;
	int bob2;
	SDL_Rect hprect;
	SDL_RenderClear(main_renderer);

	for(int x=-256;x<=256;x+=256){
		for(int y=-256;y<=256;y+=256){
			main_blit(c_stars1, c_starX[0]+x, c_starY[0]+y, NOFLIP, NULL);
			main_blit(c_stars2, c_starX[1]+x, c_starY[1]+y, TWINK1, NULL);
			main_blit(c_stars3, c_starX[2]+x, c_starY[2]+y, TWINK2, NULL);
			main_blit(c_stars4, c_starX[3]+x, c_starY[3]+y, TWINK3, NULL);
			main_blit(c_stars5, c_starX[4]+x, c_starY[4]+y, TWINK4, NULL);
			main_blit(c_stars6, c_starX[5]+x, c_starY[5]+y, TWINK5, NULL);
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

	if(c_player_thruster>0){
		combat_draw_thruster(c_player_shipX,c_player_shipY,c_player_thruster);
	}
	if(c_AI_thruster>0){
		combat_draw_thruster(c_AI_shipX,c_AI_shipY,c_AI_thruster);
	}

	if(c_player_dead<3){
		main_blit(c_ship1, c_player_shipX, c_player_shipY+bob1, NOFLIP, NULL);
	}
	if(c_AI_dead<3){
		main_blit(c_baddy1, c_AI_shipX, c_AI_shipY+bob2, NOFLIP, NULL);
	}
	
	main_blit(c_timerbar, time_pos, 0, NOFLIP, NULL);
	main_blit(c_hud, 0, 55, NOFLIP, NULL);
	main_blit(c_movetext, 4, 57, NOFLIP, NULL);
	main_blit(c_firetext, 44, 57, NOFLIP, NULL);

	if(c_player_dead==0){
		SDL_SetRenderDrawColor(main_renderer, 0, 100, 0, 255);
		hprect.x = c_player_shipX;
		hprect.y = (c_player_shipY+bob1);
		hprect.w = 16;
		hprect.h = 1;
		SDL_RenderFillRect(main_renderer, &hprect);
		SDL_SetRenderDrawColor(main_renderer, 0, 224, 0, 255);
		hprect.x = c_player_shipX;
		hprect.y = (c_player_shipY+bob1);
		hprect.w = (round(c_player_HP*16/100));
		hprect.h = 1;
		SDL_RenderFillRect(main_renderer, &hprect);
	}
	
	if(c_AI_dead==0){
		SDL_SetRenderDrawColor(main_renderer, 100, 0, 0, 255);
		hprect.x = c_AI_shipX;
		hprect.y = (c_AI_shipY+bob2);
		hprect.w = 16;
		hprect.h = 1;
		SDL_RenderFillRect(main_renderer, &hprect);
		SDL_SetRenderDrawColor(main_renderer, 224, 0, 0, 255);
		hprect.x = c_AI_shipX;
		hprect.y = (c_AI_shipY+bob2);
		hprect.w = (round(c_AI_HP*16/100));
		hprect.h = 1;
		SDL_RenderFillRect(main_renderer, &hprect);
	}

	SDL_SetRenderDrawColor(main_renderer, 0, 0, 0, 0);

	if(c_player_weapon==1){
		main_blit(c_shot2, c_player_shot1X, c_player_shot1Y, NOFLIP, combat_get_color(2));
	}
	if(c_player_weapon==10 && c_AI_dead<2){
		combat_animate(c_explosion1, c_player_shot1X-2, c_player_shot1Y-2, c_aniframe1);
		draw_number(c_AI_shipX-12,c_AI_shipY+4, -10);
	}
	if(c_player_weapon==20 && c_AI_dead<2){
		combat_animate(c_miss1, c_player_shot1X-2, c_player_shot1Y-2, c_aniframe1);
		draw_number(c_AI_shipX-12,c_AI_shipY+4, 0);
	}
	if(c_AI_weapon==1){
		main_blit(c_shot2, c_AI_shot1X, c_AI_shot1Y, FLIPHV, combat_get_color(1));
	}
	if(c_AI_weapon==10 && c_player_dead<2){
		combat_animate(c_explosion1, c_AI_shot1X-2, c_AI_shot1Y-1, c_aniframe1);
		draw_number(c_player_shipX+18,c_player_shipY+4, -10);
	}
	if(c_AI_weapon==20 && c_player_dead<2){
		combat_animate(c_miss1, c_AI_shot1X-2, c_AI_shot1Y-1, c_aniframe1);
		draw_number(c_player_shipX+18,c_player_shipY+4, 0);
	}

	if(c_player_dead > 1 && c_player_dead < 4){
		combat_animate(c_explosion2, c_AI_shot1X-14, c_AI_shot1Y-14, c_aniframe1);
	}
	if(c_AI_dead > 1 && c_AI_dead < 4){
		combat_animate(c_explosion2, c_player_shot1X-12, c_player_shot1Y-16, c_aniframe1);
	}

	if(c_player_move1>0){
		if(c_player_move1>10){
			combat_draw_action_button(c_player_move1-10, 28, 57, 2);
		}else{
			combat_draw_action_button(c_player_move1, 28, 57, 1);
		}
	}
	if(c_player_move2>0){
		if(c_player_move2>10){
			combat_draw_action_button(c_player_move2-10, 34, 57, 2);
		}else{
			combat_draw_action_button(c_player_move2, 34, 57, 1);
		}
	}
	if(c_action==1){
		main_blit(c_moveselect, 0, 55, NOFLIP, NULL);
		combat_draw_action_buttons();
	}
	if(c_action==2){
		main_blit(c_fireselect, 40, 55, NOFLIP, NULL);
		combat_draw_action_buttons();
	}
}

void combat_move_stars(int xm, int ym, int d){
	int z;
	for(int j=0;j<6;j++){
		if(d==0){
			z=j;
		}else{
			z=d-j;
		}
		c_starX[j]=c_starX[j]+(z*xm);
		c_starY[j]=c_starY[j]+(z*ym);
	}
}

void combat_show_fight(){
	int s=0;
	int s2=0;
	int back=0;
	int baddy_back=0;
	for(int h=0;h < 4; h++){ // 4 movements per round
		if(c_player_dead>0 || c_AI_dead>0){break;}
		if(h==0){
			s=c_player_move1;
			s2=c_b1_AI[c_AI_round];
		}
		if(h==1){
			s=back;
			s2=baddy_back;
			SDL_Delay(100);
		}
		if(h==2){
			s=c_player_move2; 
			back=0;
			baddy_back=0;
			s2=c_b1_AI[c_AI_round];
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
						c_player_shipX--;
						c_player_shipY--;
						if(back==s){
							combat_move_stars(1,1,6);
						}else{
							c_player_thruster=7;
							back=3;
						}
						break;
					case 8:
						c_player_shipY--;
						if(back==s){
							combat_move_stars(0,1,6);
						}else{
							c_player_thruster=8;
							back=2;
						}
						break;
					case 9:
						c_player_shipX++;
						c_player_shipY--;
						if(back==s){
							combat_move_stars(-1,1,6);
						}else{
							c_player_thruster=9;
							back=1;
						}
						break;
					case 4:
						c_player_shipX--;
						if(back==s){
							combat_move_stars(1,0,6);
						}else{
							c_player_thruster=4;
							back=6;
						}
						break;
					case 6:
						c_player_shipX++;
						if(back==s){
							combat_move_stars(-1,0,6);
						}else{
							c_player_thruster=6;
							back=4;
						}
						break;
					case 1:
						c_player_shipX--;
						c_player_shipY++;
						if(back==s){
							combat_move_stars(1,-1,6);
						}else{
							c_player_thruster=1;
							back=9;
						}
						break;
					case 2:
						c_player_shipY++;
						if(back==s){
							combat_move_stars(0,-1,6);
						}else{
							c_player_thruster=2;
							back=8;
						}
						break;
					case 3:
						c_player_shipX++;
						c_player_shipY++;
						if(back==s){
							combat_move_stars(-1,-1,6);
						}else{
							c_player_thruster=3;
							back=7;
						}
						break;
					case 17:
						c_player_weapon=1;
						c_player_shot1X+=1;
						c_player_shot1Y-=3;
						break;
					case 18:
						c_player_weapon=1;
						c_player_shot1X+=2;
						c_player_shot1Y-=3;
						break;
					case 19:
						c_player_weapon=1;
						c_player_shot1X+=3;
						c_player_shot1Y-=3;
						break;
					case 14:
						c_player_weapon=1;
						c_player_shot1X+=1;
						c_player_shot1Y-=2;
						break;
					case 15:
						c_player_weapon=1;
						c_player_shot1X+=2;
						c_player_shot1Y-=2;
						break;
					case 16:
						c_player_weapon=1;
						c_player_shot1X+=3;
						c_player_shot1Y-=2;
						break;
					case 11:
						c_player_weapon=1;
						c_player_shot1X+=1;
						c_player_shot1Y-=1;
						break;
					case 12:
						c_player_weapon=1;
						c_player_shot1X+=2;
						c_player_shot1Y-=1;
						break;
					case 13:
						c_player_weapon=1;
						c_player_shot1X+=3;
						c_player_shot1Y-=1;
						break;
				}
			}
			switch(s2){
				case 7:
					c_AI_shipX--;
					c_AI_shipY--;
					if(baddy_back==s2){
						combat_move_stars(1,1,0);
					}else{
						c_AI_thruster=7;
						baddy_back=3;
					}
					break;
				case 8:
					c_AI_shipY--;
					if(baddy_back==s2){
						combat_move_stars(0,1,0);
					}else{
						c_AI_thruster=8;
						baddy_back=2;
					}
					break;
				case 9:
					c_AI_shipX++;
					c_AI_shipY--;
					if(baddy_back==s2){
						combat_move_stars(-1,1,0);
					}else{
						c_AI_thruster=9;
						baddy_back=1;
					}
					break;
				case 4:
					c_AI_shipX--;
					if(baddy_back==s2){
						combat_move_stars(1,0,0);
					}else{
						c_AI_thruster=4;
						baddy_back=6;
					}
					break;
				case 6:
					c_AI_shipX++;
					if(baddy_back==s2){
						combat_move_stars(-1,0,0);
					}else{
						c_AI_thruster=6;
						baddy_back=4;
					}
					break;
				case 1:
					c_AI_shipX--;
					c_AI_shipY++;
					if(baddy_back==s2){
						combat_move_stars(1,-1,0);
					}else{
						c_AI_thruster=1;
						baddy_back=9;
					}
					break;
				case 2:
					c_AI_shipY++;
					if(baddy_back==s2){
						combat_move_stars(0,-1,0);
					}else{
						c_AI_thruster=2;
						baddy_back=8;
					}
					break;
				case 3:
					c_AI_shipX++;
					c_AI_shipY++;
					if(baddy_back==s2){
						combat_move_stars(-1,-1,0);
					}else{
						c_AI_thruster=3;
						baddy_back=7;
					}
					break;
				case 17:
					c_AI_weapon=1;
					c_AI_shot1X-=3;
					c_AI_shot1Y+=1;
					break;
				case 18:
					c_AI_weapon=1;
					c_AI_shot1X-=2;
					c_AI_shot1Y+=1;
					break;
				case 19:
					c_AI_weapon=1;
					c_AI_shot1X-=1;
					c_AI_shot1Y+=1;
					break;
				case 14:
					c_AI_weapon=1;
					c_AI_shot1X-=3;
					c_AI_shot1Y+=2;
					break;
				case 15:
					c_AI_weapon=1;
					c_AI_shot1X-=2;
					c_AI_shot1Y+=2;
					break;
				case 16:
					c_AI_weapon=1;
					c_AI_shot1X-=1;
					c_AI_shot1Y+=2;
					break;
				case 11:
					c_AI_weapon=1;
					c_AI_shot1X-=3;
					c_AI_shot1Y+=3;
					break;
				case 12:
					c_AI_weapon=1;
					c_AI_shot1X-=2;
					c_AI_shot1Y+=3;
					break;
				case 13:
					c_AI_weapon=1;
					c_AI_shot1X-=1;
					c_AI_shot1Y+=3;
					break;
			}
			for(int j=0;j<6;j++){
					if(c_starX[j] < -256){c_starX[j]=256;}
					if(c_starX[j] > 256){c_starX[j]=-256;}
					if(c_starY[j] < -256){c_starY[j]=256;}
					if(c_starY[j] > 256){c_starY[j]=-256;}
			}
			if(i==3 || i==5 || i==7){c_player_thruster=0;}
			if(i==2 || i==4 || i==7){c_AI_thruster=0;}

			combat_show(0);
			SDL_RenderPresent(main_renderer);
			SDL_Delay(32);
		}
		if(h==0 || h==2){
			if(s==c_b1_AI[c_AI_round]+10 || 
			  (s==15 && c_b1_AI[c_AI_round] > 10)){
				c_player_weapon=10;
				c_AI_HP-=10;
				if(c_AI_HP < 1 ){c_AI_dead=1;}
			}else{
				if(c_player_weapon==1){
					c_player_weapon=20;
				}else{
					c_player_weapon=0;
				}
			}
			if(s+10==c_b1_AI[c_AI_round] || 
			  (s>10 && c_b1_AI[c_AI_round]==15)){
				c_AI_weapon=10;
				c_player_HP-=10;
				if(c_player_HP < 1 ){c_player_dead=1;}
			}else{
				if(c_AI_weapon==1){
					c_AI_weapon=20;
				}else{
					c_AI_weapon=0;
				}
			}
			// extra render for hit explosion
			if(c_player_weapon>=10 || c_AI_weapon>=10){
				for(int i=0;i<6;i++){
					combat_show(0);
					SDL_RenderPresent(main_renderer);	
					SDL_Delay(32);
					c_aniframe1++;
				}
				c_aniframe1=0;
			}
			//extra extra render for death explosion
			if(c_player_dead>0 || c_AI_dead>0){
				if(c_player_dead==1){c_player_dead=2;}
				if(c_AI_dead==1){c_AI_dead=2;}
				for(int i=0;i<6;i++){
					if(c_player_dead==2 && i==2){c_player_dead=3;}
					if(c_AI_dead==2 && i==2){c_AI_dead=3;}
					combat_show(0);
					SDL_RenderPresent(main_renderer);	
					SDL_Delay(32);
					c_aniframe1++;
				}
				c_aniframe1=0;
				if(c_player_dead==3){c_player_dead=4;}
				if(c_AI_dead==3){c_AI_dead=4;}
			}
			c_AI_round++;
			if(c_AI_round>7){c_AI_round=0;}
		}
		c_player_weapon=0;
		c_AI_weapon=0;
		c_player_thruster=0;
		c_AI_thruster=0;
		c_AI_shot1X=40;
		c_AI_shot1Y=20;
		c_player_shot1X=26;
		c_player_shot1Y=34;
	}
}


 /* Init and start */
int combat_setup(){
	printf("loading combat...\n");
	main_scene=SCENE_COMBAT;
	frame_skip=0;

	c_player_move1=0;
	c_player_move2=0;
	c_player_HP=50;
	c_player_dead=0;
	c_player_shipX=16;
	c_player_shipY=32;
	c_player_shot1X=26;
	c_player_shot1Y=34;
	c_player_weapon=0;
	c_player_thruster=0;
	c_action=0;
	c_AI_shipX=37;
	c_AI_shipY=11;
	c_AI_shot1X=40;
	c_AI_shot1Y=20;
	c_AI_HP=50;
	c_AI_dead=0;
	c_AI_weapon=0;
	c_AI_thruster=0;
	c_AI_round=0;
	c_aniframe1=0;
	c_time_pos=0;
	int temp[8]={8,15,6,15,2,15,4,15};
	for(int i=0;i<8;i++){
		c_b1_AI[i]=temp[i];
	}

	c_stars1 = Load_tex("sprites/stars/stars1_256.png");
	c_stars2 = Load_tex("sprites/stars/stars2_256.png");
	c_stars3 = Load_tex("sprites/stars/stars3_256.png");
	c_stars4 = Load_tex("sprites/stars/stars4_256.png");
	c_stars5 = Load_tex("sprites/stars/stars5_256.png");
	c_stars6 = Load_tex("sprites/stars/stars6_256.png");
	c_ship1 = Load_tex("sprites/ship1.png");
	if (c_baddy1 == NULL)
	{
		c_baddy1 = Load_tex("sprites/baddy1.png");
	}
	c_timerbar = Load_tex("sprites/timerbar.png");
	c_hud = Load_tex("sprites/combat_hud_bottom.png");
	c_firetext = Load_tex("sprites/combat_hud_fire.png");
	c_movetext = Load_tex("sprites/combat_hud_move.png");
	c_moveselect = Load_tex("sprites/combat_hud_move_highlight.png");
	c_fireselect = Load_tex("sprites/combat_hud_fire_highlight.png");
	c_diag = Load_tex("sprites/directional_diag.png");
	c_card = Load_tex("sprites/directional_card.png");
	c_card_right = Load_tex("sprites/directional_card_right.png");
	c_mid = Load_tex("sprites/directional_mid.png");
	c_shot1 = Load_tex("sprites/projectile1.png");
	c_shot2 = Load_tex("sprites/projectile1_diag.png");
	c_shot3 = Load_tex("sprites/projectile1_right.png");
	c_explosion1 = Load_tex("sprites/explosion1.png");
	c_explosion2 = Load_tex("sprites/explosion1_big.png");
	c_miss1= Load_tex("sprites/miss1.png");

	for(int i=0;i<6;i++){
			c_starX[i]=-64;
			c_starY[i]=-64;
	}
	SDL_SetRenderDrawColor(main_renderer, 0x00, 0x00, 0x00, 255);
	combat_show(0);
	return 0;
}


void combat_handle_input(SDL_Event event){
	int direction=0;
	int mouseX; int mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
		switch (event.type) {
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
					case SDLK_KP_0:
					case SDLK_LSHIFT:
						c_action=1;
						break;
					case SDLK_KP_ENTER:
					case SDLK_SPACE:
						c_action=2;
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
				if(direction>0 && c_action==2){direction+=10;}
				if(direction>0){c_action=0;}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if(c_player_move2==0 && mouseX < (25*main_scale) 
								   && mouseY > (56*main_scale)){
					//movebutt on
						c_action=1;
				}
				if(c_player_move2==0 && mouseX > (42*main_scale) 
								   && mouseY > (56*main_scale)){
					//firebutt on
						c_action=2;
				}
				if(mouseX > (12*main_scale) && mouseX < (35*main_scale) &&
				   mouseY > (28*main_scale) && mouseY < (51*main_scale)){
					if(c_action==1){
						int xr=round(((mouseX/main_scale)-12)/8);
						int yr=round(((mouseY/main_scale)-28)/8);
						if(xr==0 && yr==0){direction=7;}
						if(xr==1 && yr==0){direction=8;}
						if(xr==2 && yr==0){direction=9;}
						if(xr==0 && yr==1){direction=4;}
						if(xr==1 && yr==1){direction=5;}
						if(xr==2 && yr==1){direction=6;}
						if(xr==0 && yr==2){direction=1;}
						if(xr==1 && yr==2){direction=2;}
						if(xr==2 && yr==2){direction=3;}
					}else if(c_action!=1 && c_player_move2==0){
						//movebutt on
							c_action=1;
					}
				}
				if(mouseX > (33*main_scale) && mouseX < (56*main_scale) &&
				   mouseY > (7*main_scale) && mouseY < (30*main_scale)){
					if(c_action==2){
						int xr=round(((mouseX/main_scale)-33)/8);
						int yr=round(((mouseY/main_scale)-7)/8);
						if(xr==0 && yr==0){direction=17;}
						if(xr==1 && yr==0){direction=18;}
						if(xr==2 && yr==0){direction=19;}
						if(xr==0 && yr==1){direction=14;}
						if(xr==1 && yr==1){direction=15;}
						if(xr==2 && yr==1){direction=16;}
						if(xr==0 && yr==2){direction=11;}
						if(xr==1 && yr==2){direction=12;}
						if(xr==2 && yr==2){direction=13;}
					}else if(c_action!=2 && c_player_move2==0){
						//firebutt on
							c_action=2;
					}
				break;
			}
		}
		if(c_player_dead==0 && c_AI_dead ==0){
			if(direction>0 && c_player_move1>0 && c_player_move2==0){
				c_player_move2=direction;
				c_action=0;
			}
			if(direction>0 && c_player_move1==0){
				c_player_move1=direction;
				c_action=0;
			}
		}else{
			c_action=0;
		}
combat_show(c_time_pos);
}


