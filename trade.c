
#include "trade.h"
#include "gui.h"

int trade_setup()
{
	printf("Loading trade...\n");
	main_scene = SCENE_TRADE;
	frame_skip=0;

	trade_mode = TRADE_MODE_SELL;

	npc_faction = 0;
	trade_scroll_offset = 0;
	trade_scroll_size = 0;
	trade_item_name = "";
	trade_item_price = -1;
	trade_total = 0;
	trade_category = TRADE_ITEM_SOLID;

	economy_items = malloc(sizeof(Vector));
	vector_init(economy_items, 5);
	Trade_Item *ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_SOLID;
	ti->name = "Ultranium Ore";
	ti->description = "Ores of the higest ultranium quality. Unfortunately, that is quite low.";
	ti->base_value = 1;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_SOLID;
	ti->name = "Inert Snags";
	ti->description = "Sausages, packed for storage, ready to move.";
	ti->base_value = 63;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_SOLID;
	ti->name = "Petrified Poo";
	ti->description = "Dung from the primates of the ancient planet of Wanfestus, long turned to stone and now traded as a highly valued knicknack. Most commonly used as a joke gift for a friend who definitely would never want one.";
	ti->base_value = 2;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_LIQUID;
	ti->name = "Lava";
	ti->description = "Some very hot rocks.";
	ti->base_value = 2;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_LIFE;
	ti->name = "Han Solo";
	ti->description = "Smuggler, rogue, hero.";
	ti->base_value = 2;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_TECH;
	ti->name = "Toasters";
	ti->description = "Manufactured on Vexx.";
	ti->base_value = 2;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_GAS;
	ti->name = "BumBum";
	ti->description = "The qualities of BumBum are unknown, but the Plink prize it highly. What they do with it is also unknown.";
	ti->base_value = 30;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_STRANGE;
	ti->name = "Weird Inducer";
	ti->description = "A strange rotary inducer. It radiates with dark energies.";
	ti->base_value = 2;
	vector_add(economy_items, ti);

	trade_npc = malloc(sizeof(Trade_Entity));
	trade_setup_entities(trade_npc);

	trade_player = malloc(sizeof(Trade_Entity));
	trade_setup_entities(trade_player);

	trade_items = malloc(sizeof(Vector));
	vector_init(trade_items, 5);
	trade_build_combined_inventory();

	trade_item_button_list = malloc(sizeof(Vector));
	vector_init(trade_item_button_list, 15);

	trade_setup_gui();

	music_stop(500);
	music_loop_group(MUSIC_ROLE_TRADE, npc_faction, 0, -1);

	//Populate npc with some random inventory

	return 0;
}

void trade_build_combined_inventory()
{
	while (vector_get_size(trade_items) > 0)
	{
		Trade_Inventory_Item* ti = (Trade_Inventory_Item *)vector_get(trade_items, 0);
		vector_remove(trade_items, 0);
		free(ti);
	}

	if (trade_mode == TRADE_MODE_BUY)
	{
		for (int i = 0; i < vector_get_size(trade_npc->cargo_items[trade_category]); ++i)
		{
			Trade_Inventory_Item* ti = (Trade_Inventory_Item*)vector_get(trade_npc->cargo_items[trade_category], i);
			if (ti->qty > 0)
			{
				Trade_Screen_Item* tsi = malloc(sizeof(Trade_Screen_Item));
				tsi->npc_qty = ti->qty;
				tsi->player_qty = 0;
				tsi->item = ti->item;
				tsi->price = tsi->item->base_value + rand() % tsi->item->base_value;
				vector_add(trade_items, tsi);
			}
		}
	}
	else //selling
	{
		for (int i = 0; i < vector_get_size(trade_player->cargo_items[trade_category]); ++i)
		{
			Trade_Inventory_Item* ti = (Trade_Inventory_Item*)vector_get(trade_npc->cargo_items[trade_category], i);
			if (ti->qty > 0)
			{
				Trade_Screen_Item* tsi = malloc(sizeof(Trade_Screen_Item));
				tsi->npc_qty = 0;
				tsi->player_qty = ti->qty;
				tsi->item = ti->item;
				tsi->price = tsi->item->base_value - rand() % (int)(tsi->item->base_value / 2 + 1) + 1;
				vector_add(trade_items, tsi);
			}
		}
	}
}

