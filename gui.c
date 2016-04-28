#include "gui.h"

/* 
	text is a char array containing the text to be displayed on the button
	x is the button left coodrinate
	y is the button right coordinate
	width is the preferred width in virtual pixels (will expand to fit text)
	state is BUTTON_STATE_DISABLED, BUTTON_STATE_ENABLED or BUTTON_STATE_SELECTED
	shortcut is the keyboard shortcut for the button (to be implemented)
	action is the function to be called when the action is cliecked
*/
int gui_add_text_button(char* text, int x, int y, int width, int state, int style, int shortcut, int (*action)(int v), int _action_value)
{
	SDL_Rect text_bounds;
	text_bounds.x = x + BUTTON_MARGIN_HORIZONTAL;
	text_bounds.y = y + BUTTON_MARGIN_VERTICAL;
	int w = -1; //Pull off space following final character
	for (int i = 0; i < strlen(text); ++i)
	{
		w += fonts[(int)text[i]].a;
	}

	if (width - BUTTON_MARGIN_HORIZONTAL * 2 <= w)
	{
		text_bounds.w = w; //todo: Account for spaces?
	}
	else
	{
		text_bounds.w = width - BUTTON_MARGIN_HORIZONTAL * 2;
	}
	text_bounds.h = 5; //todo: account for wrapped text

	SDL_Rect button_bounds;
	button_bounds.x = x;
	button_bounds.y = y;
	button_bounds.w = text_bounds.w + BUTTON_MARGIN_HORIZONTAL * 2;
	button_bounds.h = text_bounds.h + BUTTON_MARGIN_VERTICAL * 2;

	GUI_Button b;
	b.sprite = NULL;
	b.text = text;
	b.text_bounds = text_bounds;
	b.button_bounds = button_bounds;
	b.state = state;
	b.style = style;
	b.flip = NOFLIP;
	b.symbol = -1;
	b.shortcut = shortcut;
	b.action = action;
	b.action_value = _action_value;
	g_button_list[button_count] = b;
	button_count ++;

	return button_count - 1;
}

int gui_add_sprite_button(SDL_Texture* _sprite, int x, int y, int width, int state, int style, int shortcut, int (*action)(int v), int _action_value, int flip, SDL_Color color)
{
	int w, h;
	SDL_QueryTexture(_sprite, NULL, NULL, &w, &h);

	SDL_Rect text_bounds;
	text_bounds.x = x;
	text_bounds.y = y;
	if (width <= w)
	{
		text_bounds.w = w;
	}
	else
	{
		text_bounds.w = width;
	}
	text_bounds.h = 5;

	SDL_Rect button_bounds;
	button_bounds.x = x;
	button_bounds.y = y;
	button_bounds.w = text_bounds.w;
	button_bounds.h = text_bounds.h;

	GUI_Button b;
	b.sprite = _sprite;
	b.text = "";
	b.text_bounds = text_bounds;
	b.button_bounds = button_bounds;
	b.state = state;
	b.style = style;
	b.flip = flip;
	b.symbol = -1;
	b.shortcut = shortcut;
	b.action = action;
	b.action_value = _action_value;
	b.color = color;
	g_button_list[button_count] = b;
	button_count ++;

	return button_count - 1;
}

int gui_add_symbol_button(int symbol, int x, int y, int width, int state, int style, int shortcut, int (*action)(int v), int _action_value)
{
	int w = 5;
	int h = 5;

	SDL_Rect text_bounds;
	text_bounds.x = x + SYMBOL_MARGIN_HORIZONTAL;
	text_bounds.y = y + SYMBOL_MARGIN_VERTICAL;
	if (width - SYMBOL_MARGIN_HORIZONTAL * 2 <= w)
	{
		text_bounds.w = w;
	}
	else
	{
		text_bounds.w = width - SYMBOL_MARGIN_HORIZONTAL * 2;
	}
	text_bounds.h = 5;

	SDL_Rect button_bounds;
	button_bounds.x = x;
	button_bounds.y = y;
	button_bounds.w = text_bounds.w + SYMBOL_MARGIN_HORIZONTAL * 2;
	button_bounds.h = text_bounds.h + SYMBOL_MARGIN_VERTICAL * 2;

	GUI_Button b;
	b.sprite = NULL;
	b.symbol = symbol;
	b.text = "";
	b.text_bounds = text_bounds;
	b.button_bounds = button_bounds;
	b.state = state;
	b.style = style;
	b.flip = NOFLIP;

	b.shortcut = shortcut;
	b.action = action;
	b.action_value = _action_value;
	g_button_list[button_count] = b;
	button_count ++;

	return button_count - 1;
}

