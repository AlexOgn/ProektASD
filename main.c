#include "common.h"
#include "pathing.h"
#include "terrain.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int steps = 8;

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

    find_path(400, 100, 50, 1000);

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

    return 0;
}
