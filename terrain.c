#include "common.h"
#include "terrain.h"

#include <stdio.h>
#include <stdlib.h>

int E = 60;

int random(int upper, int lower) {
    int num = (rand() % (upper - lower + 1)) + lower;
    return num;
}

void smooth() {
    for (int x = 0; x < sizeX; x++) {
        for (int y = 0; y < sizeY; y++) {
            if (grid[x][y] < 0) {
                grid[x][y] = 0;
            }
            int sum = grid[x][y], cells = 1;
            if (x < sizeX - 1) {
                sum += grid[x + 1][y];
                cells++;
            }
            if (x > 0) {
                sum += grid[x - 1][y];
                cells++;
            }
            if (y < sizeY - 1) {
                sum += grid[x][y + 1];
                cells++;
            }
            if (y > 0) {
                sum += grid[x][y - 1];
                cells++;
            }
            bgrid[x][y] = sum / cells;
        }
    }

    for (int x = 0; x < sizeX; x++) {
        for (int y = 0; y < sizeY; y++) {
            grid[x][y] = bgrid[x][y];
        }
    }
}

void ds(int sx, int sy, int size) {
    float c1 = grid[sx][sy];
    float c2 = grid[sx + size - 1][sy];
    float c3 = grid[sx][sy + size - 1];
    float c4 = grid[sx + size - 1][sy + size - 1];

    int hx = sx + size / 2;
    int hy = sy + size / 2;

    grid[hx][hy] = (c1 + c2 + c3 + c4) / 4 + random(E, -E);
    float d = grid[hx][hy];

    if (grid[hx][sy] == -1) {
        grid[hx][sy] = (c1 + c2 + d) / 3 + random(E, -E);
    }

    if (grid[sx + size - 1][hy] == -1) {
        grid[sx + size - 1][hy] = (c2 + c4 + d) / 3 + random(E, -E);
    }

    if (grid[hx][sy + size - 1] == -1) {
        grid[hx][sy + size - 1] = (c3 + c4 + d) / 3 + random(E, -E);
    }

    if (grid[sx][hy] == -1) {
        grid[sx][hy] = (c1 + c3 + d) / 3 + random(E, -E);
    }

    if (size == 3) {
        return;
    }

    if (E > 2) {
        E -= 3;
    }

    ds(sx, sy, size / 2 + 1);
    ds(hx, sy, size / 2 + 1);
    ds(sx, hy, size / 2 + 1);
    ds(hx, hy, size / 2 + 1);

    E += 3;
}