void trade_setup_entities(Trade_Entity* te)
{
	te->creds = rand() % 2500;

	//TODO: Initialise cargo_items and cargo_capacities lists
	te->cargo_capacities = malloc(sizeof(int) * TRADE_ITEM_COUNT);
	te->cargo_capacities[TRADE_ITEM_SOLID] = 20;
	te->cargo_capacities[TRADE_ITEM_LIQUID] = 20;
	te->cargo_capacities[TRADE_ITEM_GAS] = 20;
	te->cargo_capacities[TRADE_ITEM_LIFE] = 20;
	te->cargo_capacities[TRADE_ITEM_TECH] = 20;
	te->cargo_capacities[TRADE_ITEM_STRANGE] = 20;

	te->cargo_items = malloc(sizeof(Vector) * TRADE_ITEM_COUNT);
	for (int i = 0; i < TRADE_ITEM_COUNT; ++i)
	{
		//FIXME: This malloc should be unnecessary? The space for it is allocated above, and the space for its contents should be allocated in vector_init()
		te->cargo_items[i] = malloc(sizeof(Vector));
		vector_init(te->cargo_items[i], 5);
	}
	
	for (int i = 0; i < vector_get_size(economy_items); ++i)
	{
		Trade_Inventory_Item *tii = malloc(sizeof(Trade_Inventory_Item));
		tii->qty = rand() % 20;
		tii->item = vector_get(economy_items, i);
		vector_add(te->cargo_items[tii->item->type], tii);
	}
}

void trade_set_faction(int f)
{
	//TODO: Get rid of this and replace it with some kind of structure that handles/tracks NPCs
	npc_faction = f;
}

void trade_set_mode(int m)
{
	trade_mode = m;
}

