#ifndef PATHING_H
#define PATHING_H

typedef struct Cell{
    int x;
    int y;
} Cell;

typedef struct Path{
    Cell* path;
    int size;
} Path;

typedef struct QNode{
    Cell cell;
    struct QNode* next;
} QNode;

typedef struct Queue{
    struct QNode *front, *rear;
} Queue;

QNode* node_init(int x, int y);
struct Queue* queue_init();
void enQueue(struct Queue* q, int x, int y);
Cell deQueue(struct Queue* q);
int queue_is_empty(struct Queue* q);

double get_diff(int x1, int y1, int x2, int y2);
int is_water(int x, int y);
Path find_path_water(int startX, int startY, int endX, int endY);
Path find_path_ground(int startX, int startY, int endX, int endY);
void find_path(int startX, int startY, int endX, int endY);

#endif
