#include "structs.h"

bool isPlaceable(tile t, tile** grid, point* positionArray, int size);

void rotateRight(tile* t);

void printTileContent(tile t);

char* returnSectionContent(sectionType st);

bool isMeeplePlaceable(tile t, section s);