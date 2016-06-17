//
// Created by bulse_eye on 17.6.2016.
//
#include "ai.h"
#include <stdio.h>
#include <stdlib.h>


void initq(r *red) {

    red->front = 0;
    red->rear = 0;
}



//insert queue
void insert(r *red, int x, int y, int dir) {

    red->rear = (red->rear + 1) % 300;
    if (red->front == red->rear) {
        printf("ERROR overflow Q");
    }
    else {

        red->q[red->rear].x = x;
        red->q[red->rear].y = y;
        red->q[red->rear].dir = dir;
        if (red->front == 0) red->front = 1;
    }
}



//delete queue
coord delete(r *red) {

    coord x;
    if (red->front == 0) {
        printf("Underflow q");
        return -1;
    }
    else{
        x = red->q[red->front];
        if (red->front == red->rear) { red->front = red->rear = 0; }
        else {

            red->front = red->(front + 1) % 300;
        }
        return x;
    }
}




coord *easybot(int x, int y, int dir, int moves, const int **brd) {
    int i, rez;
    coord out;

    srand(time(NULL));
    if (x != -1 && y != -1) {
        if (moves < 4) {
            switch (dir) {
                case 0:
                    if (brd[x - 1][y] == 0)rez = 0;
                    break;
                case 1:
                    if (brd[x][y + 1] == 0)rez = 1;
                    break;
                case 2:
                    if (brd[x + 1][y] == 0)rez = 2;
                    break;
                case 3:
                    if (brd[x][y - 1] == 0)rez = 3;
                    break;
                default:
                    skreci(brd, x, y, dir);
            }
        }
        else {
            i = rand() / (RAND_MAX + 1) * 4 + 1;
            rez = i;
        }
    }
    else rez = -1;

    out.dir = rez;
    switch (rez) {
        case 0: {
            out.x = x - 1;
            out.y = y;
            break;
        }
        case 1: {
            out.x = x;
            out.y = y + 1;
            break;
        }
        case 2: {
            out.x = x + 1;
            out.y = y;
            break;
        }
        case 3: {
            out.x = x;
            out.y = y - 1;
            break;
        }
    }
    return out;
}

int skreci(const int **brd, int x, int y, int dir) {
    if (brd[x - 1][y] == 0)return 0;
    else if (brd[x][y - 1] == 0)return 3;
    else if (brd[x + 1][y] == 0)return 2;
    else if (brd[x][y + 1] == 0)return 1;
    return dir;
}


int keepgoing(const int **brd, int x, int y, int dir) {
    int i;

    switch (dir) {
        case 0:
            if (brd[x - 1][y] == 0)return 0;
            break;
        case 1:
            if (brd[x][y + 1] == 0)return 1;
            break;
        case 2:
            if (brd[x + 1][y] == 0)return 2;
            break;
        case 3:
            if (brd[x][y - 1] == 0)return 3;
            break;
    }
    return -1;
}


coord *mediumbot(const int **brd, int x, int y, int dir) {       //pozivam za oba bota pojedinacno
    int i, skup[4], j;
    coord out;
    srand(time(NULL));

    i = keepgoing(brd, x, y, dir);
    if (i >= 0)rez = i;
    else {
        i = -1;
        if (brd[x - 1][y] == 0)skup[++i] = 0;
        if (brd[x][y + 1] == 0)skup[++i] = 1;
        if (brd[x + 1][y] == 0)skup[++i] = 2;
        if (brd[x][y - 1] == 0)skup[++i] = 3;

        j = rand() / (MAX_RAND + 1) * (i + 1);
        rez = skup[j];
        if (i == -1)rez = dir;
    }
    out.dir = rez;
    switch (rez) {
        case 0: {
            out.x = x - 1;
            out.y = y;
            break;
        }
        case 1: {
            out.x = x;
            out.y = y + 1;
            break;
        }
        case 2: {
            out.x = x + 1;
            out.y = y;
            break;
        }
        case 3: {
            out.x = x;
            out.y = y - 1;
            break;
        }
    }

    return out;
}


