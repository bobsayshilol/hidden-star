
#include "trade.h"
#include "gui.h"

int trade_setup()
{
	printf("Loading trade...\n");
	main_scene = SCENE_TRADE;
	frame_skip=0;
	
	npc_faction = 0;
	trade_scroll_offset = 0;
	trade_scroll_size = 10;

	trade_item_button_list = malloc(sizeof(Vector));
	vector_init(trade_item_button_list, 15);

	trade_setup_gui();

	music_stop(500);
	music_loop_group(MUSIC_ROLE_TRADE, npc_faction, 0, -1);

	//Populate npc with some random inventory

	return 0;
}

void trade_set_faction(int f)
{
	//TODO: Get rid of this and replace it with some kind of structure that handles/tracks NPCs
	npc_faction = f;
}

void trade_setup_gui()
{
	gui_clear();
	trade_button_scroll_down = gui_add_symbol_button(SYMBOL_ARROW_DOWN, 1, 54, -1, BUTTON_STATE_DISABLED, BUTTON_STYLE_GUI, -1, &trade_scroll_down, -1, NULL, -1, NULL, -1);
	trade_button_scroll_up = gui_add_symbol_button(SYMBOL_ARROW_UP, 9, 54, -1, BUTTON_STATE_DISABLED, BUTTON_STYLE_GUI, -1, &trade_scroll_up, -1, NULL, -1, NULL, -1);
	gui_add_text_button("?", 64-12, 38, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &trade_query, -1, NULL, -1, NULL, -1);
	gui_add_text_button("Okay", 64-47, 54, 21, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &trade_cancel, -1, NULL, -1, NULL, -1);

	int default_button;
	default_button = gui_add_text_button("Back", 64-24, 54, 21, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &trade_cancel, -1, NULL, -1, NULL, -1);

	trade_setup_trade_buttons();

	if (trade_scroll_size > 3 && trade_scroll_offset < trade_scroll_size - 3)
	{
		update_button_state(trade_button_scroll_down, BUTTON_STATE_ENABLED);
		//draw clipped, but unselectable UI?
	}

	update_button_state(default_button, BUTTON_STATE_SELECTED);
}

void trade_setup_trade_buttons()
{
	//clear any existing known trade buttons
	printf("trade item button list size %d\n", vector_get_size(trade_item_button_list));
	while(vector_get_size(trade_item_button_list) > 0)
	{
		int *temp = (int *)vector_get(trade_item_button_list, 0);
		printf("removing button %d\n", *temp);
		gui_remove_button(*temp);
		vector_remove(trade_item_button_list, 0);
		free(temp);
	}

	//TODO: combine NPC and player inventories, sort, then loop through and render buttons for each.
	//TODO: Clip and scroll
	for (int i = 0; i < 3; ++i)
	{
		int *temp = (int *)malloc(sizeof(int));
		*temp = gui_add_symbol_button(SYMBOL_ARROW_LEFT, 18, 8 + 8 * i, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &trade_buy, 0, NULL, -1, NULL, -1);
		vector_add(trade_item_button_list, temp);
		printf("adding button %d\n", *temp);

		temp = (int *)malloc(sizeof(int));
		*temp = gui_add_symbol_button(SYMBOL_CARGO_LIFEFORM, 28, 8 + 8 * i, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &trade_buy, 0, NULL, -1, NULL, -1);
		vector_add(trade_item_button_list, temp);
		printf("adding button %d\n", *temp);

		temp = (int *)malloc(sizeof(int));
		*temp = gui_add_symbol_button(SYMBOL_ARROW_RIGHT, 64 - 26, 8 + 8 * i, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &trade_sell, 0, NULL, -1, NULL, -1);
		vector_add(trade_item_button_list, temp);
		printf("adding button %d\n", *temp);
	}
}

void trade_draw()
{
	SDL_RenderClear(main_renderer);
	draw_text(1, 2, "Creds", strlen("Creds"), FONT_EARTH, -1, -1, GUI_DEFAULT_COLOR);
	char* creds = "34987306";
	draw_text(64 - strlen(creds) * 4, 2, creds, strlen(creds), FONT_EARTH, -1, -1, GUI_DEFAULT_COLOR);


	char* itemString = "Han Solos";
	draw_text(2, 40, itemString, strlen(itemString), FONT_EARTH, -1, -1, GUI_DEFAULT_COLOR);

	char* buyString = "B: 105";
	draw_text(2, 48, buyString, strlen(buyString), FONT_EARTH, -1, -1, GUI_DEFAULT_COLOR);
	char* sellString = "S: 257";
	draw_text(64 - strlen(sellString) * 4, 48, sellString, strlen(sellString), FONT_EARTH, -1, -1, GUI_DEFAULT_COLOR);

}


int trade_scroll_up(int v)
{
	printf("Scrolling up from %d to ", trade_scroll_offset);
	if (trade_scroll_offset > 0)
	{
		trade_scroll_offset = trade_scroll_offset - 1;
		trade_setup_trade_buttons();
		trade_scroll_update_button_states();
		update_button_state(trade_button_scroll_up, BUTTON_STATE_SELECTED);
	}
	printf("%d\n", trade_scroll_offset);
	return 0;
}

int trade_scroll_down(int v)
{
	printf("Scrolling up from %d to ", trade_scroll_offset);
	if (trade_scroll_offset < trade_scroll_size - 3)
	{
		trade_scroll_offset = trade_scroll_offset + 1;
		trade_setup_trade_buttons();
		trade_scroll_update_button_states();
		update_button_state(trade_button_scroll_down, BUTTON_STATE_SELECTED);
	}
	printf("%d\n", trade_scroll_offset);
	return 0;
}

void trade_scroll_update_button_states()
{
	if (trade_scroll_offset == 0)
	{
		update_button_state(trade_button_scroll_up, BUTTON_STATE_DISABLED);
	}
	else if (trade_scroll_offset == 1)
	{
		update_button_state(trade_button_scroll_up, BUTTON_STATE_ENABLED);
	}
	else if (trade_scroll_offset == trade_scroll_size - 3)
	{
		update_button_state(trade_button_scroll_down, BUTTON_STATE_DISABLED);
	}
	else if (trade_scroll_offset == trade_scroll_size - 4)
	{
		update_button_state(trade_button_scroll_down, BUTTON_STATE_ENABLED);
	}
}

int trade_query(int v)
{
	return 0;
}

int trade_buy(int v)
{
	return 0;
}

int trade_sell(int v)
{
	return 0;
}

int trade_cancel(int v)
{
	return 0;
}

int trade_apply(int v)
{
	return 0;
}
