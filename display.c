#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include "logic.h"
#include "playgame.h"
#include "mtwist.h"
#include <math.h>
#include "display.h"

#define winh 52
#define winw 80
#define BGD 2
#define BGDC 6
#define BOARD_SIZE 30
#define WHITE_BLACK 2               //parovi boja
#define GREEN_BLACK 1
#define RED_BLACK 4
#define YELLOW_BLACK 3
#define BLUE_BLACK 5
#define MAP_SIZE 50
#define OFFX 0
#define OFFY 0
#define fill 65584

chtype  * strtoch(char * string){
    int len=strlen(string), i;
    chtype * converted=(chtype*)calloc(sizeof(chtype), (unsigned)len+1);
    for (i=0;i<len+1;i++)
        converted[i]=(chtype)string[i];
    return converted;
}

chtype * numtoch(int num){
    char string[3];
    itoa(num,string,10);
    chtype * converted=strtoch(string);
    return converted;
}

void add_chstring(int top, int left, chtype * string, int color_pair, int bold){
    int i;
    if (bold) for(i=0;string[i]!=0;i++)
            mvaddch(top, left+i,string[i]|COLOR_PAIR(color_pair)|A_BOLD);
    else for(i=0;string[i]!=0;i++)
            mvaddch(top, left+i,string[i]|COLOR_PAIR(color_pair));
    return;
}

void addsqr(int top, int left, int size, int color_pair, int bold){
    int i, j;
    if (!bold)
    for (i=0;i<size;i++)
        for(j=0;j<size;j++) {
            mvaddch(top + i, left + j, fill | COLOR_PAIR(color_pair) | A_BOLD);
            refresh();
        }
    else
        for (i=0;i<size;i++)
            for(j=0;j<size;j++) {
                mvaddch(top + i, left + j, fill | COLOR_PAIR(color_pair));
                refresh();
            }
    return;
}

void updatesqr(int top_old, int left_old, int top_new, int left_new){
    int i,j; chtype edge;
    for(i=0;i<5;i++){
        if (i==0 || i==4){
            for (j=0;j<5;j++)
            mvaddch(top_old+i,left_old+j,' '|COLOR_PAIR(BGD));
        }
        else{
            mvaddch(top_old+i,left_old,' '|COLOR_PAIR(BGD));
            mvaddch(top_old+i,left_old+4,' '|COLOR_PAIR(BGD));
        }
    }
    for(i=0;i<5;i++){
        if (i==0 || i==4){
            for (j=0;j<5;j++){
                if (i==0 && j==0) edge=ACS_ULCORNER;
                else if(i==0 && j==4) edge=ACS_URCORNER;
                else if(i==4 && j==0) edge=ACS_LLCORNER;
                else if (i==4&& j==4) edge=ACS_LRCORNER;
                else edge=ACS_HLINE;
                mvaddch(top_new+i,left_new+j,edge|COLOR_PAIR(BGD));
            }
        }
        else{
            mvaddch(top_new+i,left_new,ACS_VLINE|COLOR_PAIR(BGD));
            mvaddch(top_new+i,left_new+4,ACS_VLINE|COLOR_PAIR(BGD));
        }
    }
    return;
}

void screen(int h, int w){
    initscr();
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
        init_pair(BGD, COLOR_WHITE, COLOR_BLACK); //2
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);
        init_pair(4, COLOR_RED, COLOR_BLACK);
        init_pair(5,COLOR_CYAN,COLOR_BLACK);
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
    int pair;
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