//vraca 0 ako ne moze i 1 ako moze
int check(int i, int j, int **map, int id, int t[5]) {  //prosledjujem koordinate polja na koje treba da upisem vrednost
    // id je broj kojim se popunjava kretanje te zmije po mapi

    if ((map[i - 1][j] == 1 || map[i - 1][j] == 2 || map[i - 1][j] == 3 || map[i - 1][j] == 4)) {
        if (map[i - 1][j] != id && t[map[i - 1][j]] == 0) return 0;        //ako to nije moja zmija i tek ce da se siri
    }

    if (map[i][j + 1] == 1 || map[i - 1][j + 1] == 2 || map[i][j + 1] == 3 || map[i][j + 1] == 4) {
        if (map[i][j + 1] != id && t[map[i - 1][j + 1]] == 0) return 0;
    }

    if (map[i + 1][j] == 1 || map[i + 1][j] == 2 || map[i + 1][j] == 3 || map[i + 1][j] == 4) {
        if (map[i + 1][j] != id && t[map[i + 1][j]] == 0) return 0;
    }

    if (map[i][j - 1] == 1 || map[i][j - 1] == 2 || map[i][j - 1] == 3 || map[i][j - 1] == 4) {
        if (map[i][j - 1] != id && t[map[i][j - 1]] == 0) return 0;
    }

    return 1;
}

void prosiri(int **map, int x, int y, int k, sume *suma, int id, int t[5], int n) {     //k je stepen kruga koji sirim
    int i, j, p, sf = 0;                     //sf je promeni smer i-ja
    for (j = y - k, i = x; j <= y + k; j++) {
        p = check(i, j, map, id, t);
        if (i > 0 && i <= n && j > 0 && j <= n) {
            if (map[i][j] == 0 && p == 1) {
                map[i][j] = id;
                if (j < y)(suma->w)++;
                else if (j > y)(suma->e)++;
                if (i < x)(suma->n)++;
                else if (i > x)(suma->s)++;
            }
        }

        if (sf == 0)i--;
        else i++;

        if (i == x - k)sf = 1;
    }
    //donji deo krsta
    for (j = y + k, i = x, sf = 0; j >= y - k; j--) {
        p = check(i, j, map, id, t);
        if (i > 0 && i <= n && j > 0 && j <= n) {
            if (map[i][j] == 0 && p == 1) {
                map[i][j] = id;
                if (j < y)(suma->w)++;
                else if (j > y)(suma->e)++;
                if (i < x)(suma->n)++;
                else if (i > x)(suma->s)++;
            }
        }
        if (sf == 0)i++;
        else i--;
        if (i == x + k)sf = 1;
    }

}

///////


void findspace(board b, coord *s, int **map, sume *s1, sume *s2, sume *s3, sume *s4) {
    int t[5] = {0}, k, i, j, max1, max2;


    for (k = 1; k < s.m; k++) {
        if (s[0].x != -1) {
            prosiri(map, s[0].x, s[0].y, k, s1, 1, t, b.n);
            t[1] = 1;
        }
        if (s[1].x != -1) {
            prosiri(map, s[1].x, s[1].y, k, s2, 2, t,
                    b.n);                                   //ako je saterana do zida s neke strane suma te str ce space biti 0!
            t[2] = 1;
        }
        if (s[2].x != -1) {
            prosiri(map, s[2].x, s[2].y, k, s3, 3, t, b.n);
            t[3] = 1;
        }
        if (s[3].x != -1) {
            prosiri(map, s[3].x, s[3].y, k, s4, 4, t, b.n);
            t[4] = 1;
        }

        t[1] = t[2] = t[3] = t[4] = 0;                              //if(brd[tek.x+1][tek.y]==0)insert(red,tek.x+1,tek.y,2);
    }
}


