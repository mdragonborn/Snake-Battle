#include <stdio.h>
#include "display.h"
#include "logic.h"
#include <stdlib.h>
#include "mtwist.h"
#include <windows.h>
#include <math.h>

int play_game(int player_count, int bot_count, int bot_level[2], int colors[4]){
    int sound = 1;
    int hor1, hor2, death, brojac, blank1, blank2, blank3, blank4;
    int *lives, *prev_lives;
    int i;
    next_m next;
    coord * current, * previous;
    board map=make_map(MAP_SIZE);

    time_t t;

    lives = calloc(sizeof(int), 4);
    prev_lives = calloc(sizeof(int), 4);

    while (1) {

        if (sound) PlaySound(TEXT("snake_battle_music.wav"), NULL, SND_LOOP | SND_ASYNC | SND_NODEFAULT | SND_FILENAME);
        for (i = 0; i < 4; i++) lives[i] = 0;
        for (i = 0; i < player_count; i++) lives[i] = 1;
        for (i = 0; i < bot_count; i++) lives[i+2] = 1;
        for (i = 0; i < 4; i++) prev_lives[i] = lives[i];
        blank1 = blank2 = blank3 = blank4 = 0;
        brojac = 0;
        map = reset_board(map);
        current = initialise(map, bot_count, player_count, map.moves, lives);
        map.heads = current;
        init_map();
        previous=(coord*)calloc(sizeof(coord),4);
        display_map(current,previous,colors);
        Sleep(1000);

        while(1){
            brojac = brojac % 10 + 1;
            map.moves++;
            if (brojac == 1){
                blank1 = (int) rintl((mt_ldrand() * 9)) + 1;
                blank2 = (int) rintl((mt_ldrand() * 9)) + 1;
                blank3 = (int) rintl((mt_ldrand() * 9)) + 1;
                blank4 = (int) rintl((mt_ldrand() * 9)) + 1;
            }

            next=next_move(map,current, map.moves, lives, prev_lives);
            if (brojac == blank1){
                next.next[0].blank = 1;
            }
            if (brojac == blank2){
                next.next[1].blank = 1;
            }
            if (brojac == blank3){
                next.next[2].blank = 1;
            }
            if (brojac == blank4){
                next.next[3].blank = 1;
            }
            Sleep((DWORD)next.delay);
            for (i = 0; i < 4; i++) prev_lives[i] = lives[i];
            check_death(map,next.next, lives);

            copy_coord(current,previous);
            copy_coord(next.next,current);
            map.heads = next.next;
            display_map(current,previous,colors);
            update_map(map,next.next);
            if (game_over(lives)) break;

            //print_board(map);
        }
        display_map(current,previous,colors);
        free(previous); free(current);
        Sleep(1000);
    }

}
