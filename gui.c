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
int gui_add_text_button(char* text, int x, int y, int width, int state, int style, int shortcut, int (*action)(int v), int _action_value, int (*hover)(int v), int _hover_value, int (*hover_out)(int v), int _hover_out_value)
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

	GUI_Button *b = malloc(sizeof(GUI_Button));
	b->sprite = NULL;
	b->text = text;
	b->text_bounds = text_bounds;
	b->button_bounds = button_bounds;
	b->state = state;
	b->style = style;
	b->flip = NOFLIP;
	b->symbol = -1;
	b->shortcut = shortcut;
	b->action = action;
	b->action_value = _action_value;
	b->hover = hover;
	b->hover_value = _hover_value;
	b->hover_out = hover_out;
	b->hover_out_value = _hover_out_value;
	vector_add(g_button_list, b);
	return vector_get_size(g_button_list) - 1;
}

int gui_add_sprite_button(SDL_Texture* _sprite, int x, int y, int width, int state, int style, int shortcut, int (*action)(int v), int _action_value, int (*hover)(int v), int _hover_value, int (*hover_out)(int v), int _hover_out_value, int flip, SDL_Color color)
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

	GUI_Button *b = malloc(sizeof(GUI_Button));
	b->sprite = _sprite;
	b->text = "";
	b->text_bounds = text_bounds;
	b->button_bounds = button_bounds;
	b->state = state;
	b->style = style;
	b->flip = flip;
	b->symbol = -1;
	b->shortcut = shortcut;
	b->action = action;
	b->action_value = _action_value;
	b->hover = hover;
	b->hover_value = _hover_value;
	b->hover_out = hover_out;
	b->hover_out_value = _hover_out_value;
	b->color = color;
	vector_add(g_button_list, b);
	return vector_get_size(g_button_list) - 1;
}

int gui_add_symbol_button(int symbol, int x, int y, int width, int state, int style, int shortcut, int (*action)(int v), int _action_value, int (*hover)(int v), int _hover_value, int (*hover_out)(int v), int _hover_out_value)
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
	text_bounds.h = h;

	SDL_Rect button_bounds;
	button_bounds.x = x;
	button_bounds.y = y;
	button_bounds.w = text_bounds.w + SYMBOL_MARGIN_HORIZONTAL * 2;
	button_bounds.h = text_bounds.h + SYMBOL_MARGIN_VERTICAL * 2;

	GUI_Button *b = malloc(sizeof(GUI_Button));
	b->sprite = NULL;
	b->symbol = symbol;
	b->text = "";
	b->text_bounds = text_bounds;
	b->button_bounds = button_bounds;
	b->state = state;
	b->style = style;
	b->flip = NOFLIP;

	b->shortcut = shortcut;
	b->action = action;
	b->action_value = _action_value;
	b->hover = hover;
	b->hover_value = _hover_value;
	b->hover_out = hover_out;
	b->hover_out_value = _hover_out_value;
	vector_add(g_button_list, b);
	return vector_get_size(g_button_list) - 1;
}

