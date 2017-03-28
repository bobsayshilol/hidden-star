#ifndef HS_GUI
#define HS_GUI

#include "main.h"
#include "util/vector.h"

#define BUTTON_STATE_DISABLED 0
#define BUTTON_STATE_ENABLED 1
#define BUTTON_STATE_SELECTED 2
#define BUTTON_STATE_HIDDEN 3
#define BUTTON_STATE_COUNT 4

#define BUTTON_MARGIN_VERTICAL 2
#define BUTTON_MARGIN_HORIZONTAL 4
#define SYMBOL_MARGIN_VERTICAL 2
#define SYMBOL_MARGIN_HORIZONTAL 2

#define BUTTON_STYLE_MENU 0
#define BUTTON_STYLE_GUI 1
#define BUTTON_STYLE_STARMAP 2
#define BUTTON_STYLE_TRADE_CATEGORY 3
#define BUTTON_STYLE_TRADE_CATEGORY_SELECTED 4
#define BUTTON_STYLE_COUNT 5

#define GUI_DEFAULT_COLOR (SDL_Color){255, 255, 255}
#define GUI_MOVE_BUTTON_COLOR (SDL_Color){74, 142, 255}
#define GUI_FIRE_BUTTON_COLOR (SDL_Color){255, 0, 0}
#define GUI_TEXT_COLOR_DISABLED (SDL_Color){20, 20, 20}
#define GUI_TEXT_COLOR_ENABLED (SDL_Color){40, 40, 40}
#define GUI_TEXT_COLOR_SELECTED (SDL_Color){255, 255, 255}
#define MENU_TEXT_COLOR_DISABLED (SDL_Color){80, 80, 80}
#define MENU_TEXT_COLOR_ENABLED (SDL_Color){150, 150, 150}
#define MENU_TEXT_COLOR_SELECTED (SDL_Color){255, 255, 255}

#define CARGO_COLOR_SOLID (SDL_Color){190, 148, 69}
#define CARGO_COLOR_LIQUID (SDL_Color){53, 137, 188}
#define CARGO_COLOR_GAS (SDL_Color){185, 185, 185}
#define CARGO_COLOR_LIFE (SDL_Color){72, 159, 60}
#define CARGO_COLOR_TECH (SDL_Color){185, 185, 0}
#define CARGO_COLOR_STRANGE (SDL_Color){188, 116, 192}

#define SYMBOL_ARROW_DOWN 0
#define SYMBOL_ARROW_UP 1
#define SYMBOL_ARROW_LEFT 2
#define SYMBOL_ARROW_RIGHT 3
#define SYMBOL_ARROW_OMNI 4
#define SYMBOL_ARROW_CENTRE 5
#define SYMBOL_CARGO_LIQUID 6
#define SYMBOL_CARGO_SOLID 7
#define SYMBOL_CARGO_GAS 8
#define SYMBOL_CARGO_LIFEFORM 9
#define SYMBOL_CARGO_TECH 10
#define SYMBOL_CARGO_STRANGE 11
#define SYMBOL_TRADE 12
#define SYMBOL_TRAVEL 13
#define SYMBOL_COMMS 14
#define SYMBOL_MENU 15

#define GUI_SYMBOLS 0
#define GUI_SYMBOLS_MINI 1

typedef struct GUI_Button
{
	char* text;
	SDL_Texture* sprite;
	SDL_Rect button_bounds;
	SDL_Rect text_bounds;
	int state;
	int style;
	int flip;
	int shortcut; //SDLK values?
	int symbol;
	int action_value;
	int hover_value;
	int hover_out_value;
	int font;
	SDL_Color color;
	int (*action)(int v);
	int (*hover)(int v);
	int (*hover_out)(int v);
} GUI_Button;

font_map symbols[16];
Vector *g_button_list;
SDL_Color g_button_text_colour[BUTTON_STYLE_COUNT][BUTTON_STATE_COUNT];
int current_button;
int g_blink;

SDL_Texture *g_button_bg[BUTTON_STYLE_COUNT][BUTTON_STATE_COUNT];
SDL_Texture *g_button_cap[BUTTON_STYLE_COUNT][BUTTON_STATE_COUNT];
SDL_Texture *g_card_N;
SDL_Texture *g_card_E;
SDL_Texture *g_card_S;
SDL_Texture *g_card_W;
SDL_Texture *g_card_C;
SDL_Texture *g_symbols;
SDL_Texture *g_symbols_mini;
SDL_Texture *g_symbols_mini_background;
SDL_Texture *g_symbols_background;
SDL_Texture *g_symbols_mini_highlight;
SDL_Texture *g_computer_bg;
SDL_Texture *g_scroll_bg;

int gui_add_text_button(char* _text, int x, int y, int width, int state, int style, int shortcut, int (*action)(int v), int _action_value, int (*hover)(int v), int _hover_value, int (*hover_out)(int v), int _hover_out_value);
int gui_add_sprite_button(SDL_Texture* _sprite, int x, int y, int width, int state, int style, int shortcut, int (*action)(int v), int _action_value, int (*hover)(int v), int _hover_value, int (*hover_out)(int v), int _hover_out_value, int flip, SDL_Color color);
int gui_add_symbol_button(int symbol, int x, int y, int width, int state, int style, int shortcut, int (*action)(int v), int _action_value, int (*hover)(int v), int _hover_value, int (*hover_out)(int v), int _hover_out_value);
int gui_add_symbol_button_mini(int symbol, int x, int y, int width, int state, int style, int shortcut, int (*action)(int v), int _action_value, int (*hover)(int v), int _hover_value, int (*hover_out)(int v), int _hover_out_value);
int update_button_state(int button, int state);
int update_button_style(int button, int style);
int gui_cycle_next_button(int direction);
int gui_seek_next_button_h(int direction);
int gui_seek_next_button_v(int direction);
int gui_update_hover_state(int x, int y);
void gui_do_button_action();
void gui_do_button_hover(GUI_Button *hover_button);
void gui_do_button_hover_out(GUI_Button *hover_button);
int gui_do_button_action_coords(int x, int y);
void gui_draw();
void gui_clear();
void gui_remove_button(int button);
int gui_setup();

#endif
