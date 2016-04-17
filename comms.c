#include "comms.h"

int comms_setup()
{
	//todo: Passed in subject name, subject, portrait, disposition/bounty/resource need
	subject_name = "from xornax 12";
	printf("loading comms...\n");
	main_scene = SCENE_COMMS;
	frame_skip=0;
	comms_draw_count = 0;

	comms_setup_intro();

	comms_load_npc_text();
	comms_load_player_choices();

	SDL_SetRenderDrawColor(main_renderer, 0x00, 0x00, 0x00, 255);
	return 0;
}

void comms_load_npc_text()
{
	//todo: load possible npc text from file and select appropriate one based on disposition/bounty/resource need
	comms_npc_text[COMMS_NPC_GREETING].text = "greetings @";
	comms_npc_text[COMMS_NPC_GREETING].next_state = COMMS_STATE_PLAYER_CHOICE;
	comms_npc_text[COMMS_NPC_GREETING].next_state_index = COMMS_PLAYER_CHOICE_MAIN;

	comms_npc_text[COMMS_NPC_DEFEND].text = "you want to fight?\nwe fight $";
	comms_npc_text[COMMS_NPC_DEFEND].next_state = COMMS_STATE_ENTER_COMBAT;
	comms_npc_text[COMMS_NPC_DEFEND].next_state_index = -1;

	comms_npc_text[COMMS_NPC_ATTACK_ACCEPT].text = "we are glad you\nembrace fight\nwillingly";
	comms_npc_text[COMMS_NPC_ATTACK_ACCEPT].next_state = COMMS_STATE_ENTER_COMBAT;
	comms_npc_text[COMMS_NPC_ATTACK_ACCEPT].next_state_index = -1;

	comms_npc_text[COMMS_NPC_ATTACK_FLEE].text = "you get away.\nfor today.";
	comms_npc_text[COMMS_NPC_ATTACK_FLEE].next_state = COMMS_STATE_ENTER_TRAVEL;
	comms_npc_text[COMMS_NPC_ATTACK_FLEE].next_state_index = -1;

	comms_npc_text[COMMS_NPC_ATTACK_PLEAD].text = "you too pitiful\nto fight";
	comms_npc_text[COMMS_NPC_ATTACK_PLEAD].next_state = COMMS_STATE_PLAYER_CHOICE;
	comms_npc_text[COMMS_NPC_ATTACK_PLEAD].next_state_index = COMMS_PLAYER_CHOICE_MAIN;

	comms_npc_text[COMMS_NPC_TRADE].text = "let's trade";
	comms_npc_text[COMMS_NPC_TRADE].next_state = COMMS_STATE_ENTER_TRADE;
	comms_npc_text[COMMS_NPC_TRADE].next_state_index = -1;

	comms_npc_text[COMMS_NPC_TRADE_ACCEPT].text = "good, let's trade";
	comms_npc_text[COMMS_NPC_TRADE_ACCEPT].next_state = COMMS_STATE_ENTER_TRADE;
	comms_npc_text[COMMS_NPC_TRADE_ACCEPT].next_state_index = -1;

	comms_npc_text[COMMS_NPC_TRADE_DECLINE].text = "no trade? oh well";
	comms_npc_text[COMMS_NPC_TRADE_DECLINE].next_state = COMMS_STATE_PLAYER_CHOICE;
	comms_npc_text[COMMS_NPC_TRADE_DECLINE].next_state_index = COMMS_PLAYER_CHOICE_MAIN;

	comms_npc_text[COMMS_NPC_INFO].text = "you want info?\nwe have none.";
	comms_npc_text[COMMS_NPC_INFO].next_state = COMMS_STATE_PLAYER_CHOICE;
	comms_npc_text[COMMS_NPC_INFO].next_state_index = COMMS_PLAYER_CHOICE_MAIN;

	comms_npc_text[COMMS_NPC_FAREWELL].text = "goodbye #";
	comms_npc_text[COMMS_NPC_FAREWELL].next_state = COMMS_STATE_ENTER_TRAVEL;
	comms_npc_text[COMMS_NPC_FAREWELL].next_state_index = -1;
}

