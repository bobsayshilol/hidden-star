#include "main.h"
#ifdef __MINGW64__
	#include <windows.h>
#endif

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
			SDL_RenderCopyEx(main_renderer, tex, NULL, &rect, 0, 0, 0);
			break;
		case TWINK2:
			if((rand()%32)==0){
				r=((rand() % 256) + 128);
			}else{
				r=((rand() % 16) + 48);
			}
			SDL_SetTextureAlphaMod(tex, r);
			SDL_RenderCopyEx(main_renderer, tex, NULL, &rect, 0, 0, 0);
			break;
		case TWINK3:
			if((rand()%32)==0){
				r=((rand() % 256) + 128);
			}else{
				r=((rand() % 24) + 64);
			}
			SDL_SetTextureAlphaMod(tex, r);
			SDL_RenderCopyEx(main_renderer, tex, NULL, &rect, 0, 0, 0);
			break;
		case TWINK4:
			if((rand()%48)==0){
				r=((rand() % 256) + 128);
			}else{
				r=((rand() % 32) + 80);
			}
			SDL_SetTextureAlphaMod(tex, r);
			SDL_RenderCopyEx(main_renderer, tex, NULL, &rect, 0, 0, 0);
			break;
		case TWINK5:
			if((rand()%56)==0){
				r=((rand() % 256) + 128);
			}else{
				r=((rand() % 40) + 96);
			}
			SDL_SetTextureAlphaMod(tex, r);
			SDL_RenderCopyEx(main_renderer, tex, NULL, &rect, 0, 0, 0);
			break;
	}
}

int draw_text(int x, int y, char *text, int length, int font_set, SDL_Color color){
	int index;
	int count=0;
	int offset_y = 0;
	int offset_x = 0;
	SDL_Rect srect;
	SDL_Rect drect;
	SDL_SetTextureColorMod(font[count], color.r, color.g, color.b);
	for(int i=0;i<length;i++){
		if (text[i] == '\n')
		{
			offset_x = 0;
			offset_y += 6;
			continue;
		}

		if(font_set==FONT_CYCLE){  //cycles font for random effect
			count=(rand()%3)+1;
			SDL_SetTextureColorMod(font[count], 0, 96, 0);
		}
		index=((int)text[i]);
		srect.x = fonts[index].x;
		srect.y = fonts[index].y;
		srect.w = 4;
		srect.h = 6;
		drect.w = 4;
		drect.h = 6;
		drect.x = x + offset_x;
		drect.y = y + offset_y;
		SDL_RenderCopy(main_renderer, font[count], &srect, &drect);

		if (text[i] == ' ' || text[i] == '\'' || text[i] == '.' || text[i] == ',' || text[i] == '!' || text[i] == ':' || text[i] == ';')
		{
			offset_x += 2;
		}
		else if (text[i] == '`' || text[i] == '(' || text[i] == ')' || text[i] == '[' || text[i] == ']' || text[i] == '{' || text[i] == '}')
		{
			offset_x += 3;
		}
		else
		{
			offset_x += 4;
		}
	}
	return offset_y;
}

void draw_number(int x, int y, int num){
	char s[16];
	int len=sprintf(s, "%d", num);
	draw_text(x, y, s, len, FONT_EARTH, GUI_TEXT_COLOR_DEFAULT);
}

