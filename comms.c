#include "comms.h"
#include "travel.h"

int comms_setup()
{
	printf("Loading comms...\n");

	//todo: Passed in subject name, subject, portrait, disposition/bounty/resource need
	portrait_background = Load_tex("sprites/gui/comms_portrait_background.png");
	if (portrait_image == NULL)
	{
		portrait_image = Load_tex("sprites/portraits/sneeb_1.png");
	}

	if (subject == NULL)
	{
		subject = Load_tex("sprites/gui/comms_subject_placeholder.png");
	}
	subject_name = "from xornax 12";

	comms_npc_line_files[COMMS_NPC_GREETING] = "text/comms_npc_greetings.tsv";
	comms_npc_line_files[COMMS_NPC_DEFEND] = "text/comms_npc_defend.tsv";
	comms_npc_line_files[COMMS_NPC_TRADE] = "text/comms_npc_trade.tsv";
	comms_npc_line_files[COMMS_NPC_INFO] = "text/comms_npc_info.tsv";
	comms_npc_line_files[COMMS_NPC_FAREWELL] = "text/comms_npc_farewell.tsv";
	comms_npc_line_files[COMMS_NPC_TRADE_ACCEPT] = "text/comms_npc_trade_accept.tsv";
	comms_npc_line_files[COMMS_NPC_TRADE_DECLINE] = "text/comms_npc_trade_decline.tsv";
	comms_npc_line_files[COMMS_NPC_ATTACK_ACCEPT] = "text/comms_npc_attack_accept.tsv";
	comms_npc_line_files[COMMS_NPC_ATTACK_FLEE] = "text/comms_npc_attack_flee.tsv";
	comms_npc_line_files[COMMS_NPC_ATTACK_PLEAD] = "text/comms_npc_attack_plea.tsv";

	main_scene = SCENE_COMMS;
	frame_skip=0;
	comms_draw_count = 0;

	comms_setup_intro();

	comms_set_current_npc_lines();
	comms_load_player_choices();

	SDL_SetRenderDrawColor(main_renderer, 0x00, 0x00, 0x00, 255);
	return 0;
}

void comms_set_faction(int f)
{
	if (f == TRAVEL_FACTION_KRULL)
	{
		portrait_image = Load_tex("sprites/portraits/krull_1.png");
		subject = Load_tex("sprites/gui/comms_subject_placeholder.png");
	}
	else if (f == TRAVEL_FACTION_PLINK)
	{
		portrait_image = Load_tex("sprites/portraits/plink_1.png");
		subject = Load_tex("sprites/ships/ring_ship1_closeup.png");
	}
	else if (f == TRAVEL_FACTION_SNEEB)
	{
		portrait_image = Load_tex("sprites/portraits/sneeb_1.png");
		subject = Load_tex("sprites/gui/comms_subject_placeholder.png");
	}
	comms_faction = f;
}

