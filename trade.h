#ifndef HS_TRADE
#define HS_TRADE

#include "main.h"
#include "util/vector.h"

Trade_Entity player;
Trade_Entity npc;
int npc_faction;

int trade_scroll_offset;
int trade_button_scroll_up;
int trade_button_scroll_down;

int trade_setup();
void trade_set_faction(int f);
void trade_setup_gui();
void trade_draw();

int trade_scroll_up(int v);
int trade_scroll_down(int v);
int trade_buy(int v);
int trade_sell(int v);
int trade_cancel(int v);
int trade_query(int v);

#endif
