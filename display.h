//
// Created by Milena on 11-Jun-16.
//

#ifndef DEMOMENI_DISPLAY_H
#define DEMOMENI_DISPLAY_H
#include "logic.h"
#include <curses.h>

#define winh 52
#define winw 80
#define BGD 2
#define BGDC 6
#define BOARD_SIZE 30
#define WHITE_BLACK 2               //parovi boja
#define WHITE_GREEN 1
#define WHITE_RED 4
#define WHITE_YELLOW 3
#define MAP_SIZE 50
#define OFFX 0
#define OFFY 0
#define fill 65584


typedef struct option{
    chtype * tekst;
} option;

chtype  * strtoch(char * string);
void add_chstring(int top, int left, chtype * string, int color_pair, int bold);
void screen(int h, int w);
void set_bckgd(int bckgd);
void addsqr(int top, int left, int size, int color_pair, int bold);
void updatesqr(int top_old, int left_old, int top_new, int left_new);
void add_logo(int top, int left, char logo[23][35]);
void display_main_menu(int old_option, int new_option, option * commands, char logo[23][35], int n_commands);
void display_color_menu(int old_option, int new_option, int prev_player, int curr_player, char logo[23][35], int avail[4]);
void init_map();
void display_map(coord * current, coord * prev, int colors[4], char * time, int ee);
void newgame_menu(char logo[23][35], int colors[4]);
void pick_colors(int colors[4], char logo[23][35]);
int pick_bot_levels(int * bot_count, int bot_level[2], char logo[23][35]);
int pick_botcount(int * player_count, int * bot_count, int * bot_level,char logo[23][35]);
int pick_player_number(int * player_count,int * bot_count, int bot_level[2], char logo[23][35]);
void high_scores(char * path);
void toggle_pause(int pause);
void display_about(char logo[23][35]);

#endif //DEMOMENI_DISPLAY_H