void prepare_npc_lists()
{
	vector_init(&comms_npc_lines, 10);
	vector_fill(&comms_npc_lines, NULL);

	//Allocate all the space we'll need for the default library of npc dialogue (anything else we can allocate as we go)
	for (int i = 0; i < 10; i++) //npc lines 0 to 9
	{
		Vector **lines = malloc(4 * sizeof(Vector));
		for (int j = 0; j < 4; j++)
		{
			lines[j] = malloc(4 * sizeof(Vector));
			for (int t = 0; t < 4; t++) //tones 0 to 3
			{
				vector_init(&lines[j][t], 5);
			}
		}
		vector_set(&comms_npc_lines, i, lines);

		//load in the lines from file
		int next_state, next_state_index;
		
		switch (i)
		{
			case COMMS_NPC_GREETING:
				next_state = COMMS_STATE_PLAYER_CHOICE;
				next_state_index = COMMS_PLAYER_CHOICE_MAIN;
				break;
			case COMMS_NPC_DEFEND:
				next_state = COMMS_STATE_ENTER_COMBAT;
				next_state_index = -1;
				break;
			case COMMS_NPC_TRADE: //todo: We should define these values later based on the tone (angry NPCs don't want to trade, etc.)
				next_state = COMMS_STATE_PLAYER_CHOICE; //COMMS_STATE_ENTER_TRADE;
				next_state_index = COMMS_PLAYER_CHOICE_MAIN; //-1;
				break;
			case COMMS_NPC_INFO: //todo: We should define these values later based on the tone (angry NPCs don't want to talk, etc.)
				next_state = COMMS_STATE_PLAYER_CHOICE; 
				next_state_index = COMMS_PLAYER_CHOICE_MAIN; //COMMS_PLAYER_CHOICE_INFO - if we detect that this is the next state, we need to work these out based on known topics/nearby planets
				break;
			case COMMS_NPC_FAREWELL:
				next_state = COMMS_STATE_ENTER_TRAVEL;
				next_state_index = -1;
				break;
			case COMMS_NPC_TRADE_ACCEPT:
				next_state = COMMS_STATE_ENTER_TRADE;
				next_state_index = -1;
				break;
			case COMMS_NPC_TRADE_DECLINE:
				next_state = COMMS_STATE_PLAYER_CHOICE;
				next_state_index = COMMS_PLAYER_CHOICE_MAIN;
				break;
			case COMMS_NPC_ATTACK_ACCEPT:
				next_state = COMMS_STATE_ENTER_COMBAT;
				next_state_index = -1;
				break;
			case COMMS_NPC_ATTACK_FLEE: //todo: Fleeing should cost fuel or have a random chance of failure?
				next_state = COMMS_STATE_ENTER_TRAVEL;
				next_state_index = -1;
				break;
			case COMMS_NPC_ATTACK_PLEAD: //todo: this value should be determined by how much the NPC wants to attack the player (eg: if there's no bounty, no cool loot and they don't care much, they may not bother)
				next_state = COMMS_STATE_PLAYER_CHOICE;
				next_state_index = COMMS_PLAYER_CHOICE_MAIN;
				break;
			default:
				next_state = -1;
				next_state_index = -1;
		}

		load_default_npc_lines(lines, comms_npc_line_files[i], next_state, next_state_index);
	}
}

void load_default_npc_lines(Vector **line_type, char *fname, int next_state, int next_state_index)
{
	FILE *f;
	char line[4096];
	f = fopen(fname, "r");
	if (f != NULL)
	{
		//load line by line
		while (fgets(line, 4096 -1, f) != NULL)
		{
			char *faction_name;
			char *tone_name;
			char *text;

			faction_name = strtok(line, "\t");
			int faction_id = TRAVEL_FACTION_NONE;
			if (strcmp(faction_name, "FACTION_SNEEB") == 0)
			{
				faction_id = TRAVEL_FACTION_SNEEB;
			}
			else if (strcmp(faction_name, "FACTION_KRULL") == 0)
			{
				faction_id = TRAVEL_FACTION_KRULL;
			}
			else if (strcmp(faction_name, "FACTION_PLINK") == 0)
			{
				faction_id = TRAVEL_FACTION_PLINK;
			}

			tone_name = strtok(NULL, "\t");
			int tone_id = COMMS_TONE_UNKNOWN;
			if (strcmp(tone_name, "TONE_NEUTRAL") == 0)
			{
				tone_id = COMMS_TONE_NEUTRAL;
			}
			else if (strcmp(tone_name, "TONE_AGGRESSIVE") == 0)
			{
				tone_id = COMMS_TONE_AGGRESSIVE;
			}
			else if (strcmp(tone_name, "TONE_HAPPY") == 0)
			{
				tone_id = COMMS_TONE_HAPPY;
			}
			text = strtok(NULL, "\t");

			//Create a new Comms_NPCDialogue to store all this data in and whack it in the library
			Comms_NPCDialogue *npcd;
			npcd = malloc(sizeof(Comms_NPCDialogue));
			npcd->text = wrap_text(text, 64);
			npcd->next_state = next_state;
			npcd->next_state_index = next_state_index;
			vector_add(&line_type[faction_id][tone_id], npcd);
		}
	}
	else
	{
		printf("Error loading %s", fname);
	}
}



