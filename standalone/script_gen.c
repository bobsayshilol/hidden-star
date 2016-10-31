//gcc -std=c99 -g script_gen.c -o translatorator

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//From factions.h
#define FACTION_NONE 0
#define FACTION_SNEEB 1
#define FACTION_KRULL 2
#define FACTION_PLINK 3

//From comms.h
#define COMMS_NPC_GREETING 0
#define COMMS_NPC_DEFEND 1
#define COMMS_NPC_TRADE 2
#define COMMS_NPC_INFO 3
#define COMMS_NPC_FAREWELL 4
#define COMMS_NPC_TRADE_ACCEPT 5
#define COMMS_NPC_TRADE_DECLINE 6
#define COMMS_NPC_ATTACK_ACCEPT 7
#define COMMS_NPC_ATTACK_FLEE 8
#define COMMS_NPC_ATTACK_PLEAD 9

char *comms_npc_line_files[10];
char ***translation_lookup;

void print_translation_lookup()
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j= 0; j < 26; ++j)
		{
			printf("%s\t", translation_lookup[j][i]);
		}
		printf("\n");
	}
}

void load_translation_lookup(char *fname)
{
	FILE *f;
	char line[64];
	f = fopen(fname, "r");
	if (f != NULL)
	{
		int i = 0;
		translation_lookup = malloc(26 * sizeof(void*));
		while (fgets(line, 64 -1, f) != NULL)
		{
			translation_lookup[i] = malloc(4 * sizeof(char*));
			char* l = strtok(line,",");
			translation_lookup[i][0] = malloc(50);
			strcpy(translation_lookup[i][0], l);
			l[0] = '\0';

			l = strtok(NULL,",");
			translation_lookup[i][1] = malloc(50);
			strcpy(translation_lookup[i][1], l);
			l[0] = '\0';

			l = strtok(NULL,",");
			translation_lookup[i][2] = malloc(50);
			strcpy(translation_lookup[i][2], l);
			l[0] = '\0';

			l = strtok(NULL,",\n");
			translation_lookup[i][3] = malloc(50);
			strcpy(translation_lookup[i][3], l);
			l[0] = '\0';
			i++;
		}
	}
	else
	{
		printf("Error opening file %s\n", fname);
	}
	fclose(f);
}

void translate_lines(char *fname)
{
	printf("Opening %s\n", fname);
	FILE *f;
	FILE *o;
	char line[4096];
	f = fopen(fname, "r");
	char generated_script[strlen(fname) + 16];
	generated_script[0]='\0';
	strcpy(generated_script, fname);
	strcat(generated_script, "_translated");
	o = fopen(generated_script, "w");
	if (f != NULL && o != NULL)
	{
		//load line by line
		while (fgets(line, 4096 -1, f) != NULL)
		{
			printf(".");
			char output_line[strlen(line) * 4];
			output_line[0]='\0';
			char *faction_name;
			char *tone_name;
			char *text;

			faction_name = strtok(line, "\t");
			int faction_id = FACTION_NONE;
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
			strcpy(output_line, faction_name);
			strcat(output_line, "\t");

			tone_name = strtok(NULL, "\t");
			strcat(output_line, tone_name);
			strcat(output_line, "\t");

			text = strtok(NULL, "\n");
			strcat(output_line, text);
			strcat(output_line, "\t");

			char *translated_text;
			translated_text = malloc(strlen(text) * 3 + 1);
			translated_text[0] = '\0';

			for (int i = 0; i < strlen(text); ++i)
			{
				int c = text[i] - 65;
				if (c > 25)
				{
					c = c - 32;
				}
				if (c >= 0 && c <= 25)
				{
					strcat(translated_text, translation_lookup[c][faction_id]);
				}
				else
				{
					translated_text[strlen(translated_text)] = text[i];
					translated_text[strlen(translated_text)] = '\0';
				}
			}
			if (strlen(translated_text) > 0)
			{
				strcat(output_line, translated_text);
			}
			strcat(output_line, "\n");
			fputs(output_line, o);
		}
	}
	else
	{
		printf("Error opening files %s and %s\n", fname, generated_script);
	}

	printf("\n");
	printf("Writing to %s\n\n", generated_script);
	//TODO: We should probably free all our mallocs \o/
	fclose(f);
	fclose(o);
}

int main(int argc, char* argv[])
{
	printf("Startup!\n");
	load_translation_lookup("../text/translation_phonemes.csv");
	printf("Phonemes loaded:\n");
	print_translation_lookup();
	printf("\n");
	comms_npc_line_files[COMMS_NPC_GREETING] = "../text/comms_npc_greetings.tsv";
	comms_npc_line_files[COMMS_NPC_DEFEND] = "../text/comms_npc_defend.tsv";
	comms_npc_line_files[COMMS_NPC_TRADE] = "../text/comms_npc_trade.tsv";
	comms_npc_line_files[COMMS_NPC_INFO] = "../text/comms_npc_info.tsv";
	comms_npc_line_files[COMMS_NPC_FAREWELL] = "../text/comms_npc_farewell.tsv";
	comms_npc_line_files[COMMS_NPC_TRADE_ACCEPT] = "../text/comms_npc_trade_accept.tsv";
	comms_npc_line_files[COMMS_NPC_TRADE_DECLINE] = "../text/comms_npc_trade_decline.tsv";
	comms_npc_line_files[COMMS_NPC_ATTACK_ACCEPT] = "../text/comms_npc_attack_accept.tsv";
	comms_npc_line_files[COMMS_NPC_ATTACK_FLEE] = "../text/comms_npc_attack_flee.tsv";
	comms_npc_line_files[COMMS_NPC_ATTACK_PLEAD] = "../text/comms_npc_attack_plea.tsv";
	for (int i = 0; i < 10; i++)
	{
		if (strlen(comms_npc_line_files[i]) > 0)
		{
			translate_lines(comms_npc_line_files[i]);
		}
	}
	printf("Done!\n");
}