int update_button_state(int button, int state)
{
	if (button > -1 && button < button_count)
	{
		int old_state = g_button_list[button].state;

		if (state == BUTTON_STATE_SELECTED)
		{
			update_button_state(current_button, BUTTON_STATE_ENABLED);
			current_button = button;
		}
		if ((state == BUTTON_STATE_HIDDEN || state == BUTTON_STATE_DISABLED) && button == current_button){
			current_button=-1;
		}
		//TODO: Find next available button
		g_button_list[button].state = state;

		return old_state;
	}
	else
	{
		return -1;
	}
}

int gui_do_button_action_coords(int x, int y)
{
	int handled=0;
	//FIXME: There's a stack of redundancy here that should be cleaned up.
	//We need to ensure that clicking triggers what's under the cursor regardless of whether keyboard input has moved focus away.
	gui_update_hover_state(x, y);
	for(int i = 0; i < button_count; i++)
	{
		SDL_Rect b = g_button_list[i].button_bounds;
		if (x > b.x && x < b.x + b.w)
		{
			if (y > b.y && y < b.y + b.h)
			{
				if (g_button_list[i].state == BUTTON_STATE_SELECTED)
				{
					gui_do_button_action();
					handled=1;
				}
				break;
			}
		}
	}
	return handled;
}

void gui_do_button_action()
{
	if (current_button > -1 && current_button < button_count)
	{
		g_button_list[current_button].action(g_button_list[current_button].action_value);
		//TODO: Handle non-zero returns
	}
	else
	{
		printf("Invalid button %d. Cannot perform action.\n", current_button);
	}
}

void gui_draw()
{
	for(int i = 0; i < button_count; i++)
	{
		if (g_button_list[i].symbol >= 0)
		{
			//Draw symbol button
			if (g_button_list[i].style == BUTTON_STYLE_GUI)
			{
				for (int j = 0; j < g_button_list[i].text_bounds.w - 2; j++)
				{
					main_blit(g_button_bg[g_button_list[i].style][g_button_list[i].state], g_button_list[i].text_bounds.x + j, g_button_list[i].button_bounds.y, NOFLIP, NULL);
				}
				main_blit(g_button_cap[g_button_list[i].style][g_button_list[i].state], g_button_list[i].button_bounds.x, g_button_list[i].button_bounds.y, NOFLIP, NULL);
				main_blit(g_button_cap[g_button_list[i].style][g_button_list[i].state], g_button_list[i].button_bounds.x + g_button_list[i].button_bounds.w - SYMBOL_MARGIN_HORIZONTAL -2, g_button_list[i].button_bounds.y, FLIPH, NULL);
			}
			else
			{
				main_blit(g_button_cap[g_button_list[i].style][g_button_list[i].state], g_button_list[i].button_bounds.x, g_button_list[i].button_bounds.y, NOFLIP, NULL);
			}
			SDL_Color c = g_button_text_colour[g_button_list[i].style][g_button_list[i].state];
			SDL_Rect srect;
			srect.x = symbols[g_button_list[i].symbol].x;
			srect.y = symbols[g_button_list[i].symbol].y;
			srect.w = 5;
			srect.h = 5;
			SDL_SetTextureColorMod(g_symbols, c.r, c.g, c.b);
			SDL_RenderCopy(main_renderer, g_symbols, &srect, &g_button_list[i].text_bounds);
		}
		else if (g_button_list[i].sprite == NULL)
		{
			//Draw text button
			if (g_button_list[i].style == BUTTON_STYLE_GUI)
			{
				for (int j = 0; j < g_button_list[i].text_bounds.w; j++)
				{
					main_blit(g_button_bg[g_button_list[i].style][g_button_list[i].state], g_button_list[i].text_bounds.x + j, g_button_list[i].button_bounds.y, NOFLIP, NULL);
				}
				main_blit(g_button_cap[g_button_list[i].style][g_button_list[i].state], g_button_list[i].button_bounds.x, g_button_list[i].button_bounds.y, NOFLIP, NULL);
				main_blit(g_button_cap[g_button_list[i].style][g_button_list[i].state], g_button_list[i].button_bounds.x + g_button_list[i].button_bounds.w - BUTTON_MARGIN_HORIZONTAL, g_button_list[i].button_bounds.y, FLIPH, NULL);
			}
			else
			{
				main_blit(g_button_cap[g_button_list[i].style][g_button_list[i].state], g_button_list[i].button_bounds.x, g_button_list[i].button_bounds.y, NOFLIP, NULL);
			}
			draw_text(g_button_list[i].text_bounds.x, g_button_list[i].text_bounds.y, g_button_list[i].text, strlen(g_button_list[i].text), FONT_EARTH, -1, -1, g_button_text_colour[g_button_list[i].style][g_button_list[i].state]);
		}
		else
		{
			//Draw sprite button
			SDL_Color color;
			color=g_button_list[i].color;
			if (g_button_list[i].state == BUTTON_STATE_SELECTED)
			{
				if(g_button_list[i].style==BUTTON_STYLE_GUI){
					if(g_blink<4){
						draw_text( g_button_list[i].button_bounds.x+7, g_button_list[i].button_bounds.y+1, "]", 1, FONT_EARTH, -1, -1, GUI_DEFAULT_COLOR);
						draw_text( g_button_list[i].button_bounds.x-2, g_button_list[i].button_bounds.y+1, "[", 1, FONT_EARTH, -1, -1, GUI_DEFAULT_COLOR);
					}
				}else{
					color=GUI_DEFAULT_COLOR;
				}
				g_blink++;
				if(g_blink>4){g_blink=0;}
			}
			if(g_button_list[i].state!=BUTTON_STATE_HIDDEN){
				main_blit(g_button_list[i].sprite, g_button_list[i].button_bounds.x, g_button_list[i].button_bounds.y, g_button_list[i].flip, &color);
			}
		}
	}
}

