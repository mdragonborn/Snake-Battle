#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "logic.h"
#include "mtwist.h"
#include <math.h>


typedef struct queue_element {
    int x;
    int y;
    int level;
    struct queue_element *next;
} QueueElement;

typedef struct queue {
    QueueElement *head;
    QueueElement *tail;
} Queue;

Queue initializeQueue() {
    Queue ret;
    ret.head = NULL;
    ret.tail = NULL;
    return ret;
}

void insertt(Queue *q, int x, int y, int level) {
    QueueElement *new_element = malloc(sizeof(QueueElement));
    if (new_element == NULL) exit(123); // Allocation error
    new_element->x = x;
    new_element->y = y;
    new_element->level = level;
    new_element->next = NULL;
    if (q->head == NULL) q->head = new_element;
    else q->tail->next = new_element;
    q->tail = new_element;
}

void gett(Queue *q, int *x, int *y, int *level) {
    QueueElement *old_head = q->head;
    *x = q->head->x;
    *y = q->head->y;
    *level = q->head->level;
    q->head = q->head->next;
    if (q->head == NULL) q->tail = NULL;
    free(old_head);
}

int isEmpty(Queue q) {
    if (q.head == NULL) return 1;
    else return 0;
}

int **initializeVisited(int n) {
    int i;
    int **ret = malloc(n * sizeof(int *));
    if (ret == NULL) exit(123);
    for (i = 0; i < n; i++) {
        ret[i] = calloc(n, sizeof(int));
        if (ret[i] == NULL) exit(123);
    }
    return ret;
}

void freeMatrix(int **m, int n) {
    int i;
    for (i = 0; i < n; i++) free(m[i]);
    free(m);
}

int isHead(int x, int y, coord *heads) {
    int i;
    for (i = 0; i < 4; i++) if (heads[i].x == x && heads[i].y == y) return 1;
    return 0;
}

void bfsCount(board map, int start_x, int start_y, int *head_level, int *cell_count) {
    Queue q = initializeQueue();
    int **visited;
    *cell_count = 0;
    *head_level = INT_MAX;
    if (map.brd[start_x][start_y] != 0) return;
    visited = initializeVisited(map.n + 2);
    visited[start_x][start_y] = 1;
    insertt(&q, start_x, start_y, 0);
    while (!isEmpty(q)) {
        int x, y, level;
        gett(&q, &x, &y, &level);
        if (isHead(x, y, map.heads) && *head_level == INT_MAX) *head_level = level;
        ++*cell_count;
        if (map.brd[x - 1][y] == 0 && visited[x - 1][y] == 0) insertt(&q, x - 1, y, level + 1), visited[x - 1][y] = 1;
        if (map.brd[x + 1][y] == 0 && visited[x + 1][y] == 0) insertt(&q, x + 1, y, level + 1), visited[x + 1][y] = 1;
        if (map.brd[x][y - 1] == 0 && visited[x][y - 1] == 0) insertt(&q, x, y - 1, level + 1), visited[x][y - 1] = 1;
        if (map.brd[x][y + 1] == 0 && visited[x][y + 1] == 0) insertt(&q, x, y + 1, level + 1), visited[x][y + 1] = 1;
    }
    freeMatrix(visited, map.n + 2);
}

int findMaxInArray(int *arr, int last_dir) {
    int ret = 0;
    int i;
    int skip = (last_dir + 2) % 4;
    for (i = 0; i < 4; i++) if (i != skip && arr[i] > ret) ret = arr[i];
    return ret;
}

int findMaxWithCount(int *head_levels, int *cell_counts, int count, int last_dir) {
    int max_head_level = 0;
    int i;
    int skip = (last_dir + 2) % 4;
    int ret_array[4];
    int cnt = 0;
    int index;
    for (i = 0; i < 4; i++) if (i != skip && cell_counts[i] == count && head_levels[i] > max_head_level) max_head_level = head_levels[i];
    for (i = 0; i < 4; i++) if (i != skip && cell_counts[i] == count && head_levels[i] == max_head_level) ret_array[cnt++] = i;
    index = rintl(mt_ldrand() * (cnt - 1));
    return ret_array[index];
}

coord myBot(board b, coord s) {
    int head_levels[4] = {0, 0, 0, 0};
    int cell_counts[4] = {0, 0, 0, 0};
    coord ret;
    int highest_count;
    bfsCount(b, s.x - 1, s.y, &head_levels[0], &cell_counts[0]);
    bfsCount(b, s.x, s.y + 1, &head_levels[1], &cell_counts[1]);
    bfsCount(b, s.x + 1, s.y, &head_levels[2], &cell_counts[2]);
    bfsCount(b, s.x, s.y - 1, &head_levels[3], &cell_counts[3]);
    highest_count = findMaxInArray(cell_counts, s.dir);
    ret.dir = findMaxWithCount(head_levels, cell_counts, highest_count, s.dir);
    switch(ret.dir) {
        case 0: ret.x = s.x - 1, ret.y = s.y; break;
        case 1: ret.x = s.x, ret.y = s.y + 1; break;
        case 2: ret.x = s.x + 1, ret.y = s.y; break;
        case 3: ret.x = s.x; ret.y = s.y - 1; break;
        default: break;
    }
    return ret;
}