void trade_setup_gui()
{
	gui_clear();
	gui_add_symbol_button_mini(SYMBOL_CARGO_SOLID, 27, 7, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_TRADE_CATEGORY, -1, &trade_update_category, TRADE_ITEM_SOLID, NULL, -1, NULL, -1);
	gui_add_symbol_button_mini(SYMBOL_CARGO_LIQUID, 33, 7, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_TRADE_CATEGORY, -1, &trade_update_category, TRADE_ITEM_LIQUID, NULL, -1, NULL, -1);
	gui_add_symbol_button_mini(SYMBOL_CARGO_GAS, 39, 7, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_TRADE_CATEGORY, -1, &trade_update_category, TRADE_ITEM_GAS, NULL, -1, NULL, -1);
	gui_add_symbol_button_mini(SYMBOL_CARGO_LIFEFORM, 45, 7, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_TRADE_CATEGORY, -1, &trade_update_category, TRADE_ITEM_LIFE, NULL, -1, NULL, -1);
	gui_add_symbol_button_mini(SYMBOL_CARGO_TECH, 51, 7, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_TRADE_CATEGORY, -1, &trade_update_category, TRADE_ITEM_TECH, NULL, -1, NULL, -1);
	gui_add_symbol_button_mini(SYMBOL_CARGO_STRANGE, 57, 7, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_TRADE_CATEGORY, -1, &trade_update_category, TRADE_ITEM_STRANGE, NULL, -1, NULL, -1);

	trade_button_scroll_down = gui_add_symbol_button(SYMBOL_ARROW_DOWN, 1, 55, -1, BUTTON_STATE_DISABLED, BUTTON_STYLE_GUI, -1, &trade_scroll_down, -1, NULL, -1, NULL, -1);
	trade_button_scroll_up = gui_add_symbol_button(SYMBOL_ARROW_UP, 9, 55, -1, BUTTON_STATE_DISABLED, BUTTON_STYLE_GUI, -1, &trade_scroll_up, -1, NULL, -1, NULL, -1);
	trade_button_info = gui_add_text_button("?", 64-12, 39, -1, BUTTON_STATE_DISABLED, BUTTON_STYLE_GUI, -1, &trade_query, -1, NULL, -1, NULL, -1);
	char *confirm_text;
	if (trade_mode == TRADE_MODE_BUY)
	{
		confirm_text = "Buy ";
	}
	else
	{
		confirm_text = "Sell";
	}
	gui_add_text_button(confirm_text, 64-47, 55, 21, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &trade_cancel, -1, NULL, -1, NULL, -1);

	int default_button;
	default_button = gui_add_text_button("Back", 64-24, 55, 21, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &trade_cancel, -1, NULL, -1, NULL, -1);

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
	//printf("trade item button list size %d\n", vector_get_size(trade_item_button_list));
	while(vector_get_size(trade_item_button_list) > 0)
	{
		int *temp = (int *)vector_get(trade_item_button_list, 0);
		//printf("removing button %d\n", *temp);
		gui_remove_button(*temp);
		vector_remove(trade_item_button_list, 0);
		free(temp);
	}

	trade_scroll_size = vector_get_size(trade_items);

	//TODO: combine NPC and player inventories, sort, then loop through and render buttons for each.
	//TODO: Clip and scroll
	for (int i = 0; i < 3; ++i)
	{
		if (trade_scroll_offset + i >= trade_scroll_size)
		{
			break;
		}
		int *temp = (int *)malloc(sizeof(int));
		Trade_Inventory_Item* tempII = (Trade_Inventory_Item*)vector_get(trade_npc->cargo_items[trade_category], trade_scroll_offset + i);

		*temp = gui_add_symbol_button(SYMBOL_ARROW_LEFT, 18, 15 + 8 * i, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, trade_scroll_offset + i, &trade_buy, trade_scroll_offset + i, &trade_item_hover, trade_scroll_offset + i, &trade_item_out, trade_scroll_offset + i);
		vector_add(trade_item_button_list, temp);

		temp = (int *)malloc(sizeof(int));
		int item_type = SYMBOL_CARGO_SOLID;
		if (tempII->item->type == TRADE_ITEM_SOLID)
		{
			item_type = SYMBOL_CARGO_SOLID;
		}
		else if (tempII->item->type == TRADE_ITEM_LIQUID)
		{
			item_type = SYMBOL_CARGO_LIQUID;
		}
		else if (tempII->item->type == TRADE_ITEM_GAS)
		{
			item_type = SYMBOL_CARGO_GAS;
		}
		else if (tempII->item->type == TRADE_ITEM_LIFE)
		{
			item_type = SYMBOL_CARGO_LIFEFORM;
		}
		else if (tempII->item->type == TRADE_ITEM_TECH)
		{
			item_type = SYMBOL_CARGO_TECH;
		}
		else if (tempII->item->type == TRADE_ITEM_STRANGE)
		{
			item_type = SYMBOL_CARGO_STRANGE;
		}
		*temp = gui_add_symbol_button(item_type, 28, 15 + 8 * i, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, trade_scroll_offset + i, &trade_buy, 0, &trade_item_hover, trade_scroll_offset + i, &trade_item_out, trade_scroll_offset + i);
		vector_add(trade_item_button_list, temp);

		temp = (int *)malloc(sizeof(int));
		*temp = gui_add_symbol_button(SYMBOL_ARROW_RIGHT, 64 - 26, 15 + 8 * i, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, trade_scroll_offset + i, &trade_sell, trade_scroll_offset + i, &trade_item_hover, trade_scroll_offset + i, &trade_item_out, trade_scroll_offset + i);
		vector_add(trade_item_button_list, temp);
	}
}