void load_fonts(){
	printf("loading fonts..\n");
	font[FONT_EARTH] = Load_tex("sprites/gui/font_5x3_earth.png");
	font[FONT_KRULL] = Load_tex("sprites/gui/font_5x3_krull.png");
	font[FONT_PLINK] = Load_tex("sprites/gui/font_5x3_plink.png");
	font[FONT_SNEEB] = Load_tex("sprites/gui/font_5x3_sneeb.png");
	for(int i=0;i<256;i++){
		fonts[i].x=60;
		fonts[i].y=58;
	}
	fonts[48].x = 0; fonts[48].y = 0; // 0
	fonts[49].x = 4; fonts[49].y = 0; // 1
	fonts[50].x = 8; fonts[50].y = 0; // 2
	fonts[51].x= 12; fonts[51].y = 0; // 3
	fonts[52].x= 16; fonts[52].y = 0; // 4
	fonts[53].x= 20; fonts[53].y = 0; // 5
	fonts[54].x= 24; fonts[54].y = 0; // 6
	fonts[55].x= 28; fonts[55].y = 0; // 7
	fonts[56].x= 32; fonts[56].y = 0; // 8
	fonts[57].x= 36; fonts[57].y = 0; // 9
	fonts[45].x= 40; fonts[45].y = 0; // -
	fonts[43].x= 44; fonts[43].y = 0; // +
	fonts[61].x= 48; fonts[61].y = 0; // =
	fonts[37].x= 52; fonts[37].y = 0; // %
	fonts[42].x= 56; fonts[42].y = 0; // *
	fonts[96].x= 20; fonts[96].y =24; // `
	fonts[65].x = 0; fonts[65].y = 6; // A
	fonts[66].x = 4; fonts[66].y = 6; // B
	fonts[67].x = 8; fonts[67].y = 6; // C
	fonts[68].x =12; fonts[68].y = 6; // D
	fonts[69].x =16; fonts[69].y = 6; // E
	fonts[70].x =20; fonts[70].y = 6; // F
	fonts[71].x =24; fonts[71].y = 6; // G
	fonts[72].x =28; fonts[72].y = 6; // H
	fonts[73].x =32; fonts[73].y = 6; // I
	fonts[74].x =36; fonts[74].y = 6; // J
	fonts[75].x =40; fonts[75].y = 6; // K
	fonts[76].x =44; fonts[76].y = 6; // L
	fonts[77].x =48; fonts[77].y = 6; // M
	fonts[78].x =52; fonts[78].y = 6; // N
	fonts[79].x =56; fonts[79].y = 6; // O
	fonts[80].x = 0; fonts[80].y =12; // P
	fonts[81].x = 4; fonts[81].y =12; // Q
	fonts[82].x = 8; fonts[82].y =12; // R
	fonts[83].x =12; fonts[83].y =12; // S
	fonts[84].x =16; fonts[84].y =12; // T
	fonts[85].x =20; fonts[85].y =12; // U
	fonts[86].x =24; fonts[86].y =12; // V
	fonts[87].x =28; fonts[87].y =12; // W
	fonts[88].x =32; fonts[88].y =12; // X
	fonts[89].x =36; fonts[89].y =12; // Y
	fonts[90].x =40; fonts[90].y =12; // Z
	fonts[97].x = 0; fonts[97].y = 6; // a
	fonts[98].x = 4; fonts[98].y = 6; // b
	fonts[99].x = 8; fonts[99].y = 6; // c
	fonts[100].x=12; fonts[100].y= 6; // d
	fonts[101].x=16; fonts[101].y= 6; // e
	fonts[102].x=20; fonts[102].y= 6; // f
	fonts[103].x=24; fonts[103].y= 6; // g
	fonts[104].x=28; fonts[104].y= 6; // h
	fonts[105].x=32; fonts[105].y= 6; // i
	fonts[106].x=36; fonts[106].y= 6; // j
	fonts[107].x=40; fonts[107].y= 6; // k
	fonts[108].x=44; fonts[108].y= 6; // l
	fonts[109].x=48; fonts[109].y= 6; // m
	fonts[110].x=52; fonts[110].y= 6; // n
	fonts[111].x=56; fonts[111].y= 6; // o
	fonts[112].x= 0; fonts[112].y=12; // p
	fonts[113].x= 4; fonts[113].y=12; // q
	fonts[114].x= 8; fonts[114].y=12; // r
	fonts[115].x=12; fonts[115].y=12; // s
	fonts[116].x=16; fonts[116].y=12; // t
	fonts[117].x=20; fonts[117].y=12; // u
	fonts[118].x=24; fonts[118].y=12; // v
	fonts[119].x=28; fonts[119].y=12; // w
	fonts[120].x=32; fonts[120].y=12; // x
	fonts[121].x=36; fonts[121].y=12; // y
	fonts[122].x=40; fonts[122].y=12; // z
	fonts[33].x= 44; fonts[33].y= 12; // !
	fonts[63].x= 48; fonts[63].y= 12; // ?
	fonts[40].x= 52; fonts[40].y= 12; // (
	fonts[41].x= 56; fonts[41].y= 12; // )
	fonts[60].x = 4; fonts[60].y= 18; // >
	fonts[62].x = 0; fonts[62].y= 18; // <
	fonts[95].x = 8; fonts[95].y= 18; // _
	fonts[46].x= 12; fonts[46].y= 18; // .
	fonts[44].x= 16; fonts[44].y= 18; // ,
	fonts[39].x= 20; fonts[39].y= 18; // '
	fonts[34].x= 24; fonts[34].y= 18; // "
	fonts[58].x= 28; fonts[58].y= 18; // :
	fonts[59].x= 16; fonts[59].y= 24; // ;
	fonts[94].x= 32; fonts[94].y= 18; // ^
	fonts[91].x= 36; fonts[91].y= 18; // [
	fonts[93].x= 40; fonts[93].y= 18; // ]
	fonts[123].x=44; fonts[123].y=18; // {
	fonts[125].x=48; fonts[125].y=18; // }
	fonts[126].x=24; fonts[126].y=24; // }
	fonts[47].x= 52; fonts[47].y= 18; // forwardslash
	fonts[92].x= 56; fonts[92].y= 18; // backslash
	fonts[64].x = 0; fonts[64].y= 24; // @ smiley
	fonts[36].x = 4; fonts[36].y= 24; // $ poutey
	fonts[38].x = 8; fonts[38].y= 24; // & hmmmmm
	fonts[35].x= 12; fonts[35].y= 24; // # solid block
	fonts[32].x= 60; fonts[32].y= 0; // space
}

