#include "planets.h"

void planet_draw(Planet *p){
	int width=(int)(pow(2,(p->size+3)))*2;

	SDL_Texture* auxtexture = SDL_CreateTexture(main_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width/2, width/2);
	//Set the new texture as the render target
	SDL_SetRenderTarget(main_renderer, auxtexture);
	
	main_blit(ocean[p->ocn][p->size],p->tx1,0, P_MODE0, planet_get_color(p,0));
	main_blit(ocean[p->ocn][p->size],p->tx1-width,0, P_MODE0, planet_get_color(p,0));
	if(p->con<P_MAX){
		main_blit(continents[p->con][p->size],p->tx1,0, P_MODE0, planet_get_color(p,1));
		main_blit(continents[p->con][p->size],p->tx1-width,0, P_MODE0, planet_get_color(p,1));
	}
	if(p->cap<P_MAX){
		main_blit(caps[p->cap][p->size],p->tx1,0, P_MODE0, planet_get_color(p,2));
		main_blit(caps[p->cap][p->size],p->tx1-width,0, P_MODE0, planet_get_color(p,2));
	}
	if(p->cld<P_MAX){
		main_blit(clouds[p->cld][p->size],p->tx2,0, P_MODE0, planet_get_color(p,3));
		main_blit(clouds[p->cld][p->size],p->tx2-width,0, P_MODE0, planet_get_color(p,3));
	}

	main_blit(planet_mask[p->msk][p->size],0,0, P_MODE1, NULL);

	SDL_Surface *surf = SDL_CreateRGBSurface(0, width/2, width/2, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
	SDL_RenderReadPixels(main_renderer, NULL, SDL_PIXELFORMAT_RGBA8888, surf->pixels, surf->pitch);
	SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf->format,0,0,0));
	SDL_Texture* newtexture=SDL_CreateTextureFromSurface(main_renderer, surf);
	SDL_FreeSurface(surf);

	SDL_SetRenderTarget(main_renderer, NULL);
	SDL_RenderClear(main_renderer);
	main_blit(stars1,0,0, P_MODE0, NULL);
	main_blit(newtexture,p->x,p->y,P_MODE2, NULL);
	SDL_DestroyTexture(newtexture);
	SDL_DestroyTexture(auxtexture);
	p->tx1+=(int)round(p->speed);
	p->tx2+=(int)round((p->speed*2));
	if(p->tx1>width){p->tx1=0;}
	if(p->tx2>width){p->tx2=0;}
	if(p->tx1<0){p->tx1=width;}
	if(p->tx2<0){p->tx2=width;}
}

int planet_setup(){
	printf("loading planet...\n");
//	main_scene=SCENE_PLANET_GEN;
//	frame_skip=1;

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

SDL_Color* planet_get_color(Planet *p, int index){
	if(p->color_state==1){
		return &p->color[index];
	}
	return NULL;
}

void planet_random_pos(Planet *p){
	int width=(int)(pow(2,(p->size+3)));
	p->x=(rand()%64+width)-(width+(width/2));
	p->y=(rand()%64+width)-(width+(width/2));
}

void planet_set_random(Planet *p){
	p->size=rand()%4;
	p->ocn=(rand()%P_MAX);
	p->con=(rand()%(P_MAX+1));
	p->cap=(rand()%(P_MAX+1));
	p->cld=(rand()%P_MAX);
}

void planet_set_default(Planet *p, int type){
	p->speed=1;
	p->tx1=0;
	p->tx2=0;
	p->x=0;
	p->y=0;
	p->size=2;
	p->msk=0;
	p->color_state=0;
	p->ocn = type;
	p->con = type;
	p->cap = type;
	p->cld = type;
}

/*
void planets_handle_input(SDL_Event event){
		switch (event.type) {
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
					case SDLK_q:
						p->ocn++;
						if(p->ocn>=P_MAX){p->ocn=0;}
						break;
					case SDLK_w:
						p->con++;
						if(p->con>=P_MAX+1){p->con=0;}
						break;
					case SDLK_e:
						p->cap++;
						if(p->cap>=P_MAX+1){p->cap=0;}
						break;
					case SDLK_r:
						p->cld++;
						if(p->cld>=P_MAX){p->cld=0;}
						break;
					case SDLK_t:
						p->msk++;
						if(p->msk>=P_MAX){p->msk=0;}
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
						if(p->speed<10){p->speed+=0.2;}
						break;
					case SDLK_d:
						if(p->speed>-10){p->speed-=0.2;}
						break;
					case SDLK_s:
						p->size++;
						p->x=0; p->y=0;
						if(p->size>4){p->size=0;}
						break;
				}
			break;
			case SDL_MOUSEWHEEL:
				if(event.wheel.y<0){
					p->size++;
					p->x=0; p->y=0;
					if(p->size>4){p->size=4;}
				}
				if(event.wheel.y>0){
					p->size--;
					p->x=0; p->y=0;
					if(p->size<0){p->size=0;}
				}
			case SDL_MOUSEBUTTONDOWN:
				switch(event.button.button){
					case SDL_BUTTON_LEFT:
						planet_get_random();
						break;
					case SDL_BUTTON_RIGHT:
						if(p->color_state==0){p->color_state=1;}else{p->color_state=0;}
						for(int i=0;i<4;i++){
							p->color[i].r=(rand()%(255));
							p->color[i].g=(rand()%(255));
							p->color[i].b=(rand()%(255));
						}
						break;
				}
				break;
		}
	}

*/
