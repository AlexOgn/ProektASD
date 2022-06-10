#include "common.h"
#include "pathing.h"

#include <stdio.h>
#include <stdlib.h>

int durability = 500;

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

double get_diff(int x1, int y1, int x2, int y2){
    return abs(grid[x1][y1] - grid[x2][y2]);
}

int is_water(int x, int y){
    return (grid[x][y] < 75 && grid[x][y] >= 0);
}

Path find_path_water(int startX, int startY, int endX, int endY){
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

        if(x-1 >= 0 && visited[x-1][y] == 0){
            if(is_water(x-1, y) == 0){
                if(get_diff(x, y, x-1, y) <= diff){
                    visited[x-1][y] = 1;
                    enQueue(q, x-1, y);

                    Cell cell = {x, y};
                    prev[x-1][y] = cell;
                }
            }else{
                visited[x-1][y] = 1;
                enQueue(q, x-1, y);

                Cell cell = {x, y};
                prev[x-1][y] = cell;
            }

        }

        if(x+1 < sizeX && visited[x+1][y] == 0){
            if(is_water(x+1, y) == 0){
                if(get_diff(x, y, x+1, y) <= diff){
                    visited[x+1][y] = 1;
                    enQueue(q, x+1, y);

                    Cell cell = {x, y};
                    prev[x+1][y] = cell;
                }
            }else{
                visited[x+1][y] = 1;
                enQueue(q, x+1, y);

                Cell cell = {x, y};
                prev[x+1][y] = cell;
            }

        }

        if(y-1 >= 0 && visited[x][y-1] == 0){
            if(is_water(x, y-1) == 0){
                if(get_diff(x, y, x, y-1) <= diff){
                    visited[x][y-1] = 1;
                    enQueue(q, x, y-1);

                    Cell cell = {x, y};
                    prev[x][y-1] = cell;
                }
            }else{
                visited[x][y-1] = 1;
                enQueue(q, x, y-1);

                Cell cell = {x, y};
                prev[x][y-1] = cell;
            }

        }

        if(y+1 < sizeY && visited[x][y+1] == 0){
            if(is_water(x, y+1) == 0){
                if(get_diff(x, y, x, y+1) <= diff){
                    visited[x][y+1] = 1;
                    enQueue(q, x, y+1);

                    Cell cell = {x, y};
                    prev[x][y+1] = cell;
                }
            }else{
                visited[x][y+1] = 1;
                enQueue(q, x, y+1);

                Cell cell = {x, y};
                prev[x][y+1] = cell;
            }
        }
    }

    free(q);

    Cell* path = calloc(sizeX, sizeof(Cell));
    Cell at = {endX, endY};
    int index = 0;
    int capacity = sizeX;

    for(at; at.x != -1; at = prev[at.x][at.y]){
        if(index == capacity-1){
            capacity *= 2;
            path = realloc(path, capacity * sizeof(Cell));
        }

        path[index] = at;
        index++;
    }

    for(int i=0;i<sizeX;i++){
        free(visited[i]);
        free(prev[i]);
    }
    free(visited);
    free(prev);

    for(int i=0;i<index/2;i++){
        Cell t = path[i];
        path[i] = path[index-i-1];
        path[index-i-1] = t;
    }

    Path r_path = {path, index};

    for(int i=0;i<index;i++){
        if(is_water(path[i].x, path[i].y) == 1){
            if(durability <= 0){
                printf("schupi se");
                r_path.size = 0;
                break;
            }
            durability--;
        }
    }



    return r_path;
}

Path find_path_ground(int startX, int startY, int endX, int endY){
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

        if(x-1 >= 0 && visited[x-1][y] == 0 && get_diff(x, y, x-1, y) <= diff && is_water(x-1, y) == 0){
            visited[x-1][y] = 1;
            enQueue(q, x-1, y);

            Cell cell = {x, y};
            prev[x-1][y] = cell;
        }

        if(x+1 < sizeX && visited[x+1][y] == 0 && get_diff(x, y, x+1, y) <= diff && is_water(x+1, y) == 0){
            visited[x+1][y] = 1;
            enQueue(q, x+1, y);

            Cell cell = {x, y};
            prev[x+1][y] = cell;
        }

        if(y-1 >= 0 && visited[x][y-1] == 0 && get_diff(x, y, x, y-1) <= diff && is_water(x, y-1) == 0){
            visited[x][y-1] = 1;
            enQueue(q, x, y-1);

            Cell cell = {x, y};
            prev[x][y-1] = cell;
        }

        if(y+1 < sizeY && visited[x][y+1] == 0 && get_diff(x, y, x, y+1) <= diff && is_water(x, y+1) == 0){
            visited[x][y+1] = 1;
            enQueue(q, x, y+1);

            Cell cell = {x, y};
            prev[x][y+1] = cell;
        }
    }

    free(q);

    Cell* path = calloc(sizeX, sizeof(Cell));
    Cell at = {endX, endY};
    int index = 0;
    int capacity = sizeX;

    for(at; at.x != -1; at = prev[at.x][at.y]){
        if(index == capacity-1){
            capacity *= 2;
            path = realloc(path, capacity * sizeof(Cell));
        }

        path[index] = at;
        index++;
    }

    for(int i=0;i<sizeX;i++){
        free(visited[i]);
        free(prev[i]);
    }
    free(visited);
    free(prev);

    for(int i=0;i<index/2;i++){
        Cell t = path[i];
        path[i] = path[index-i-1];
        path[index-i-1] = t;
    }

    Path r_path = {path, index};

    return r_path;
}

void find_path(int startX, int startY, int endX, int endY){
    Path path_water = find_path_water(startX, startY, endX, endY);
    Path path_ground = find_path_ground(startX, startY, endX, endY);

    Path path = (path_water.size > 0) ? (path_ground.size > 0) ? (path_water.size > path_ground.size) ? path_ground : path_water : path_water : path_ground;

    for(int k=0;k<path.size;k++){
        int i = path.path[k].x;
        int j = path.path[k].y;
        grid[i][j] = -2;
        grid[i][j-1] = -2;
        grid[i][j+1] = -2;

        grid[i+1][j] = -2;
        grid[i+1][j+1] = -2;
        grid[i+1][j-1] = -2;

        grid[i-1][j] = -2;
        grid[i-1][j+1] = -2;
        grid[i-1][j-1] = -2;
    }
}