int gui_cycle_next_button(int direction)
{
	//direction should be 0 or 1
	int temp = current_button;
	do
	{
		if (direction >= 1)
		{
			if (temp < button_count - 1)
			{
				temp ++;
			}
			else
			{
				temp = 0;
			}
		}
		else
		{
			if (temp > 0)
			{
				temp --;
			}
			else
			{
				temp = button_count - 1;
			}
		}
	} while(g_button_list[temp].state == BUTTON_STATE_DISABLED); //TODO: What happens if there are no enabled buttons?
	update_button_state(temp, BUTTON_STATE_SELECTED);
}

int gui_seek_next_button_h(int direction)
{
	//direction should be 0 or 1
	int temp = current_button;
	int closest_button = -1;
	int current_x = g_button_list[temp].button_bounds.x;
	int current_y = g_button_list[temp].button_bounds.y;
	if (direction >= 1)
	{
		int closest_x = 64;
		int closest_y = 64;
		for(int i = 0; i < button_count; i++)
		{
			if (g_button_list[i].state == BUTTON_STATE_ENABLED)
			{
				if (g_button_list[i].button_bounds.x - current_x > 0)
				{
					if (g_button_list[i].button_bounds.x - current_x == closest_x)
					{
						if (abs(g_button_list[i].button_bounds.y - current_y) < abs(closest_y))
						{
							closest_button = i;
							closest_x = g_button_list[i].button_bounds.x - current_x;
							closest_y = g_button_list[i].button_bounds.y - current_y;
						}
					}
					else if (g_button_list[i].button_bounds.x - current_x < closest_x)
					{
						closest_button = i;
						closest_x = g_button_list[i].button_bounds.x - current_x;
						closest_y = g_button_list[i].button_bounds.y - current_y;
					}
				}
			}
		}
	}
	else
	{
		int closest_x = -64;
		int closest_y = -64;
		for(int i = 0; i < button_count; i++)
		{
			if (g_button_list[i].state == BUTTON_STATE_ENABLED)
			{
				if (g_button_list[i].button_bounds.x - current_x < 0)
				{
					if (g_button_list[i].button_bounds.x - current_x == closest_x)
					{
						if (abs(g_button_list[i].button_bounds.y - current_y) < abs(closest_y))
						{
							closest_button = i;
							closest_x = g_button_list[i].button_bounds.x - current_x;
							closest_y = g_button_list[i].button_bounds.y - current_y;
						}
					}
					else if (g_button_list[i].button_bounds.x - current_x > closest_x)
					{
						closest_button = i;
						closest_x = g_button_list[i].button_bounds.x - current_x;
						closest_y = g_button_list[i].button_bounds.y - current_y;
					}
				}
			}
		}
	}
	update_button_state(closest_button, BUTTON_STATE_SELECTED);
}

