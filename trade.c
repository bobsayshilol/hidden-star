
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
	trade_item_name = "";
	trade_item_price = -1;
	trade_total = 0;
	trade_category_count = 0;
	trade_category = TRADE_ITEM_SOLID;

	trade_items = malloc(sizeof(Vector) * TRADE_ITEM_COUNT);
	for (int i = 0; i < TRADE_ITEM_COUNT; ++i)
	{
		trade_items[i] = malloc(sizeof(Vector));
		vector_init(trade_items[i], 5);
		trade_build_combined_inventory(trade_items[i], i);
	}

	trade_item_button_list = malloc(sizeof(Vector));
	vector_init(trade_item_button_list, 15);

	trade_setup_gui();

	music_stop(500);
	music_loop_group(MUSIC_ROLE_TRADE, npc_faction, 0, -1);

	//Populate npc with some random inventory

	return 0;
}

void trade_setup_items()
{
	economy_items = malloc(sizeof(Vector));
	vector_init(economy_items, 5);
	Trade_Item *ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_SOLID;
	ti->name = "Ultranium Ore";
	ti->description = "Ores of the higest ultranium quality. Unfortunately, that is quite low.";
	ti->base_value = 5;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_SOLID;
	ti->name = "Inert Snags";
	ti->description = "Sausages, packed for storage, ready to move.";
	ti->base_value = 13;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_SOLID;
	ti->name = "Petrified Poo";
	ti->description = "Dung from the primates of the ancient planet of Wanfestus, long turned to stone and now traded as a highly valued knicknack. Most commonly used as a joke gift for a friend who definitely would never want one.";
	ti->base_value = 50;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_SOLID;
	ti->name = "Solid Fuel";
	ti->description = "Solid fuel is great. It's not what your ship runs on though.";
	ti->base_value = 44;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_LIQUID;
	ti->name = "Lava";
	ti->description = "Some very hot rocks.";
	ti->base_value = 25;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_LIQUID;
	ti->name = "Algresian Ale";
	ti->description = "It smells like water, it tastes light water, and chemical analysis confirms it's water. You think the Algresians must be lightweight, and you're right. They're a race of sentient feathers.";
	ti->base_value = 83;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_LIQUID;
	ti->name = "Eel Lubricant";
	ti->description = "Freshly squeezed.";
	ti->base_value = 17;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_LIQUID;
	ti->name = "Decanted Tena";
	ti->description = "Tena is the juice of a rare gourd found in the hinterlands of Greldar 4. It even comes with its own decanter.";
	ti->base_value = 36;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_LIQUID;
	ti->name = "Yak Milk";
	ti->description = "The yak trade yields many commodities. Most of them are milk.";
	ti->base_value = 6;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_LIFE;
	ti->name = "Han Solo";
	ti->description = "Smuggler, rogue, hero.";
	ti->base_value = 800;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_LIFE;
	ti->name = "Pet Rocks";
	ti->description = "You doubt they're alive, but the manifest says they're lifeforms.";
	ti->base_value = 3;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_LIFE;
	ti->name = "Circus Fleas";
	ti->description = "There's even a tiny trapeze.";
	ti->base_value = 20;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_LIFE;
	ti->name = "Dinosaurs";
	ti->description = "Spare no expense.";
	ti->base_value = 328;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_LIFE;
	ti->name = "Space Cattle";
	ti->description = "Cattle. In space.";
	ti->base_value = 61;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_LIFE;
	ti->name = "Dairy Yaks";
	ti->description = "Spacefolk gotta get their milks from somewheres.";
	ti->base_value = 75;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_LIFE;
	ti->name = "Howler Monkeys";
	ti->description = "They're really loud.";
	ti->base_value = 45;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_TECH;
	ti->name = "Toasters";
	ti->description = "Manufactured on Vexx.";
	ti->base_value = 26;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_TECH;
	ti->name = "Circuit Bands";
	ti->description = "Remember when circuits used to be made as boards? Me neither, that was millenia ago.";
	ti->base_value = 18;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_TECH;
	ti->name = "Transducers";
	ti->description = "What do these even do?";
	ti->base_value = 33;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_TECH;
	ti->name = "Bio FabKits";
	ti->description = "Bio FabKits are used to construct self contained biomes suitable for outposts or small scale colonisation.";
	ti->base_value = 500;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_TECH;
	ti->name = "Fab BioKits";
	ti->description = "For the discerning biotech on the move who needs to look FABULOUS at a moment's notice.";
	ti->base_value = 500;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_TECH;
	ti->name = "Weapon Parts";
	ti->description = "Containers full of misc weapon parts. Fingers crossed that they don't jostle into a fully functional Greeble Ray Beam Blaster during transport. It takes forever to get greebles off.";
	ti->base_value = 250;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_GAS;
	ti->name = "BumBum";
	ti->description = "The qualities of BumBum are unknown, but the Plink prize it highly. What they do with it is also unknown.";
	ti->base_value = 160;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_GAS;
	ti->name = "Earth Air";
	ti->description = "Earth air is rare. Rarer than the Earth itself.";
	ti->base_value = 380;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_GAS;
	ti->name = "Hentozime";
	ti->description = "Hentozime gas is primarily used in the construction of FabKits and fashionable yak hats. Gotta keep those yaks looking good.";
	ti->base_value = 40;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_GAS;
	ti->name = "Aaaaaargon";
	ti->description = "Isn't there a St. Aaaaaargh's in Cornwall?";
	ti->base_value = 30;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_GAS;
	ti->name = "Hyperwind";
	ti->description = "Wind from the clefts of the Prune of Dome. It's a bit pongy.";
	ti->base_value = 25;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_STRANGE;
	ti->name = "Weird Inducer";
	ti->description = "A strange rotary inducer. It radiates with dark energies.";
	ti->base_value = 150;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_STRANGE;
	ti->name = "Bizzare Screws";
	ti->description = "These are useless without some unlikely nuts. They hum with dark energies.";
	ti->base_value = 62;
	vector_add(economy_items, ti);

	ti = malloc(sizeof(Trade_Item));
	ti->type = TRADE_ITEM_STRANGE;
	ti->name = "Unlikely Nuts";
	ti->description = "These probably go on bizzare screws. They smell like dark energies.";
	ti->base_value = 30;
	vector_add(economy_items, ti);
}

