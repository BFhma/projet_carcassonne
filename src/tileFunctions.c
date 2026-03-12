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