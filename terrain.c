#include <stdio.h>
#include <stdlib.h>

#define sizeX 1024
#define sizeY 1024
#define diff 5

//steps - kolko puti da se smooth-ne terena
//E - range na random
float grid[sizeX][sizeY];
float bgrid[sizeX][sizeY];
int steps = 8;
int E = 60;
int durability = 99999;

//nqkva random funkciq deto vzeh ot neta shtoto bqh mnogo
//ubeden che moita ne bachka
int random(int upper, int lower){
    int num = (rand() % (upper - lower + 1)) + lower;
    return num;
}

//samiq smooth
//za vsqka kletka vzima 4te suseda i smqta sredno aritmetichnoto im
void smooth(){
    for(int x = 0;x < sizeX;x ++) {
        for(int y = 0;y < sizeY;y ++) {
            if(grid[x][y] < 0){
                grid[x][y] = 0;
            }
            int sum=grid[x][y], cells=1;
            if(x<sizeX-1){
                sum+=grid[x+1][y];cells++;
            }
            if(x>0){
                sum+=grid[x-1][y];cells++;
            }
            if(y<sizeY-1){
                sum+=grid[x][y+1];cells++;
            }
            if(y>0){
                sum+=grid[x][y-1];cells++;
            }
            bgrid[x][y]=sum/cells;
        }
    }

    //apply-va smetkite
    for(int x = 0;x < sizeX;x ++) {
        for(int y=0;y<sizeY;y++){
            grid[x][y]=bgrid[x][y];
        }
    }
}

//samiq diamond-square
void ds(int sx, int sy, int size){
    //4te ugula
    float c1 = grid[sx][sy];
    float c2 = grid[sx+size-1][sy];
    float c3 = grid[sx][sy+size-1];
    float c4 = grid[sx+size-1][sy+size-1];

    //koordinati na sredata
    int hx = sx + size/2;
    int hy = sy + size/2;

    //smqta srednoto na 4te ugula i go slaga na srednoto
    grid[hx][hy] = (c1 + c2 + c3 + c4) / 4 + random(E, -E);
    //stoinostta na srednoto
    float d = grid[hx][hy];

    //ako e prazno na s1(vij snimkata) smqta srednoto ot
    //dvata nai blizki ugula i sredata i go slaga tam
    if(grid[hx][sy] == -1) {
        grid[hx][sy] = (c1 + c2 + d) / 3 + random(E, -E);
    }

    //sushtoto ama za s2
    if(grid[sx + size - 1][hy] == -1) {
        grid[sx + size - 1][hy] = (c2 + c4 + d) / 3 + random(E, -E);
    }

    //sushtoto ama za s3
    if(grid[hx][sy + size - 1] == -1) {
        grid[hx][sy + size - 1] = (c3 + c4 + d) / 3 + random(E, -E);
    }

    //sushtoto ama za s4
    if(grid[sx][hy] == -1) {
        grid[sx][hy] = (c1 + c3 + d) / 3 + random(E, -E);
    }

    //ako e 3x3 nqma smisul da se muchim i spira rekursiqta
    if(size == 3) {
        return;
    }

    //barame E za da moje terena da e po smooth
    //kolkoto poveche se izpulnqva, tolkova po malko stava E

    //za da ne stava <0
    if(E>2){
        E-=3;
    }

    //samata rekursiq
    //vika q v kvadrant 2, 1, 3, 4
    ds(sx, sy, size / 2 + 1);
    ds(hx, sy, size / 2 + 1);
    ds(sx, hy, size / 2 + 1);
    ds(hx, hy, size / 2 + 1);

    //dobavqme za da moje da ne stava 0 prekaleno burzo
    E+=3;
}


//////////////////////////TUMOR\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//////////////////////////TUMOR\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//////////////////////////TUMOR\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//////////////////////////TUMOR\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//////////////////////////TUMOR\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//////////////////////////TUMOR\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//////////////////////////TUMOR\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//////////////////////////TUMOR\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//////////////////////////TUMOR\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

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