int update_button_state(int button, int state)
{
	if (button > -1 && button < vector_get_size(g_button_list))
	{
		GUI_Button *old_button = NULL;
		if (current_button >= 0)
		{
			old_button = (GUI_Button *)vector_get(g_button_list, current_button);
		}
		GUI_Button *new_button = (GUI_Button *)vector_get(g_button_list, button);

		int old_state = new_button->state;

		if (state == BUTTON_STATE_SELECTED)
		{
			gui_do_button_hover_out(old_button);
			update_button_state(current_button, BUTTON_STATE_ENABLED);
			current_button = button;
			gui_do_button_hover(new_button);
		}
		if ((state == BUTTON_STATE_HIDDEN || state == BUTTON_STATE_DISABLED) && button == current_button){
			current_button=-1;
		}
		//TODO: Find next available button
		new_button->state = state;

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
	for(int i = 0; i < vector_get_size(g_button_list); i++)
	{
		GUI_Button *button = vector_get(g_button_list, i);
		SDL_Rect b = button->button_bounds;
		if (x > b.x && x < b.x + b.w)
		{
			if (y > b.y && y < b.y + b.h)
			{
				if (button->state == BUTTON_STATE_SELECTED)
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
	if (current_button > -1 && current_button < vector_get_size(g_button_list))
	{
		GUI_Button *button = vector_get(g_button_list, current_button);
		if (button->action != NULL)
		{
			button->action(button->action_value);
			//TODO: Handle non-zero returns
		}
	}
	else
	{
		printf("Invalid button %d. Cannot perform action.\n", current_button);
	}
}

void gui_do_button_hover(GUI_Button *button_hover)
{
	if (button_hover != NULL)
	{
		if (button_hover->hover != NULL)
		{
			button_hover->hover(button_hover->hover_value);
			//TODO: Handle non-zero returns
		}
	}
	else
	{
		//printf("Invalid button %d. Cannot perform hover action.\n", button_hover);
	}
}

void gui_do_button_hover_out(GUI_Button *button_hover)
{
	if (button_hover != NULL)
	{
		if (button_hover->hover_out != NULL)
		{
			button_hover->hover_out(button_hover->hover_out_value);
			//TODO: Handle non-zero returns
		}
	}
	else
	{
		//printf("Invalid button %d. Cannot perform hover action.\n", button_hover);
	}
}

void gui_draw()
{
	for(int i = 0; i <  vector_get_size(g_button_list); i++)
	{
		GUI_Button *button = vector_get(g_button_list, i);
		if (button->symbol >= 0)
		{
			//Draw symbol button
			if (button->style == BUTTON_STYLE_GUI)
			{
				for (int j = 0; j < button->text_bounds.w - 2; j++)
				{
					main_blit(g_button_bg[button->style][button->state], button->text_bounds.x + j, button->button_bounds.y, NOFLIP, NULL);
				}
				main_blit(g_button_cap[button->style][button->state], button->button_bounds.x, button->button_bounds.y, NOFLIP, NULL);
				main_blit(g_button_cap[button->style][button->state], button->button_bounds.x + button->button_bounds.w - SYMBOL_MARGIN_HORIZONTAL -2, button->button_bounds.y, FLIPH, NULL);
			}
			else
			{
				main_blit(g_button_cap[button->style][button->state], button->button_bounds.x, button->button_bounds.y, NOFLIP, NULL);
			}
			SDL_Color c = g_button_text_colour[button->style][button->state];
			SDL_Rect srect;
			srect.x = symbols[button->symbol].x;
			srect.y = symbols[button->symbol].y;
			srect.w = 5;
			srect.h = 5;
			SDL_SetTextureColorMod(g_symbols, c.r, c.g, c.b);
			SDL_RenderCopy(main_renderer, g_symbols, &srect, &button->text_bounds);
		}
		else if (button->sprite == NULL)
		{
			//Draw text button
			if (button->style == BUTTON_STYLE_GUI)
			{
				for (int j = 0; j < button->text_bounds.w; j++)
				{
					main_blit(g_button_bg[button->style][button->state], button->text_bounds.x + j, button->button_bounds.y, NOFLIP, NULL);
				}
				main_blit(g_button_cap[button->style][button->state], button->button_bounds.x, button->button_bounds.y, NOFLIP, NULL);
				main_blit(g_button_cap[button->style][button->state], button->button_bounds.x + button->button_bounds.w - BUTTON_MARGIN_HORIZONTAL, button->button_bounds.y, FLIPH, NULL);
			}
			else
			{
				main_blit(g_button_cap[button->style][button->state], button->button_bounds.x, button->button_bounds.y, NOFLIP, NULL);
			}
			draw_text(button->text_bounds.x, button->text_bounds.y, button->text, strlen(button->text), FONT_EARTH, -1, -1, g_button_text_colour[button->style][button->state]);
		}
		else
		{
			//Draw sprite button
			SDL_Color color;
			color=button->color;
			if (button->state == BUTTON_STATE_SELECTED)
			{
				if(button->style==BUTTON_STYLE_GUI){
					if(g_blink<4){
						draw_text( button->button_bounds.x+7, button->button_bounds.y+1, "]", 1, FONT_EARTH, -1, -1, GUI_DEFAULT_COLOR);
						draw_text( button->button_bounds.x-2, button->button_bounds.y+1, "[", 1, FONT_EARTH, -1, -1, GUI_DEFAULT_COLOR);
					}
				}else{
					color=GUI_DEFAULT_COLOR;
				}
				g_blink++;
				if(g_blink>4){g_blink=0;}
			}
			if(button->state!=BUTTON_STATE_HIDDEN){
				main_blit(button->sprite, button->button_bounds.x, button->button_bounds.y, button->flip, &color);
			}
		}
	}
}

int gui_cycle_next_button(int direction)
{
	//direction should be 0 or 1
	GUI_Button *button = vector_get(g_button_list, current_button);
	int temp = current_button;
	do
	{
		if (direction >= 1)
		{
			if (temp < vector_get_size(g_button_list) - 1)
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
				temp = vector_get_size(g_button_list) - 1;
			}
		}
		button = vector_get(g_button_list, temp);
	} while(button->state == BUTTON_STATE_DISABLED); //TODO: What happens if there are no enabled buttons?
	update_button_state(temp, BUTTON_STATE_SELECTED);
	return temp;
}

int gui_seek_next_button_h(int direction)
{
	//direction should be 0 or 1
	int closest_button = -1;

	int current_x = 0;
	int current_y = 0;
	if (current_button >= 0)
	{
		current_x = ((GUI_Button *)vector_get(g_button_list, current_button))->button_bounds.x;
		current_y = ((GUI_Button *)vector_get(g_button_list, current_button))->button_bounds.y;
	}

	if (direction >= 1)
	{
		int closest_x = 64;
		int closest_y = 64;
		for(int i = 0; i < vector_get_size(g_button_list); i++)
		{
			GUI_Button *button = (GUI_Button *)vector_get(g_button_list, i);
			if (button->state == BUTTON_STATE_ENABLED)
			{
				if (button->button_bounds.x - current_x > 0)
				{
					if (button->button_bounds.x - current_x == closest_x)
					{
						if (abs(button->button_bounds.y - current_y) < abs(closest_y))
						{
							closest_button = i;
							closest_x = button->button_bounds.x - current_x;
							closest_y = button->button_bounds.y - current_y;
						}
					}
					else if (button->button_bounds.x - current_x < closest_x)
					{
						closest_button = i;
						closest_x = button->button_bounds.x - current_x;
						closest_y = button->button_bounds.y - current_y;
					}
				}
			}
		}
	}
	else
	{
		int closest_x = -64;
		int closest_y = -64;
		for(int i = 0; i < vector_get_size(g_button_list); i++)
		{
			GUI_Button *button = (GUI_Button *)vector_get(g_button_list, i);
			if (button->state == BUTTON_STATE_ENABLED)
			{
				if (button->button_bounds.x - current_x < 0)
				{
					if (button->button_bounds.x - current_x == closest_x)
					{
						if (abs(button->button_bounds.y - current_y) < abs(closest_y))
						{
							closest_button = i;
							closest_x = button->button_bounds.x - current_x;
							closest_y = button->button_bounds.y - current_y;
						}
					}
					else if (button->button_bounds.x - current_x > closest_x)
					{
						closest_button = i;
						closest_x = button->button_bounds.x - current_x;
						closest_y = button->button_bounds.y - current_y;
					}
				}
			}
		}
	}
	update_button_state(closest_button, BUTTON_STATE_SELECTED);
	return closest_button;
}

int gui_seek_next_button_v(int direction)
{
	//direction should be 0 or 1
	int closest_button = -1;

	int current_x = 0;
	int current_y = 0;
	if (current_button >= 0)
	{
		current_x = ((GUI_Button *)vector_get(g_button_list, current_button))->button_bounds.x;
		current_y = ((GUI_Button *)vector_get(g_button_list, current_button))->button_bounds.y;
	}

	if (direction >= 1)
	{
		int closest_x = 64;
		int closest_y = 64;
		for(int i = 0; i < vector_get_size(g_button_list); i++)
		{
			GUI_Button *button = (GUI_Button *)vector_get(g_button_list, i);
			if (button->state == BUTTON_STATE_ENABLED)
			{
				if (button->button_bounds.y - current_y > 0)
				{
					if (button->button_bounds.y - current_y == closest_y)
					{
						if (abs(button->button_bounds.x - current_x) < abs(closest_x))
						{
							closest_button = i;
							closest_x = button->button_bounds.x - current_x;
							closest_y = button->button_bounds.y - current_y;
						}
					}
					else if (button->button_bounds.y - current_y < closest_y)
					{
						closest_button = i;
						closest_x = button->button_bounds.x - current_x;
						closest_y = button->button_bounds.y - current_y;
					}
				}
			}
		}
	}
	else
	{
		int closest_x = -64;
		int closest_y = -64;
		for(int i = 0; i < vector_get_size(g_button_list); i++)
		{
			GUI_Button *button = (GUI_Button *)vector_get(g_button_list, i);
			if (button->state == BUTTON_STATE_ENABLED)
			{
				if (button->button_bounds.y - current_y < 0)
				{
					if (button->button_bounds.y - current_y == closest_y)
					{
						if (abs(button->button_bounds.x - current_x) < abs(closest_x))
						{
							closest_button = i;
							closest_x = button->button_bounds.x - current_x;
							closest_y = button->button_bounds.y - current_y;
						}
					}
					else if (button->button_bounds.y - current_y > closest_y)
					{
						closest_button = i;
						closest_x = button->button_bounds.x - current_x;
						closest_y = button->button_bounds.y - current_y;
					}
				}
			}
		}
	}
	update_button_state(closest_button, BUTTON_STATE_SELECTED);
	return closest_button;
}

int gui_update_hover_state(int x, int y)
{
	for(int i = 0; i < vector_get_size(g_button_list); i++)
	{
		GUI_Button *button = (GUI_Button *)vector_get(g_button_list, i);
		SDL_Rect b = button->button_bounds;
		if (x > b.x && x < b.x + b.w)
		{
			if (y > b.y && y < b.y + b.h)
			{
				if (button->state == BUTTON_STATE_ENABLED)
				{
					update_button_state(i, BUTTON_STATE_SELECTED);
					return i;
				}
				return -1;
			}
		}
	}
	return -1;
}

/* Prevents previously declared buttons from displaying */
void gui_clear()
{
	while (vector_get_size(g_button_list) > 0)
	{
		GUI_Button *n = (GUI_Button *)vector_get(g_button_list, 0);
		vector_remove(g_button_list, 0);
		free(n);
	}

	current_button = -1;
	//TODO: Do we want to loop through and clear out the old data? Unless something goes wrong, we should never touch it anyway
}

int gui_setup()
{
	printf("Loading gui...\n");
	current_button = -1;

	g_button_list = malloc(sizeof(Vector));
	vector_init(g_button_list, 5);

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
