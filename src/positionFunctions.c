#include "positionFunctions.h"

// AJOUTE A LA LISTE DE POSITION LA POSITION D'UNE TUILE NOUVELLEMENT POSÉE
void addTilePosition(point* positionArray, int x, int y, int size) {
    positionArray[size].x = x;
    positionArray[size].y = y;
}