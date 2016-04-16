#ifndef HS_GUI
#define HS_GUI

#include "main.h"


#define BUTTON_STATE_DISABLED 0
#define BUTTON_STATE_ENABLED 1
#define BUTTON_STATE_SELECTED 2
#define BUTTON_MARGIN_VERTICAL 2
#define BUTTON_MARGIN_HORIZONTAL 4


typedef struct GUI_Button
{
	char* text;
	SDL_Rect button_bounds;
	SDL_Rect text_bounds;
	int state;
	int shortcut; //SDLK values?
	int (*action)();
} GUI_Button;

GUI_Button g_button_list[20];
int button_count;
int current_button;
SDL_Texture *g_button_bg[3];
SDL_Texture *g_button_cap[3];

int add_button(char* _text, int x, int y, int width, int state, int shortcut, int (*action)());
int update_button_state(int button, int state);
int gui_cycle_next_button(int direction);
int gui_update_hover_state(int x, int y);
void gui_do_button_action();
void gui_do_button_action_coords(int x, int y);
void gui_draw();
void gui_clear();
int gui_setup();

#endif
