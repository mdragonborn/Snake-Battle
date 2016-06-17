#include <stdlib.h>
#include <curses.h>
#include "display.h"

int main() {
    WINDOW * win;
    option commands[6];
    int colors[4];
    colors[0]=1; colors[1]=2; colors[2]=3; colors[3]=4;
    commands[0].tekst = strtoch("Nova igra");
    commands[1].tekst = strtoch("High scores");
    commands[2].tekst = strtoch("Podesavanja");
    commands[3].tekst = strtoch("Demo");
    commands[4].tekst = strtoch("About");
    commands[5].tekst = strtoch("Izlaz");
    int options = 6;
    //logo = load_logo();
    char logo[23][35]={"....[[[[[[[[......................\n",
            "....[......[......................\n",
            "....[.####.[......................\n",
            "....[.#..#.[[[[[[[[[[[[[[[[[......\n",
            "....[.#................###.[......\n",
            "....[.####.###.###.#.#.#...[......\n",
            "....[....#.#.#.#.#.#.#.##..[......\n",
            "....[.#..#.#.#.###.##..#...[......\n",
            "....[.####.#.#.#.#.#.#.###.[......\n",
            "....[......#.#.#.#.#.#.....[[[....\n",
            "....[.###....................[....\n",
            "....[.#..#.............#.###.[....\n",
            "....[.###..###.###.###.#.#...[....\n",
            "....[.#..#.#.#..#...#..#.##..[....\n",
            "....[.#..#.###..#...#..#.#...[....\n",
            "....[.###..#.#..#...#..#.###.[....\n",
            "[[[[[........................[[[[[\n",
            "[................................[\n",
            "[.==...====..====..=====.====....[\n",
            "[.=========================[=....[\n",
            "[.======..====..=====.=======....[\n",
            "[.....................=======]]..[\n",
            "[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[["};

// glavni meni
    int new_select = 0, past_select = -1;
    screen(winh, winw);

    display_main_menu(past_select, new_select, commands, logo, options);
    int key = getch();
    while (key != 'Q' && key != 'q') {
        switch (key) {
            case KEY_ENTER:
            case 10:
            case 13:
            case 'e':
                past_select = -1;
                erase();
                refresh();
                switch (new_select) {
                    case 4:
                        exit(0);
                    case 0:
                        newgame_menu(logo, colors);
                        break;
                    case 2:
                        pick_colors(&colors, logo);
                        break;
                    default:
                        break;
                }
                erase();
                display_main_menu(past_select, new_select, commands, logo, options);
                break;
            case 'w':
            case 'W':
            case KEY_UP:
                if (new_select != 0) {
                    past_select = new_select;
                    new_select--;
                    display_main_menu(past_select, new_select, commands, logo, options);
                }
                break;
            case 's':
            case 'S':
            case KEY_DOWN:
                if (new_select != 4) {
                    past_select = new_select;
                    new_select++;
                    display_main_menu(past_select, new_select, commands, logo, options);
                }
                break;
            default:
                break;
        }
        noecho();
        keypad(stdscr, TRUE);
        raw();
        key = getch();
    }
    int i;
    for (i=0;i<options;i++)
        free(commands[i].tekst);
}