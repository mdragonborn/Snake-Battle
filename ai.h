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

    coord q[300];
    int front;
    int rear;
} r;

void initq(r *red);

void insert(r *red, int x, int y, int dir);

coord delete(r *red);

#endif //DEMOMENI_AI_H
