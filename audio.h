#ifndef HS_AUDIO
#define HS_AUDIO
#include "SDL_mixer.h"
#include "SDL_timer.h"

#include "user_events.h"

typedef enum {
	AUDIO_GROUP_NUM
} AudioEffectGroup;

int audio_setup();
int audio_quit();

int music_loop(char const *file, int fade_in_ms, int loops);
int music_play(char const *file, int fade_in_ms);
int music_stop(int fade_in_ms);
int music_playing();
int music_schedule(char const *file, int fade_in_ms, int loops);
int music_set_finished_callback(UserEventCallback function, void *data);

int audio_load_sample(AudioEffectGroup g, char const *file);
#endif
