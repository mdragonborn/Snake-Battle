#include <stdio.h>
#include "display.h"
#include "logic.h"
#include <stdlib.h>
#include "mtwist.h"
#include <windows.h>
#include <math.h>

int play_game(int player_count, int bot_count, int bot_level[2], int col1, int col2){
    int hor1, hor2, death, brojac, blank1, blank2, blank3, blank4;
    int *lives;
    int i;
    next_m next;
    coord * current, * previous;
    board map=make_map(MAP_SIZE);

    time_t t;

    lives = calloc(sizeof(int), 4);
    for (i = 0; i <4; i++) lives[i] = 1;

    while (1) {
        for (i = 0; i <4; i++) lives[i] = 1;
        blank1 = blank2 = blank3 = blank4 = 0;
        brojac = 0;
        map = reset_board(map);
        current = initialise(map, bot_count, player_count, map.moves, lives);
        map.heads = current;
        init_map();
        previous=(coord*)calloc(sizeof(coord),4);
        display_map(current,previous,col1, col2);
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

            next=next_move(map,current, map.moves, lives);
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
            check_death(map,next.next, lives);
            if (game_over(lives)) break;
            copy_coord(current,previous);
            copy_coord(next.next,current);
            map.heads = next.next;
            display_map(current,previous,col1, col2);
            update_map(map,next.next);
            //print_board(map);
        }
        display_map(current,previous,col1,col2);
        free(previous); free(current);
        Sleep(1000);
    }

}