int ishead(board s, int x, int y) {
    int i;
    for (i = 0; i < 4; i++) {
        if (x == s.heads[i].x && y == s.heads->y) return 1;
    }
    return 0;
}

int issurrounded(int x, int y, int dir, int **map, board s) {
    r *red;
    int i, j;
    coord tek;

    if (map[x - 1][y] == 0) insert(red, x - 1, y, 0);
    if (map[x][y + 1] == 0) insert(red, x, y + 1, 1);
    if (map[x + 1][y] == 0) insert(red, x + 1, y, 2);
    if (map[x][y - 1] == 0) insert(red, x, y - 1, 3);

    while (red->front != red->rear) {
        tek = delete(red);
        if (i = ishead(s, tek.x, tek.y) == 1)return 0;
        switch (tek.dir) {
            case 0: {
                if (map[tek.x - 1][tek.y] == 0) {
                    insert(red, tek.x - 1, tek.y, 0);
                    map[tek.x - 1][tek.y] = 5;
                }
                if (map[tek.x][tek.y + 1] == 0) {
                    insert(red, tek.x, tek.y + 1, 1);
                    map[tek.x][tek.y + 1] = 5;
                }
                if (map[tek.x][tek.y - 1] == 0) {
                    insert(red, tek.x, tek.y - 1, 3);
                    map[tek.x][tek.y - 1] = 5;
                }
                break;
            }
            case 1: {
                if (map[tek.x - 1][tek.y] == 0) {
                    insert(red, tek.x - 1, tek.y, 0);
                    map[tek.x - 1][tek.y] = 5;
                }
                if (map[tek.x][tek.y + 1] == 0) {
                    insert(red, tek.x, tek.y + 1, 1);
                    map[tek.x][tek.y + 1] = 5;
                }
                if (map[tek.x + 1][tek.y] == 0) {
                    insert(red, tek.x + 1, tek.y, 2);
                    map[tek.x + 1][tek.y] = 5;
                }
                break;
            }
            case 2: {
                if (map[tek.x][tek.y + 1] == 0) {
                    insert(red, tek.x, tek.y + 1, 1);
                    map[tek.x][tek.y + 1] = 5;
                }
                if (map[tek.x + 1][tek.y] == 0) {
                    insert(red, tek.x + 1, tek.y, 2);
                    map[tek.x + 1][tek.y] = 5;
                }
                if (map[tek.x][tek.y - 1] == 0) {
                    insert(red, tek.x, tek.y - 1, 3);
                    map[tek.x][tek.y - 1] = 5;
                }
                break;
            }
            case 3: {
                if (map[tek.x - 1][tek.y] == 0) {
                    insert(red, tek.x - 1, tek.y, 0);
                    map[tek.x - 1][tek.y] = 5;
                }
                if (map[tek.x + 1][tek.y] == 0) {
                    insert(red, tek.x + 1, tek.y, 2);
                    map[tek.x + 1][tek.y] = 5;
                }
                if (map[tek.x][tek.y - 1] == 0) {
                    insert(red, tek.x, tek.y - 1, 3);
                    map[tek.x][tek.y - 1] = 5;
                }
                break;
            }
        }
    }
    return 1;
}

coord gotoedges(int **brd, coord out, int n) {         //pozivam samo neposredno cim provalim da je ogradjena
    int a, b, x = out.x, y = out.y, dir = out.dir, di1, dir2;

    if (y > n - y) {
        dir1 = 1;
        a = n - y;
    }
    else {
        dir1 = 3;
        a = y;
    }

    if (x > n - x) {
        dir2 = 2;
        b = n - x;
    }
    else {
        dir2 = 0;
        b = x;
    }
    if (a > b)dir = dir2;
    else dir = dir1;

    switch (dir) {
        case 0: {
            out.x = x - 1;
            out.y = y;
            break;
        }
        case 1: {
            out.x = x;
            out.y = y + 1;
            break;
        }
        case 2: {
            out.x = x + 1;
            out.y = y;
            break;
        }
        case 3: {
            out.x = x;
            out.y = y - 1;
            break;
        }
    }
    out.dir = dir;
    return out;
}

