#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <math.h>
#include "mtwist.h"
#include "logic.h"
#define W 119
#define A 97
#define S 115
#define D 100
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80

void main() {
    coord *current;
    board map;
    int input, hor1, hor2, death, i, t;
    clock_t target, start, end;
    double delay;
    mt_seed();
    map = make_map(40);
    while (1) {

        map = reset_board(map);

        current = initialise(map);

        for (i = 0; i < 3; i++){
            system("cls");
            print_board(map);
            printf("\nGAME STARTS IN %d SECONDS!!!", 3-i);
            Sleep(1000);
        }

        while (1) {
            input = 0;

            target = clock() + (clock_t) TIMEOUT;

            while ((end = clock()) < target) {
                if (_kbhit()) {
                    input = _getch();
                    break;
                }
            }
            //printf(" %d %d ", TIMEOUT, end - start);

            delay = TIMEOUT - (int) (end - target);
            printf("\n\n***\n%lf\n***\n\n", delay);
            if (delay < 0) delay = TIMEOUT;
            Sleep((DWORD)delay);

            hor1 = (current[0].dir == 1 || current[0].dir == 3);
            hor2 = (current[1].dir == 1 || current[1].dir == 3);
            current = move_player(current, input, hor1, hor2);
            death = check_death(map, current);
            if (death) {
                game_over(death);
                break;
            }
            update_map(map, current);
            system("cls");
            print_board(map);

        }

    }
}