void comms_load_player_choices()
{
	//default player choices
	//event-specific player choices should use whatever we use for the info dialogue trees
	comms_player_choices[COMMS_PLAYER_CHOICE_MAIN].text0 = "attack";
	comms_player_choices[COMMS_PLAYER_CHOICE_MAIN].choice0 = COMMS_NPC_DEFEND;
	comms_player_choices[COMMS_PLAYER_CHOICE_MAIN].text2 = "trade";
	comms_player_choices[COMMS_PLAYER_CHOICE_MAIN].choice2 = COMMS_NPC_TRADE;
	comms_player_choices[COMMS_PLAYER_CHOICE_MAIN].text3 = "talk";
	comms_player_choices[COMMS_PLAYER_CHOICE_MAIN].choice3 = COMMS_NPC_INFO;
	comms_player_choices[COMMS_PLAYER_CHOICE_MAIN].text1 = "disengage";
	comms_player_choices[COMMS_PLAYER_CHOICE_MAIN].choice1 = COMMS_NPC_FAREWELL;

	comms_player_choices[COMMS_PLAYER_CHOICE_TRADE].text0 = "agree";
	comms_player_choices[COMMS_PLAYER_CHOICE_TRADE].choice0 = COMMS_NPC_TRADE_ACCEPT;
	comms_player_choices[COMMS_PLAYER_CHOICE_TRADE].text1 = "decline";
	comms_player_choices[COMMS_PLAYER_CHOICE_TRADE].choice1 = COMMS_NPC_TRADE_DECLINE;
	comms_player_choices[COMMS_PLAYER_CHOICE_TRADE].text2 = NULL;
	comms_player_choices[COMMS_PLAYER_CHOICE_TRADE].choice2 -1;
	comms_player_choices[COMMS_PLAYER_CHOICE_TRADE].text3 = NULL;
	comms_player_choices[COMMS_PLAYER_CHOICE_TRADE].choice3 -1;

	comms_player_choices[COMMS_PLAYER_CHOICE_DEFEND].text0 = "attack";
	comms_player_choices[COMMS_PLAYER_CHOICE_DEFEND].choice0 = COMMS_NPC_ATTACK_ACCEPT;
	comms_player_choices[COMMS_PLAYER_CHOICE_DEFEND].text1 = "flee";
	comms_player_choices[COMMS_PLAYER_CHOICE_DEFEND].choice1 = COMMS_NPC_ATTACK_FLEE;
	comms_player_choices[COMMS_PLAYER_CHOICE_DEFEND].text2 = "plead";
	comms_player_choices[COMMS_PLAYER_CHOICE_DEFEND].choice2 = COMMS_NPC_ATTACK_PLEAD;
	comms_player_choices[COMMS_PLAYER_CHOICE_DEFEND].text3 = NULL;
	comms_player_choices[COMMS_PLAYER_CHOICE_DEFEND].choice3 = -1;
}

void comms_setup_intro()
{
	portrait_background = Load_tex("sprites/gui/comms_portrait_background.png");
	portrait_image = Load_tex("sprites/portraits/sneeb_1.png");
	subject = Load_tex("sprites/gui/comms_subject_placeholder.png");

	int w, h;
	SDL_QueryTexture(subject, NULL, NULL, &w, &h);
	comms_subject_pos[0] = -w;
	comms_subject_pos[1] = h = 32 - h / 2;
	comms_subject_final_pos = 64 - w * 0.75;

	comms_intro_text = ">incoming signal";
	SDL_QueryTexture(portrait_background, NULL, NULL, &w, &h);
	comms_portrait_background_pos[0] = 2;
	comms_portrait_background_pos[1] = -h;
	comms_portrait_background_final_pos = 0;

	comms_text_offset = h + 1;

	SDL_QueryTexture(portrait_image, NULL, NULL, &w, &h);
	comms_portrait_pos[0] = 2;
	comms_portrait_pos[1] = abs(comms_portrait_background_pos[1]) / 2 - h / 2;

	comms_state = COMMS_STATE_INTRO;
	current_npc_text = 0;
	current_player_choice = 0;
	gui_clear();

	int default_button;
	default_button = gui_add_button(">", 64 - 11, 64 - 9, 0, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &advance_comms);
	update_button_state(default_button, BUTTON_STATE_SELECTED);
}