void trade_build_combined_inventory(Vector *trade_item_category, int category)
{
	printf("Building combined inventory\n");
	if (trade_category_limits != NULL)
	{
		free(trade_category_limits);
	}
	trade_category_limits = malloc(sizeof(int) * TRADE_ITEM_COUNT);

	if (trade_mode == TRADE_MODE_BUY)
	{
		for (int i = 0; i < vector_get_size(trade_npc->cargo_items[category]); ++i)
		{
			Trade_Inventory_Item* ti = (Trade_Inventory_Item*)vector_get(trade_npc->cargo_items[category], i);
			if (ti->qty > 0)
			{
				Trade_Screen_Item* tsi = malloc(sizeof(Trade_Screen_Item));
				tsi->npc_qty = ti->qty;
				tsi->player_qty = 0;
				tsi->item = ti->item;
				tsi->price = tsi->item->base_value + rand() % tsi->item->base_value;
				vector_add(trade_item_category, tsi);
			}
		}
		
		for (int i = 0; i < TRADE_ITEM_COUNT; ++i)
		{
			trade_category_limits[i] = trade_player->cargo_limits[i];
			for (int j = 0; j < vector_get_size(trade_player->cargo_items[i]); ++j)
			{
				trade_category_limits[i] = trade_category_limits[i] - ((Trade_Inventory_Item*)vector_get(trade_player->cargo_items[i], j))->qty;
			}
		}
	}
	else //selling
	{
		for (int i = 0; i < vector_get_size(trade_player->cargo_items[category]); ++i)
		{
			Trade_Inventory_Item* ti = (Trade_Inventory_Item*)vector_get(trade_player->cargo_items[category], i);
			if (ti->qty > 0)
			{
				Trade_Screen_Item* tsi = malloc(sizeof(Trade_Screen_Item));
				tsi->npc_qty = 0;
				tsi->player_qty = ti->qty;
				tsi->item = ti->item;
				printf("%p %p %s %p\n", tsi, tsi->item, ti->item->name, ti->item);
				tsi->price = tsi->item->base_value - rand() % (int)(tsi->item->base_value / 2 + 1) + 1;
				vector_add(trade_item_category, tsi);
			}
		}
		
		for (int i = 0; i < TRADE_ITEM_COUNT; ++i)
		{
			trade_category_limits[i] = trade_npc->cargo_limits[i];
			for (int j = 0; j < vector_get_size(trade_npc->cargo_items[i]); ++j)
			{
				trade_category_limits[i] = trade_category_limits[i] - ((Trade_Inventory_Item*)vector_get(trade_npc->cargo_items[i], j))->qty;
			}
		}
	}	
}