void display_main_menu(int old_option, int new_option, option * commands, char logo[23][35], int n_commands) { //n_commands ???
    chtype dot[]={'<', ' ' ,'>', ' ',0}, selected[]={'<',ACS_DIAMOND,'>', ' ',0};
    int lmarg = (winw-17)/2,
            tmarg = 35;
    int i;
    if (old_option == -1) {
        set_bckgd(2);
        add_logo(8, lmarg-10, logo);
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

void display_color_menu(int old_option, int new_option, int prev_player, int curr_player, char logo[23][35], int avail[4]){
    int lmarg = (winw-16)/2,
            tmarg = 40;
    int color_options[]={BLUE_BLACK,GREEN_BLACK,RED_BLACK,WHITE_BLACK};
    int i;
    if (old_option == -1 || prev_player!=curr_player) {
        clear();
        set_bckgd(2);
        add_logo(8, (winw-17)/2-10, logo);
        for (i = 0; i < 4; i++)
        {
            addsqr(tmarg+((i>1)?5:0), lmarg+(i%2)*5,3,color_options[i],avail[i]);
        }
        i=0; while (avail[i]) i++;
        updatesqr(tmarg+((i>1)?5:0)-1,lmarg+(i%2)*5-1, tmarg+((i>1)?5:0)-1, lmarg+(i%2)*5-1);
    }
    else {
        updatesqr(tmarg+((old_option>1)?5:0)-1,lmarg+(old_option%2)*5-1, tmarg+((new_option>1)?5:0)-1, lmarg+(new_option%2)*5-1);
    }
    refresh();
    return;
}

void init_map(){
    erase();
    set_bckgd(1); int i;
    mvaddch(0,0,ACS_ULCORNER|COLOR_PAIR(YELLOW_BLACK)|A_BOLD);
    mvaddch(0,MAP_SIZE+1,ACS_URCORNER|COLOR_PAIR(YELLOW_BLACK)|A_BOLD);
    mvaddch(MAP_SIZE+1,0,ACS_LLCORNER|COLOR_PAIR(YELLOW_BLACK)|A_BOLD);
    mvaddch(MAP_SIZE+1,MAP_SIZE+1,ACS_LRCORNER|COLOR_PAIR(YELLOW_BLACK)|A_BOLD);
    for(i=1;i<=MAP_SIZE;i++) {
        mvaddch(i, 0, ACS_VLINE | COLOR_PAIR(YELLOW_BLACK) | A_BOLD);
        mvaddch(i, MAP_SIZE+1, ACS_VLINE | COLOR_PAIR(YELLOW_BLACK) | A_BOLD);
        mvaddch(0, i, ACS_HLINE | COLOR_PAIR(YELLOW_BLACK) | A_BOLD);
        mvaddch(MAP_SIZE+1,i, ACS_HLINE | COLOR_PAIR(YELLOW_BLACK) | A_BOLD);
    }
    return;
}

void toggle_pause(int pause){
    chtype psch[]={ACS_CKBOARD, ' ', ACS_CKBOARD, 0};
    chtype clear[]={' ', ' ', ' ', 0};
    if(pause) {
        add_chstring(8, MAP_SIZE + 8, psch, YELLOW_BLACK, 1);
        add_chstring(6, MAP_SIZE+8, psch, YELLOW_BLACK,1);
        add_chstring(7, MAP_SIZE+8, psch, YELLOW_BLACK,1);
    }
    else {
        add_chstring(8, MAP_SIZE + 8, clear, YELLOW_BLACK, 1);
        add_chstring(6, MAP_SIZE+8, clear, YELLOW_BLACK,1);
        add_chstring(7, MAP_SIZE+8, clear, YELLOW_BLACK,1);
    }
    return;
}

void update_score(chtype names[4][10], int color[],coord * current, int color_pairs[]){
    int stampano[4]={0}; int i,j, max, player, allPrinted=4;
    chtype clear[]={' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',0};
    chtype * score;
    while(allPrinted){
        max=-1;
        for(j=0;j<4;j++){
            if ((allPrinted==4 || allPrinted==3) && current[j].x==-1) {
                stampano[j]=1;
                allPrinted--;
            }
            if (current[j].x!=-1 && current[j].score>max && !stampano[j]){
                max=current[j].score;
                player=j;
            }
        }
        score=numtoch(current[player].score);
        add_chstring(14-allPrinted,MAP_SIZE+4,clear,BGD,1);
        add_chstring(14-allPrinted,MAP_SIZE+4,names[color[player]],color_pairs[color[player]],1);
        add_chstring(14-allPrinted, MAP_SIZE+16, score, color_pairs[color[player]],1);
        free(score);
        stampano[player]=1; allPrinted--;
    }
    return;
}

void display_map(coord * current, coord * prev, int colors[4], char * time, int ee){
    int i, col;
    chtype names[4][10]={{'B','l','u','e','b','e','l','l',0},{'G','r','e','e','n','l','e','e',0},{'F','r','e','d',0},{'G','r','e','y','d','o','n',0}};
    int color_options[]={BLUE_BLACK,GREEN_BLACK,RED_BLACK,WHITE_BLACK};
    chtype * timech=strtoch(time);
    add_chstring(4, MAP_SIZE+6,timech,YELLOW_BLACK,1);
    update_score(names,colors, current, color_options);
    free(timech);
    if (prev[0].x==prev[1].x && prev[0].x==0){
        for (i=0;i<4;i++) {
            if (current[i].x==-1) continue;
            col=color_options[colors[(ee)?(int)(rintl((mt_ldrand() * 3)) + 1):i]];
            switch (current[i].dir) {
                case 0:
                    mvaddch(current[i].x + OFFX+1, current[i].y + OFFY, ACS_VLINE | COLOR_PAIR(col) | A_BOLD);
                    mvaddch(current[i].x + OFFX, current[i].y + OFFY, ACS_DIAMOND | COLOR_PAIR((ee)?(int)(rintl((mt_ldrand() * 5)) + 1):3) | A_BOLD);
                    break;
                case 1:
                    mvaddch(current[i].x + OFFX, current[i].y + OFFY-1, ACS_HLINE | COLOR_PAIR(col) | A_BOLD);
                    mvaddch(current[i].x + OFFX, current[i].y + OFFY, ACS_DIAMOND | COLOR_PAIR((ee)?(int)(rintl((mt_ldrand() * 5)) + 1):3) | A_BOLD);
                    break;
                case 2:
                    mvaddch(current[i].x + OFFX-1, current[i].y + OFFY, ACS_VLINE | COLOR_PAIR(col) | A_BOLD);
                    mvaddch(current[i].x + OFFX, current[i].y + OFFY, ACS_DIAMOND | COLOR_PAIR((ee)?(int)(rintl((mt_ldrand() * 5)) + 1):3) | A_BOLD);
                    break;
                case 3:
                    mvaddch(current[i].x + OFFX, current[i].y + OFFY+1, ACS_HLINE | COLOR_PAIR(col) | A_BOLD);
                    mvaddch(current[i].x + OFFX, current[i].y + OFFY, ACS_DIAMOND | COLOR_PAIR((ee)?(int)(rintl((mt_ldrand() * 5)) + 1):3) | A_BOLD);
                    break;
                default:
                    break;
            }
            refresh();
        }
    }
    else{
        chtype next=ACS_DIAMOND;
        for(i=0;i<4;i++){
            if (current[i].x==-1) continue;
            col=color_options[colors[(ee)?(int)(rintl((mt_ldrand() * 3))) : i]];
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
            mvaddch(current[i].x+OFFX,current[i].y+OFFX,ACS_DIAMOND|COLOR_PAIR((ee)?(int)(rintl((mt_ldrand() * 5)) + 1):3)|A_BOLD);


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
        add_chstring(34, 33,pick,BGD,1); mvaddch(34,37,(i==0)?'1':'2');
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

void pick_colors(int colors[4], char logo[23][35]){
    option players[4]; int old_option=-1, new_option=0, prevplayer=0, currplayer=0, key, i;
    int avail[4]={0};
    players[0].tekst=strtoch("Player 1 - WASD");
    players[1].tekst=strtoch("Player 2 - Arrows");
    players[2].tekst=strtoch("Bot 1");
    players[3].tekst=strtoch("Bot 2");
    display_color_menu(old_option,new_option, prevplayer, currplayer,logo, avail);
    while (currplayer!=4){
        add_chstring(34, 30,players[currplayer].tekst,BGD,1);
        key=getch();
        switch (key){
            case KEY_UP: case 'w': case 'W':
                if (new_option>1 && !avail[new_option%2]) {
                    old_option = new_option;
                    new_option = new_option % 2;
                    display_color_menu(old_option,new_option, prevplayer,currplayer,logo,avail);
                }
                break;
            case KEY_DOWN: case 's': case 'S':
                if (new_option<=1 && !avail[new_option%2+2]) {
                    old_option = new_option;
                    new_option = new_option % 2+2;
                    display_color_menu(old_option,new_option, prevplayer,currplayer,logo,avail);
                }
                else if (new_option<=1 && avail[0] && avail[3] && !avail[2] && !avail[1]){
                    old_option=new_option; new_option=2;
                    display_color_menu(old_option,new_option,prevplayer,currplayer,logo,avail);
                }
                else if (new_option<=1 && avail[1] && avail[2] && !avail[0] && !avail[3]){
                    old_option=new_option; new_option=3;
                    display_color_menu(old_option,new_option,prevplayer,currplayer,logo,avail);
                }
                break;
            case KEY_LEFT: case 'a': case 'A':
                if (new_option%2 && !avail[new_option-1]){
                    old_option=new_option;
                    new_option--;
                    display_color_menu(old_option,new_option, prevplayer,currplayer,logo,avail);
                }
                else if (new_option%2 && avail[0] && avail[3] && !avail[2] && !avail[1]){
                    old_option=new_option; new_option=2;
                    display_color_menu(old_option,new_option,prevplayer,currplayer,logo,avail);
                }
                else if (new_option%2 && avail[1] && avail[2] && !avail[0] && !avail[3]){
                    old_option=new_option; new_option=0;
                    display_color_menu(old_option,new_option,prevplayer,currplayer,logo,avail);
                }
                break;
            case KEY_RIGHT: case 'd': case 'D':
                if (!(new_option%2)&&!avail[new_option+1]){
                    old_option=new_option;
                    new_option++;
                    display_color_menu(old_option,new_option, prevplayer,currplayer,logo,avail);
                }
                else if (!(new_option%2) && avail[0] && avail[3] && !avail[2] && !avail[1]){
                    old_option=new_option; new_option=1;
                    display_color_menu(old_option,new_option,prevplayer,currplayer,logo,avail);
                }
                else if (!(new_option%2) && avail[1] && avail[2] && !avail[0] && !avail[3]){
                    old_option=new_option; new_option=3;
                    display_color_menu(old_option,new_option,prevplayer,currplayer,logo,avail);
                }
                break;
            case KEY_ENTER: case 10: case 13: case 'e': case 'E':
                avail[new_option]=1;
                colors[currplayer]=new_option;
                prevplayer=currplayer++;
                if (currplayer==4) break;
                i=0; while (avail[i]) i++; new_option=i;
                display_color_menu(old_option,new_option,prevplayer,currplayer,logo,avail);
                prevplayer=currplayer;
                break;
            default: break;
        }
    }
    return;
}

void high_scores(char * path){
    FILE * dat=fopen(path,"rb"); int i;
    player temp; chtype * buffer;
    clear();
    for (i=0;i<10;i++) {
        fread(&temp, sizeof(player), 1, dat);
        buffer=strtoch(temp.name);
        add_chstring(10+i,10,buffer,2,1);
        free(buffer);
        buffer=numtoch(temp.score);
        add_chstring(10+i, 21, buffer, 2, 1);
        free(buffer);
    }
    i=getch();
    fclose(dat);
    return;
}

void display_about(char logo[23][35]){
    int slider=1;
    chtype * buffer;
    int lmarg = (winw-5)/2,
            tmarg = 35;
    add_logo(8, (winw-17)/2-10, logo);
    buffer=strtoch("Milena Markovic - grafika");
    add_chstring(tmarg+slider,lmarg-12,buffer,YELLOW_BLACK, 1);
    free(buffer); slider++;
    buffer=strtoch("Jelena Savic - vestacka inteligencija");
    add_chstring(tmarg+slider,lmarg-17,buffer,YELLOW_BLACK, 1);
    free(buffer); slider++;
    buffer=strtoch("Milan Zaric - logika igre");
    add_chstring(tmarg+slider,lmarg-12,buffer,YELLOW_BLACK, 1);
    free(buffer); slider++;
    buffer=strtoch("Nikola Bebic - mentor");
    add_chstring(tmarg+slider,lmarg-10,buffer,YELLOW_BLACK, 1);
    free(buffer); slider+=3;
    buffer=strtoch("Elektrotehnicki fakultet");
    add_chstring(tmarg+slider,lmarg-11,buffer,RED_BLACK, 0);
    free(buffer); slider++;
    buffer=strtoch("Univerziteta u Beogradu");
    add_chstring(tmarg+slider,lmarg-11,buffer,RED_BLACK, 0);
    free(buffer); slider+=2;
    buffer=strtoch("2016");
    add_chstring(tmarg+slider,lmarg-2,buffer,YELLOW_BLACK, 1);
    free(buffer);
    refresh();
    slider=getch();
    return;
}

void loadScore(int score, int color){
    
}

char * name_input(int lmarg, int tmarg, int maxlen, int color){
    char * string=(char*)calloc(sizeof(char),(unsigned)maxlen+1);
    chtype * printbuffer=(chtype*)calloc(sizeof(chtype),(unsigned)maxlen+1);
    int i=0, key;
    while(1){
        mvaddch(tmarg,lmarg+i,ACS_BOARD|COLOR_PAIR(color));
        key=getch();
        if(key==KEY_ENTER)
            return string;
        if(key>=32 && key<=126 && i<10) {
            string[i++] = (char) key;
            printbuffer[i++]=(chtype) key;
        }
        if(key==KEY_BACKSPACE) {
            string[i--] = 0;
            printbuffer[i++]=0;
        }
        for (key=0;key<maxlen;key++) mvaddch(tmarg,lmarg,' '|COLOR_PAIR(BGD));
        add_chstring(tmarg,lmarg,printbuffer,color,1);
    }
}

/*void pitajzaime(char *ime) {
    char brojac, i, j;
    char c;
    resize_term(5, 30);
    printw("\n          Unesi ime:\n\n     "); //10 spejsova gore, 5 dole
    brojac = 0;
    attron(COLOR_PAIR(MENITEKST));
    curs_set(2);
    while (1) {
        c = getch();
        if (c == BACKSPACE) {
            if (brojac > 0) {
                brojac--;
                mvaddch(i, --j, ' ');
                move(i, j);
            }
        }
        else if (c == '\n' || brojac == MAXSTR - 1) {
            ime[brojac] = '\0';
            resize_term(25, 50);
            clear();
            curs_set(0);
            return;
        }
            //moram ovako jer kad sam stavio isprint() iz headera ctype pucalo je kad unesem numericki minus
        else if (c >= 'a'&&c <= 'z' || c >= 'A'&&c <= 'Z' || c >= '0'&&c <= '9' || c == ' ' || c == '.' || c == '#' || c == '_') {
            ime[brojac++] = c;
            mvaddch(i, j++, c); //da bi se videlo sta se upisuje
        }
        refresh();
    }
}
*/