int is_water(int x, int y){
    return (grid[x][y] < 75 && grid[x][y] >= 0);
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

        if(x-1 >= 0 && visited[x-1][y] == 0){
            if(is_water(x-1, y) == 1 && durability > 1){
                durability--;
            }
            if(get_diff(x, y, x-1, y) <= diff){
                visited[x-1][y] = 1;
                enQueue(q, x-1, y);

                Cell cell = {x, y};
                prev[x-1][y] = cell;
            }
        }

        if(x+1 < sizeX && visited[x+1][y] == 0){
            if(is_water(x+1, y) == 1 && durability > 1){
                durability--;
            }
            if(get_diff(x, y, x+1, y) <= diff){
                visited[x+1][y] = 1;
                enQueue(q, x+1, y);

                Cell cell = {x, y};
                prev[x+1][y] = cell;
            }
        }

        if(y-1 >= 0 && visited[x][y-1] == 0){
            if(is_water(x, y-1) == 1 && durability > 1){
                durability--;
            }
            if(get_diff(x, y, x, y-1) <= diff){
                visited[x][y-1] = 1;
                enQueue(q, x, y-1);

                Cell cell = {x, y};
                prev[x][y-1] = cell;
            }
        }

        if(y+1 < sizeY && visited[x][y+1] == 0){
            if(is_water(x, y+1) == 1 && durability > 1){
                durability--;
            }
            if(get_diff(x, y, x, y+1) <= diff){
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

    for(int i=0;i<index/2;i++){
        Cell t = path[i];
        path[i] = path[index-i-1];
        path[index-i-1] = t;
    }

    for(int i=0;i<index;i++){
        grid[path[i].x][path[i].y] = -2;
    }
}

int main(){
    //seed na rand(hint: kato e v ds ne bachka ama nikak)
    srand((unsigned) time(0));

    //set-va masiva na -1
    for(int x = 0;x < sizeX;x ++) {
        for(int y = 0;y < sizeY;y ++) {
            grid[x][y] = -1;
            bgrid[x][y] = -1;
        }
    }

    //pravi 4te ugula na nqkvi raboti da ima da pochnem ot neshto
    grid[0][0] = 128;
    grid[sizeX-1][0] = 128;
    grid[0][sizeY-1] = 128;
    grid[sizeX-1][sizeY-1] = 128;

    ds(0, 0, sizeX);

    //vika se smooth()
    for(int i=0;i<steps;i++){
        smooth();
    }

    find_path(0, 0, 0, sizeX-1);

    //otvarq se faila
    FILE * terrain = fopen("Terrain.ppm","wb");

    //set-va se golemina na image-a
    fprintf(terrain,"P3\n");
    fprintf(terrain,"%d %d\n",sizeX,sizeY);
    fprintf(terrain,"255\n");

    //minava prez masiva
    for(int i = 0; i < sizeX; i++){
        for(int j = 0; j < sizeY; j++){
                int k = grid[j][i];
                if(k>305){
                    k=305;
                }
                if(k == -2){
                    ///puteka
                    fprintf(terrain,"%d %d %d ", 0,0,0);
                }else if(k < 75)
                    ///0 - 75 sinio
                    fprintf(terrain,"%d %d %d ", 0,0,abs(255-abs(k)));
                else if(k < 160)
                    ///75 - 160 zeleno
                    fprintf(terrain,"%d %d %d ", 0,k,0);
                else if(k < 233)
                    ///160 - 230 kafqvo
                    fprintf(terrain,"%d %d %d ", k-30,k/2-30,0);
                else {
                    ///230 - 255 bqlo
                    fprintf(terrain,"%d %d %d ", k-50, k-50, k-50);
                }
        }
        fprintf(terrain,"\n");
    }
    fclose(terrain);
}
