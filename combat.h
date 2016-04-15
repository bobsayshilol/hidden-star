#ifndef HS_COMBAT
#define HS_COMBAT

#include "main.h"


int c_starX[6];
int c_starY[6];
int c_player_move1;
int c_player_move2;
int c_player_HP;
int c_player_dead;
int c_player_shipX;
int c_player_shipY;
int c_player_shot1X;
int c_player_shot1Y;
int c_player_weapon;
int c_player_thruster;
int c_action;
int c_AI_shipX;
int c_AI_shipY;
int c_AI_shot1X;
int c_AI_shot1Y;
int c_AI_HP;
int c_AI_dead;
int c_AI_weapon;
int c_AI_thruster;
int c_AI_round;
int c_aniframe1;
int c_time_pos;
int c_b1_AI[8];

SDL_Texture *c_stars1;
SDL_Texture *c_stars2;
SDL_Texture *c_stars3;
SDL_Texture *c_stars4;
SDL_Texture *c_stars5;
SDL_Texture *c_stars6;
SDL_Texture *c_ship1;
SDL_Texture *c_baddy1;
SDL_Texture *c_timerbar;
SDL_Texture *c_hud;
SDL_Texture *c_movetext;
SDL_Texture *c_firetext;
SDL_Texture *c_moveselect;
SDL_Texture *c_fireselect;
SDL_Texture *c_diag;
SDL_Texture *c_card;
SDL_Texture *c_card_right;
SDL_Texture *c_mid;
SDL_Texture *c_shot1;
SDL_Texture *c_shot2;
SDL_Texture *c_shot3;
SDL_Texture *c_explosion1;
SDL_Texture *c_explosion2;
SDL_Texture *c_miss1;

void combat_animate(SDL_Texture *tex, int x, int y, int frame);
void combat_draw_action_button(int direction, int x, int y, int mask);
void combat_draw_action_buttons();
void combat_draw_thruster(int x, int y, int thruster);
void combat_draw();
void combat_show(int time_pos);
void combat_move_stars(int xm, int ym, int d);
void combat_show_fight();
SDL_Color* combat_get_color(int index);
void combat_handle_input(SDL_Event event);
int combat_setup();

#endif
