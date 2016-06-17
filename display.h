//
// Created by Milena on 11-Jun-16.
//

#ifndef DEMOMENI_DISPLAY_H
#define DEMOMENI_DISPLAY_H
#include "logic.h"
#include <curses.h>

#define winh 37
#define winw 52
#define BGD 2
#define BGDC 6
#define BOARD_SIZE 30
#define WHITE_BLACK 2               //parovi boja
#define WHITE_GREEN 1
#define WHITE_RED 4
#define WHITE_YELLOW 3
#define MAP_SIZE 35
#define OFFX 0
#define OFFY 0
#define fill 65584

typedef struct option{
    chtype * tekst;
    void * function;
} option;

chtype  * strtoch(char * string);
void add_chstring(int top, int left, chtype * string, int color_pair, int bold);
int screen(int h, int w);
void set_bckgd(int bckgd);
void add_logo(int top, int left, char logo[23][35]);
void display_main_menu(int old_option, int new_option, option * commands, char logo[23][35], int n_commands);
void init_map();
void display_map(coord * current, coord * prev, int colors[4]);
void newgame_menu(char logo[23][35], int colors[4]);
void pick_colors(int colors[4]);
void high_scores();
void demo_game();
void display_about();

#endif //DEMOMENI_DISPLAY_H
