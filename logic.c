//
// Created by bulse_eye on 9.6.2016.
//
#include "logic.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <math.h>
#include "mtwist.h"
#include "ai.h"
#include <Windows.h>
#pragma comment(lib, "winmm.lib")
extern E;
void time_to_str(double x, char* timer){
    int pom;
    char pom2[4];

    pom = (((int)x) / 60000);
    itoa(pom, pom2, 10);
    timer[2] = ':';
    timer[3] = '\0';
    if (pom < 10){
        timer[0] = '0';
        timer[1] = pom2[0];
    }
    else {
        timer[0] = '\0';
        strcat(timer, pom2);
    }

    pom = (((int)x % 60000) / 1000);
    itoa(pom, pom2, 10);
    timer[5] = ':';
    timer[6] = '\0';

    if (pom < 10) {
        timer[3] = '0';
        timer[4] = pom2[0];
    }
    else {
        timer[3] = pom2[0];
        timer[4] = pom2[1];
    }

    pom = ((int)x % 1000) / 10;
    itoa(pom, pom2, 10);
    if (pom < 10){
        timer[6] = '0';
        timer[7] = pom2[0];
    }

    else {
        strcat(timer, pom2);
    }
    timer[8] = '\0';
}

int toggle(int a){
    return a == 1 ? 0 : 1;
}

void copy_coord(coord *source, coord *target) {
    int i;
    for (i = 0; i < 4; i++) {
        target[i].x = source[i].x;
        target[i].y = source[i].y;
        target[i].dir = source[i].dir;
        target[i].blank = source[i].blank;
    }
    return;
}

board reset_board(board map) {
    int i, j;
    for (i = 0; i < map.n + 2; i++) {
        map.brd[0][i] = map.brd[i][0] = map.brd[map.n + 1][i] = map.brd[i][map.n + 1] = -1;
    }
    for (i = 1; i < map.n + 1; i++) {
        for (j = 1; j < map.n + 1; j++) {
            map.brd[i][j] = 0;
        }

    }
    return map;

}

void error() {
    printf("LOL");
    system("pause");
    exit(1);
}

next_m next_move(board map, coord *current, int moves, int* lives, int* prev_lives, int* zvuk, int E) {
    next_m vrati;
    int input, hor1, hor2;
    double TIMEOUT;
    clock_t target, start, end;

    vrati.next = calloc(sizeof(coord), 4);
    vrati.ee = (E == 1) ? 1 : 0;
    input = 0;
    if (players_dead(lives))TIMEOUT = (0.01 * CLOCKS_PER_SEC);
    else TIMEOUT = (0.04 * CLOCKS_PER_SEC);
    target = clock() + (clock_t) TIMEOUT;

    while ((end = clock()) < target) {
        if (_kbhit()) {
            input = _getch();
            break;
        }
    }
    vrati.delay = TIMEOUT - (int) (end - target);
    if (vrati.delay < 0) vrati.delay = TIMEOUT;
    hor1 = (current[0].dir == 1 || current[0].dir == 3);
    hor2 = (current[1].dir == 1 || current[1].dir == 3);
    if (input == 'm'){
        vrati.ee = 0;
        if (*zvuk == 1) {
            PlaySound(NULL, NULL, 0);
            *zvuk = toggle(*zvuk);
        }
        else if (*zvuk == 0) {
            *zvuk = toggle(*zvuk);
            PlaySound(TEXT("snake_battle_music.wav"), NULL, SND_LOOP | SND_ASYNC | SND_NODEFAULT | SND_FILENAME);
        }
        vrati.next = move_player(current, 0, hor1, hor2, moves, map, lives, prev_lives);
    }
    else if (input == 32){
        vrati.delay = -32;
    }
    else if (input == 27){
        vrati.delay = -27;
    }

    if (input != 'm'){
        if (input == '9'){
            vrati.ee = 1;
            PlaySound(TEXT("snake_battle.wav"), NULL, SND_LOOP | SND_ASYNC | SND_NODEFAULT | SND_FILENAME);
        }
        vrati.next = move_player(current, input, hor1, hor2, moves, map, lives, prev_lives);
    }

    return vrati;
}

int *check_death(board map, coord current[4], int* lives) {

    int i;
    //if (current[0].x == current[1].x && current[0].y == current[1].y) return 3;
    for (i = 0; i < 4; i++) {
        if (current[i].x != -1) {
            if ((current[i].x < 0 || current[i].x > map.n + 1) || (current[i].y < 0 || current[i].y > map.n + 1)){
                lives[i] = 0;
                continue;
            }
            if (map.brd[current[i].x][current[i].y] == -1 || map.brd[current[i].x][current[i].y] != 0) {
                lives[i] = 0;
            }
        }
        else lives[i] = 0;
    }

    for (i = 1; i < 4; i++) {
        if (current[0].x == current[i].x && current[0].y == current[i].y) {
            lives[i] = lives[0] = 0;
        }
    }
    for (i = 2; i < 4; i++) {
        if (current[1].x == current[i].x && current[1].y == current[i].y) {
            lives[i] = lives[1] = 0;
        }
    }
    if (current[2].x == current[3].x && current[2].y == current[3].y) {
        lives[2] = lives[3] = 0;
    }

    return lives;
}