int isclear(int **brd, int dir, int x, int y) {
    switch (dir) {
        case 0: {
            if (brd[x - 1][y] == 0) return 1;
            break;
        }
        case 1: {
            if (brd[x][y + 1] == 0) return 1;
            break;
        }
        case 2: {
            if (brd[x + 1][y] == 0) return 1;
            break;
        }
        case 3: {
            if (brd[x][y - 1] == 0) return 1;
        }
    }
    return 0;

}

coord surrounded(int **brd, coord out) {
    int dir = out.dir, x = out.x, y = out.y, i;

    i = (dir + 1) % 4;
    if (iscelar(brd, i, x, y) == 1)dir = i;
    else {
        i = (dir) % 4;
        if (iscelar(brd, i, x, y) == 1)dir = i;

        else {
            i = (dir + 3) % 4;
            if (iscelar(brd, i, x, y) == 1)dir = i;
        }
    }

    switch (dir) {
        case 0: {
            out.x = x - 1;
            out.y = y;
            break;
        }
        case 1: {
            out.x = x;
            out.y = y + 1;
            break;
        }
        case 2: {
            out.x = x + 1;
            out.y = y;
            break;
        }
        case 3: {
            out.x = x;
            out.y = y - 1;
            break;
        }
    }
    out.dir = dir;
    return out;

}




coord *hardbot(board b, coord *s, int **map, sume *suma) {
    sume s1, s2, s3, s4;
    coord *out;
    int i, j, dir1, dir2, dir;
    out = calloc(2 * sizeof(coord));

    if (s[2].x != -1) {
        if (issurrounded(s[2].x, s[2].y, s[2].dir, map, b) == 0) {
            findspace(b, s, map, &s1, &s2, &s3, &s4);
            if (s2.n > s2.s) {
                i = s2.n;
                dir1 = 0;
            }
            else {
                i = s2.s;
                dir1 = 2;
            }
            if (s2.e > s2.w) {
                j = s2.e;
                dir2 = 1;
            }
            else {
                j = s2.w;
                dir2 = 3;
            }

            if (i > j)dir = dir1;
            else dir = dir2;

            switch (dir) {
                case 0: {
                    out[0].x = x - 1;
                    out[0].y = y;
                    break;
                }
                case 1: {
                    out[0].x = x;
                    out[0].y = y + 1;
                    break;
                }
                case 2: {
                    out[0].x = x + 1;
                    out[0].y = y;
                    break;
                }
                case 3: {
                    out[0].x = x;
                    out[0].y = y - 1;
                    break;
                }
            }

            out[0].dir = dir;
        }
        else{
            out[0]=surrounded(brd,out);
        }

    }

    if (s[3].x != -1) {
        if (issurrounded(s[3].x, s[3].y, s[3].dir, map, b) == 0) {
            findspace(b, s, map, &s1, &s2, &s3, &s4);
            if (s3.n > s3.s) {
                i = s3.n;
                dir1 = 0;
            }
            else {
                i = s3.s;
                dir1 = 2;
            }
            if (s3.e > s3.w) {
                j = s3.e;
                dir2 = 1;
            }
            else {
                j = s3.w;
                dir2 = 3;
            }

            if (i > j)dir = dir1;
            else dir = dir2;

            switch (dir) {
                case 0: {
                    out[1].x = x - 1;
                    out[1].y = y;
                    break;
                }
                case 1: {
                    out[1].x = x;
                    out[1].y = y + 1;
                    break;
                }
                case 2: {
                    out[1].x = x + 1;
                    out[1].y = y;
                    break;
                }
                case 3: {
                    out[1].x = x;
                    out[1].y = y - 1;
                    break;
                }
            }

            out[0].dir = dir;
        }
        else{
            out[0]=surrounded(brd,out);
        }

    }
}