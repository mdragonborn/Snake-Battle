#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include "logic.h"
#include "playgame.h"

#define winh 40
#define winw 60
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

chtype  * strtoch(char * string){
    int len=strlen(string), i;
    chtype * converted=(chtype*)calloc(sizeof(chtype), len+1);
    for (i=0;i<len+1;i++)
        converted[i]=(chtype)string[i];
    return converted;
}

void add_chstring(int top, int left, chtype * string, int color_pair, int bold){
    int i;
    if (bold) for(i=0;string[i]!=0;i++)
            mvaddch(top, left+i,string[i]|COLOR_PAIR(color_pair)|A_BOLD);
    else for(i=0;string[i]!=0;i++)
            mvaddch(top, left+i,string[i]|COLOR_PAIR(color_pair));
}

void screen(int h, int w){
    initscr();
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
        init_pair(BGD, COLOR_WHITE, COLOR_BLACK); //2
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);
        init_pair(4, COLOR_RED, COLOR_BLACK);
    }
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    raw();
    resize_term(h,w);
}

void set_bckgd(int bckgd){
    int i, j; chtype space=' ';
    for (i=0;i<winh;i++)
        for(j=0;j<winw;j++){
            mvaddch(i, j, space|COLOR_PAIR(bckgd));
        }
    return;
}

void add_logo(int top, int left, char logo[23][35]){
    int i,j; chtype space=' ';
    int pair=1;
    for (i=0;i<23; i++)
        for(j=0;j<34;j++) {
            if(logo[i][j]!='.'){
                switch(logo[i][j]){
                    case '[': pair=2; break;
                    case '#': pair=3; break;
                    case ']': pair=4; break;
                    case '=': pair=1; break;
                    default: pair=1; break;
                }
                mvaddch(top + i, left + j, fill | COLOR_PAIR(pair) | A_BOLD);
            }
            else
                mvaddch(top + i, left + j, space | COLOR_PAIR(1));
        }
    return;
}


void display_main_menu(int old_option, int new_option, option * commands, char logo[23][35], int n_commands)  //n_commands ???
{
    chtype dot[]={'<', ' ' ,'>', ' ',0}, selected[]={'<',ACS_DIAMOND,'>', ' ',0};
    int lmarg = 18,
            tmarg = 26;
    int i;
    if (old_option == -1) {
        set_bckgd(2);
        add_logo(1, 9, logo);
        for (i = 0; i < n_commands; i++)
        {
            if (i==new_option)
                add_chstring(tmarg+i+1,lmarg,selected, 2, 0);
            else
                add_chstring(tmarg+i+1,lmarg,dot, 2, 0);
            add_chstring(tmarg+i+1,lmarg+4,commands[i].tekst, 2, 0);

        }
    }
    else {
        add_chstring(tmarg+old_option+1,lmarg,dot, 2, 0);
        add_chstring(tmarg+new_option+1,lmarg,selected, 2, 0);
    }
    refresh();
}

void display_color_menu(int old_option, int new_option, int prev_player, int curr_player, char logo[23][35], int avail){
    int lmarg = 18,
            tmarg = 26;
    int i, n_commands=4;

    if (old_option == -1) {
        set_bckgd(2);
        add_logo(1, 9, logo);
        for (i = 0; i < n_commands; i++)
        {


        }
    }
    else {
    }
    refresh();
}

void init_map(){
    erase();
    set_bckgd(1); int i, j;
    for(i=0;i<MAP_SIZE+1;i++){
        mvaddch(0,i,fill|COLOR_PAIR(1));
        mvaddch(i,0,fill|COLOR_PAIR(1));
        mvaddch(i,MAP_SIZE+1, fill|COLOR_PAIR(1));
        mvaddch(MAP_SIZE+1,i,fill|COLOR_PAIR(1));
        for (j=0;j<winw-MAP_SIZE;j++)
            mvaddch(i,MAP_SIZE+2+j, fill|COLOR_PAIR(1));
    }
    for (j=0;j<winw-MAP_SIZE;j++)
        mvaddch(MAP_SIZE+1,MAP_SIZE+1+j, fill|COLOR_PAIR(1));
    //samo stampu
}

