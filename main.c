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
	switch(mode){
		case P_MODE0:
			if(color){
				SDL_SetTextureColorMod(tex, color->r, color->g, color->b);
			}else{
				SDL_SetTextureColorMod(tex, 255,255,255);
			}
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
	}
	free(color);
}

void draw_text(int x, int y, char *text, int length, int font_set){
	int w, h, f, index;
	int count=0;
	SDL_QueryTexture(font[count], NULL, NULL, &w, &h);
	SDL_SetTextureColorMod(font[count], 255, 255, 255);
	for(int i=0;i<length;i++){
		if(font_set==4){  //cycles font for random effect
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
		srect.x=(f*4)+1;
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
	font[0] = Load_tex("sprites/font_5x3_earth.png");
	font[1] = Load_tex("sprites/font_5x3_krull.png");
	font[2] = Load_tex("sprites/font_5x3_plink.png");
	font[3] = Load_tex("sprites/font_5x3_sneeb.png");
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

void draw_scene(int time_pos){
	switch(main_scene){
		case 0:
			if(intro_draw()==1){
				planet_setup(); //should be main menu
			}
			break;
		case 1:
			planet_draw();
			break;
	}
}

void main_input(SDL_Event event){
	switch(main_scene){
		case 0:
			planet_setup();	//skip intro
			break;
		case 1:
			planets_handle_input(event);
			break;
	}
}

int main(int argc, char *argv[]){
	int check_init=main_setup();
	if(check_init > 0){
		return check_init; // :(
	}

	int time_pos=0;
	int frame=0;
	int current_time;
	int last_time = SDL_GetTicks();
	int mouseX; int mouseY;

	intro_setup();

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
			}
		}

		current_time=SDL_GetTicks();
		if((current_time-last_time) > 64){
			last_time=current_time;
			frame++;
			if(frame>=frame_skip){
				frame=0;
				draw_scene(time_pos);
			}
			SDL_RenderPresent(main_renderer);
		}
	}

	return 0;
}