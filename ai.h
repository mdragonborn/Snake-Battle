//
// Created by bulse_eye on 17.6.2016.
//

#ifndef DEMOMENI_AI_H
#define DEMOMENI_AI_H
#include "logic.h"

typedef struct sss {
    int n;
    int s;
    int e;
    int w;
} sume;

typedef struct rr {

    coord q[1001];
    int front;
    int rear;
    int id[1001];
} r;

typedef struct p{
    coord info;
    int id;
}polje;

void initq(r *red);

void insert(r *red, int x, int y, int dir,int i);

coord delete(r *red);

polje get(r*red);

sume* krstovi(int **map, coord *s,sume suma[4],int *lives);
int skreci(const int **brd, int x, int y, int dir);
int keepgoing(int**brd,int x, int y,int dir);
coord mediumbot(int x, int y, int dir, int **brd);
int check(int i, int j, int **map, int id, int t[5], int n);
void prosiri(int **map, int x, int y, int k, sume *suma, int id, int t[5], int n);
void findspace(board b, coord *s, int **map, sume *s1, sume *s2, sume *s3, sume *s4,int *lives);
int ishead(coord *s, int x, int y);
int issurrounded(int x, int y, int dir, int **map, coord *s);
int isclear(int **brd, int dir, int x, int y);
coord surrounded(int **brd, coord out);
coord *hardbot(board b, coord *s, int **map,int *lives);



#endif //DEMOMENI_AI_H