void comms_set_current_npc_lines()
{
	printf("Loading NPC text...\n");
	prepare_npc_lists();

	vector_free(&comms_current_npc_lines);
	vector_init(&comms_current_npc_lines, 10);
	vector_fill(&comms_current_npc_lines, NULL);

	//todo: load possible npc text from file and select appropriate one based on disposition/bounty/resource need
	Comms_NPCDialogue *npcd;
	Vector ** line_type;
	int line_count = 0;

	line_type = (Vector **)vector_get(&comms_npc_lines, COMMS_NPC_GREETING);
	line_count = vector_get_size(&line_type[comms_faction][COMMS_TONE_NEUTRAL]);
	npcd = vector_get(&line_type[comms_faction][COMMS_TONE_NEUTRAL], rand() % line_count);
	vector_set(&comms_current_npc_lines, COMMS_NPC_GREETING, npcd);

	line_type = (Vector **)vector_get(&comms_npc_lines, COMMS_NPC_DEFEND);
	line_count = vector_get_size(&line_type[comms_faction][COMMS_TONE_NEUTRAL]);
	npcd = vector_get(&line_type[comms_faction][COMMS_TONE_NEUTRAL], rand() % line_count);
	vector_set(&comms_current_npc_lines, COMMS_NPC_DEFEND, npcd);

	npcd = malloc(sizeof(Comms_NPCDialogue));
	npcd->text = wrap_text("We are glad you embrace fight willingly", 64);
	npcd->next_state = COMMS_STATE_ENTER_COMBAT;
	npcd->next_state_index = -1;
	vector_set(&comms_current_npc_lines, COMMS_NPC_ATTACK_ACCEPT, npcd);

	npcd = malloc(sizeof(Comms_NPCDialogue));
	npcd->text = wrap_text("You get away.for today.", 64);
	npcd->next_state = COMMS_STATE_ENTER_TRAVEL;
	npcd->next_state_index = -1;
	vector_set(&comms_current_npc_lines, COMMS_NPC_ATTACK_FLEE, npcd);

	npcd = malloc(sizeof(Comms_NPCDialogue));
	npcd->text = wrap_text("You are too pitiful to fight", 64);
	npcd->next_state = COMMS_STATE_PLAYER_CHOICE;
	npcd->next_state_index = COMMS_PLAYER_CHOICE_MAIN;
	vector_set(&comms_current_npc_lines, COMMS_NPC_ATTACK_PLEAD, npcd);

	line_type = (Vector **)vector_get(&comms_npc_lines, COMMS_NPC_TRADE);
	line_count = vector_get_size(&line_type[comms_faction][COMMS_TONE_NEUTRAL]);
	npcd = vector_get(&line_type[comms_faction][COMMS_TONE_NEUTRAL], rand() % line_count);
	vector_set(&comms_current_npc_lines, COMMS_NPC_TRADE, npcd);

	npcd = malloc(sizeof(Comms_NPCDialogue));
	npcd->text = wrap_text("Good, let's trade", 64);
	npcd->next_state = COMMS_STATE_ENTER_TRADE;
	npcd->next_state_index = -1;
	vector_set(&comms_current_npc_lines, COMMS_NPC_TRADE_ACCEPT, npcd);

	npcd = malloc(sizeof(Comms_NPCDialogue));
	npcd->text = wrap_text("No trade? oh well", 64);
	npcd->next_state = COMMS_STATE_PLAYER_CHOICE;
	npcd->next_state_index = COMMS_PLAYER_CHOICE_MAIN;
	vector_set(&comms_current_npc_lines, COMMS_NPC_TRADE_DECLINE, npcd);

	line_type = (Vector **)vector_get(&comms_npc_lines, COMMS_NPC_INFO);
	line_count = vector_get_size(&line_type[comms_faction][COMMS_TONE_NEUTRAL]);
	npcd = vector_get(&line_type[comms_faction][COMMS_TONE_NEUTRAL], rand() % line_count);
	vector_set(&comms_current_npc_lines, COMMS_NPC_INFO, npcd);

	line_type = (Vector **)vector_get(&comms_npc_lines, COMMS_NPC_FAREWELL);
	line_count = vector_get_size(&line_type[comms_faction][COMMS_TONE_NEUTRAL]);
	npcd = vector_get(&line_type[comms_faction][COMMS_TONE_NEUTRAL], rand() % line_count);
	vector_set(&comms_current_npc_lines, COMMS_NPC_FAREWELL, npcd);

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
	Travel_Node *t = (Travel_Node *)vector_get(&node_list, current_node);
	int w=(int)(pow(2,(t->p->size+3)));
	int h=w;
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
	Travel_Node *t = (Travel_Node *)vector_get(&node_list, current_node);
	t->p->x=comms_subject_final_pos;
	comms_portrait_background_pos[1] = comms_portrait_background_final_pos;
	gui_clear();

	int default_button;
	default_button = gui_add_text_button(">", 64 - 11, 64 - 9, 0, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &comms_skip, -1);
	update_button_state(default_button, BUTTON_STATE_SELECTED);
}

