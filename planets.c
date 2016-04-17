#include "planets.h"

void planet_draw(){
	int width=(int)(pow(2,(p_size+3)))*2;

	SDL_Texture* auxtexture = SDL_CreateTexture(main_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width/2, width/2);
	//Set the new texture as the render target
	SDL_SetRenderTarget(main_renderer, auxtexture);
	
	main_blit(ocean[p_ocn][p_size],p_tx1,0, P_MODE0, planet_get_color(0));
	main_blit(ocean[p_ocn][p_size],p_tx1-width,0, P_MODE0, planet_get_color(0));
	if(p_con<P_MAX){
		main_blit(continents[p_con][p_size],p_tx1,0, P_MODE0, planet_get_color(1));
		main_blit(continents[p_con][p_size],p_tx1-width,0, P_MODE0, planet_get_color(1));
	}
	if(p_cap<P_MAX){
		main_blit(caps[p_cap][p_size],p_tx1,0, P_MODE0, planet_get_color(2));
		main_blit(caps[p_cap][p_size],p_tx1-width,0, P_MODE0, planet_get_color(2));
	}
	if(p_cld<P_MAX){
		main_blit(clouds[p_cld][p_size],p_tx2,0, P_MODE0, planet_get_color(3));
		main_blit(clouds[p_cld][p_size],p_tx2-width,0, P_MODE0, planet_get_color(3));
	}

	main_blit(planet_mask[p_msk][p_size],0,0, P_MODE1, NULL);

	SDL_Surface *surf = SDL_CreateRGBSurface(0, width/2, width/2, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
	SDL_RenderReadPixels(main_renderer, NULL, SDL_PIXELFORMAT_RGBA8888, surf->pixels, surf->pitch);
	SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf->format,0,0,0));
	SDL_Texture* newtexture=SDL_CreateTextureFromSurface(main_renderer, surf);


	SDL_SetRenderTarget(main_renderer, NULL);
	SDL_RenderClear(main_renderer);
	main_blit(stars1,0,0, P_MODE0, NULL);
	main_blit(newtexture,p_x,p_y,P_MODE2, NULL);
	p_tx1+=(int)round(p_speed);
	p_tx2+=(int)round((p_speed*2));
	if(p_tx1>width){p_tx1=0;}
	if(p_tx2>width){p_tx2=0;}
	if(p_tx1<0){p_tx1=width;}
	if(p_tx2<0){p_tx2=width;}
}

int planet_setup(){
	printf("loading planet...\n");
	main_scene=SCENE_PLANET_GEN;
	frame_skip=1;

	p_speed=1;
	p_tx1,p_tx2=0;
	p_x, p_y=0;
	p_size=3;
	p_ocn=0;
	p_cap=0;
	p_con=0;
	p_cld=0;
	p_msk=0;
	p_color=0;

	stars1 = Load_tex("sprites/stars/stars1_256.png");
	for(int j=0;j<5;j++){
		char file[128];
		for(int i=0;i<4;i++){
			int size=(int)(pow(2,(j+3)));
			if(i<P_MAX){
				sprintf(file,"sprites/planets/caps%d_%d.png",(i+1), size);
				caps[i][j] = Load_tex(file);
				sprintf(file,"sprites/planets/clouds%d_%d.png",(i+1), size);
				clouds[i][j] = Load_tex(file);
				sprintf(file,"sprites/planets/continents%d_%d.png",(i+1), size);
				continents[i][j] = Load_tex(file);
				sprintf(file,"sprites/planets/ocean%d_%d.png",(i+1), size);
				ocean[i][j] = Load_tex(file);
			}
			sprintf(file,"sprites/planets/planet_mask%d_%d.png",(i+1), size);
			planet_mask[i][j] = Load_tex(file);
		}
	}
	return 0;
}

SDL_Color* planet_get_color(int index){
	SDL_Color *color=malloc(sizeof *color);
	if(p_color==1){
		color->r=p_r[index]; color->g=p_g[index]; color->b=p_b[index];
		return color;
	}
	return NULL;
}

void planet_random_pos(){
	int width=(int)(pow(2,(p_size+3)));
	p_x=(rand()%64+width)-(width+(width/2));
	p_y=(rand()%64+width)-(width+(width/2));
}

void planet_get_random(){
	p_ocn=(rand()%P_MAX);
	p_con=(rand()%(P_MAX+1));
	p_cap=(rand()%(P_MAX+1));
	p_cld=(rand()%P_MAX);
}

void planet_get_default(int p){
	p_ocn = p;
	p_con = p;
	p_cap = p;
	p_cld = p;
}

void planets_handle_input(SDL_Event event){
		switch (event.type) {
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
					case SDLK_q:
						p_ocn++;
						if(p_ocn>=P_MAX){p_ocn=0;}
						break;
					case SDLK_w:
						p_con++;
						if(p_con>=P_MAX+1){p_con=0;}
						break;
					case SDLK_e:
						p_cap++;
						if(p_cap>=P_MAX+1){p_cap=0;}
						break;
					case SDLK_r:
						p_cld++;
						if(p_cld>=P_MAX){p_cld=0;}
						break;
					case SDLK_t:
						p_msk++;
						if(p_msk>=P_MAX){p_msk=0;}
						break;
					case SDLK_1:
					case SDLK_KP_1:
						planet_get_default(0);
						break;
					case SDLK_2:
					case SDLK_KP_2:
						planet_get_default(1);
						break;
					case SDLK_3:
					case SDLK_KP_3:
						planet_get_default(2);
						break;
					case SDLK_4:
					case SDLK_KP_4:
						planet_get_default(3);
						break;
					case SDLK_5:
					case SDLK_KP_5:
						//TODO: Make a nicer way of doing this that automatically respects P_MAX
						break;
					case SDLK_RETURN:
					case SDLK_KP_ENTER:
						planet_get_random();
						break;
					case SDLK_SPACE:
						planet_random_pos();
						break;
					case SDLK_a:
						if(p_speed<10){p_speed+=0.2;}
						break;
					case SDLK_d:
						if(p_speed>-10){p_speed-=0.2;}
						break;
					case SDLK_s:
						p_size++;
						p_x=0; p_y=0;
						if(p_size>4){p_size=0;}
						break;
				}
			break;
			case SDL_MOUSEWHEEL:
				if(event.wheel.y<0){
					p_size++;
					p_x=0; p_y=0;
					if(p_size>4){p_size=4;}
				}
				if(event.wheel.y>0){
					p_size--;
					p_x=0; p_y=0;
					if(p_size<0){p_size=0;}
				}
			case SDL_MOUSEBUTTONDOWN:
				switch(event.button.button){
					case SDL_BUTTON_LEFT:
						planet_get_random();
						break;
					case SDL_BUTTON_RIGHT:
						if(p_color==0){p_color=1;}else{p_color=0;}
						for(int i=0;i<4;i++){
							p_r[i]=(rand()%(255));
							p_g[i]=(rand()%(255));
							p_b[i]=(rand()%(255));
						}
						break;
				}
				break;
		}
	}


