//
// Created by Milena on 17-Jun-16.
//

#ifndef DEMOMENI_PLAYGAME_H
#define DEMOMENI_PLAYGAME_H
#define PATH "C:\\Users\\bulse_eye\\Documents\\Snake-Battle\\high_scores.bin"
int play_game(int player_count, int bot_count, int bot_level[2], int colors[4]);
int was_modified(char* path);
void sort_players(player players[11]);
void write_xor(char* path, int already_xored);
void create_new_bin(char* path);
void write_one_high(char* path, coord current, char* player_name);
int is_worthy(char* path, int score);
#endif //DEMOMENI_PLAYGAME_H