void trade_draw_item_text()
{
	for (int i = 0; i < 3; ++i)
	{
		if (trade_scroll_offset + i >= trade_scroll_size)
		{
			break;
		}
		Trade_Screen_Item* temp = (Trade_Screen_Item*)vector_get(trade_items, trade_scroll_offset + i);

		char player_qty[20];
		sprintf(player_qty, "%d", temp->player_qty);
		draw_text(18 - strlen(player_qty) * 4, 17 + 8 * i, player_qty, strlen(player_qty), FONT_EARTH, -1, -1, GUI_DEFAULT_COLOR);

		char npc_qty[20];
		sprintf(npc_qty, "%d", temp->npc_qty);
		draw_text(64 - strlen(npc_qty) * 4, 17 + 8 * i, npc_qty, strlen(npc_qty), FONT_EARTH, -1, -1, GUI_DEFAULT_COLOR);
	}
}

void trade_draw()
{
	SDL_RenderClear(main_renderer);

	if (trade_mode == TRADE_MODE_BUY)
	{
		draw_text(1, 1, "You", strlen("You"), FONT_EARTH, -1, -1, GUI_DEFAULT_COLOR);
	}
	else
	{
		draw_text(1, 1, "Seller", strlen("Seller"), FONT_EARTH, -1, -1, GUI_DEFAULT_COLOR);
	}
	char creds[11];
	if (trade_mode == TRADE_MODE_BUY)
	{
		sprintf(creds, "%d", trade_player->creds - trade_total);
	}
	else
	{
		sprintf(creds, "%d", trade_npc->creds - trade_total);
	}
	draw_text(64 - strlen(creds) * 4, 1, creds, strlen(creds), FONT_EARTH, -1, -1, GUI_DEFAULT_COLOR);

	if (strlen(trade_item_name) > 0)
	{
		draw_text(2, 41, trade_item_name, strlen(trade_item_name), FONT_EARTH, -1, -1, GUI_DEFAULT_COLOR);
	}

	char total_string[20];
	sprintf(total_string, "T: %d", trade_total);
	draw_text(2, 49, total_string, strlen(total_string), FONT_EARTH, -1, -1, GUI_DEFAULT_COLOR);

	if (trade_item_price >= 0)
	{
		char cost_string[20];
		if (trade_mode == TRADE_MODE_BUY)
		{
			sprintf(cost_string, "B: %d", trade_item_price);
		}
		else
		{
			sprintf(cost_string, "S: %d", trade_item_price);
		}
		draw_text(64 - strlen(cost_string) * 4, 49, cost_string, strlen(cost_string), FONT_EARTH, -1, -1, GUI_DEFAULT_COLOR);
	}

	main_blit(g_symbols_mini_background, 27, 7, NOFLIP, &CARGO_COLOR_SOLID);
	main_blit(g_symbols_mini_background, 33, 7, NOFLIP, &CARGO_COLOR_LIQUID);
	main_blit(g_symbols_mini_background, 39, 7, NOFLIP, &CARGO_COLOR_GAS);
	main_blit(g_symbols_mini_background, 45, 7, NOFLIP, &CARGO_COLOR_LIFE);
	main_blit(g_symbols_mini_background, 51, 7, NOFLIP, &CARGO_COLOR_TECH);
	main_blit(g_symbols_mini_background, 57, 7, NOFLIP, &CARGO_COLOR_STRANGE);

	g_symbols_mini_background = Load_tex("sprites/gui/symbol_background_mini.png");
	g_symbols_mini_hightlight = Load_tex("sprites/gui/symbol_highlight_mini.png");

	trade_draw_item_text();
}