void comms_setup_npc_text()
{
	comms_state = COMMS_STATE_NPC_TEXT;
	comms_subject_pos[0] = comms_subject_final_pos;
	comms_portrait_background_pos[1] = comms_portrait_background_final_pos;
	//current_npc_text = comms_npc_text[current_npc_text].next_state_index;
	gui_clear();

	int default_button;
	default_button = gui_add_button(">", 64 - 11, 64 - 9, 0, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &advance_comms);
	update_button_state(default_button, BUTTON_STATE_SELECTED);
}

void comms_setup_player_choices()
{
	comms_state = COMMS_STATE_PLAYER_CHOICE;
	current_player_choice = comms_npc_text[current_npc_text].next_state_index;
	comms_subject_pos[0] = comms_subject_final_pos;
	comms_portrait_background_pos[1] = comms_portrait_background_final_pos;
	current_npc_text = comms_npc_text[current_npc_text].next_state_index;

	gui_clear();

	int default_button;
	default_button = gui_add_button(comms_player_choices[current_player_choice].text0, 0, comms_text_offset - 1, 35, BUTTON_STATE_ENABLED, BUTTON_STYLE_MENU, -1, &comms_action0);

	if (comms_player_choices[current_player_choice].text1 != NULL)
	{
		gui_add_button(comms_player_choices[current_player_choice].text1, 0, comms_text_offset + 6, 35, BUTTON_STATE_ENABLED, BUTTON_STYLE_MENU, -1, &comms_action1);

		if (comms_player_choices[current_player_choice].text2 != NULL)
		{
			gui_add_button(comms_player_choices[current_player_choice].text2, 0, comms_text_offset + 13, 35, BUTTON_STATE_ENABLED, BUTTON_STYLE_MENU, -1, &comms_action2);

			if (comms_player_choices[current_player_choice].text3 != NULL)
			{
				gui_add_button(comms_player_choices[current_player_choice].text3, 0, comms_text_offset + 20, 35, BUTTON_STATE_ENABLED, BUTTON_STYLE_MENU, -1, &comms_action3);
			}
		}
	}
	update_button_state(default_button, BUTTON_STATE_SELECTED);
}

void comms_draw()
{
	comms_draw_count++;
	SDL_RenderClear(main_renderer);

	switch (comms_state)
	{
		case COMMS_STATE_INTRO:
			comms_draw_intro();
			break;
		case COMMS_STATE_NPC_TEXT:
			comms_draw_npc_text();
			break;
		case COMMS_STATE_PLAYER_CHOICE:
			comms_draw_player_choices();
			break;
	}
}

void comms_draw_intro()
{
	if (comms_subject_pos[0] < comms_subject_final_pos)
	{
		comms_subject_pos[0]++;
	}
	main_blit(subject, comms_subject_pos[0], comms_subject_pos[1], NOFLIP, NULL);

	if (comms_draw_count < (strlen(comms_intro_text) + strlen(subject_name)) * 2)
	{
		draw_text(1, 2, comms_intro_text, comms_draw_count / 2, FONT_EARTH, GUI_TEXT_COLOR_DEFAULT);
		if (comms_draw_count > strlen(comms_intro_text) * 2)
		{
			draw_text(1, 2, comms_intro_text, strlen(comms_intro_text), FONT_EARTH, GUI_TEXT_COLOR_DEFAULT);
			if (comms_draw_count < (strlen(comms_intro_text) + strlen(subject_name)) * 2)
			{
				draw_text(1, 8, subject_name, comms_draw_count / 2 - strlen(comms_intro_text), FONT_EARTH, GUI_TEXT_COLOR_DEFAULT);
			}
			else
			{
				draw_text(1, 8, subject_name, strlen(subject_name), FONT_EARTH, GUI_TEXT_COLOR_DEFAULT);
			}
		}
	}
	else
	{
		if (comms_draw_count < (strlen(comms_intro_text) + strlen(subject_name)) * 2 + 12)
		{
			draw_text(1, 2, comms_intro_text, strlen(comms_intro_text), FONT_EARTH, GUI_TEXT_COLOR_DEFAULT);
			draw_text(1, 8, subject_name, strlen(subject_name), FONT_EARTH, GUI_TEXT_COLOR_DEFAULT);
		}
		else
		{
			if (comms_portrait_background_pos[1] < comms_portrait_background_final_pos)
			{
				comms_portrait_background_pos[1]+= 2;
			}
			main_blit(portrait_background, comms_portrait_background_pos[0], comms_portrait_background_pos[1], NOFLIP, NULL);

			if (comms_portrait_background_pos[1] >= comms_portrait_background_final_pos)
			{
				main_blit(portrait_image, comms_portrait_pos[0], comms_portrait_pos[1], NOFLIP, NULL);
				advance_comms();
			}
		}
	}
}