board make_map(int n) {
    board map;
    int i;
    map.n = n;
    map.moves = 0;
    map.heads = calloc(sizeof(coord), 4);
    map.brd = malloc(sizeof(int *) * (n + 2));
    if (map.brd == NULL) error();
    for (i = 0; i < n + 2; i++) {
        map.brd[i] = malloc(sizeof(int) * (n + 2));
        if (map.brd[i] == NULL) {
            map.brd = NULL;
            error();
            break;
        }
    }
    map.timer = calloc(sizeof(char), 10);
    return map;

}

void print_board(board map) {
    system("cls");
    int i, j;
    for (i = 1; i <= map.n; i++) {
        for (j = 1; j <= map.n; j++) {
            if (map.brd[i][j]) printf("%d", map.brd[i][j]);
            else putchar(' ');
        }
        putchar('\n');
    }
}


coord zero_case(coord current) {
    coord new;
    new.x = current.x;
    new.y = current.y;
    new.dir = current.dir;
    switch (current.dir) {
        case 0: {
            new.x = current.x - 1;
            break;
        }
        case 1: {
            new.y = current.y + 1;
            break;
        }
        case 2: {
            new.x = current.x + 1;
            break;
        }
        case 3: {
            new.y = current.y - 1;
            break;
        }
        default:
            break;
    }
    return new;
}
int* is_in_wall(coord* current, board map){
    int i;
    int* vrati;

    vrati = calloc(sizeof(int), 4);
    for (i = 0; i < 4; i++){
        if (current[i].x == 0 || current[i].y == 0 || current[i].x == (map.n + 1) || current[i].y == (map.n + 1)){
            vrati[i] = 1;
        }
        else vrati[i] = 0;
    }
    return vrati;
}

int* did_just_die(int* prev_lives, int* lives){

    int* vrati;
    int i;
    vrati = calloc(sizeof(int), 4);
    for (i = 0; i < 4; i++) vrati[i] = lives[i] ^ prev_lives[i];
    return vrati;

}
coord *move_player(coord current[4], int input, int hor1, int hor2, int moves, board map, int* lives, int* prev_lives) {

    coord *new_coord;
    coord *backup;
    int *overwrite;
    int* wall;
    int** kopija;
    coord* hard_botovi;
    int i, j, k;
    new_coord = malloc(sizeof(coord) * 4);
    backup = calloc(sizeof(coord), 4);
    copy_coord(current, backup);
    new_coord[0] = current[0];
    new_coord[1] = current[1];
    kopija = malloc(sizeof(int *) * (map.n + 2));
    if (kopija == NULL) error();
    for (i = 0; i < map.n + 2; i++) {
        kopija[i] = malloc(sizeof(int) * (map.n + 2));
        for (k = 0; k < map.n + 2; k++){
            kopija[i][k] = map.brd[i][k];
        }
        if (kopija[i] == NULL) {
            kopija = NULL;
            error();
            break;
        }
    }

    if (lives[2] != 0) new_coord[2] = mediumbot(current[2].x, current[2].y, current[2].dir, map.brd );
    else new_coord[2] = current[2];

    if (lives[3] != 0) new_coord[3] = mediumbot(current[3].x, current[3].y, current[3].dir, map.brd );
    else new_coord[3] = current[3];
    if (input == 224 && current[1].x != -1) {
        input = _getch();
        if (lives[0] != 0) new_coord[0] = zero_case(current[0]);
        else new_coord[0] = current[0];
        if (lives[1] != 0) {
            switch (input) {
                case LEFT: {
                    if (hor2) {
                        new_coord[1] = zero_case(current[1]);
                        break;
                    }
                    new_coord[1].y = current[1].y - 1;
                    new_coord[1].dir = 3;
                    break;
                }
                case UP: {
                    if (!hor2) {
                        new_coord[1] = zero_case(current[1]);
                        break;
                    }
                    new_coord[1].x = current[1].x - 1;
                    new_coord[1].dir = 0;
                    break;
                }
                case RIGHT: {
                    if (hor2) {
                        new_coord[1] = zero_case(current[1]);
                        break;
                    }
                    new_coord[1].y = current[1].y + 1;
                    new_coord[1].dir = 1;
                    break;
                }
                case DOWN: {
                    if (!hor2) {
                        new_coord[1] = zero_case(current[1]);
                        break;
                    }
                    new_coord[1].x = current[1].x + 1;
                    new_coord[1].dir = 2;
                    break;
                }
                default:
                    break;
            }


        }
        else new_coord[1] = current[1];
    }
    else if (input != A && input != S && input != D && input != W) {
        if (current[0].x != -1)
            new_coord[0] = zero_case(current[0]);
        if (current[1].x != -1)
            new_coord[1] = zero_case(current[1]);
    }
    else {
        if (lives[1] != 0) new_coord[1] = zero_case(current[1]);
        else new_coord[1] = current[1];
        if (lives[0] != 0) {
            switch (input) {
                case A: {
                    if (hor1) {
                        new_coord[0] = zero_case(current[0]);
                        break;
                    }
                    new_coord[0].y = current[0].y - 1;
                    new_coord[0].dir = 3;
                    break;
                }
                case W: {
                    if (!hor1) {
                        new_coord[0] = zero_case(current[0]);
                        break;
                    }
                    new_coord[0].x = current[0].x - 1;
                    new_coord[0].dir = 0;
                    break;
                }
                case D: {
                    if (hor1) {
                        new_coord[0] = zero_case(current[0]);
                        break;
                    }
                    new_coord[0].y = current[0].y + 1;
                    new_coord[0].dir = 1;
                    break;
                }
                case S: {
                    if (!hor1) {
                        new_coord[0] = zero_case(current[0]);
                        break;
                    }
                    new_coord[0].x = current[0].x + 1;
                    new_coord[0].dir = 2;
                    break;
                }
            }
        }
        else new_coord[0] = current[0];
    }
    check_death(map, new_coord, lives);
    overwrite = did_just_die(prev_lives, lives);
    wall = is_in_wall(new_coord, map);
    for (i = 0; i < 4; i++){
        if ((lives[i] == 0 && !overwrite[i]) || wall[i]){
            new_coord[i] = backup[i];
        }
    }
    return new_coord;
}

