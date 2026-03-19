#include "positionFunctions.h"

// AJOUTE A LA LISTE DE POSITION LA POSITION D'UNE TUILE NOUVELLEMENT POSÉE
void addTilePosition(point* positionArray, int x, int y, int size) {
    positionArray[size].x = x;
    positionArray[size].y = y;
}

bool isPositionInArray(point position, point* array) {
    int length = sizeof(array) / sizeof(array[0]);
    for (int i = 0 ; i < length ; i++) {
        if (position.x == array[i].x && position.y == array[i].y) {
            return true;
        }
    }
    return false;
}