void display_map(coord * current, coord * prev, int colors[4]){
    int i, col;
    if (prev[0].x==prev[1].x && prev[0].x==0){
        for (i=0;i<4;i++) {
            if (current[i].x==-1) continue;
            col=colors[i];
            switch (current[i].dir) {
                case 0:
                    mvaddch(current[i].x + OFFX+1, current[i].y + OFFY, ACS_VLINE | COLOR_PAIR(col) | A_BOLD);
                    mvaddch(current[i].x + OFFX, current[i].y + OFFY, ACS_DIAMOND | COLOR_PAIR(3) | A_BOLD);
                    break;
                case 1:
                    mvaddch(current[i].x + OFFX, current[i].y + OFFY-1, ACS_HLINE | COLOR_PAIR(col) | A_BOLD);
                    mvaddch(current[i].x + OFFX, current[i].y + OFFY, ACS_DIAMOND | COLOR_PAIR(3) | A_BOLD);
                    break;
                case 2:
                    mvaddch(current[i].x + OFFX-1, current[i].y + OFFY, ACS_VLINE | COLOR_PAIR(col) | A_BOLD);
                    mvaddch(current[i].x + OFFX, current[i].y + OFFY, ACS_DIAMOND | COLOR_PAIR(3) | A_BOLD);
                    break;
                case 3:
                    mvaddch(current[i].x + OFFX, current[i].y + OFFY+1, ACS_HLINE | COLOR_PAIR(col) | A_BOLD);
                    mvaddch(current[i].x + OFFX, current[i].y + OFFY, ACS_DIAMOND | COLOR_PAIR(3) | A_BOLD);
                    break;
                default:
                    break;
            }
            refresh();
        }
    }
    else{
        chtype next;
        for(i=0;i<4;i++){
            if (current[i].x==-1) continue;
            col=colors[i];
            if (prev[i].blank!=1) switch(current[i].dir){
                    case 0:{
                        switch(prev[i].dir){
                            case 0: next=ACS_VLINE; break;
                            case 1: next=ACS_LRCORNER; break;
                            case 3: next=ACS_LLCORNER; break;
                            default: exit(462);
                        }
                        break;
                    }
                    case 1:{
                        switch(prev[i].dir){
                            case 0: next=ACS_ULCORNER; break;
                            case 1: next=ACS_HLINE; break;
                            case 2: next=ACS_LLCORNER; break;
                            default: exit(462);
                        }
                        break;
                    }
                    case 2:{
                        switch(prev[i].dir){
                            case 1: next=ACS_URCORNER; break;
                            case 2: next=ACS_VLINE; break;
                            case 3: next=ACS_ULCORNER; break;
                            default: exit(462);
                        }
                        break;
                    }
                    case 3:{
                        switch(prev[i].dir){
                            case 0: next=ACS_URCORNER; break;
                            case 2: next=ACS_LRCORNER; break;
                            case 3: next=ACS_HLINE; break;
                            default: exit(462);
                        }
                        break;
                        default: exit(462);
                    }
                }
            if (prev[i].blank!=1) mvaddch(prev[i].x+OFFX, prev[i].y+OFFY, next|COLOR_PAIR(col)|A_BOLD);
            else mvaddch(prev[i].x+OFFX, prev[i].y+OFFY, ' '|COLOR_PAIR(col)|A_BOLD);
            mvaddch(current[i].x+OFFX,current[i].y+OFFX,ACS_DIAMOND|COLOR_PAIR(3)|A_BOLD);


        }
    }
    refresh();
}

int pick_bot_levels(int * bot_count, int bot_level[2], char logo[23][35]){
    option bot_levels[3];
    bot_levels[0].tekst=strtoch("Easy");
    bot_levels[1].tekst=strtoch("Hard");
    bot_levels[2].tekst=strtoch("Nazad");
    chtype pick[]={'B', 'o', 't', 0};
    int i, old_option, new_option, key;
    for (i=0;i<*bot_count;i++){
        erase();
        old_option=-1; new_option=0;
        display_main_menu(old_option, new_option, bot_levels,logo, 3);
        add_chstring(25, 15,pick,BGD,1); mvaddch(25,19,(i==0)?'1':'2');
        refresh();
        while (bot_level[i] == 0) {
            key = getch();
            switch (key) {
                case 'w':
                case 'W':
                case KEY_UP:
                    if (new_option > 0) {
                        old_option = new_option;
                        new_option--;
                        display_main_menu(old_option, new_option, bot_levels, logo, 3);
                    }
                    break;
                case 's':
                case 'S':
                case KEY_DOWN:
                    if (new_option < 2) {
                        old_option = new_option;
                        new_option++;
                        display_main_menu(old_option, new_option, bot_levels, logo, 3);
                    }
                    break;
                case 'e':
                case KEY_ENTER:
                case 10:
                case 13:
                    if (new_option!=2)
                        bot_level[i] = new_option + 1;
                    else {
                        bot_level[0] = 0;
                        bot_level[1] = 0;
                        return 1;
                    }
                    break;
                default:
                    break;
            }
        }
    }
    return 0;
    }

