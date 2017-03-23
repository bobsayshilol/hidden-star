#ifndef HS_TRADE
#define HS_TRADE

#include "main.h"
#include "util/vector.h"

Trade_Entity player;
Trade_Entity npc;
int npc_faction;

int trade_scroll_offset;
int trade_scroll_size;
int trade_button_scroll_up;
int trade_button_scroll_down;
Vector* trade_item_button_list;

int trade_setup();
void trade_set_faction(int f);
void trade_setup_gui();
void trade_setup_trade_buttons();
void trade_draw();

int trade_scroll_up(int v);
int trade_scroll_down(int v);
void trade_scroll_update_button_states();
int trade_buy(int v);
int trade_sell(int v);
int trade_cancel(int v);
int trade_query(int v);

#endif