void trade_setup_entity(Trade_Entity* te)
{
	te->creds = rand() % 2500;

	te->cargo_limits = malloc(sizeof(int) * TRADE_ITEM_COUNT);
	te->cargo_limits[TRADE_ITEM_SOLID] = rand() % 20;
	te->cargo_limits[TRADE_ITEM_LIQUID] = rand() % 20;
	te->cargo_limits[TRADE_ITEM_GAS] = rand() % 20;
	te->cargo_limits[TRADE_ITEM_LIFE] = rand() % 20;
	te->cargo_limits[TRADE_ITEM_TECH] = rand() % 20;
	te->cargo_limits[TRADE_ITEM_STRANGE] = rand() % 20;

	te->cargo_items = malloc(sizeof(Vector) * TRADE_ITEM_COUNT);
	for (int i = 0; i < TRADE_ITEM_COUNT; ++i)
	{
		//FIXME: This malloc should be unnecessary? The space for it is allocated above, and the space for its contents should be allocated in vector_init()
		te->cargo_items[i] = malloc(sizeof(Vector));
		vector_init(te->cargo_items[i], 5);
	}
	
	for (int i = 0; i < vector_get_size(economy_items); ++i)
	{
		if (rand() % 10 < 4)
		{
			Trade_Inventory_Item *tii = malloc(sizeof(Trade_Inventory_Item));
			tii->qty = rand() % 10;
			tii->item = vector_get(economy_items, i);
			vector_add(te->cargo_items[tii->item->type], tii);
			te->cargo_limits[tii->item->type] = te->cargo_limits[tii->item->type] + tii->qty;
		}
	}
}

void trade_setup_player()
{
	trade_player = malloc(sizeof(Trade_Entity));

	trade_player->creds = rand() % 2500;

	trade_player->cargo_limits = malloc(sizeof(int) * TRADE_ITEM_COUNT);
	trade_player->cargo_limits[TRADE_ITEM_SOLID] = 20;
	trade_player->cargo_limits[TRADE_ITEM_LIQUID] = 20;
	trade_player->cargo_limits[TRADE_ITEM_GAS] = 20;
	trade_player->cargo_limits[TRADE_ITEM_LIFE] = 20;
	trade_player->cargo_limits[TRADE_ITEM_TECH] = 20;
	trade_player->cargo_limits[TRADE_ITEM_STRANGE] = 20;

	trade_player->cargo_items = malloc(sizeof(Vector) * TRADE_ITEM_COUNT);
	for (int i = 0; i < TRADE_ITEM_COUNT; ++i)
	{
		//FIXME: This malloc should be unnecessary? The space for it is allocated above, and the space for its contents should be allocated in vector_init()
		trade_player->cargo_items[i] = malloc(sizeof(Vector));
		vector_init(trade_player->cargo_items[i], 5);
	}

	for (int i = 0; i < vector_get_size(economy_items); ++i)
	{
		if (rand() % 10 > -1)
		{
			Trade_Inventory_Item *tii = malloc(sizeof(Trade_Inventory_Item));
			tii->qty = rand() % 5;
			tii->item = vector_get(economy_items, i);
			vector_add(trade_player->cargo_items[tii->item->type], tii);
			trade_player->cargo_limits[tii->item->type] = trade_player->cargo_limits[tii->item->type] + tii->qty;
			printf("Adding %dx %s (%d)\n", tii->qty, tii->item->name, vector_get_size(trade_player->cargo_items[tii->item->type]));
		}
	}
}

void trade_set_faction(int f)
{
	//TODO: Get rid of this and replace it with some kind of structure that handles/tracks NPCs
	npc_faction = f;
}

