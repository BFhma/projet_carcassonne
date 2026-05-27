#pragma once
#include "structs.h"

void printBoard(tile** gameGrid, int gridSize, point* tilePositions, int tilesSet);
void printTile(tile t);
void printBoardWithPossibilities(tile** gameGrid, int gridSize, point* tilePositions, int tilesSet, point* possibilities, int possibilitiesCount);

