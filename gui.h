#ifndef HS_GUI
#define HS_GUI

#include "main.h"

#define BUTTON_STATE_DISABLED 0
#define BUTTON_STATE_ENABLED 1
#define BUTTON_STATE_SELECTED 2
#define BUTTON_MARGIN_VERTICAL 2
#define BUTTON_MARGIN_HORIZONTAL 4

#define BUTTON_STYLE_GUI 0
#define BUTTON_STYLE_MENU 1

#define GUI_TEXT_COLOR_DEFAULT (SDL_Color){255, 255, 255}
#define GUI_TEXT_COLOR_DISABLED (SDL_Color){20, 20, 20}
#define GUI_TEXT_COLOR_ENABLED (SDL_Color){20, 20, 20}
#define GUI_TEXT_COLOR_SELECTED (SDL_Color){255, 255, 255}
#define MENU_TEXT_COLOR_DISABLED (SDL_Color){100, 100, 100}
#define MENU_TEXT_COLOR_ENABLED (SDL_Color){200, 200, 200}
#define MENU_TEXT_COLOR_SELECTED (SDL_Color){255, 255, 255}


typedef struct GUI_Button
{
	char* text;
	SDL_Rect button_bounds;
	SDL_Rect text_bounds;
	int state;
	int style;
	int shortcut; //SDLK values?
	int (*action)();
} GUI_Button;

GUI_Button g_button_list[20];
SDL_Color g_button_text_colour[2][3];
int button_count;
int current_button;
SDL_Texture *g_button_bg[2][3];
SDL_Texture *g_button_cap[2][3];

int gui_add_button(char* _text, int x, int y, int width, int state, int style, int shortcut, int (*action)());
int update_button_state(int button, int state);
int gui_cycle_next_button(int direction);
int gui_seek_next_button_h(int direction);
int gui_seek_next_button_v(int direction);
int gui_update_hover_state(int x, int y);
void gui_do_button_action();
void gui_do_button_action_coords(int x, int y);
void gui_draw();
void gui_clear();
int gui_setup();

#endif