void trade_set_npc_entity(Trade_Entity* te)
{
	printf("Setting NPC trade entity %p\n", te);
	//TODO: Get rid of this and replace it with some kind of structure that handles/tracks NPCs
	trade_npc = te;
}

void trade_set_mode(int m)
{
	trade_mode = m;
}

void trade_setup_gui()
{
	gui_clear();
	trade_category_buttons = malloc(sizeof(int) * TRADE_ITEM_COUNT);
	trade_category_buttons[TRADE_ITEM_SOLID] = gui_add_symbol_button_mini(SYMBOL_CARGO_SOLID, 1, 7, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_TRADE_CATEGORY_SELECTED, -1, &trade_update_category, TRADE_ITEM_SOLID, NULL, -1, NULL, -1);
	trade_category_buttons[TRADE_ITEM_LIQUID] = gui_add_symbol_button_mini(SYMBOL_CARGO_LIQUID, 7, 7, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_TRADE_CATEGORY, -1, &trade_update_category, TRADE_ITEM_LIQUID, NULL, -1, NULL, -1);
	trade_category_buttons[TRADE_ITEM_GAS] = gui_add_symbol_button_mini(SYMBOL_CARGO_GAS, 13, 7, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_TRADE_CATEGORY, -1, &trade_update_category, TRADE_ITEM_GAS, NULL, -1, NULL, -1);
	trade_category_buttons[TRADE_ITEM_LIFE] = gui_add_symbol_button_mini(SYMBOL_CARGO_LIFEFORM, 19, 7, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_TRADE_CATEGORY, -1, &trade_update_category, TRADE_ITEM_LIFE, NULL, -1, NULL, -1);
	trade_category_buttons[TRADE_ITEM_TECH] = gui_add_symbol_button_mini(SYMBOL_CARGO_TECH, 25, 7, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_TRADE_CATEGORY, -1, &trade_update_category, TRADE_ITEM_TECH, NULL, -1, NULL, -1);
	trade_category_buttons[TRADE_ITEM_STRANGE] = gui_add_symbol_button_mini(SYMBOL_CARGO_STRANGE, 31, 7, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_TRADE_CATEGORY, -1, &trade_update_category, TRADE_ITEM_STRANGE, NULL, -1, NULL, -1);

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

	trade_scroll_size = vector_get_size(trade_items[trade_category]);
	printf("Setting up trade buttons (%d, %d)\n", trade_scroll_offset, trade_scroll_size);

	for (int i = 0; i < 3; ++i)
	{
		if (trade_scroll_offset + i >= trade_scroll_size)
		{
			break;
		}
		int *temp = (int *)malloc(sizeof(int));
		Trade_Screen_Item* tempII = (Trade_Screen_Item*)vector_get(trade_items[trade_category], trade_scroll_offset + i);

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
		Trade_Screen_Item* temp = (Trade_Screen_Item*)vector_get(trade_items[trade_category], trade_scroll_offset + i);

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
		draw_text(2, 1, "You", strlen("You"), FONT_EARTH, -1, -1, GUI_DEFAULT_COLOR);
	}
	else
	{
		draw_text(2, 1, "Buyer", strlen("Seller"), FONT_EARTH, -1, -1, GUI_DEFAULT_COLOR);
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
	draw_text(63 - strlen(creds) * 4, 1, creds, strlen(creds), FONT_EARTH, -1, -1, GUI_DEFAULT_COLOR);

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

	//TODO: Draw cargo capacities
	char capacities_string[20];
	sprintf(capacities_string, "%d/%d", trade_category_count, trade_category_limits[trade_category]);
	draw_text(63 - strlen(capacities_string) * 4, 8, capacities_string, strlen(capacities_string), FONT_EARTH, -1, -1, GUI_DEFAULT_COLOR);
	

	//TODO: Draw cargo category labels?

	main_blit(g_symbols_mini_background, 1, 7, NOFLIP, &CARGO_COLOR_SOLID);
	main_blit(g_symbols_mini_background, 7, 7, NOFLIP, &CARGO_COLOR_LIQUID);
	main_blit(g_symbols_mini_background, 13, 7, NOFLIP, &CARGO_COLOR_GAS);
	main_blit(g_symbols_mini_background, 19, 7, NOFLIP, &CARGO_COLOR_LIFE);
	main_blit(g_symbols_mini_background, 25, 7, NOFLIP, &CARGO_COLOR_TECH);
	main_blit(g_symbols_mini_background, 31, 7, NOFLIP, &CARGO_COLOR_STRANGE);

	main_blit(g_symbols_mini_highlight, 1 + 6 * trade_category, 7, NOFLIP, &GUI_DEFAULT_COLOR);

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
	//TODO: Update category button styles?
	update_button_style(trade_category_buttons[trade_category], BUTTON_STYLE_TRADE_CATEGORY);
	trade_category = v;
	update_button_style(trade_category_buttons[trade_category], BUTTON_STYLE_TRADE_CATEGORY_SELECTED);
	trade_setup_trade_buttons();
	trade_scroll_offset = 0;
	trade_scroll_update_button_states();
	trade_category_count = 0;
	
	for (int i = 0; i < vector_get_size(trade_items[trade_category]); ++i)
	{
		trade_category_count = trade_category_count + ((Trade_Screen_Item*)vector_get(trade_items[trade_category], i))->npc_qty;
	}
	return 0;
}

int trade_item_hover(int v)
{
	Trade_Screen_Item* tsi = (Trade_Screen_Item*)vector_get(trade_items[trade_category], v);
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
	Trade_Screen_Item* tsi = (Trade_Screen_Item*)vector_get(trade_items[trade_category], v);
	int getting = 0;
	for (int i = 0; i < vector_get_size(trade_items[trade_category]); ++i)
	{
		getting = getting + ((Trade_Screen_Item*)vector_get(trade_items[trade_category], i))->npc_qty;
	}
	bool can_trade = true;
	if (tsi->npc_qty > 0)
	{
		if (trade_mode == TRADE_MODE_BUY)
		{
			if (trade_total + tsi->price > trade_player->creds)
			{
				can_trade = false;
			}

			getting = getting + 1;
			if (getting > trade_category_limits[trade_category])
			{
				can_trade = false;
				printf("Not enough space!\n");
			}
		}
		else
		{
			getting = getting - 1;
		}
	}
	else
	{
		can_trade = false;
		printf("None to trade!\n");
	}

	if (can_trade)
	{
		trade_category_count = getting;
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
	Trade_Screen_Item* tsi = (Trade_Screen_Item*)vector_get(trade_items[trade_category], v);
	int getting = 0;
	for (int i = 0; i < vector_get_size(trade_items[trade_category]); ++i)
	{
		getting = getting + ((Trade_Screen_Item*)vector_get(trade_items[trade_category], i))->npc_qty;
	}

	bool can_trade = true;
	if (tsi->player_qty > 0)
	{
		if (trade_mode == TRADE_MODE_SELL)
		{
			if (trade_total + tsi->price > trade_npc->creds)
			{
				can_trade = false;
			}

			getting = getting + 1;
			if (getting > trade_category_limits[trade_category])
			{
				can_trade = false;
				printf("Not enough space!\n");
			}
		}
		else
		{
			getting = getting - 1;
		}
	}
	else
	{
		can_trade = false;
		printf("None to trade!\n");
	}

	if (can_trade)
	{
		trade_category_count = getting;
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
	for (int i = 0; i <  TRADE_ITEM_COUNT; ++i)
	{
		Vector* trade_item_category = trade_items[i];
		while (vector_get_size(trade_item_category) > 0)
		{
			Trade_Inventory_Item* ti = (Trade_Inventory_Item *)vector_get(trade_item_category, 0);
			vector_remove(trade_item_category, 0);
			free(ti);
		}
		free (trade_item_category);
	}
	free (trade_items);
	comms_setup();
	//Return to comms (what happens if we're salvaging cargo from smashed ships? Do we want to show a cancel button in that case?)
	return 0;
}

int trade_apply(int v)
{
	//Adjust both entities' creds
	//Adjust both entities' inventory numbers, remove 0 qty entries
	//Return to comms (what happens if we're salvaging cargo from smashed ships?)
	trade_cancel(v);
	return 0;
}
