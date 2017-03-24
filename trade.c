
#include "trade.h"
#include "gui.h"

int trade_setup()
{
	printf("Loading trade...\n");
	main_scene = SCENE_TRADE;
	frame_skip=0;
	
	npc_faction = 0;
	trade_scroll_offset = 0;
	trade_scroll_size = 0;
	trade_item_name = "";
	trade_item_buy = -1;
	trade_item_sell = -1;

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
	ti->type = TRADE_ITEM_SOLID;
	ti->name = "BumBum";
	ti->description = "The qualities of BumBum are unknown, but the Plink prize it highly. What they do with it is also unknown.";
	ti->base_value = 30;
	vector_add(economy_items, ti);





	trade_npc.creds = rand() % 2500;
	trade_npc.limit_solid = 20;
	trade_npc.limit_liquid = 20;
	trade_npc.limit_gas = 20;
	trade_npc.limit_life = 20;
	trade_npc.limit_tech = 20;
	trade_npc.limit_strange = 20;

	trade_npc.items_solid = malloc(sizeof(Vector));
	vector_init(trade_npc.items_solid, 5);
	trade_npc.items_liquid = malloc(sizeof(Vector));
	vector_init(trade_npc.items_liquid, 5);
	trade_npc.items_gas = malloc(sizeof(Vector));
	vector_init(trade_npc.items_gas, 5);
	trade_npc.items_life = malloc(sizeof(Vector));
	vector_init(trade_npc.items_life, 5);
	trade_npc.items_tech = malloc(sizeof(Vector));
	vector_init(trade_npc.items_tech, 5);
	trade_npc.items_strange = malloc(sizeof(Vector));
	vector_init(trade_npc.items_strange, 5);

	Trade_Inventory_Item* tii = malloc(sizeof(Trade_Inventory_Item));
	tii->qty = rand() % 20;
	tii->item = vector_get(economy_items, 0);
	vector_add(trade_npc.items_solid, tii);

	tii = malloc(sizeof(Trade_Inventory_Item));
	tii->qty = rand() % 20;
	tii->item = vector_get(economy_items, 1);
	vector_add(trade_npc.items_solid, tii);

	tii = malloc(sizeof(Trade_Inventory_Item));
	tii->qty = rand() % 20;
	tii->item = vector_get(economy_items, 2);
	vector_add(trade_npc.items_solid, tii);

	tii = malloc(sizeof(Trade_Inventory_Item));
	tii->qty = rand() % 20;
	tii->item = vector_get(economy_items, 3);
	vector_add(trade_npc.items_solid, tii);



	trade_player.creds = rand() % 2500;
	trade_player.limit_solid = 20;
	trade_player.limit_liquid = 20;
	trade_player.limit_gas = 20;
	trade_player.limit_life = 20;
	trade_player.limit_tech = 20;
	trade_player.limit_strange = 20;

	trade_player.items_solid = malloc(sizeof(Vector));
	vector_init(trade_player.items_solid, 5);
	trade_player.items_liquid = malloc(sizeof(Vector));
	vector_init(trade_player.items_liquid, 5);
	trade_player.items_gas = malloc(sizeof(Vector));
	vector_init(trade_player.items_gas, 5);
	trade_player.items_life = malloc(sizeof(Vector));
	vector_init(trade_player.items_life, 5);
	trade_player.items_tech = malloc(sizeof(Vector));
	vector_init(trade_player.items_tech, 5);
	trade_player.items_strange = malloc(sizeof(Vector));
	vector_init(trade_player.items_strange, 5);

	tii = malloc(sizeof(Trade_Inventory_Item));
	tii->qty = rand() % 20;
	tii->item = vector_get(economy_items, 0);
	vector_add(trade_player.items_solid, tii);

	tii = malloc(sizeof(Trade_Inventory_Item));
	tii->qty = rand() % 20;
	tii->item = vector_get(economy_items, 1);
	vector_add(trade_player.items_solid, tii);

	tii = malloc(sizeof(Trade_Inventory_Item));
	tii->qty = rand() % 20;
	tii->item = vector_get(economy_items, 2);
	vector_add(trade_player.items_solid, tii);

	tii = malloc(sizeof(Trade_Inventory_Item));
	tii->qty = rand() % 20;
	tii->item = vector_get(economy_items, 3);
	printf("tii->item %p\n", tii->item);
	vector_add(trade_player.items_solid, tii);


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
	trade_items = malloc(sizeof(Vector));
	vector_init(trade_items, 5);

	for (int i = 0; i < vector_get_size(trade_npc.items_solid); ++i)
	{
		Trade_Inventory_Item* ti = (Trade_Inventory_Item*)vector_get(trade_npc.items_solid, i);
		Trade_Screen_Item* tsi = malloc(sizeof(Trade_Screen_Item));
		tsi->npc_qty = ti->qty;
		tsi->item = ti->item;
		tsi->price_buy = tsi->item->base_value + rand() % tsi->item->base_value;
		tsi->price_sell = tsi->item->base_value - rand() % (int)(tsi->item->base_value / 2 + 1);
		if (tsi->price_sell == 0)
		{
			tsi->price_sell = 1;
		}
		vector_add(trade_items, tsi);
	}

	for (int i = 0; i < vector_get_size(trade_player.items_solid); ++i)
	{
		Trade_Inventory_Item* ti = (Trade_Inventory_Item*)vector_get(trade_player.items_solid, i);
		bool found = false;
		for (int j = 0; j < vector_get_size(trade_items); ++j)
		{
			Trade_Screen_Item* tsi = (Trade_Screen_Item*)vector_get(trade_items, j);
			if (ti->item == tsi->item)
			{
				found = true;
				tsi->player_qty = ti->qty;
				break;
			}
		}

		if (!found)
		{
			Trade_Screen_Item* tsi = malloc(sizeof(Trade_Screen_Item));
			tsi->npc_qty = 0;
			tsi->player_qty = ti->qty;
			tsi->item = ti->item;
			tsi->price_buy = tsi->item->base_value + rand() % tsi->item->base_value;
			tsi->price_sell = tsi->item->base_value - rand() % (int)(tsi->item->base_value / 2 + 1);
			if (tsi->price_sell == 0)
			{
				tsi->price_sell = 1;
			}
			vector_add(trade_items, tsi);
		}
	}
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
	trade_button_info = gui_add_text_button("?", 64-12, 38, -1, BUTTON_STATE_DISABLED, BUTTON_STYLE_GUI, -1, &trade_query, -1, NULL, -1, NULL, -1);
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
		Trade_Inventory_Item* tempII = (Trade_Inventory_Item*)vector_get(trade_npc.items_solid, trade_scroll_offset + i);
		Trade_Item* tempI = tempII->item;
		printf("adding buttons for %s\n", tempI->name);

		*temp = gui_add_symbol_button(SYMBOL_ARROW_LEFT, 18, 8 + 8 * i, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, trade_scroll_offset + i, &trade_buy, trade_scroll_offset + i, &trade_item_hover, trade_scroll_offset + i, &trade_item_out, trade_scroll_offset + i);
		vector_add(trade_item_button_list, temp);

		temp = (int *)malloc(sizeof(int));
		*temp = gui_add_symbol_button(SYMBOL_CARGO_LIFEFORM, 28, 8 + 8 * i, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, trade_scroll_offset + i, &trade_buy, 0, &trade_item_hover, trade_scroll_offset + i, &trade_item_out, trade_scroll_offset + i);
		vector_add(trade_item_button_list, temp);

		temp = (int *)malloc(sizeof(int));
		*temp = gui_add_symbol_button(SYMBOL_ARROW_RIGHT, 64 - 26, 8 + 8 * i, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, trade_scroll_offset + i, &trade_sell, trade_scroll_offset + i, &trade_item_hover, trade_scroll_offset + i, &trade_item_out, trade_scroll_offset + i);
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
		draw_text(18 - strlen(player_qty) * 4, 10 + 8 * i, player_qty, strlen(player_qty), FONT_EARTH, -1, -1, GUI_DEFAULT_COLOR);

		char npc_qty[20];
		sprintf(npc_qty, "%d", temp->npc_qty);
		draw_text(64 - strlen(npc_qty) * 4, 10 + 8 * i, npc_qty, strlen(npc_qty), FONT_EARTH, -1, -1, GUI_DEFAULT_COLOR);
	}
}