void main_menu_setup()
{
	int default_button;
/*	default_button = gui_add_text_button("combat", 0, 2, 35, BUTTON_STATE_ENABLED, BUTTON_STYLE_MENU, -1, &combat_setup, SCENE_COMBAT);
	gui_add_text_button("planets", 0, 10, 35 , BUTTON_STATE_ENABLED, BUTTON_STYLE_MENU, -1, &planet_setup, SCENE_PLANET_GEN);
	gui_add_text_button("trade", 0, 18, 35,  BUTTON_STATE_DISABLED, BUTTON_STYLE_MENU, -1, &travel_setup, SCENE_TRAVEL);
	gui_add_text_button("travel", 0, 36, 35,  BUTTON_STATE_ENABLED, BUTTON_STYLE_MENU, -1, &travel_setup, SCENE_TRAVEL);
	gui_add_text_button("nav", 0, 44, 35,  BUTTON_STATE_DISABLED, BUTTON_STYLE_MENU, -1, &travel_setup, SCENE_TRAVEL);
	gui_add_text_button("comms", 0, 52, 35,  BUTTON_STATE_ENABLED, BUTTON_STYLE_MENU, -1, &comms_setup, SCENE_COMMS);
*/
	default_button = gui_add_text_button("new game", 0, 2, 35, BUTTON_STATE_ENABLED, BUTTON_STYLE_MENU, -1, &travel_setup, SCENE_TRAVEL);
	gui_add_text_button("quit", 0, 10, 35, BUTTON_STATE_ENABLED, BUTTON_STYLE_MENU, -1, &menu_quit, 0);
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

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
	SDL_RenderSetLogicalSize(main_renderer, main_resX, main_resY);
	SDL_SetWindowTitle(main_window, "Hidden Star");
	atexit(SDL_Quit);

	srand(time(NULL));

	load_fonts();
	return 0;
}

void draw_scene(){
	switch(main_scene){
		case SCENE_INTRO:
			if(intro_draw()==1){
				//should be main menu
				gui_draw();
			}
			break;
		case SCENE_PLANET_GEN:
			planet_draw();
			break;
		case SCENE_COMBAT:
			combat_draw();
			break;
		case SCENE_TRAVEL:
			travel_draw();
			gui_draw();
			break;
		case SCENE_COMMS:
			comms_draw();
			gui_draw();
			break;
	}
}

int menu_quit()
{
	SDL_Event e;
	e.type = SDL_QUIT;
	SDL_PushEvent(&e);
}

void main_input(SDL_Event event){
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	mouseX /= main_scale;
	mouseY /= main_scale;

	//since gui junk isn't scene specific, we should handle gui input all the time?
	switch (event.type)
	{
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
			break;
	}

	switch(main_scene){
		case SCENE_INTRO:
			break;
		case SCENE_PLANET_GEN:
			planets_handle_input(event);
			break;
		case SCENE_COMBAT:
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
	generate_starmap();
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
				SDL_RenderPresent(main_renderer);
			}
		}
	}

	return 0;
}

#ifdef __MINGW64__
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	char* argv[0];
	main(0, argv);
}
#endif
