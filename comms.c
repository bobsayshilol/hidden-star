#include "comms.h"

int comms_setup()
{
	printf("Loading comms...\n");

	//todo: Passed in subject name, subject, portrait, disposition/bounty/resource need
	portrait_background = Load_tex("sprites/gui/comms_portrait_background.png");
	portrait_image = Load_tex("sprites/portraits/sneeb_1.png");
	subject = Load_tex("sprites/gui/comms_subject_placeholder.png");
	subject_name = "from xornax 12";

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
	printf("Loading NPC text...\n");

	vector_free_and_free(&comms_npc_text);
	vector_init(&comms_npc_text, 10);
	vector_fill(&comms_npc_text, NULL);

	//todo: load possible npc text from file and select appropriate one based on disposition/bounty/resource need
	Comms_NPCDialogue *npcd;

	npcd = malloc(sizeof(Comms_NPCDialogue));
	npcd->text = "Greetings (@)";
	npcd->next_state = COMMS_STATE_PLAYER_CHOICE;
	npcd->next_state_index = COMMS_PLAYER_CHOICE_MAIN;
	vector_set(&comms_npc_text, COMMS_NPC_GREETING, npcd);

	npcd = malloc(sizeof(Comms_NPCDialogue));
	npcd->text = "You want to fight?\nWe fight ($)";
	npcd->next_state = COMMS_STATE_ENTER_COMBAT;
	npcd->next_state_index = -1;
	vector_set(&comms_npc_text, COMMS_NPC_DEFEND, npcd);

	npcd = malloc(sizeof(Comms_NPCDialogue));
	npcd->text = "We are glad you\nembrace fight\nwillingly";
	npcd->next_state = COMMS_STATE_ENTER_COMBAT;
	npcd->next_state_index = -1;
	vector_set(&comms_npc_text, COMMS_NPC_ATTACK_ACCEPT, npcd);

	npcd = malloc(sizeof(Comms_NPCDialogue));
	npcd->text ="You get away.\nfor today.";
	npcd->next_state = COMMS_STATE_ENTER_TRAVEL;
	npcd->next_state_index = -1;
	vector_set(&comms_npc_text, COMMS_NPC_ATTACK_FLEE, npcd);

	npcd = malloc(sizeof(Comms_NPCDialogue));
	npcd->text = "You are too pitiful\nto fight";
	npcd->next_state = COMMS_STATE_PLAYER_CHOICE;
	npcd->next_state_index = COMMS_PLAYER_CHOICE_MAIN;
	vector_set(&comms_npc_text, COMMS_NPC_ATTACK_PLEAD, npcd);

	npcd = malloc(sizeof(Comms_NPCDialogue));
	npcd->text = "No trade";
	npcd->next_state = COMMS_STATE_PLAYER_CHOICE;
	npcd->next_state_index = COMMS_PLAYER_CHOICE_MAIN;
	//npcd->next_state = COMMS_STATE_ENTER_TRADE;
	//npcd->next_state_index = -1;
	vector_set(&comms_npc_text, COMMS_NPC_TRADE, npcd);

	npcd = malloc(sizeof(Comms_NPCDialogue));
	npcd->text = "Good, let's trade";
	npcd->next_state = COMMS_STATE_ENTER_TRADE;
	npcd->next_state_index = -1;
	vector_set(&comms_npc_text, COMMS_NPC_TRADE_ACCEPT, npcd);

	npcd = malloc(sizeof(Comms_NPCDialogue));
	npcd->text = "No trade? oh well";
	npcd->next_state = COMMS_STATE_PLAYER_CHOICE;
	npcd->next_state_index = COMMS_PLAYER_CHOICE_MAIN;
	vector_set(&comms_npc_text, COMMS_NPC_TRADE_DECLINE, npcd);

	npcd = malloc(sizeof(Comms_NPCDialogue));
	npcd->text = "You want info?\nwe have none.";
	npcd->next_state = COMMS_STATE_PLAYER_CHOICE;
	npcd->next_state_index = COMMS_PLAYER_CHOICE_MAIN;
	vector_set(&comms_npc_text, COMMS_NPC_INFO, npcd);

	npcd = malloc(sizeof(Comms_NPCDialogue));
	npcd->text = "Goodbye (&)";
	npcd->next_state = COMMS_STATE_ENTER_TRAVEL;
	npcd->next_state_index = -1;
	vector_set(&comms_npc_text, COMMS_NPC_FAREWELL, npcd);

	printf("NPC text loaded\n");
}

void comms_load_player_choices()
{
	vector_free_and_free(&comms_player_choices);
	vector_init(&comms_player_choices, 3);
	vector_fill(&comms_player_choices, NULL);


	Comms_PlayerChoice *pc;

	pc = malloc(sizeof(Comms_PlayerChoice));
	pc->text0 = "attack";
	pc->choice0 = COMMS_NPC_DEFEND;
	pc->text1 = "trade";
	pc->choice1 = COMMS_NPC_TRADE;
	pc->text2 = "talk";
	pc->choice2 = COMMS_NPC_INFO;
	pc->text3 = "disengage";
	pc->choice3 = COMMS_NPC_FAREWELL;
	vector_set(&comms_player_choices, COMMS_PLAYER_CHOICE_MAIN, pc);

	pc = malloc(sizeof(Comms_PlayerChoice));
	pc->text0 = "aggree";
	pc->choice0 = COMMS_NPC_TRADE_ACCEPT;
	pc->text1 = "decline";
	pc->choice1 = COMMS_NPC_TRADE_DECLINE;
	pc->text2 = NULL;
	pc->choice2 = -1;
	pc->text3 = NULL;
	pc->choice3 = -1;
	vector_set(&comms_player_choices, COMMS_PLAYER_CHOICE_TRADE, pc);

	pc = malloc(sizeof(Comms_PlayerChoice));
	pc->text0 = "attack";
	pc->choice0 = COMMS_NPC_ATTACK_ACCEPT;
	pc->text1 = "flee";
	pc->choice1 = COMMS_NPC_ATTACK_FLEE;
	pc->text2 = "plead";
	pc->choice2 = COMMS_NPC_ATTACK_PLEAD;
	pc->text3 = NULL;
	pc->choice3 = -1;
	vector_set(&comms_player_choices, COMMS_PLAYER_CHOICE_DEFEND, pc);
}