void trade_draw()
{
	SDL_RenderClear(main_renderer);
	draw_text(1, 2, "Creds", strlen("Creds"), FONT_EARTH, -1, -1, GUI_DEFAULT_COLOR);
	char* creds = "34987306";
	draw_text(64 - strlen(creds) * 4, 2, creds, strlen(creds), FONT_EARTH, -1, -1, GUI_DEFAULT_COLOR);

	if (strlen(trade_item_name) > 0)
	{
		draw_text(2, 40, trade_item_name, strlen(trade_item_name), FONT_EARTH, -1, -1, GUI_DEFAULT_COLOR);
	}

	if (trade_item_buy >= 0)
	{
		char buy_string[20];
		sprintf(buy_string, "B: %d", trade_item_buy);
		draw_text(2, 48, buy_string, strlen(buy_string), FONT_EARTH, -1, -1, GUI_DEFAULT_COLOR);
	}

	if (trade_item_sell >= 0)
	{
		char sell_string[20];
		sprintf(sell_string, "S: %d", trade_item_sell);
		draw_text(64 - strlen(sell_string) * 4, 48, sell_string, strlen(sell_string), FONT_EARTH, -1, -1, GUI_DEFAULT_COLOR);
	}

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
		printf("Disabling up button\n");
	}
	else if (trade_scroll_offset == 1)
	{
		update_button_state(trade_button_scroll_up, BUTTON_STATE_ENABLED);
		printf("Enabling up button\n");
	}
	if (trade_scroll_offset == trade_scroll_size - 3)
	{
		update_button_state(trade_button_scroll_down, BUTTON_STATE_DISABLED);
		printf("Disabling down button\n");
	}
	else if (trade_scroll_offset == trade_scroll_size - 4)
	{
		update_button_state(trade_button_scroll_down, BUTTON_STATE_ENABLED);
		printf("Enabling down button\n");
	}
}

