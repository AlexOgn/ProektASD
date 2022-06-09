#include "common.h"
#include "terrain.h"

#include <stdio.h>
#include <stdlib.h>

int E = 60;

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
