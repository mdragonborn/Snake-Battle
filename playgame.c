#include <stdio.h>
#include "display.h"
#include "logic.h"
#include <stdlib.h>
#include "mtwist.h"
#include <Windows.h>
#include <math.h>
#include "timers.h"
#pragma comment(lib, "winmm.lib")

int play_game(int player_count, int bot_count, int bot_level[2], int colors[4]){
    int mode = 1;
    int sound;
    int hor1, hor2, death, brojac, blank1, blank2, blank3, blank4;
    int *lives, *prev_lives;
    int i;
    int pause = 0;
    double current_time = 0;
    int prva = 1;
    next_m next;
    coord * current, * previous;
    board map=make_map(MAP_SIZE);

    Timer t;

    lives = calloc(sizeof(int), 4);
    prev_lives = calloc(sizeof(int), 4);
    PlaySound(TEXT("snake_battle_music.wav"), NULL, SND_LOOP | SND_ASYNC | SND_NODEFAULT | SND_FILENAME);


    while (1) {
        prva = 1;
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
            if (!prva) current_time += stop_timer(&t);
            time_to_str(current_time, map.timer);
            brojac = brojac % 10 + 1;
            map.moves++;
            if (brojac == 1){
                blank1 = (int) rintl((mt_ldrand() * 9)) + 1;
                blank2 = (int) rintl((mt_ldrand() * 9)) + 1;
                blank3 = (int) rintl((mt_ldrand() * 9)) + 1;
                blank4 = (int) rintl((mt_ldrand() * 9)) + 1;
            }

            next=next_move(map,current, map.moves, lives, prev_lives, &sound);
            if (next.delay == -32){
                //display sa pauzom
                while (1) {
                    mode = getch();
                    if (mode == 32) break;
                    else if (mode == 'm') {
                        if (sound) {
                            sound = toggle(sound);
                            PlaySound(NULL, NULL, 0);
                        }
                        else {
                            sound = toggle(sound);
                            PlaySound(TEXT("snake_battle_music.wav"), NULL, SND_LOOP | SND_ASYNC | SND_NODEFAULT | SND_FILENAME);
                        }
                    }
                    else if (mode == 27){
                        PlaySound(NULL, NULL, 0);
                        return 0;
                    }
                    continue;
                }
            }
            else if (next.delay == -27){
                PlaySound(NULL, NULL, 0);
                return 0;
            }
            if (next.delay != -32) {
                start_timer(&t);

                if (brojac == blank1) {
                    next.next[0].blank = 1;
                }
                if (brojac == blank2) {
                    next.next[1].blank = 1;
                }
                if (brojac == blank3) {
                    next.next[2].blank = 1;
                }
                if (brojac == blank4) {
                    next.next[3].blank = 1;
                }
                Sleep((DWORD) next.delay);
                for (i = 0; i < 4; i++) prev_lives[i] = lives[i];
                check_death(map, next.next, lives);

                copy_coord(current, previous);
                copy_coord(next.next, current);
                map.heads = next.next;
                display_map(current, previous, colors);
                update_map(map, next.next);
                if (game_over(lives)) break;

                //print_board(map);
            }
            prva = 0;
            stop_timer(&t);
        }
        display_map(current,previous,colors);
        free(previous); free(current);
        Sleep(1000);
    }

}
