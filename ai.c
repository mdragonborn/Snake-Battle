//
// Created by bulse_eye on 17.6.2016.
//
#include "ai.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int **copyMatrix(int **matrix, int n) {
    int i, j;
    int **ret = malloc(n * sizeof(int *));
    if (ret == NULL) exit(123);
    for (i = 0; i < n; i++) {
        ret[i] = malloc(n * sizeof(int));
        if (ret[i] == NULL) exit(123);
        for (j = 0; j < n; j++) ret[i][j] = matrix[i][j];
    }
    return ret;
}


void initq(r *red) {

    red->front = 0;
    red->rear = 0;
}

polje get(r*red){
    polje x;

    x.info = red->q[red->front];
    x.id=red->id[red->front];
    if (red->front == red->rear) { red->front = red->rear = 0; }
    else {

        red->front = (red->front ) % 1000+1;
    }
    return x;
}




//insert queue
void insert(r *red, int x, int y, int dir,int i) {

    red->rear = (red->rear) % 1000+1;
    if (red->front == red->rear) {
        printf("   Q"); //overflow  ERROR overflow Q
    }
    else {

        red->q[red->rear].x = x;
        red->q[red->rear].y = y;
        red->q[red->rear].dir = dir;
        red->id[red->rear]=i;
        if (red->front == 0) red->front = 1;
    }
}



//delete queue
coord delete(r *red) {

    coord x;
    /*if (red->front == 0) {
        printf("   q");  //Underflow q
    }
    else{*/
        x = red->q[red->front];
        if (red->front == red->rear) { red->front = red->rear = 0; }
        else {

            red->front = (red->front ) % 1000+1;
        }
        return x;
    }
//}


int skreci(const int **brd, int x, int y, int dir) {
    if (brd[x - 1][y] == 0)return 0;
    else if (brd[x][y - 1] == 0)return 3;
    else if (brd[x + 1][y] == 0)return 2;
    else if (brd[x][y + 1] == 0)return 1;
    return dir;
}


