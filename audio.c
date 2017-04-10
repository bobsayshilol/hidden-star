#include <stdlib.h>

#include "audio.h"
#include "user_events.h"
#include "factions.h"

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

int music_setup()
{
	music_groups = malloc(MUSIC_ROLE_MAX * sizeof(Vector));
	//set up music group structure
	for (int i = 0; i < MUSIC_ROLE_MAX; ++i)
	{
		//TODO: Longer term, this should be an array of 2D vectors rather than an array of array of vectors
		music_groups[i] = malloc(4 * sizeof(Vector));
		for (int j = 0; j < 4; ++j)
		{
			vector_init(&music_groups[i][j], 4);
		}
		printf("music_groups size %lu, %d %d\n", sizeof(music_groups), MUSIC_ROLE_MAX, i);
	}
	//load files
	music_read_config(music_groups, "audio/music.tsv");
	return 1;
}


int music_read_config(Vector **groups, char *fname)
{
	FILE *f;
	char line[4096];
	f = fopen(fname, "r");
	Mix_Music *m;

	if (f != NULL)
	{
		//load line by line
		while (fgets(line, 4096 -1, f) != NULL)
		{
			char *role;
			char *faction_name;
			char *file_path;

			role = strtok(line, "\t");
			int role_id = MUSIC_ROLE_SPECIAL;
			if (strcmp(role, "MUSIC_ROLE_COMMS") == 0)
			{
				role_id = MUSIC_ROLE_COMMS;
			}
			else if (strcmp(role, "MUSIC_ROLE_COMBAT") == 0)
			{
				role_id = MUSIC_ROLE_COMBAT;
			}
			else if (strcmp(role, "MUSIC_ROLE_STARMAP") == 0)
			{
				role_id = MUSIC_ROLE_STARMAP;
			}
			else if (strcmp(role, "MUSIC_ROLE_TRADE") == 0)
			{
				role_id = MUSIC_ROLE_TRADE;
			}

			faction_name = strtok(NULL, "\t");
			int faction_id = FACTION_NONE;
			if (role_id != MUSIC_ROLE_SPECIAL)
			{
				if (strcmp(faction_name, "FACTION_SNEEB") == 0)
				{
					faction_id = FACTION_SNEEB;
				}
				else if (strcmp(faction_name, "FACTION_KRULL") == 0)
				{
					faction_id = FACTION_KRULL;
				}
				else if (strcmp(faction_name, "FACTION_PLINK") == 0)
				{
					faction_id = FACTION_PLINK;
				}
			}
			else
			{
				faction_id = MUSIC_SPECIAL_MENU;
				//TODO: This feels kinda ugly, but it allows us to use the same data structures and can be expanded later
			}
			file_path = strtok(NULL, "\n");
			m = Mix_LoadMUS(file_path);
			if (m == NULL)
			{
				fprintf(stderr, "Mix_LoadMUS: %s\n", Mix_GetError());
				continue;
			}
			Mix_FreeMusic(m);
			char *fp = (char*) malloc(strlen(file_path) + 1);
			fp[0] = '\0';
			strcat(fp, file_path);
			vector_add(&groups[role_id][faction_id], fp);
		}
	}
	else
	{
		printf("Error loading music config from %s\n", fname);
	}
	return 1;
}

int audio_read_config(char *fname)
{
	FILE *f;
	char line[4096];
	f = fopen(fname, "r");

	if (f != NULL)
	{
		//load line by line
		while (fgets(line, 4096 -1, f) != NULL)
		{
			char *group;
			char *file_path;

			group = strtok(line, "\t");
			int ag = AUDIO_GROUP_BUTTON_HOVER;
			if (strcmp(group, "AUDIO_GROUP_BUTTON_ACTIVATE") == 0)
			{
				ag = AUDIO_GROUP_BUTTON_ACTIVATE;
			}
			else if (strcmp(group, "AUDIO_GROUP_BUTTON_ERROR") == 0)
			{
				ag = AUDIO_GROUP_BUTTON_ERROR;
			}
			else if (strcmp(group, "AUDIO_GROUP_STARMAP_HOVER") == 0)
			{
				ag = AUDIO_GROUP_STARMAP_HOVER;
			}
			else if (strcmp(group, "AUDIO_GROUP_STARMAP_ACTIVATE") == 0)
			{
				ag = AUDIO_GROUP_STARMAP_ACTIVATE;
			}
			else if (strcmp(group, "AUDIO_GROUP_COMBAT_FIRE") == 0)
			{
				ag = AUDIO_GROUP_COMBAT_FIRE;
			}
			else if (strcmp(group, "AUDIO_GROUP_COMBAT_HIT") == 0)
			{
				ag = AUDIO_GROUP_COMBAT_HIT;
			}
			else if (strcmp(group, "AUDIO_GROUP_COMBAT_DIE") == 0)
			{
				ag = AUDIO_GROUP_COMBAT_DIE;
			}
			file_path = strtok(NULL, "\n");
			audio_load_sample(ag, file_path);
		}
	}
	else
	{
		printf("Error loading sfx config from %s\n", fname);
	}
	return 1;
}



int audio_setup() {
	Mix_Init(MIX_INIT_OGG);
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		fprintf(stderr, "Mix_OpenAudio: %s\n", Mix_GetError());
		return -1;
	}

	// Music stuff
	Mix_HookMusicFinished(&music_finished_callback);

	music_setup();

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
	audio_read_config("audio/sfx.tsv");

	//Set initial volume to 40%
	Mix_VolumeMusic(MIX_MAX_VOLUME * 0.40);
	Mix_Volume(-1, MIX_MAX_VOLUME * 0.40);
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
	if (current_music == NULL)
	{
		fprintf(stderr, "Mix_LoadMUS: %s\n", Mix_GetError());
	}
	if(fade_in_ms) {
		return Mix_FadeInMusic(current_music, loops, fade_in_ms);
	} else {
		return Mix_PlayMusic(current_music, loops);
	}
}

int music_loop_group(int mgroup, int mrole, int fade_in_ms, int loops)
{
	int n = vector_get_size(&music_groups[mgroup][mrole]);
	if (n > 0)
	{
		return music_loop((char*) vector_get(&music_groups[mgroup][mrole], rand() % n), fade_in_ms, loops);
	}
	else
	{
		return 0;
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
		fprintf(stderr, "Mix_LoadWAV: %s (%s)\n", Mix_GetError(), file);
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
	return audio_play_sample(g, rand() % audio_groups[g].n);
}
