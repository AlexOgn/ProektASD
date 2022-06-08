#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define sizeX 8
#define sizeY 8
#define diff 10

//TODO napravi go taka che da ima znachenie kolko e visoko
float grid[sizeX][sizeY] = {
    {74, 73, 72, 69, 63, 56, 49, 46},
    {71, 71, 69, 67, 62, 53, 45, 41},
    {67, 65, 100, 90, 80, 70, 60, 50},
    {64, 62, 60, 59, 53, 42, 31, 26},
    {64, 61, 58, 56, 50, 38, 26, 21},
    {66, 63, 60, 57, 50, 40, 30, 21},
    {71, 68, 64, 60, 52, 41, 30, 24},
    {73, 71, 67, 62, 54, 43, 33, 27}
};

typedef struct Cell{
    int x;
    int y;
} Cell;

typedef struct QNode{
    Cell cell;
    struct QNode* next;
} QNode;

typedef struct Queue{
    struct QNode *front, *rear;
} Queue;

QNode* node_init(int x, int y){
    struct QNode* temp = (struct QNode*)malloc(sizeof(struct QNode));
    struct Cell cell = {x, y};
    temp->cell = cell;
    temp->next = NULL;
    return temp;
}

struct Queue* queue_init(){
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    return q;
}

void enQueue(struct Queue* q, int x, int y){
    struct QNode* temp = node_init(x, y);

    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }

    q->rear->next = temp;
    q->rear = temp;
}

Cell deQueue(struct Queue* q){
    if (q->front == NULL)
        return;

    struct QNode* temp = q->front;
    Cell cell = temp->cell;

    q->front = q->front->next;

    if (q->front == NULL){
        q->rear = NULL;
    }

    free(temp);

    return cell;
}

int queue_is_empty(struct Queue* q){
    return (q->front == NULL && q->rear == NULL);
}

double distance(int x, int y, int x1, int y1){
    return sqrt((x-x1)*(x-x1) + (y-y1) * (y-y1));
}

double get_diff(int x1, int y1, int x2, int y2){
    return abs(grid[x1][y1] - grid[x2][y2]);
}

void find_path(int startX, int startY, int endX, int endY){
    int** visited = calloc(sizeof(int*), sizeX);
    Cell** prev = calloc(sizeof(Cell*), sizeX);
    for(int i=0;i<sizeX;i++){
        visited[i] = calloc(sizeY, sizeof(int));
        prev[i] = calloc(sizeY, sizeof(Cell));
    }

    for(int i=0;i<sizeX;i++){
        for(int j=0;j<sizeY;j++){
            prev[i][j].x = -1;
            prev[i][j].y = -1;
        }
    }

    Queue *q = queue_init();
    enQueue(q, startX, startY);
    visited[startX][startY] = 1;

    while(!queue_is_empty(q)){
        Cell node = deQueue(q);
        int x = node.x;
        int y = node.y;

        if(x-1 >= 0 && visited[x-1][y] == 0 && get_diff(x, y, x-1, y) <= diff){
            visited[x-1][y] = 1;
            enQueue(q, x-1, y);

            Cell cell = {x, y};
            prev[x-1][y] = cell;
        }

        if(x+1 < sizeX && visited[x+1][y] == 0 && get_diff(x, y, x+1, y) <= diff){
            visited[x+1][y] = 1;
            enQueue(q, x+1, y);

            Cell cell = {x, y};
            prev[x+1][y] = cell;
        }

        if(y-1 >= 0 && visited[x][y-1] == 0 && get_diff(x, y, x, y-1) <= diff){
            visited[x][y-1] = 1;
            enQueue(q, x, y-1);

            Cell cell = {x, y};
            prev[x][y-1] = cell;
        }

        if(y+1 < sizeY && visited[x][y+1] == 0 && get_diff(x, y, x, y+1) <= diff){
            visited[x][y+1] = 1;
            enQueue(q, x, y+1);

            Cell cell = {x, y};
            prev[x][y+1] = cell;
        }
    }

    free(q);

    for(int i=0;i<sizeX;i++){
        for(int j=0;j<sizeY;j++){
            printf("%d %d\t", prev[i][j].x, prev[i][j].y);
        }
        printf("\n");
    }

    Cell* path = calloc(sizeX, sizeof(Cell));
    Cell at = {endX, endY};
    int index = 0;
    int capacity = sizeX;

    for(at; at.x != -1; at = prev[at.x][at.y]){
        if(index == capacity-1){
            capacity *= 2;
            path = realloc(path, capacity * sizeof(Cell));
        }

        printf("asdfadsf:");
        printf("%d %d \n", at.x, at.y);

        path[index] = at;
        index++;
    }

    printf("\n\n\n");
    for(int i=0;i<index;i++){
        printf("%d %d \n", path[i].x, path[i].y);
    }

    for(int i=0;i<index/2;i++){
        Cell t = path[i];
        path[i] = path[index-i-1];
        path[index-i-1] = t;
    }


}

int main(){
    find_path(0, 0, 2, 2);


}