int keepgoing(int **brd, int x, int y, int dir) {
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

coord mediumbot(int x, int y, int dir, int **brd){       //pozivam za oba bota pojedinacno
    int i,skup[4],j;
    coord out;
    int rez;


    i=keepgoing(brd,x,y,dir);
    if(i>=0)rez=i;
    else {
        i = -1;
        if (brd[x - 1][y] == 0 && dir != 2)skup[++i] = 0;
        if (brd[x][y + 1] == 0 && dir != 3)skup[++i] = 1;
        if (brd[x + 1][y] == 0 && dir != 0)skup[++i] = 2;
        if (brd[x][y - 1] == 0 && dir != 1)skup[++i] = 3;

        j=rand()/(double)(RAND_MAX+1)*(i+1);
        rez=skup[j];
        if (i == -1) rez = dir;

    }
    out.dir=rez;
    switch(rez){
        case 0:{out.x=x-1; out.y=y; break;}
        case 1:{out.x=x; out.y=y+1;break;}
        case 2:{out.x=x+1; out.y=y; break;}
        case 3:{out.x=x; out.y=y-1; break;}
        default: break;
    }

    return out;
}

int check(int i, int j, int **map, int id, int t[5],int n) {  //prosledjujem koordinate polja na koje treba da upisem vrednost
    // id je broj kojim se popunjava kretanje te zmije po mapi
    if(i >= n-1 || j>=n-1 || i <= 0 || j <= 0)return 2;
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
        p = check(i, j, map, id, t,n);
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
        p = check(i, j, map, id, t,n);
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


void findspace(board b, coord *s, int **map, sume *s1, sume *s2, sume *s3, sume *s4,int *lives) {
    int t[5] = {0}, k, i, j, max1, max2;


    for (k = 1; k < b.n; k++) {
        if (s[0].x != -1 && lives[0]==1) {
            prosiri(map, s[0].x, s[0].y, k, s1, 1, t, b.n);
            t[1] = 1;
        }
        if (s[1].x != -1 && lives[1]==1) {
            prosiri(map, s[1].x, s[1].y, k, s2, 2, t,
                    b.n);                                   //ako je saterana do zida s neke strane suma te str ce space biti 0!
            t[2] = 1;
        }
        if (s[2].x != -1 && lives[2]==1) {
            prosiri(map, s[2].x, s[2].y, k, s3, 3, t, b.n);
            t[3] = 1;
        }
        if (s[3].x != -1 && lives[3]==1) {
            prosiri(map, s[3].x, s[3].y, k, s4, 4, t, b.n);
            t[4] = 1;
        }

        t[1] = t[2] = t[3] = t[4] = 0;                              //if(brd[tek.x+1][tek.y]==0)insert(red,tek.x+1,tek.y,2);
    }
    for(i=0;i<b.n;i++){
        for (j=0;j<b.n;j++){
            printf("%d ",map[i][j]);
        }
        printf("\n");
    }
    system("pause");
    exit(24);
}

sume* krstovi(int **brd, coord *s,sume suma[4],int *lives) {   //proveri ovo ovde s mapom ili kopijom sta treba
    r *red;
    int i, j,x,y,**map;
    polje tek;
    map=copyMatrix(brd,52);

    red = malloc(sizeof (r));
    initq(red);

    for(i=3;i>=0;i--){
        if(lives[i]==1) {
            x = s[i].x;
            y = s[i].y;
            if (map[x - 1][y] == 0) insert(red, x - 1, y, 0, i);
            if (map[x][y + 1] == 0) insert(red, x, y + 1, 1, i);
            if (map[x + 1][y] == 0) insert(red, x + 1, y, 2, i);
            if (map[x][y - 1] == 0) insert(red, x, y - 1, 3, i);
        }
    }

    while (!(red->front==0 && red->rear==0)) {
        tek = get(red);
        if (tek.info.y < s[tek.id].y)(suma[tek.id].w)++;
        else if (tek.info.y > s[tek.id].y)(suma[tek.id].e)++;
        if (tek.info.x < s[tek.id].x)(suma[tek.id].n)++;
        else if (tek.info.x > s[tek.id].x)(suma[tek.id].s)++;

        /*
        if (map[tek.info.x - 1][tek.info.y] == 0) {
            insert(red, tek.info.x - 1, tek.info.y, 0, tek.id);
            map[tek.info.x - 1][tek.info.y] = 1;
        }
        else if (map[tek.info.x][tek.info.y + 1] == 0) {
            insert(red, tek.info.x, tek.info.y + 1, 1, tek.id);
            map[tek.info.x][tek.info.y + 1] = 1;
        }
        else if (map[tek.info.x + 1][tek.info.y] == 0) {
            insert(red, tek.info.x + 1, tek.info.y, 2, tek.id);
            map[tek.info.x + 1][tek.info.y] = 1;
        }
        else if (map[tek.info.x][tek.info.y - 1] == 0) {
            insert(red, tek.info.x, tek.info.y - 1, 3, tek.id);
            map[tek.info.x][tek.info.y - 1] = 1;
        }
        */


        switch (tek.info.dir) {
            case 0: {

                if (map[tek.info.x - 1][tek.info.y] == 0) {
                    insert(red, tek.info.x - 1, tek.info.y, 0,tek.id);
                    map[tek.info.x - 1][tek.info.y] = 1;
                }

                if (map[tek.info.x][tek.info.y + 1] == 0) {
                    insert(red, tek.info.x, tek.info.y + 1, 1,tek.id);
                    map[tek.info.x][tek.info.y + 1] = 1;
                }

                if (map[tek.info.x][tek.info.y - 1] == 0) {
                    insert(red, tek.info.x, tek.info.y - 1, 3,tek.id);
                    map[tek.info.x][tek.info.y - 1] = 1;
                }
                break;
            }
            case 1: {

                if (map[tek.info.x - 1][tek.info.y] == 0) {
                    insert(red, tek.info.x - 1, tek.info.y, 0,tek.id);
                    map[tek.info.x - 1][tek.info.y] = 1;
                }

                if (map[tek.info.x][tek.info.y + 1] == 0) {
                    insert(red, tek.info.x, tek.info.y + 1, 1,tek.id);
                    map[tek.info.x][tek.info.y + 1] = 1;
                }

                if (map[tek.info.x + 1][tek.info.y] == 0) {
                    insert(red, tek.info.x + 1, tek.info.y, 2,tek.id);
                    map[tek.info.x + 1][tek.info.y] = 1;
                }
                break;
            }
            case 2: {

                if (map[tek.info.x][tek.info.y + 1] == 0) {
                    insert(red, tek.info.x, tek.info.y + 1, 1,tek.id);
                    map[tek.info.x][tek.info.y + 1] = 1;
                }

                if (map[tek.info.x + 1][tek.info.y] == 0) {
                    insert(red, tek.info.x + 1, tek.info.y, 2,tek.id);
                    map[tek.info.x + 1][tek.info.y] = 1;
                }

                if (map[tek.info.x][tek.info.y - 1] == 0) {
                    insert(red, tek.info.x, tek.info.y - 1, 3,tek.id);
                    map[tek.info.x][tek.info.y - 1] = 1;
                }
                break;
            }
            case 3: {

                if (map[tek.info.x - 1][tek.info.y] == 0) {
                    insert(red, tek.info.x - 1, tek.info.y, 0,tek.id);
                    map[tek.info.x - 1][tek.info.y] = 1;
                }

                if (map[tek.info.x + 1][tek.info.y] == 0) {
                    insert(red, tek.info.x + 1, tek.info.y, 2,tek.id);
                    map[tek.info.x + 1][tek.info.y] = 1;
                }

                if (map[tek.info.x][tek.info.y - 1] == 0) {
                    insert(red, tek.info.x, tek.info.y - 1, 3,tek.id);
                    map[tek.info.x][tek.info.y - 1] = 1;
                }
                break;
            }
        }

    }
    free(red);

    /*for(i=0;i<52;i++){
        for (j=0;j<52;j++){
            printf("%d ",map[i][j]);
        }
        printf("\n");


    }
    system("pause");
    exit(24);*/

}

int ishead(coord *s, int x, int y) {
    int i;
    for (i = 0; i < 4; i++) {
        if (x == s[i].x && y == s[i].y) return 1;
    }
    return 0;
}

int issurrounded(int x, int y, int dir, int **map, coord *s) {   //proveri ovo ovde s mapom ili kopijom sta treba
    r *red;
    int i, j,**map2;
    coord tek;


    red = malloc(sizeof (r));
    initq(red);

    if (map[x - 1][y] == 0) insert(red, x - 1, y, 0,0), map[x - 1][y] = 1;
    if (map[x][y + 1] == 0) insert(red, x, y + 1, 1,0), map[x][y + 1] = 1;
    if (map[x + 1][y] == 0) insert(red, x + 1, y, 2,0), map[x + 1][y] = 1;
    if (map[x][y - 1] == 0) insert(red, x, y - 1, 3,0), map[x][y - 1] = 1;

    while (!(red->front==0 && red->rear==0)) {
        tek = delete(red);
       // if ((ishead(s, tek.x, tek.y)) == 1)return 0;
        switch (tek.dir) {
            case 0: {
                if ((ishead(s, tek.x-1, tek.y)) == 1)return 0;
                if (map[tek.x - 1][tek.y] == 0) {
                    insert(red, tek.x - 1, tek.y, 0,0);
                    map[tek.x - 1][tek.y] = 1;
                }
                if ((ishead(s, tek.x, tek.y+1)) == 1)return 0;
                if (map[tek.x][tek.y + 1] == 0) {
                    insert(red, tek.x, tek.y + 1, 1,0);
                    map[tek.x][tek.y + 1] = 1;
                }
                if ((ishead(s, tek.x, tek.y-1)) == 1)return 0;
                if (map[tek.x][tek.y - 1] == 0) {
                    insert(red, tek.x, tek.y - 1, 3,0);
                    map[tek.x][tek.y - 1] = 1;
                }
                break;
            }
            case 1: {
                if ((ishead(s, tek.x-1, tek.y)) == 1)return 0;
                if (map[tek.x - 1][tek.y] == 0) {
                    insert(red, tek.x - 1, tek.y, 0,0);
                    map[tek.x - 1][tek.y] = 1;
                }
                if ((ishead(s, tek.x, tek.y+1)) == 1)return 0;
                if (map[tek.x][tek.y + 1] == 0) {
                    insert(red, tek.x, tek.y + 1, 1,0);
                    map[tek.x][tek.y + 1] = 1;
                }
                if ((ishead(s, tek.x+1, tek.y)) == 1)return 0;
                if (map[tek.x + 1][tek.y] == 0) {
                    insert(red, tek.x + 1, tek.y, 2,0);
                    map[tek.x + 1][tek.y] = 1;
                }
                break;
            }
            case 2: {
                if ((ishead(s, tek.x, tek.y+1)) == 1)return 0;
                if (map[tek.x][tek.y + 1] == 0) {
                    insert(red, tek.x, tek.y + 1, 1,0);
                    map[tek.x][tek.y + 1] = 1;
                }
                if ((ishead(s, tek.x+1, tek.y)) == 1)return 0;
                if (map[tek.x + 1][tek.y] == 0) {
                    insert(red, tek.x + 1, tek.y, 2,0);
                    map[tek.x + 1][tek.y] = 1;
                }
                if ((ishead(s, tek.x, tek.y-1)) == 1)return 0;
                if (map[tek.x][tek.y - 1] == 0) {
                    insert(red, tek.x, tek.y - 1, 3,0);
                    map[tek.x][tek.y - 1] = 1;
                }
                break;
            }
            case 3: {
                if ((ishead(s, tek.x-1, tek.y)) == 1)return 0;
                if (map[tek.x - 1][tek.y] == 0) {
                    insert(red, tek.x - 1, tek.y, 0,0);
                    map[tek.x - 1][tek.y] = 1;
                }
                if ((ishead(s, tek.x+1, tek.y)) == 1)return 0;
                if (map[tek.x + 1][tek.y] == 0) {
                    insert(red, tek.x + 1, tek.y, 2,0);
                    map[tek.x + 1][tek.y] = 1;
                }
                if ((ishead(s, tek.x, tek.y-1)) == 1)return 0;
                if (map[tek.x][tek.y - 1] == 0) {
                    insert(red, tek.x, tek.y - 1, 3,0);
                    map[tek.x][tek.y - 1] = 1;
                }
                break;
            }
        }
    }
    free(red);
    return 1;
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
    if (isclear(brd, i, x, y) == 1)dir = i;
    else {
        i = (dir) % 4;
        if (isclear(brd, i, x, y) == 1)dir = i;

        else {
            i = (dir + 3) % 4;
            if (isclear(brd, i, x, y) == 1)dir = i;
        }
    }

    switch (dir) {
        case 0: {
            out.x = x - 1;
            out.y = y;
            out.dir = 0;
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

int ideunazad(int dir1, int dir2) {
    if ((dir1 + 2) % 4 == dir2) return 1;
    else return 0;
}

int glave(int x, int y,coord *s){
    int br=0;

    if(ishead(s,x-1,y)) br++;
    if(ishead(s,x+1,y)) br++;
    if(ishead(s,x,y-1)) br++;
    if(ishead(s,x,y+1)) br++;
    return br;
}

int okosebe(int x, int y, int **map) {
    int br = 0;
    if (map[x - 1][y]) br++;
    if (map[x + 1][y]) br++;
    if (map[x][y - 1]) br++;
    if (map[x][y + 1]) br++;
    return br;
}

coord *hardbot(board b, coord *s, int **map,int *lives) {
    sume suma[4],s1, s2, s3, s4;

    suma[0].e=suma[0].n=suma[0].s=suma[0].w=suma[1].e=suma[1].n=suma[1].s=suma[1].w = suma[2].e=suma[2].n=suma[2].s=suma[2].w = suma[3].e=suma[3].n=suma[3].s=suma[3].w=0;
    coord *out;
    int i, j, dir1, dir2, dir,**map2;
    out = calloc(3, sizeof(coord));

    map2=copyMatrix(map,52);

    if (s[2].x != -1) {
        if (issurrounded(s[2].x, s[2].y, s[2].dir, map, s) == 0) {
            int niz[4];
            int indeksi[4] = {0, 1, 2, 3};
            krstovi(b.brd,s,suma,lives);
            s1=suma[0]; s2=suma[1]; s3=suma[2]; s4=suma[3];
            niz[0] = s3.n; niz[1] = s3.e; niz[2] = s3.s; niz[3] = s3.w;
            for (i = 0; i < 4; i++) {
                for (j = i + 1; j < 4; j++) {
                    if (niz[j] > niz[i]) {
                        int temp = niz[i];
                        niz[i] = niz[j];
                        niz[j] = temp;
                        temp = indeksi[i];
                        indeksi[i] = indeksi[j];
                        indeksi[j] = temp;
                    }
                }
            }
            for (i = 0; i < 4; i++) {
                switch (indeksi[i]) {
                    case 0: if (glave(s[2].x - 1, s[2].y, s) > 1) continue;
                    case 1: if (glave(s[2].x, s[2].y + 1, s) > 1) continue;
                    case 2: if (glave(s[2].x + 1, s[2].y, s) > 1) continue;
                    case 3: if (glave(s[2].x, s[2].y - 1, s) > 1) continue;
                }
                if (isclear(b.brd, indeksi[i], s[2].x, s[2].y) && !ideunazad(indeksi[i], s[2].dir) && okosebe(s[2].x, s[2].y, b.brd) != 4) {
                    out[0].dir = indeksi[i];
                    switch (indeksi[i]) {
                        case 0: {
                            out[0].x = s[2].x - 1;
                            out[0].y = s[2].y;
                            break;
                        }
                        case 1: {
                            out[0].x = s[2].x;
                            out[0].y = s[2].y + 1;
                            break;
                        }
                        case 2: {
                            out[0].x = s[2].x + 1;
                            out[0].y = s[2].y;
                            break;
                        }
                        case 3: {
                            out[0].x = s[2].x;
                            out[0].y = s[2].y - 1;
                            break;
                        }
                    }
                    break;
                }
            }
        }
        else{
            out[0]=surrounded(b.brd,s[2]);
        }

    }

    if (s[3].x != -1) {
        if (issurrounded(s[3].x, s[3].y, s[3].dir, map2, s) == 0) {
            int niz[4];
            int indeksi[4] = {0, 1, 2, 3};
            krstovi(b.brd,s,suma,lives);
            s1=suma[0]; s2=suma[1]; s3=suma[2]; s4=suma[3];
            niz[0] = s4.n; niz[1] = s4.e; niz[2] = s4.s; niz[3] = s4.w;
            for (i = 0; i < 4; i++) {
                for (j = i + 1; j < 4; j++) {
                    if (niz[j] > niz[i]) {
                        int temp = niz[i];
                        niz[i] = niz[j];
                        niz[j] = temp;
                        temp = indeksi[i];
                        indeksi[i] = indeksi[j];
                        indeksi[j] = temp;
                    }
                }
            }
            for (i = 0; i < 4; i++) {
                if (isclear(b.brd, indeksi[i], s[3].x, s[3].y) && !ideunazad(indeksi[i], s[3].dir) && okosebe(s[3].x, s[3].y, b.brd) != 4) {
                    out[1].dir = indeksi[i];
                    switch (indeksi[i]) {
                        case 0: {
                            out[1].x = s[3].x - 1;
                            out[1].y = s[3].y;
                            break;
                        }
                        case 1: {
                            out[1].x = s[3].x;
                            out[1].y = s[3].y + 1;
                            break;
                        }
                        case 2: {
                            out[1].x = s[3].x + 1;
                            out[1].y = s[3].y;
                            break;
                        }
                        case 3: {
                            out[1].x = s[3].x;
                            out[1].y = s[3].y - 1;
                            break;
                        }
                    }
                    break;
                }
            }
        }
        else{
            out[1]=surrounded(b.brd,s[3]);
        }

    }
    return out;
}