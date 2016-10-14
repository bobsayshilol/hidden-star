#include "audio.h"
#include "user_events.h"

int audio_setup() {
	Mix_Init(MIX_INIT_OGG);
	if(Mix_OpenAudio(48000, AUDIO_U16LSB, 2, 1024) == -1) {
		fprintf(stderr, "Mix_OpenAudio: %s\n", Mix_GetError());
		return -1;
	}
	return 0;
}

int audio_quit() {
	Mix_CloseAudio();
	Mix_Quit();
	return 0;
}
