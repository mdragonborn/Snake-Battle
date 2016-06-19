//
// Created by Milena on 17-Jun-16.
//

#ifndef DEMOMENI_PLAYGAME_H
#define DEMOMENI_PLAYGAME_H

int play_game(int player_count, int bot_count, int bot_level[2], int colors[4]);
int was_modified(char* path);
void write_high(char *path, coord current[4]);
int read_score(FILE* high);
void read_name(FILE* high, char* name);
void write_xor(char* path);
#endif //DEMOMENI_PLAYGAME_H
