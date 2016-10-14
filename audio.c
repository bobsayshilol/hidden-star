#include "audio.h"
#include "user_events.h"

int audio_setup() {
	Mix_Init(MIX_INIT_OGG);
	return 0;
}

int audio_quit() {
	Mix_Quit();
	return 0;
}
