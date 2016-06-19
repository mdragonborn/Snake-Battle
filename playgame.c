#include <stdio.h>
#include "display.h"
#include "logic.h"
#include <stdlib.h>
#include "mtwist.h"
#include <Windows.h>
#include <math.h>
#include "timers.h"
#pragma comment(lib, "winmm.lib")

/*int was_modified(char* path){
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
void read_name(FILE* high, char* name){

    int i = 0;
    int c;
    while ((c = fgetc(high)) != '|' ) {
        if (feof(high)) break;
        if (c == '\n') break;
        name[i++] = (char) c;
    }
    name[i] = '\0';
}


int read_score(FILE* high){
    char br[4];
    int score;
    int c;
    int i = 0;

    while ((c = fgetc(high)) != '\n'){
        if (feof(high)){
            break;
        }
        br[i++] = (char) c;
    }
    br[i] = '\0';
    if (i == 0) return -1;
    score = atoi(br);
    return score;
}

void write_one_high(FILE* high, coord current, FILE* backup char* player_name){
    int i;
    int cur_score;
    char name[11];
    char buffer[15];
    int printed;

    if (!high || !backup){
        exit(420);
    }
    printed = 0;
    while (!feof(high)){
        fscanf(high, "%s %d\n", name, &cur_score);
        printed = 0;
        if (current.score < cur_score) {
            fprintf(backup, "%s %d\n", name, cur_score);
        }
        else {
            fprintf(backup, "%s %d\n", player_name, current.score);
            printed = 1;
            break;
        }
    }
    if (!printed){
        fprintf(backup, "%s %d\n", player_name, current.score);
    }
    while (!feof(high)) {
        fscanf(high, "%s %d\n", name, &cur_score);
        fprintf(backup, "%s %d\n", name, cur_score);
    }
}

void write_high(FILE* high, coord current[4]){
    FILE* backup;
    int i;
    int cur_score;
    char name[11];
    int printed;

    for (i = 0; i < 2; i++) {
        if (current[i].x == -1) continue;
        if (i == 1 && current[i].x != -1) {
            write_one_high(path, current[i]);
        }
    }

}
*/
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
        display_map(current,previous,colors, map.timer, next.ee);
        free(previous); free(current);
        Sleep(1000);
        if (!play) break;


    }

}
