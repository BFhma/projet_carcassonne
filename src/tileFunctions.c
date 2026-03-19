#include "tileFunctions.h"

bool isPlaceable(tile t, tile** grid, point* positionArray, int size) {
    int rotations = 0;
    int i = 0;
    while (rotations < 4) {
        if (grid[positionArray[i].x][positionArray[i].y].east.type == t.west.type) {
            if (grid[positionArray[i].x][positionArray[i].y - 1].center.type == empty) {
                return true;
            }
        }
        if (grid[positionArray[i].x][positionArray[i].y].west.type == t.east.type) {
            if (grid[positionArray[i].x][positionArray[i].y + 1].center.type == empty) {
                return true;
            }
        }
        if (grid[positionArray[i].x][positionArray[i].y].south.type == t.north.type) {
            if (grid[positionArray[i].x + 1][positionArray[i].y].center.type == empty) {
                return true;
            }
        }
        if (grid[positionArray[i].x][positionArray[i].y].north.type == t.south.type) {
            if (grid[positionArray[i].x - 1][positionArray[i].y].center.type == empty) {
                return true;
            }
        }

        i++;
        if (i == size) {
            i = 0;
            rotateRight(&t);
            rotations++;
        }
    }
    return false;
}

void rotateRight(tile* t) {
    tile new;

    new.center.type = t->center.type;
    new.center.meepleId = t->center.meepleId;

    new.east.type = t->north.type;
    new.east.meepleId = t->north.meepleId;

    new.south.type = t->east.type;
    new.south.meepleId = t->east.meepleId;

    new.west.type = t->south.type;
    new.west.meepleId = t->south.meepleId;

    new.north.type = t->west.type;
    new.north.meepleId = t->west.meepleId;

    *t = new;
}

void printTileContent(tile t) {
    printf("Nord : %s.\n", returnSectionContent(t.north.type));
    printf("Est : %s.\n", returnSectionContent(t.east.type));
    printf("Sud : %s.\n", returnSectionContent(t.south.type));
    printf("Ouest : %s.\n", returnSectionContent(t.west.type));
    printf("Centre : %s.\n", returnSectionContent(t.center.type));
}

char* returnSectionContent(sectionType st) {
    switch(st) {
        case abbey: {
            return("Abbaye");
        }
        case aegis: {
            return("Egide");
        }
        case village: {
            return("Village");
        }
        case town: {
            return("Ville");
        }
        case road: {
            return("Route");
        }
        case meadow: {
            return("Pre");
        }
        case empty: {
            return ("Vide");
        }
        default: {
            return ("Vide");
        }
    }
}

bool isMeeplePlaceable(tile t, section s) {
    switch (s.type) {
        case abbey: {
            return true;
        }
        case town: {
            return true;
        }
        case road: {
            // TODO
        }
        default: {
            return false;
        }
    }
}

point* returnPossibilities(tile t, tile** grid, point* positionArray, int size) {
    int nbCells = (sizeof grid) / (sizeof grid[0][0]);
    point *res = malloc(nbCells * sizeof *res);
    int possibilities = 0;
    for (int i = 0 ; i < size ; i++) {
        if (grid[positionArray[i].x - 1][positionArray[i].y].center.type == empty) {
            if (grid[positionArray[i].x][positionArray[i].y].north.type == t.south.type) {
                res[possibilities].x = positionArray[i].x - 1;
                res[possibilities].y = positionArray[i].y;
                possibilities++;
            }
        }
        if (grid[positionArray[i].x + 1][positionArray[i].y].center.type == empty) {
            if (grid[positionArray[i].x][positionArray[i].y].south.type == t.north.type) {
                res[possibilities].x = positionArray[i].x + 1;
                res[possibilities].y = positionArray[i].y;
                possibilities++;
            }
        }
        if (grid[positionArray[i].x][positionArray[i].y - 1].center.type == empty) {
            if (grid[positionArray[i].x][positionArray[i].y].west.type == t.east.type) {
                res[possibilities].x = positionArray[i].x;
                res[possibilities].y = positionArray[i].y - 1;
                possibilities++;
            }
        }
        if (grid[positionArray[i].x][positionArray[i].y + 1].center.type == empty) {
            if (grid[positionArray[i].x][positionArray[i].y].east.type == t.west.type) {
                res[possibilities].x = positionArray[i].x;
                res[possibilities].y = positionArray[i].y + 1;
                possibilities++;
            }
        }
    }
    res = realloc(res, sizeof(point) * possibilities);
    return res;
}

void printPossibilities(point* possibilities, int size) {
    int length = sizeof(possibilities) / sizeof(possibilities[0]);
    for (int i = 0 ; i < length ; i++) {
        printf("Tuile placable en : %d;%d.\n", possibilities[i].x, possibilities[i].y);
    }
}