int gui_seek_next_button_v(int direction)
{
	//direction should be 0 or 1
	int temp = current_button;
	int closest_button = -1;
	int current_x = g_button_list[temp].button_bounds.x;
	int current_y = g_button_list[temp].button_bounds.y;

	if (direction >= 1)
	{
		int closest_x = 64;
		int closest_y = 64;
		for(int i = 0; i < button_count; i++)
		{
			if (g_button_list[i].state == BUTTON_STATE_ENABLED)
			{
				if (g_button_list[i].button_bounds.y - current_y > 0)
				{
					if (g_button_list[i].button_bounds.y - current_y == closest_y)
					{
						if (abs(g_button_list[i].button_bounds.x - current_x) < abs(closest_x))
						{
							closest_button = i;
							closest_x = g_button_list[i].button_bounds.x - current_x;
							closest_y = g_button_list[i].button_bounds.y - current_y;
						}
					}
					else if (g_button_list[i].button_bounds.y - current_y < closest_y)
					{
						closest_button = i;
						closest_x = g_button_list[i].button_bounds.x - current_x;
						closest_y = g_button_list[i].button_bounds.y - current_y;
					}
				}
			}
		}
	}
	else
	{
		int closest_x = -64;
		int closest_y = -64;
		for(int i = 0; i < button_count; i++)
		{
			if (g_button_list[i].state == BUTTON_STATE_ENABLED)
			{
				if (g_button_list[i].button_bounds.y - current_y < 0)
				{
					if (g_button_list[i].button_bounds.y - current_y == closest_y)
					{
						if (abs(g_button_list[i].button_bounds.x - current_x) < abs(closest_x))
						{
							closest_button = i;
							closest_x = g_button_list[i].button_bounds.x - current_x;
							closest_y = g_button_list[i].button_bounds.y - current_y;
						}
					}
					else if (g_button_list[i].button_bounds.y - current_y > closest_y)
					{
						closest_button = i;
						closest_x = g_button_list[i].button_bounds.x - current_x;
						closest_y = g_button_list[i].button_bounds.y - current_y;
					}
				}
			}
		}
	}
	update_button_state(closest_button, BUTTON_STATE_SELECTED);
}

int gui_update_hover_state(int x, int y)
{
	for(int i = 0; i < button_count; i++)
	{
		SDL_Rect b = g_button_list[i].button_bounds;
		if (x > b.x && x < b.x + b.w)
		{
			if (y > b.y && y < b.y + b.h)
			{
				if (g_button_list[i].state == BUTTON_STATE_ENABLED)
				{
					update_button_state(i, BUTTON_STATE_SELECTED);
				}
				break;
			}
		}
	}
}

/* Prevents previously declared buttons from displaying */
void gui_clear()
{
	button_count = 0;
	current_button = -1;
	//TODO: Do we want to loop through and clear out the old data? Unless something goes wrong, we should never touch it anyway
}