int trade_item_hover(int v)
{
	if (v < 0 || v >= vector_get_size(trade_npc.items_solid))
	{
		return -1;
	}
	Trade_Screen_Item* tsi = (Trade_Screen_Item*)vector_get(trade_items, v);
	trade_item_name = tsi->item->name;
	trade_item_buy = tsi->price_buy;
	trade_item_sell = tsi->price_sell;
	update_button_state(trade_button_info, BUTTON_STATE_ENABLED);
	return 0;
}

int trade_item_out(int v)
{
	trade_item_name = "";
	trade_item_buy = -1;
	trade_item_sell = -1;
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
	if (tsi->npc_qty > 0)
	{
		tsi->npc_qty = tsi->npc_qty - 1;
		tsi->player_qty = tsi->player_qty + 1;
		//TODO: Check moneys, cargo capacity
	}
	else
	{
		printf("Can't buy! NPC has none!\n");
	}

	return 0;
}

int trade_sell(int v)
{
	Trade_Screen_Item* tsi = (Trade_Screen_Item*)vector_get(trade_items, v);
	if (tsi->player_qty > 0)
	{
		tsi->npc_qty = tsi->npc_qty + 1;
		tsi->player_qty = tsi->player_qty - 1;
		//TODO: Check moneys, cargo capacity
	}
	else
	{
		printf("Can't sell! Player has none!\n");
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