int pick_botcount(int * player_count, int * bot_count, int * bot_level,char logo[23][35]){
    option bot_ngcommands[4];
    bot_ngcommands[0].tekst=strtoch("Jedan bot");
    bot_ngcommands[1].tekst=strtoch("Dva bota");
    bot_ngcommands[2].tekst=strtoch("Bez botova");
    bot_ngcommands[3].tekst=strtoch("Natrag");
    int bot_options, new_option=0, old_option=-1, key, back=0;
    if (*player_count==1) {
        bot_ngcommands[2].tekst=bot_ngcommands[3].tekst;
        bot_options=3;
    }
    else bot_options=4;
    while (!back) {
        clear(); new_option=0; old_option=-1;
        display_main_menu(old_option,new_option, bot_ngcommands,logo, bot_options);
        while (*bot_count == -1) {
            key = getch();
            switch (key) {
                case 'w':
                case 'W':
                case KEY_UP:
                    if (new_option > 0) {
                        old_option = new_option;
                        new_option--;
                        display_main_menu(old_option, new_option, bot_ngcommands, logo, bot_options);
                    }
                    break;
                case 's':
                case 'S':
                case KEY_DOWN:
                    if (new_option < bot_options - 1) {
                        old_option = new_option;
                        new_option++;
                        display_main_menu(old_option, new_option, bot_ngcommands, logo, bot_options);
                    }
                    break;
                case 'e':
                case 10:
                case 13:
                case KEY_ENTER:
                    if (bot_options == 3) {
                        if (new_option == 2) return 1;
                        else *bot_count = new_option + 1;
                    }
                    else {
                        if (new_option == 3) return 1;
                        else {
                            if (new_option == 2) {
                                *bot_count = 0;
                            }
                            else *bot_count = new_option + 1;
                        }
                    }
                    break;
                default:
                    break;
            }
        }
        if (bot_count) {
            back = pick_bot_levels(bot_count, bot_level, logo);
            if (back){
                *bot_count=-1;
                back=0;
            }
            else return 0;
        }

    }
}

int pick_player_number(int * player_count,int * bot_count, int bot_level[2], char logo[23][35]){
    int key, new_option=0, old_option=-1, back=0;
    option player_ngcommands[3];
    player_ngcommands[0].tekst=strtoch("Jedan igrac");
    player_ngcommands[1].tekst=strtoch("Dva igraca");
    player_ngcommands[2].tekst=strtoch("Natrag");
    while (!back) {
        display_main_menu(old_option, new_option, player_ngcommands, logo, 3);
        key = getch();
        switch (key) {
            case 'w':
            case 'W':
            case KEY_UP:
                if (new_option > 0) {
                    old_option = new_option;
                    new_option--;
                    display_main_menu(old_option, new_option, player_ngcommands, logo, 3);
                }
                break;
            case 's':
            case 'S':
            case KEY_DOWN:
                if (new_option < 2) {
                    old_option = new_option;
                    new_option++;
                    display_main_menu(old_option, new_option, player_ngcommands, logo, 3);
                }
                break;
            case 'e':
            case 10:
            case 13:
            case KEY_ENTER:
                if (new_option != 2) {
                    *player_count = new_option + 1;
                    break;
                }
                else return 1;
            default:
                break;
        }
        if (*player_count) {
            back = pick_botcount(player_count, bot_count, bot_level, logo);
            if (back) {
                back=0;
                *player_count=0;
                clear();
                new_option=0; old_option=-1;
            }
            else return 0;
        }
    }
}

void newgame_menu(char logo[23][35], int colors[4]){

    // izbor broja igraca
    int new_option=0,old_option=-1, key;
    int player_count=0, bot_count=-1, bot_level[2]={0};
    int back=pick_player_number(&player_count,&bot_count,bot_level,logo);
    if (back) return;
    else
        play_game(player_count, bot_count, bot_level, colors);
    //high score update ???
    return;
}

void pick_colors(int colors[4]){

}

void high_scores(){

}

void demo_game(){

}

void display_about(){

}