int gui_setup()
{
	printf("Loading gui...\n");
	button_count = 0;
	current_button = -1;
	g_card_N=Load_tex("sprites/directional_card.png");
	g_card_E=Load_tex("sprites/directional_card_right.png");
	g_card_S=Load_tex("sprites/directional_card.png");
	g_card_W=Load_tex("sprites/directional_card_right.png");
	g_card_C=Load_tex("sprites/directional_mid.png");

	SDL_Texture *trans;
	trans = Load_tex("sprites/gui/trans1x1.png");

	g_button_text_colour[BUTTON_STYLE_GUI][BUTTON_STATE_DISABLED] = GUI_TEXT_COLOR_DISABLED;
	g_button_text_colour[BUTTON_STYLE_GUI][BUTTON_STATE_ENABLED] = GUI_TEXT_COLOR_ENABLED;
	g_button_text_colour[BUTTON_STYLE_GUI][BUTTON_STATE_SELECTED] = GUI_TEXT_COLOR_SELECTED;

	g_button_text_colour[BUTTON_STYLE_MENU][BUTTON_STATE_DISABLED] = MENU_TEXT_COLOR_DISABLED;
	g_button_text_colour[BUTTON_STYLE_MENU][BUTTON_STATE_ENABLED] = MENU_TEXT_COLOR_ENABLED;
	g_button_text_colour[BUTTON_STYLE_MENU][BUTTON_STATE_SELECTED] = MENU_TEXT_COLOR_SELECTED;

	g_button_bg[BUTTON_STYLE_GUI][BUTTON_STATE_DISABLED] = Load_tex("sprites/gui/button_background_d.png");
	g_button_bg[BUTTON_STYLE_GUI][BUTTON_STATE_ENABLED] = Load_tex("sprites/gui/button_background.png");
	g_button_bg[BUTTON_STYLE_GUI][BUTTON_STATE_SELECTED] = Load_tex("sprites/gui/button_background_h.png");
	g_button_cap[BUTTON_STYLE_GUI][BUTTON_STATE_DISABLED] = Load_tex("sprites/gui/button_left_d.png");
	g_button_cap[BUTTON_STYLE_GUI][BUTTON_STATE_ENABLED] = Load_tex("sprites/gui/button_left.png");
	g_button_cap[BUTTON_STYLE_GUI][BUTTON_STATE_SELECTED] = Load_tex("sprites/gui/button_left_h.png");

	g_button_bg[BUTTON_STYLE_MENU][BUTTON_STATE_DISABLED] = trans;
	g_button_bg[BUTTON_STYLE_MENU][BUTTON_STATE_ENABLED] = trans;
	g_button_bg[BUTTON_STYLE_MENU][BUTTON_STATE_SELECTED] = trans;
	g_button_cap[BUTTON_STYLE_MENU][BUTTON_STATE_DISABLED] = trans;
	g_button_cap[BUTTON_STYLE_MENU][BUTTON_STATE_ENABLED] = trans;
	g_button_cap[BUTTON_STYLE_MENU][BUTTON_STATE_SELECTED] = Load_tex("sprites/gui/menu_left_h.png");

	g_symbols = Load_tex("sprites/gui/symbols_5x5.png");

	symbols[SYMBOL_CARGO_LIQUID  ].x = 0; symbols[SYMBOL_CARGO_LIQUID  ].y = 6; symbols[SYMBOL_CARGO_LIQUID  ].a= 6;
	symbols[SYMBOL_CARGO_SOLID   ].x = 6; symbols[SYMBOL_CARGO_SOLID   ].y = 6; symbols[SYMBOL_CARGO_SOLID   ].a= 6;
	symbols[SYMBOL_CARGO_GAS     ].x =12; symbols[SYMBOL_CARGO_GAS     ].y = 6; symbols[SYMBOL_CARGO_GAS     ].a= 6;
	symbols[SYMBOL_CARGO_LIFEFORM].x =18; symbols[SYMBOL_CARGO_LIFEFORM].y = 6; symbols[SYMBOL_CARGO_LIFEFORM].a= 6;
	symbols[SYMBOL_CARGO_TECH    ].x =24; symbols[SYMBOL_CARGO_TECH    ].y = 6; symbols[SYMBOL_CARGO_TECH    ].a= 6;
	symbols[SYMBOL_CARGO_STRANGE ].x =30; symbols[SYMBOL_CARGO_STRANGE ].y = 6; symbols[SYMBOL_CARGO_STRANGE ].a= 6;
	symbols[SYMBOL_ARROW_DOWN    ].x = 0; symbols[SYMBOL_ARROW_DOWN    ].y = 0; symbols[SYMBOL_ARROW_DOWN    ].a= 6;
	symbols[SYMBOL_ARROW_UP      ].x = 6; symbols[SYMBOL_ARROW_UP      ].y = 0; symbols[SYMBOL_ARROW_UP      ].a= 6;
	symbols[SYMBOL_ARROW_LEFT    ].x =12; symbols[SYMBOL_ARROW_LEFT    ].y = 0; symbols[SYMBOL_ARROW_LEFT    ].a= 6;
	symbols[SYMBOL_ARROW_RIGHT   ].x =18; symbols[SYMBOL_ARROW_RIGHT   ].y = 0; symbols[SYMBOL_ARROW_RIGHT   ].a= 6;
	symbols[SYMBOL_ARROW_OMNI    ].x =24; symbols[SYMBOL_ARROW_OMNI    ].y = 0; symbols[SYMBOL_ARROW_OMNI    ].a= 6;
	symbols[SYMBOL_ARROW_CENTRE  ].x =30; symbols[SYMBOL_ARROW_CENTRE  ].y = 0; symbols[SYMBOL_ARROW_CENTRE  ].a= 6;
	symbols[SYMBOL_TRADE         ].x =36; symbols[SYMBOL_TRADE         ].y = 0; symbols[SYMBOL_TRADE         ].a= 6;
	symbols[SYMBOL_TRAVEL        ].x =42; symbols[SYMBOL_TRAVEL        ].y = 0; symbols[SYMBOL_TRAVEL        ].a= 6;
	symbols[SYMBOL_COMMS         ].x =48; symbols[SYMBOL_COMMS         ].y = 0; symbols[SYMBOL_COMMS         ].a= 6;
	symbols[SYMBOL_MENU          ].x =48; symbols[SYMBOL_MENU          ].y = 0; symbols[SYMBOL_MENU          ].a= 6;

	g_blink=0;
	return 0;
}
