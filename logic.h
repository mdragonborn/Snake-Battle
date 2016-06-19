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
    int score;
    int bot_level;
}coord;

typedef struct board_with_dimens {
    int **brd;
    int n;
    int moves;
    char* timer;
    coord* heads;
}board;

typedef struct sPlayer {
    char name[10];
    int score;
}player;

typedef struct next_move_with_delay {
    coord* next;
    char time[10];
    int ee;
    double delay;
}next_m;
int time_to_str(double x, char* timer);
int toggle(int a);
next_m next_move(board map, coord* current, int moves, int* lives, int* prev_lives, int* zvuk, int E);
coord* initialise(board map, int br_botova, int br_igraca, int moves, int* lives, int first, int* scores, int* bot_level);
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
int did_death_occur(int* lives, int* prev);
#endif //PROJEKAT_LOGIC_H
