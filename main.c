#include "main.h"

SDL_Texture* Load_tex(char *filename){
	SDL_Texture* texture = IMG_LoadTexture(main_renderer, filename);
	if (!texture){
		printf("Load image error: %s, %s \n",filename, IMG_GetError());
		exit(0);
	}
	return texture;
}

void main_blit(SDL_Texture *tex, int x, int y, int mode, SDL_Color *color){
	int w, h, r;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	SDL_RendererFlip flip;
	SDL_Rect rect;
	rect.x=x;
	rect.y=y;
	rect.w=w;
	rect.h=h;
	if(color){
		SDL_SetTextureColorMod(tex, color->r, color->g, color->b);
	}else{
		SDL_SetTextureColorMod(tex, 255,255,255);
	}
	switch(mode){
		case P_MODE0:
			SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
			SDL_RenderCopy(main_renderer, tex, NULL, &rect);
			break;
		case P_MODE1:
			SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_MOD);
			SDL_RenderCopy(main_renderer, tex, NULL, &rect);
			break;
		case P_MODE2:
			SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
			flip=SDL_FLIP_VERTICAL;
			SDL_RenderCopyEx(main_renderer, tex, NULL, &rect, 0, 0, flip);
			break;
		case NOFLIP:
			SDL_RenderCopy(main_renderer, tex, NULL, &rect);
			break;
		case FLIPV:
			flip = SDL_FLIP_VERTICAL;
			SDL_RenderCopyEx(main_renderer, tex, NULL, &rect, 0, 0, flip);
			break;
		case FLIPH:
			flip = SDL_FLIP_HORIZONTAL;
			SDL_RenderCopyEx(main_renderer, tex, NULL, &rect, 0, 0, flip);
			break;
		case FLIPHV:
			flip = SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL;
			SDL_RenderCopyEx(main_renderer, tex, NULL, &rect, 0, 0, flip);
			break;
		case TWINK1:
			if((rand()%64)==0){
				r=((rand() % 256) + 128);
			}else{
				r=((rand() % 8) + 32);
			}
			SDL_SetTextureAlphaMod(tex, r);
			SDL_RenderCopyEx(main_renderer, tex, NULL, &rect, 90, 0, 0);
			break;
		case TWINK2:
			if((rand()%32)==0){
				r=((rand() % 256) + 128);
			}else{
				r=((rand() % 16) + 48);
			}
			SDL_SetTextureAlphaMod(tex, r);
			SDL_RenderCopyEx(main_renderer, tex, NULL, &rect, 90, 0, 0);
			break;
		case TWINK3:
			if((rand()%32)==0){
				r=((rand() % 256) + 128);
			}else{
				r=((rand() % 24) + 64);
			}
			SDL_SetTextureAlphaMod(tex, r);
			SDL_RenderCopyEx(main_renderer, tex, NULL, &rect, 90, 0, 0);
			break;
		case TWINK4:
			if((rand()%48)==0){
				r=((rand() % 256) + 128);
			}else{
				r=((rand() % 32) + 80);
			}
			SDL_SetTextureAlphaMod(tex, r);
			SDL_RenderCopyEx(main_renderer, tex, NULL, &rect, 90, 0, 0);
			break;
		case TWINK5:
			if((rand()%56)==0){
				r=((rand() % 256) + 128);
			}else{
				r=((rand() % 40) + 96);
			}
			SDL_SetTextureAlphaMod(tex, r);
			SDL_RenderCopyEx(main_renderer, tex, NULL, &rect, 90, 0, 0);
			break;
	}
	free(color);
}

void draw_text(int x, int y, char *text, int length, int font_set, SDL_Color color){
	int w, h, f, index;
	int count=0;
	SDL_QueryTexture(font[count], NULL, NULL, &w, &h);
	SDL_SetTextureColorMod(font[count], color.r, color.g, color.b);
	for(int i=0;i<length;i++){
		if(font_set==FONT_CYCLE){  //cycles font for random effect
			count++;
			if(count>3){count=1;}
			SDL_SetTextureColorMod(font[count], 0, 96, 0);
		}
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
		SDL_RenderCopy(main_renderer, font[count], &srect, &drect);
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
		srect.x=(f*4);
		srect.y=0;
		srect.w=4;
		srect.h=6;
		drect.x=x-c;
		drect.y=y;
		SDL_RenderCopy(main_renderer, font[0], &srect, &drect);
	}
	if(n<0){
		c+=4;
		srect.x=41;
		srect.y=0;
		srect.w=4;
		srect.h=6;
		drect.x=x-c;
		drect.y=y;
		SDL_RenderCopy(main_renderer, font[0], &srect, &drect);
	}
}

void load_fonts(){
	printf("loading fonts..\n");
	font[FONT_EARTH] = Load_tex("sprites/gui/font_5x3_earth.png");
	font[FONT_KRULL] = Load_tex("sprites/gui/font_5x3_krull.png");
	font[FONT_PLINK] = Load_tex("sprites/gui/font_5x3_plink.png");
	font[FONT_SNEEB] = Load_tex("sprites/gui/font_5x3_sneeb.png");
}

