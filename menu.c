#include "menu.h"


int menu_setup_main_menu(int i)
{
	gui_clear();
	int default_button;

	default_button = gui_add_text_button("new game", 0, 2, 35, BUTTON_STATE_ENABLED, BUTTON_STYLE_MENU, -1, &init_game, SCENE_STARMAP);
	gui_add_text_button("quit", 0, 10, 35, BUTTON_STATE_ENABLED, BUTTON_STYLE_MENU, -1, &menu_quit, 0);
	gui_add_text_button("debug", 0, 52, 35, BUTTON_STATE_ENABLED, BUTTON_STYLE_MENU, -1, &menu_setup_debug_menu, 0);

	update_button_state(default_button, BUTTON_STATE_SELECTED);

	return 0;
}


int menu_setup_debug_menu(int i)
{
	gui_clear();
	int default_button;
	default_button = gui_add_text_button("combat", 0, 2, 35, BUTTON_STATE_ENABLED, BUTTON_STYLE_MENU, -1, &combat_setup, SCENE_COMBAT);
	gui_add_text_button("planets", 0, 10, 35 , BUTTON_STATE_ENABLED, BUTTON_STYLE_MENU, -1, &planet_setup, SCENE_PLANET_GEN);
	gui_add_text_button("trade", 0, 18, 35,  BUTTON_STATE_DISABLED, BUTTON_STYLE_MENU, -1, &starmap_setup, SCENE_TRAVEL);
	gui_add_text_button("travel", 0, 36, 35,  BUTTON_STATE_DISABLED, BUTTON_STYLE_MENU, -1, &starmap_setup, SCENE_TRAVEL);
	gui_add_text_button("nav", 0, 44, 35,  BUTTON_STATE_ENABLED, BUTTON_STYLE_MENU, -1, &starmap_setup, SCENE_STARMAP);
	gui_add_text_button("comms", 0, 52, 35,  BUTTON_STATE_ENABLED, BUTTON_STYLE_MENU, -1, &comms_setup, SCENE_COMMS);
	update_button_state(default_button, BUTTON_STATE_SELECTED);

	return 0;
}


int menu_setup_combat_win(int i)
{
	gui_clear();
	int default_button;

	default_button = gui_add_text_button("Scavenge Wreck", 0, 2, 64, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &starmap_setup, SCENE_STARMAP);
	gui_add_text_button("Space Burial", 0, 10, 64, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &starmap_setup, SCENE_STARMAP);
	gui_add_text_button("Continue", 0, 18, 64, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &starmap_setup, SCENE_STARMAP);

	update_button_state(default_button, BUTTON_STATE_SELECTED);

	return 0;
}


int menu_setup_combat_lose(int i)
{
	gui_clear();
	int default_button;

	default_button = gui_add_text_button("Gasp For Air", 0, 39, 64, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &menu_setup_main_menu, SCENE_STARMAP);
	gui_add_text_button("Flail About", 0, 47, 64, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &menu_setup_main_menu, SCENE_STARMAP);
	gui_add_text_button("Expire", 0, 55, 64, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &menu_setup_main_menu, SCENE_STARMAP);

	update_button_state(default_button, BUTTON_STATE_SELECTED);
}