int game_over(int *lives) {
    int i;
    int t = 0;

    for (i = 0; i < 4; i++) {
        if (lives[i] == 0) t++;
    }
    if (t >= 3) return 1;
    else return 0;
}

void update_map(board map, coord *current) {

    int i;

    for (i = 0; i < 4; i++) {
        if (current[i].blank != 1 && current[i].x != -1) map.brd[current[i].x][current[i].y] = i + 1;
    }


}

coord *initialise(board map, int br_botova, int br_igraca, int moves, int* lives) {
    coord *new;
    int i;
    new = malloc(sizeof(coord) * 4);
    new[0].x = (int) rintl((mt_ldrand() * (map.n - 1))) + 1;
    new[0].y = (int) rintl((mt_ldrand() * (map.n - 1))) + 1;
    new[0].dir = (int) rintl((mt_ldrand() * 3));
    while (new[0].x == 1 && new[0].dir == 0) {
        new[0].dir = (int) rintl((mt_ldrand() * 3));
    }
    while (new[0].y == 1 && new[0].dir == 3) {
        new[0].dir = (int) rintl((mt_ldrand() * 3));
    }
    while (new[0].x == map.n && new[0].dir == 2) {
        new[0].dir = (int) rintl((mt_ldrand() * 3));
    }
    while (new[0].y == map.n && new[0].dir == 1) {
        new[0].dir = (int) rintl((mt_ldrand() * 3));
    }

    if (br_igraca == 2) {
        new[1].x = (int) rintl((mt_ldrand() * (map.n - 1))) + 1;
        new[1].y = (int) rintl((mt_ldrand() * (map.n - 1))) + 1;
        new[1].dir = (int) rintl((mt_ldrand() * 3));
        while (new[1].x == 1 && new[1].dir == 0) {
            new[1].dir = (int) rintl((mt_ldrand() * 3));
        }
        while (new[1].y == 1 && new[1].dir == 3) {
            new[1].dir = (int) rintl((mt_ldrand() * 3));
        }
        while (new[1].x == map.n && new[1].dir == 2) {
            new[1].dir = (int) rintl((mt_ldrand() * 3));
        }
        while (new[1].y == map.n && new[1].dir == 1) {
            new[1].dir = (int) rintl((mt_ldrand() * 3));
        }

    }
    else {
        new[1].x = -1;
        new[1].y = -1;
        new[1].dir = -1;

    }
    for (i = 0; i < br_botova; i++) {
        new[i + 2].x = (int) rintl((mt_ldrand() * (map.n - 1))) + 1;
        new[i + 2].y = (int) rintl((mt_ldrand() * (map.n - 1))) + 1;
        new[i + 2].dir = (int) rintl((mt_ldrand() * 3));
        while (new[0].x == 1 && new[i + 2].dir == 0) {
            new[i + 2].dir = (int) rintl((mt_ldrand() * 3));
        }
        while (new[i + 2].y == 1 && new[i + 2].dir == 3) {
            new[i + 2].dir = (int) rintl((mt_ldrand() * 3));
        }
        while (new[i + 2].x == map.n && new[i + 2].dir == 2) {
            new[i + 2].dir = (int) rintl((mt_ldrand() * 3));
        }
        while (new[i + 2].y == map.n && new[i + 2].dir == 1) {
            new[i + 2].dir = (int) rintl((mt_ldrand() * 3));
        }
    }

    for (i = 0; i < (2 - br_botova); i++) {
        new[3-i].x = -1;
        new[3-i].y = -1;
        new[3-i].dir = -1;
    }

    update_map(map, new);
    new = move_player(new, 0, 0, 0, moves, map, lives, lives);
    update_map(map, new);


    return new;
}

int players_dead(int* lives){
    int i;

    for (i = 0; i < 2; i++){
        if (lives[i] == 1) return 0;
    }
    return 1;
}