void comms_setup_intro()
{
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

	comms_portrait_pos[0] = 2;
	comms_portrait_pos[1] = 0;

	comms_state = COMMS_STATE_INTRO;
	current_npc_text = 0;
	current_player_choice = 0;
	gui_clear();

	int default_button;
	default_button = gui_add_text_button(">", 64 - 11, 64 - 9, 0, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &comms_skip, -1);
	update_button_state(default_button, BUTTON_STATE_SELECTED);
}

void comms_setup_npc_text()
{
	comms_state = COMMS_STATE_NPC_TEXT;
	comms_subject_pos[0] = comms_subject_final_pos;
	comms_portrait_background_pos[1] = comms_portrait_background_final_pos;
	gui_clear();

	int default_button;
	default_button = gui_add_text_button(">", 64 - 11, 64 - 9, 0, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &comms_skip, -1);
	update_button_state(default_button, BUTTON_STATE_SELECTED);
}

void comms_setup_player_choices()
{
	comms_state = COMMS_STATE_PLAYER_CHOICE;
	current_player_choice = ((Comms_NPCDialogue *)vector_get(&comms_npc_text, current_npc_text))->next_state_index;
	comms_subject_pos[0] = comms_subject_final_pos;
	comms_portrait_background_pos[1] = comms_portrait_background_final_pos;
	current_npc_text = ((Comms_NPCDialogue *)vector_get(&comms_npc_text, current_npc_text))->next_state_index;

	gui_clear();

	int default_button;
	default_button = gui_add_text_button(((Comms_PlayerChoice *)vector_get(&comms_player_choices, current_player_choice))->text0, 0, comms_text_offset - 2, 35, BUTTON_STATE_ENABLED, BUTTON_STYLE_MENU, -1, &comms_action, ((Comms_PlayerChoice *)vector_get(&comms_player_choices, current_player_choice))->choice0);

	if (((Comms_PlayerChoice *)vector_get(&comms_player_choices, current_player_choice))->text1 != NULL)
	{
		gui_add_text_button(((Comms_PlayerChoice *)vector_get(&comms_player_choices, current_player_choice))->text1, 0, comms_text_offset + 5, 35, BUTTON_STATE_ENABLED, BUTTON_STYLE_MENU, -1, &comms_action, ((Comms_PlayerChoice *)vector_get(&comms_player_choices, current_player_choice))->choice1);

		if (((Comms_PlayerChoice *)vector_get(&comms_player_choices, current_player_choice))->text2 != NULL)
		{
			gui_add_text_button(((Comms_PlayerChoice *)vector_get(&comms_player_choices, current_player_choice))->text2, 0, comms_text_offset + 12, 35, BUTTON_STATE_ENABLED, BUTTON_STYLE_MENU, -1, &comms_action, ((Comms_PlayerChoice *)vector_get(&comms_player_choices, current_player_choice))->choice2);

			if (((Comms_PlayerChoice *)vector_get(&comms_player_choices, current_player_choice))->text3 != NULL)
			{
				gui_add_text_button(((Comms_PlayerChoice *)vector_get(&comms_player_choices, current_player_choice))->text3, 0, comms_text_offset + 19, 35, BUTTON_STATE_ENABLED, BUTTON_STYLE_MENU, -1, &comms_action, ((Comms_PlayerChoice *)vector_get(&comms_player_choices, current_player_choice))->choice3);
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
	if (comms_draw_count < strlen(((Comms_NPCDialogue *)vector_get(&comms_npc_text, current_npc_text))->text) * 2)
	{
		draw_text(1, comms_text_offset, ((Comms_NPCDialogue *)vector_get(&comms_npc_text, current_npc_text))->text, comms_draw_count / 2, FONT_EARTH, GUI_TEXT_COLOR_DEFAULT);
	}
	else
	{
		draw_text(1, comms_text_offset, ((Comms_NPCDialogue *)vector_get(&comms_npc_text, current_npc_text))->text, strlen(((Comms_NPCDialogue *)vector_get(&comms_npc_text, current_npc_text))->text), FONT_EARTH, GUI_TEXT_COLOR_DEFAULT);
	}
}

void comms_draw_player_choices()
{
	main_blit(subject, comms_subject_pos[0], comms_subject_pos[1], NOFLIP, NULL);
	main_blit(portrait_background, comms_portrait_background_pos[0], comms_portrait_background_pos[1], NOFLIP, NULL);
	main_blit(portrait_image, comms_portrait_pos[0], comms_portrait_pos[1], NOFLIP, NULL);
}

//todo: make button actions accept a parameter so that we can do this sort of junk in one function
int comms_action(int v)
{
	selected_player_choice = v;
	advance_comms();
	return 0;
}

int comms_skip(int v)
{
	advance_comms();
	return 0;
}

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
			switch(((Comms_NPCDialogue *)vector_get(&comms_npc_text, current_npc_text))->next_state)
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
					travel_setup(); //exit comms, start travel
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
