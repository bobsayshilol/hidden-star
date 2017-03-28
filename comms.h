#ifndef HS_COMMS
#define HS_COMMS

#include "main.h"
#include "util/vector.h"

int comms_draw_count;

#define COMMS_STATE_INTRO 0
#define COMMS_STATE_NPC_TEXT 1
#define COMMS_STATE_PLAYER_CHOICE 2
#define COMMS_STATE_ENTER_COMBAT 3
#define COMMS_STATE_ENTER_TRAVEL 4
#define COMMS_STATE_ENTER_TRADE_BUY 5
#define COMMS_STATE_ENTER_TRADE_SELL 6

#define COMMS_PLAYER_CHOICE_MAIN 0
#define COMMS_PLAYER_CHOICE_TRADE 1
#define COMMS_PLAYER_CHOICE_TRADE_INVITE 2
#define COMMS_PLAYER_CHOICE_DEFEND 3
#define COMMS_PLAYER_CHOICE_COUNT 4

#define COMMS_NPC_GREETING 0
#define COMMS_NPC_DEFEND 1
#define COMMS_NPC_TRADE 2
#define COMMS_NPC_INFO 3
#define COMMS_NPC_FAREWELL 4
#define COMMS_NPC_TRADE_ACCEPT_BUY 5
#define COMMS_NPC_TRADE_ACCEPT_SELL 6
#define COMMS_NPC_TRADE_DECLINE 7
#define COMMS_NPC_ATTACK_ACCEPT 8
#define COMMS_NPC_ATTACK_FLEE 9
#define COMMS_NPC_ATTACK_PLEAD 10
#define COMMS_NPC_COUNT 11

#define COMMS_TONE_UNKNOWN 0
#define COMMS_TONE_NEUTRAL 1
#define COMMS_TONE_AGGRESSIVE 2
#define COMMS_TONE_HAPPY 3


typedef struct Comms_NPCDialogue
{
	char* text;
	int next_state;
	int next_state_index;
} Comms_NPCDialogue;

typedef struct Comms_PlayerChoice
{
	char* text0;
	char* text1;
	char* text2;
	char* text3;
	int choice0;
	int choice1;
	int choice2;
	int choice3;
} Comms_PlayerChoice;

SDL_Texture *portrait_background;
SDL_Texture *portrait_image;
SDL_Texture *subject;

char *comms_npc_line_files[10];
SDL_Texture *comms_faction_portraits[4];
SDL_Texture *comms_faction_ship_closeups[4][3];

Vector comms_player_choices;
Vector comms_current_npc_lines;
Vector comms_npc_lines;
int current_npc_text;
int current_player_choice;
int selected_player_choice;

int comms_state;
int comms_portrait_background_pos[2];
int comms_portrait_background_final_pos;
int comms_subject_pos[2];
int comms_subject_final_pos;
int comms_portrait_pos[2];
int comms_portrait_final_pos;
int comms_text_offset;
int comms_translation_offset;

char *comms_intro_text;
int comms_faction;
int comms_tone;

void comms_init();
int comms_setup();
void comms_return();
void comms_set_subject_name(char name[]);
void comms_set_faction(int f);
void prepare_npc_lists();
void prepare_line_lists(Vector comms_id[4][4]);
void load_default_npc_lines(Vector **line_type, char *fname, int next_state, int next_state_index);
void comms_set_current_npc_lines();
void comms_load_player_choices();
void comms_setup_intro();
void comms_setup_player_choices();
void comms_setup_npc_text();
void comms_draw();
void comms_draw_intro();
void comms_draw_npc_text();
void comms_draw_player_choices();
int advance_comms();
int comms_action(int v);
int comms_skip(int v);

#endif