void comms_draw_npc_text()
{
	main_blit(subject, comms_subject_pos[0], comms_subject_pos[1], NOFLIP, NULL);
	main_blit(portrait_background, comms_portrait_background_pos[0], comms_portrait_background_pos[1], NOFLIP, NULL);
	main_blit(portrait_image, comms_portrait_pos[0], comms_portrait_pos[1], NOFLIP, NULL);
	if (comms_draw_count < strlen(comms_npc_text[current_npc_text].text) * 2)
	{
		draw_text(1, comms_text_offset, comms_npc_text[current_npc_text].text, comms_draw_count / 2, FONT_EARTH, GUI_TEXT_COLOR_DEFAULT);
	}
	else
	{
		draw_text(1, comms_text_offset, comms_npc_text[current_npc_text].text, strlen(comms_npc_text[current_npc_text].text), FONT_EARTH, GUI_TEXT_COLOR_DEFAULT);
	}
}

void comms_draw_player_choices()
{
	main_blit(subject, comms_subject_pos[0], comms_subject_pos[1], NOFLIP, NULL);
	main_blit(portrait_background, comms_portrait_background_pos[0], comms_portrait_background_pos[1], NOFLIP, NULL);
	main_blit(portrait_image, comms_portrait_pos[0], comms_portrait_pos[1], NOFLIP, NULL);
}

//todo: make button actions accept a parameter so that we can do this sort of junk in one function
int comms_action0() { selected_player_choice = comms_player_choices[current_player_choice].choice0; advance_comms(); return 0; }
int comms_action1() { selected_player_choice = comms_player_choices[current_player_choice].choice1; advance_comms(); return 0; }
int comms_action2() { selected_player_choice = comms_player_choices[current_player_choice].choice2; advance_comms(); return 0; }
int comms_action3() { selected_player_choice = comms_player_choices[current_player_choice].choice3; advance_comms(); return 0; }

int advance_comms()
{
	comms_draw_count = 0;
	switch (comms_state)
	{
		case COMMS_STATE_INTRO:
			comms_state = COMMS_STATE_NPC_TEXT;
			comms_setup_npc_text();
			break;
		case COMMS_STATE_NPC_TEXT:
			switch(comms_npc_text[current_npc_text].next_state)
			{
				case COMMS_STATE_INTRO:
					//restart dialogue? don't know why we might want to do this. could be fun?
					comms_setup_intro();
					break;
				case COMMS_STATE_PLAYER_CHOICE:
					comms_setup_player_choices();
					break;
				case COMMS_STATE_NPC_TEXT:
					comms_setup_npc_text();
					break;
				case COMMS_STATE_ENTER_COMBAT:
					gui_clear();
					combat_setup(); //exit comms, start combat
					break;
				case COMMS_STATE_ENTER_TRAVEL:
					gui_clear();
					intro_setup(); 
					main_menu_setup(); //exit comms, start travel
					break;
				case COMMS_STATE_ENTER_TRADE:
					gui_clear();
					intro_setup();
					main_menu_setup(); //exit comms, start trade
					break;
			}
			break;
		case COMMS_STATE_PLAYER_CHOICE:
			comms_state = COMMS_STATE_NPC_TEXT;
			comms_setup_npc_text();
			current_npc_text = selected_player_choice;
			break;
	}

	return 0;
}
