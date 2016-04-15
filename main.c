#include "main.h"

SDL_Texture* Load_tex(char *filename){
	SDL_Texture* texture = IMG_LoadTexture(main_renderer, filename);
    if (!texture){
	    printf("Load image error: %s, %s \n",filename, IMG_GetError());
		exit(0);
    }
    return texture;
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
			SDL_SetTextureColorMod(font[count], 16, 128, 16);
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
			intro_draw();
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
	int frame_skip;
	int current_time;
	int last_time = SDL_GetTicks();
	int mouseX; int mouseY;

	main_scene=0;
	frame_skip=2;
	intro_setup();

	while(main_event.type != SDL_QUIT){
		SDL_GetMouseState(&mouseX, &mouseY);
		if(SDL_PollEvent(&main_event)<1){SDL_Delay(10);}else{
			switch (main_event.type) {
				case SDL_QUIT:
					exit(0);
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
