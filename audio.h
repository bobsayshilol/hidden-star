#include "SDL_mixer.h"
#include "SDL_timer.h"

int audio_setup();
int audio_quit();

int music_loop(char const *file, int fade_in_ms, int loops);
int music_play(char const *file, int fade_in_ms);
int music_stop(int fade_in_ms);
int music_playing();
int music_schedule(char const *file, int fade_in_ms, int loops);
