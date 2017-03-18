#ifndef HS_AUDIO
#define HS_AUDIO
#include "SDL_mixer.h"
#include "SDL_timer.h"

#include "user_events.h"
#include "util/vector.h"

#define SFX_CHANNELS 8

#define MUSIC_ROLE_SPECIAL	0
#define MUSIC_ROLE_COMMS	1
#define MUSIC_ROLE_COMBAT	2
#define MUSIC_ROLE_STARMAP	3
#define MUSIC_ROLE_TRADE	4
#define MUSIC_ROLE_MAX		5

#define MUSIC_SPECIAL_MENU 0

typedef enum {
	AUDIO_GROUP_BUTTON_HOVER,
	AUDIO_GROUP_BUTTON_ACTIVATE,
	AUDIO_GROUP_BUTTON_ERROR,
	AUDIO_GROUP_STARMAP_HOVER,
	AUDIO_GROUP_STARMAP_ACTIVATE,
	AUDIO_GROUP_COMBAT_FIRE,
	AUDIO_GROUP_COMBAT_HIT,
	AUDIO_GROUP_COMBAT_DIE,
	AUDIO_GROUP_NUM
} AudioEffectGroup;

Vector **music_groups;

int audio_setup();
int audio_read_config(char *fname);
int audio_quit();

int music_setup();
int music_read_config(Vector **groups, char *fname);
int music_loop(char const *file, int fade_in_ms, int loops);
int music_loop_group(int mgroup, int mrole, int fade_in_ms, int loops);
int music_play(char const *file, int fade_in_ms);
int music_stop(int fade_in_ms);
int music_playing();
int music_schedule(char const *file, int fade_in_ms, int loops);
int music_set_finished_callback(UserEventCallback function, void *data);

int audio_load_sample(AudioEffectGroup g, char const *file);
int audio_play_sample(AudioEffectGroup g, int sample);
int audio_play_group(AudioEffectGroup g);
#endif