int trade_scroll_up(int v)
{
	printf("Scrolling up from %d to ", trade_scroll_offset);
	if (trade_scroll_offset > 0)
	{
		trade_scroll_offset = trade_scroll_offset - 1;
		trade_setup_trade_buttons();
		trade_scroll_update_button_states();
		if (trade_scroll_offset > 0)
		{
			update_button_state(trade_button_scroll_up, BUTTON_STATE_SELECTED);
		}
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
		if (trade_scroll_offset < trade_scroll_size - 3)
		{
			update_button_state(trade_button_scroll_down, BUTTON_STATE_SELECTED);
		}
	}
	printf("%d\n", trade_scroll_offset);
	return 0;
}

void trade_scroll_update_button_states()
{
	if (trade_scroll_offset == 0)
	{
		update_button_state(trade_button_scroll_up, BUTTON_STATE_DISABLED);
		//printf("Disabling up button\n");
	}
	else if (trade_scroll_offset == 1)
	{
		update_button_state(trade_button_scroll_up, BUTTON_STATE_ENABLED);
		//printf("Enabling up button\n");
	}
	if (trade_scroll_offset == trade_scroll_size - 3)
	{
		update_button_state(trade_button_scroll_down, BUTTON_STATE_DISABLED);
		//printf("Disabling down button\n");
	}
	else if (trade_scroll_offset == trade_scroll_size - 4)
	{
		update_button_state(trade_button_scroll_down, BUTTON_STATE_ENABLED);
		//printf("Enabling down button\n");
	}
}

int trade_update_category(int v)
{
	trade_category = v;
	trade_build_combined_inventory();
	trade_setup_trade_buttons();
	return 0;
}

int trade_item_hover(int v)
{
	if (v < 0 || v >= vector_get_size(trade_npc->cargo_items[trade_category]))
	{
		return -1;
	}
	Trade_Screen_Item* tsi = (Trade_Screen_Item*)vector_get(trade_items, v);
	trade_item_name = tsi->item->name;
	trade_item_price = tsi->price;
	update_button_state(trade_button_info, BUTTON_STATE_ENABLED);
	return 0;
}

int trade_item_out(int v)
{
	trade_item_name = "";
	trade_item_price = -1;
	update_button_state(trade_button_info, BUTTON_STATE_DISABLED);
	return 0;
}

int trade_query(int v)
{
	return 0;
}

int trade_buy(int v)
{
	Trade_Screen_Item* tsi = (Trade_Screen_Item*)vector_get(trade_items, v);
	bool can_trade = true;
	if (tsi->npc_qty > 0)
	{
		if (trade_mode == TRADE_MODE_BUY)
		{
			if (trade_total + tsi->price > trade_player->creds)
			{
				can_trade = false;
			}
			//TODO: Check moneys, cargo capacity
		}
	}
	else
	{
		can_trade = false;
		printf("None to trade!\n");
	}

	if (can_trade)
	{
		tsi->npc_qty = tsi->npc_qty - 1;
		tsi->player_qty = tsi->player_qty + 1;
		if (trade_mode == TRADE_MODE_BUY)
		{
			trade_total = trade_total + tsi->price;
		}
		else
		{
			trade_total = trade_total - tsi->price;
		}
	}

	return 0;
}

int trade_sell(int v)
{
	Trade_Screen_Item* tsi = (Trade_Screen_Item*)vector_get(trade_items, v);
	bool can_trade = true;
	if (tsi->player_qty > 0)
	{
		if (trade_mode == TRADE_MODE_SELL)
		{
			if (trade_total + tsi->price > trade_npc->creds)
			{
				can_trade = false;
			}
			//TODO: Check moneys, cargo capacity
		}
	}
	else
	{
		can_trade = false;
		printf("None to trade!\n");
	}

	if (can_trade)
	{
		tsi->npc_qty = tsi->npc_qty + 1;
		tsi->player_qty = tsi->player_qty - 1;
		if (trade_mode == TRADE_MODE_BUY)
		{
			trade_total = trade_total - tsi->price;
		}
		else
		{
			trade_total = trade_total + tsi->price;
		}
	}
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