void main_menu_setup()
{
	int default_button;
	default_button = gui_add_button("combat", 0, 2, 35, BUTTON_STATE_ENABLED, BUTTON_STYLE_MENU, -1, &combat_setup);
	gui_add_button("planets", 0, 10, 35 , BUTTON_STATE_ENABLED, BUTTON_STYLE_MENU, -1, &planet_setup);
	gui_add_button("trade", 0, 18, 35,  BUTTON_STATE_DISABLED, BUTTON_STYLE_MENU, -1, &travel_setup);
	gui_add_button("travel", 0, 36, 35,  BUTTON_STATE_ENABLED, BUTTON_STYLE_MENU, -1, &travel_setup);
	gui_add_button("nav", 0, 44, 35,  BUTTON_STATE_DISABLED, BUTTON_STYLE_MENU, -1, &travel_setup);
	gui_add_button("comms", 0, 52, 35,  BUTTON_STATE_DISABLED, BUTTON_STYLE_MENU, -1, &travel_setup);

	update_button_state(default_button, BUTTON_STATE_SELECTED);
}

 /* Init and start */
int main_setup(){
	main_scale=8;

	if (SDL_Init(SDL_INIT_VIDEO) <0){
		 SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
			"Couldn't initialize SDL: %s", SDL_GetError());
		return 3;
	}

	if (SDL_CreateWindowAndRenderer(main_resX *main_scale, main_resY * main_scale, 
									SDL_WINDOW_OPENGL, &main_window, &main_renderer)) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
			"Couldn't create window and renderer: %s", SDL_GetError());
		return 3;
	}

	SDL_RenderSetLogicalSize(main_renderer, main_resX, main_resY);
	SDL_SetWindowTitle(main_window, "Hidden Star");
	atexit(SDL_Quit);

	srand(time(NULL));

	load_fonts();
	return 0;
}

void draw_scene(){
	switch(main_scene){
		case 0:
			if(intro_draw()==1){
				//combat_setup(); //should be main menu
				gui_draw();
			}
			break;
		case 1:
			planet_draw();
			break;
		case 2:
			combat_draw();
			break;
	}
}

void main_input(SDL_Event event){
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	mouseX /= main_scale;
	mouseY /= main_scale;
	switch(main_scene){
		case 0:
		switch (event.type) {
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
					case SDLK_TAB:
						if (event.key.keysym.mod == KMOD_LSHIFT || event.key.keysym.mod == KMOD_RSHIFT)
						{
							gui_cycle_next_button(0);
						}
						else
						{
							gui_cycle_next_button(1);
						}
						break;
						gui_do_button_action();
					case SDLK_RETURN:
					case SDLK_KP_ENTER:
						gui_do_button_action();
						break;
					case SDLK_LEFT:
						gui_seek_next_button_h(0);
						break;
					case SDLK_RIGHT:
						gui_seek_next_button_h(1);
						break;
					case SDLK_UP:
						gui_seek_next_button_v(0);
						break;
					case SDLK_DOWN:
						gui_seek_next_button_v(1);
						break;
					}
					break;
			case SDL_MOUSEMOTION:
				gui_update_hover_state(mouseX, mouseY);
				break;
			case SDL_MOUSEBUTTONDOWN:
				gui_do_button_action_coords(mouseX, mouseY);
			}
			break;
		case 1:
			planets_handle_input(event);
			break;
		case 2:
			combat_handle_input(event);
			break;
	}
}

int main(int argc, char *argv[]){
	int check_init=main_setup();
	if(check_init > 0){
		return check_init; // :(
	}

	int frame=0;
	int current_time;
	int last_time = SDL_GetTicks();
	int mouseX; int mouseY;

	intro_setup();
	gui_setup();
	main_menu_setup();

	SDL_Event main_event;
	while(SDL_PollEvent(&main_event)){} //flush event queue

	while(main_event.type != SDL_QUIT){
		SDL_GetMouseState(&mouseX, &mouseY);
		if(SDL_PollEvent(&main_event)<1){SDL_Delay(10);}else{
			switch (main_event.type) {
				case SDL_QUIT:
					exit(0);
					break;
				case SDL_KEYDOWN:
					switch(main_event.key.keysym.sym){
						case SDLK_F2:
							if(main_scale>1){
								main_scale/=2;
							}
							SDL_SetWindowSize(main_window, main_resX*main_scale, 
							main_resY*main_scale);
							break;
						case SDLK_F3:
							if(main_scale<32){
								main_scale*=2;
							}
							SDL_SetWindowSize(main_window, main_resX*main_scale, 
							main_resY*main_scale);
							break;
						case SDLK_ESCAPE:
							//return to main menu?
							{
								SDL_Event e;
								e.type = SDL_QUIT;
								SDL_PushEvent(&e);
							}
							break;
						default:
							main_input(main_event);
							break;
					}
					break;
				case SDL_MOUSEWHEEL:
					main_input(main_event);
					break;
				case SDL_MOUSEBUTTONDOWN:
					main_input(main_event);
					break;
				case SDL_MOUSEMOTION:
					main_input(main_event);
					break;
			}
		}

		current_time=SDL_GetTicks();
		if((current_time-last_time) > 64){
			last_time=current_time;
			frame++;
			if(frame>=frame_skip){
				frame=0;
				draw_scene();
			}
			SDL_RenderPresent(main_renderer);
		}
	}

	return 0;
}
