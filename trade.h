#ifndef HS_TRADE
#define HS_TRADE

#include "main.h"
#include "util/vector.h"


#define TRADE_MODE_SELL 0
#define TRADE_MODE_BUY 1

typedef struct Trade_Screen_Item
{
	int player_qty;
	int npc_qty;
	int price;
	Trade_Item* item;
} Trade_Screen_Item;

int trade_mode;
int trade_query_screen; //TODO: Make this a proper mode?
Trade_Entity* trade_player;
Trade_Entity* trade_npc;
int npc_faction;
Vector* economy_items;
Vector** trade_items;
char* trade_item_name;
int trade_item_current;
int trade_item_price;
int trade_total;
int trade_category_count;
int* trade_category_limits;
int* trade_category_buttons;
int trade_category;

int trade_scroll_offset;
int trade_scroll_size;
int trade_button_scroll_up;
int trade_button_scroll_down;
int trade_button_info;
int trade_scroll_info_offset;
char* trade_scroll_info_text;
int trade_scroll_info_size;
int trade_button_scroll_info_up;
int trade_button_scroll_info_down;
Vector* trade_item_button_list;

int trade_setup();
void trade_setup_items();
void trade_build_combined_inventory();
void trade_setup_entity(Trade_Entity* te);
void trade_setup_player();
void trade_set_faction(int f);
void trade_set_npc_entity(Trade_Entity* te);
void trade_setup_gui();
void trade_setup_query_gui();
void trade_setup_trade_buttons();
void trade_draw();
void trade_draw_item_text();
void trade_draw_item_detail();

int trade_scroll_up(int v);
int trade_scroll_down(int v);
int trade_scroll_info_up(int v);
int trade_scroll_info_down(int v);
void trade_scroll_update_button_states();
void trade_scroll_info_update_button_states();
int trade_update_category(int v);
int trade_item_hover(int v);
int trade_item_out(int v);
int trade_buy(int v);
int trade_sell(int v);
int trade_cancel(int v);
int trade_query(int v);
int trade_query_hover(int v);
int trade_query_back(int v);

#endif
