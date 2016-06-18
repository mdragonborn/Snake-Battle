//
// Created by bulse_eye on 9.6.2016.
//

#ifndef PROJEKAT_LOGIC_H
#define PROJEKAT_LOGIC_H
#define W 119
#define A 97
#define S 115
#define D 100
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80



typedef struct coordinates {
    int x;
    int y;
    int dir;
    int blank;
}coord;

typedef struct board_with_dimens {
    int **brd;
    int n;
    int moves;
    coord* heads;
}board;

typedef struct next_move_with_delay {
    coord* next;
    char time[10];
    double delay;
}next_m;

int toggle(int a);
next_m next_move(board map, coord* current, int moves, int* lives, int* prev_lives, int* zvuk);
coord* initialise(board map, int br_botova, int br_igraca, int moves, int* lives);
void copy_coord(coord * source, coord * target);
board reset_board(board map);
void error();
int* check_death(board map, coord current[4], int* lives);
board make_map(int n);
void print_board(board map);
coord zero_case(coord current);
coord *move_player(coord current[4], int input, int hor1, int hor2, int moves, board map, int* lives, int* prev_lives);
int game_over(int* lives);
void update_map(board map, coord* current);

int players_dead(int* lives);

#endif //PROJEKAT_LOGIC_H