void comms_setup_player_choices()
{
	comms_state = COMMS_STATE_PLAYER_CHOICE;
	current_player_choice = ((Comms_NPCDialogue *)vector_get(&comms_current_npc_lines, current_npc_text))->next_state_index;
	comms_subject_pos[0] = comms_subject_final_pos;
	comms_portrait_background_pos[1] = comms_portrait_background_final_pos;
	current_npc_text = ((Comms_NPCDialogue *)vector_get(&comms_current_npc_lines, current_npc_text))->next_state_index;

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
//	main_blit(subject, comms_subject_pos[0], comms_subject_pos[1], NOFLIP, NULL);

	Travel_Node *t = (Travel_Node *)vector_get(&node_list, current_node);
	t->p->x=comms_subject_pos[0];
	t->p->y=comms_subject_pos[1];
	planet_draw(t->p);

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
	Travel_Node *t = (Travel_Node *)vector_get(&node_list, current_node);
	planet_draw(t->p);

//	main_blit(subject, comms_subject_pos[0], comms_subject_pos[1], NOFLIP, NULL);
	main_blit(portrait_background, comms_portrait_background_pos[0], comms_portrait_background_pos[1], NOFLIP, NULL);
	main_blit(portrait_image, comms_portrait_pos[0], comms_portrait_pos[1], NOFLIP, NULL);
	if (comms_draw_count < strlen(((Comms_NPCDialogue *)vector_get(&comms_current_npc_lines, current_npc_text))->text) * 2)
	{
		draw_text(1, comms_text_offset, ((Comms_NPCDialogue *)vector_get(&comms_current_npc_lines, current_npc_text))->text, comms_draw_count / 2, FONT_EARTH, GUI_TEXT_COLOR_DEFAULT);
	}
	else
	{
		draw_text(1, comms_text_offset, ((Comms_NPCDialogue *)vector_get(&comms_current_npc_lines, current_npc_text))->text, strlen(((Comms_NPCDialogue *)vector_get(&comms_current_npc_lines, current_npc_text))->text), FONT_EARTH, GUI_TEXT_COLOR_DEFAULT);
	}
}

void comms_draw_player_choices()
{
	Travel_Node *t = (Travel_Node *)vector_get(&node_list, current_node);
	planet_draw(t->p);

//	main_blit(subject, comms_subject_pos[0], comms_subject_pos[1], NOFLIP, NULL);
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
			switch(((Comms_NPCDialogue *)vector_get(&comms_current_npc_lines, current_npc_text))->next_state)
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
					combat_set_faction(comms_faction);
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
