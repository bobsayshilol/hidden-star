#include "intro.h"

int intro_setup(){
	printf("loading intro..\n");
	main_scene=0;
	frame_skip=2;

	intro_bg=Load_tex("sprites/title_planet.png");
	intro_flare=Load_tex("sprites/title_flare.png");
	intro_count1=0;
	intro_count2=0;
	intro_count3=1;
	intro_font=4;
	SDL_SetRenderDrawColor(main_renderer, 0x05, 0x09, 0x14, 255);
	return 0;
}

int intro_draw(){
	if(intro_count1<6){intro_count1++;}else{if(intro_count2<4){intro_count2++;}}
	if(intro_count2==4){intro_count3++;}
	if(intro_count3>5){intro_font=0;}
	SDL_RenderClear(main_renderer);
	if(intro_count3>4){
		SDL_RenderCopy(main_renderer, intro_flare, NULL, NULL);
	}
	SDL_RenderCopy(main_renderer, intro_bg, NULL, NULL);
	draw_text(32-((intro_count1*4)), 28, "hidden", intro_count1, intro_font);
	draw_text(40, 28, "star", intro_count2, intro_font);
	if(intro_count3<=5){
		draw_number(36, 28, intro_count1);
		draw_number(40, 28, intro_count2);
	}

	if(intro_count3>10){
		return 1;
	}else{
		return 0;
	}
}

