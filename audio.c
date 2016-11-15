#include <stdlib.h>

#include "audio.h"
#include "user_events.h"

Mix_Music *current_music;

struct {
	char const *file;
	int fade_in_ms;
	int loops;
} scheduled_music;

struct {
	UserEventCallback fun;
	void *data;
} cb_music_finished;

struct {
	size_t n;
	size_t n_max;
	Mix_Chunk **samples;
} audio_groups[AUDIO_GROUP_NUM];

void music_finished_callback();
void music_finished_callback_real();

int audio_setup() {
	Mix_Init(MIX_INIT_OGG);
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		fprintf(stderr, "Mix_OpenAudio: %s\n", Mix_GetError());
		return -1;
	}

	// Music stuff
	
	Mix_HookMusicFinished(&music_finished_callback);

	music_set_finished_callback(NULL, NULL);
	music_schedule(NULL, 0, 0);

	// Sound effect stuff
	Mix_AllocateChannels(SFX_CHANNELS);
	
	for(size_t i = 0; i < AUDIO_GROUP_NUM; i++) {
		audio_groups[i].n = 0;
		audio_groups[i].n_max = 10;
		audio_groups[i].samples = calloc(audio_groups[i].n_max, sizeof(Mix_Chunk *));
	}

	// Preload all sound effects here?

	return 0;
}

int audio_quit() {
	Mix_CloseAudio();
	Mix_Quit();
	return 0;
}

// Loop a file loops times, with optional fade in for the first
// iteration. Stops any currently playing music first.
int music_loop(char const *file, int fade_in_ms, int loops) {
	scheduled_music.file = NULL;

	if(music_playing()) {
		music_stop(0);
	}

	current_music = Mix_LoadMUS(file);
	
	if(fade_in_ms) {
		return Mix_FadeInMusic(current_music, loops, fade_in_ms);
	} else {
		return Mix_PlayMusic(current_music, loops);
	}
}

// Play a file once, with optional fade in. Stops any playing music
// first.
int music_play(char const *file, int fade_in_ms) {
	return music_loop(file, fade_in_ms, 1);
}

// Returns 0 if no music is playing, non-zero otherwise.
int music_playing() {
	return Mix_PlayingMusic();
}

// Stop the currently playing track, if any, and free its resources.
// If there is another track scheduled, it will begin playing
// afterwards.
int music_stop(int fade_in_ms) {
	if(!music_playing()) {
		return 0;
	}

	if(fade_in_ms > 0) {
		Mix_FadeOutMusic(fade_in_ms);
	} else {
		Mix_HaltMusic();
	}

	Mix_FreeMusic(current_music);
	current_music = NULL;

	return 0;
}

// Schedule a track for when the current one has finished. Can be
// cancelled by calling with a null pointer as file, or by calling
// music_play().
int music_schedule(char const *file, int fade_in_ms, int loops) {
	scheduled_music.file = file;
	scheduled_music.fade_in_ms = fade_in_ms;
	scheduled_music.loops = loops;

	return 0;
}

// Called by SDL when a track has finished playing. Pushes the real
// callback into the event queue.
void music_finished_callback() {
	userevent_add(USEREVENT_CALLBACK, &music_finished_callback_real, NULL);
}

// Does the actual work. Is not run from inside the SDL callback, so
// it can use mixer functions.
void music_finished_callback_real(void *unused) {
	if(scheduled_music.file != NULL) {
		if(current_music != NULL) {
			Mix_FreeMusic(current_music);
		}
		music_loop(scheduled_music.file,
				   scheduled_music.fade_in_ms,
				   scheduled_music.loops);
	}

	if(cb_music_finished.fun) {
		cb_music_finished.fun(cb_music_finished.data);
	}
}

// Set a custom user callback for when music has finished playing.
int music_set_finished_callback(UserEventCallback function, void *data) {
	cb_music_finished.fun = function;
	cb_music_finished.data = data;

	return 0;
}

// Load a new sample into group g. Returns its index or a negative
// number if an error occurs.
int audio_load_sample(AudioEffectGroup g, char const *file) {
	if(audio_groups[g].n == audio_groups[g].n_max) {
		audio_groups[g].n_max *= 2;
		void *n = realloc(audio_groups[g].samples,
						  audio_groups[g].n_max * sizeof(Mix_Chunk *));
		if(n != NULL) {
			audio_groups[g].samples = n;
		} else {
			return -1;
		}
	}

	audio_groups[g].samples[audio_groups[g].n] = Mix_LoadWAV(file);

	if(audio_groups[g].samples[audio_groups[g].n] == NULL) {
		return -2;
	}

	return audio_groups[g].n++;
}

// Play a sample from a group.
int audio_play_sample(AudioEffectGroup g, int sample) {
	return Mix_PlayChannel(-1, audio_groups[g].samples[sample], 0);
}

// Play a random sample from a group.
int audio_play_group(AudioEffectGroup g) {
	if(audio_groups[g].n == 0) {
		return -1;
	}

	// Crappy, unseeded random. Might want to fix this at some
	// point.
	return audio_play_sample(g, random() % audio_groups[g].n);
}
