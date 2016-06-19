#include <stdio.h>
#include "display.h"
#include "logic.h"
#include <stdlib.h>
#include "mtwist.h"
#include <Windows.h>
#include <math.h>
#include "timers.h"
#include "playgame.h"

#pragma comment(lib, "winmm.lib")

int was_modified(char* path){
    FILE* high;
    int result;
    int last = 0;

    high = fopen(path, "r");
    result = fgetc(high);
    while (!feof(high)){
        last = fgetc(high);
        result ^= last;
    }
    result ^= last;
    fclose(high);
    if (last != result) return 1;
    return 0;
}

void write_xor(char* path){
    FILE* high;
    int result;

    high = fopen(path, "r");
    result = fgetc(high);
    while (!feof(high)) {
        result ^= fgetc(high);
    }
    fclose(high);
    high = fopen(path, "a");
    fputc(result, high);
    fclose(high);
}

void sort_players(player players[11]){
    int i, j;
    player t;

    for (i = 0; i < 10; i++){
        for (j = i+1; j < 11; j++){
            if (players[i].score < players[j].score) {
                t = players[i];
                players[i] = players[j];
                players[j] = t;
            }
        }
    }
}
void create_new_bin(char* path){
    FILE* init;
    int i;
    player temp;

    init = fopen(path, "wb");
    for (i = 0; i < 10; i++){
        strcpy(temp.name, "---");
        temp.score = 0;
        fwrite(&temp, sizeof(player), 1, init);
    }
    fclose(init);
}
void write_one_high(char* path, coord current, char* player_name){
    int i;
    int position = 0;
    int end;
    int cur_score;
    player players[11];
    char name[11];
    char buffer[15];
    int printed;
    FILE* high;

    high = fopen(path, "rb");
    if (!high){
        exit(420);
    }
    //popuni ako je prazno
    for (i = 0; i < 10; i++) {
        fread(&players[i], sizeof(player), 1, high);
    }
    players[10].score =  current.score;
    strcpy(players[10].name, name);
    sort_players(players);
    fclose(high);
    high = fopen(path, "wb");

    for (i = 0; i < 10; i++){
        fwrite(&players[i], sizeof(player), 1, high);
    }

    fclose(high);

}

int play_game(int player_count, int bot_count, int bot_level[2], int colors[4]){
    int mode = 1;
    int sound = 1;
    int hor1, hor2, death, brojac, blank1, blank2, blank3, blank4;
    int *lives, *prev_lives;
    int i, E = 0;
    int pause = 0;
    double current_time = 0;
    int prva = 1;
    int first;
    int scores[4] = {0, 0, 0, 0};
    next_m next;
    coord * current, * previous;
    board map=make_map(MAP_SIZE);
    Timer t;
    int pom_timer;
    int play = 1;
    lives = calloc(sizeof(int), 4);
    prev_lives = calloc(sizeof(int), 4);
    PlaySound(TEXT("snake_battle_music.wav"), NULL, SND_LOOP | SND_ASYNC | SND_NODEFAULT | SND_FILENAME);

    current_time = 0;
    first = 1;
    while (1) {

        prva = 1;
        for (i = 0; i < 4; i++) lives[i] = 0;
        for (i = 0; i < player_count; i++) lives[i] = 1;
        for (i = 0; i < bot_count; i++) lives[i+2] = 1;
        for (i = 0; i < 4; i++) prev_lives[i] = lives[i];
        blank1 = blank2 = blank3 = blank4 = 0;
        brojac = 0;
        map = reset_board(map);
        current = initialise(map, bot_count, player_count, map.moves, lives, first, scores, bot_level);
        first = 0;
        map.heads = current;
        init_map();
        previous=(coord*)calloc(sizeof(coord),4);
        display_map(current,previous,colors, map.timer, 0);

        Sleep(1000);

        while(1){
            if (!prva) current_time += stop_timer(&t);
            play = time_to_str(current_time, map.timer);
            start_timer(&t);
            brojac = brojac % 10 + 1;
            map.moves++;
            if (brojac == 1){
                blank1 = (int) rintl((mt_ldrand() * 9)) + 1;
                blank2 = (int) rintl((mt_ldrand() * 9)) + 1;
                blank3 = (int) rintl((mt_ldrand() * 9)) + 1;
                blank4 = (int) rintl((mt_ldrand() * 9)) + 1;
            }
            if (E == 0){
                next=next_move(map,current, map.moves, lives, prev_lives, &sound, 0);

            }
            else {
                next=next_move(map,current, map.moves, lives, prev_lives, &sound, next.ee);
            }
            E = 1;
            if (next.delay == -32){
                current_time += stop_timer(&t);
                time_to_str(current_time, map.timer);
                toggle_pause(1);
                while (1) {
                    mode = getch();
                    if (mode == 32) {
                        start_timer(&t);
                        toggle_pause(0);
                        break;
                    }
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

                if (brojac == blank1) {
                    next.next[0].blank = 1;
                }

                else next.next[0].blank = 0;

                if (brojac == blank2) {
                    next.next[1].blank = 1;
                }
                else next.next[1].blank = 0;

                if (brojac == blank3) {
                    next.next[2].blank = 1;
                }
                else next.next[2].blank = 0;

                if (brojac == blank4) {
                    next.next[3].blank = 1;
                }
                else next.next[3].blank = 0;

                Sleep((DWORD) next.delay);
                for (i = 0; i < 4; i++) prev_lives[i] = lives[i];
                check_death(map, next.next, lives);

                copy_coord(current, previous);
                copy_coord(next.next, current);
                map.heads = next.next;
                display_map(current, previous, colors, map.timer, next.ee);
                update_map(map, next.next);
                for (i = 0; i < 4; i++){
                    scores[i] = next.next[i].score;
                }
                if (game_over(lives)) break;

                //print_board(map);
            }
            prva = 0;
            stop_timer(&t);

        }
        //write_high("C:\\Users\\bulse_eye\\Desktop\\Snake-Battle\\high_scores.txt", next.next);
        write_one_high("C:\\Users\\bulse_eye\\Documents\\Snake-Battle\\high_scores.bin", current[0], "MISKO");
        write_one_high("C:\\Users\\bulse_eye\\Documents\\Snake-Battle\\high_scores.bin", current[1], "DUSKO");
        display_map(current,previous,colors, map.timer, next.ee);
        free(previous); free(current);
        Sleep(1000);
        if (!play) break;


    }

}
