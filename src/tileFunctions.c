#include "tileFunctions.h"

bool section_types_match(sectionType a, sectionType b) {
    if (a == b) {
        return true;
    }
    if ((a == town || a == aegis) && (b == town || b == aegis)) {
        return true;
    }
    return false;
}

bool isPlaceable(tile t, tile** grid, point* positionArray, int size) {
    int rotations = 0;
    int i = 0;
    while (rotations < 4) {
        if (section_types_match(grid[positionArray[i].x][positionArray[i].y].east.type, t.west.type)) {
            if (grid[positionArray[i].x][positionArray[i].y - 1].center.type == empty) {
                return true;
            }
        }
        if (section_types_match(grid[positionArray[i].x][positionArray[i].y].west.type, t.east.type)) {
            if (grid[positionArray[i].x][positionArray[i].y + 1].center.type == empty) {
                return true;
            }
        }
        if (section_types_match(grid[positionArray[i].x][positionArray[i].y].south.type, t.north.type)) {
            if (grid[positionArray[i].x + 1][positionArray[i].y].center.type == empty) {
                return true;
            }
        }
        if (section_types_match(grid[positionArray[i].x][positionArray[i].y].north.type, t.south.type)) {
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
        }
        default: {
            return false;
        }
    }
}

point* returnPossibilities(tile t, tile** grid, int gridSize, point* positionArray, int size, int* count) {
    int maxPossibilities = size * 4;
    if (maxPossibilities <= 0) {
        *count = 0;
        return NULL;
    }

    point *res = malloc(maxPossibilities * sizeof *res);
    if (!res) {
        *count = 0;
        return NULL;
    }

    int possibilities = 0;
    int dx[4] = {-1, 1, 0, 0};
    int dy[4] = {0, 0, -1, 1};

    for (int i = 0; i < size; i++) {
        int baseX = positionArray[i].x;
        int baseY = positionArray[i].y;

        for (int dir = 0; dir < 4; dir++) {
            int cx = baseX + dx[dir];
            int cy = baseY + dy[dir];

            bool validLocation = true;
            if (cx < 0 || cy < 0 || cx >= gridSize || cy >= gridSize) {
                validLocation = false;
            }
            if (validLocation && grid[cx][cy].center.type != empty) {
                validLocation = false;
            }

            if (validLocation) {
                bool duplicate = false;
                for (int k = 0; k < possibilities; k++) {
                    if (res[k].x == cx && res[k].y == cy) {
                        duplicate = true;
                    }
                }
                if (duplicate) {
                    validLocation = false;
                }
            }

            if (validLocation) {
                bool hasNeighbor = false;
                bool allMatch = true;

                for (int checkDir = 0; checkDir < 4; checkDir++) {
                    int nx = cx + dx[checkDir];
                    int ny = cy + dy[checkDir];

                    bool neighborExists = true;
                    if (nx < 0 || ny < 0 || nx >= gridSize || ny >= gridSize) {
                        neighborExists = false;
                    }
                    if (neighborExists && grid[nx][ny].center.type == empty) {
                        neighborExists = false;
                    }

                    if (neighborExists) {
                        hasNeighbor = true;
                        section candidateSection;
                        section neighborSection;

                        if (checkDir == 0) {
                            candidateSection = t.north;
                            neighborSection = grid[nx][ny].south;
                        } else if (checkDir == 1) {
                            candidateSection = t.south;
                            neighborSection = grid[nx][ny].north;
                        } else if (checkDir == 2) {
                            candidateSection = t.west;
                            neighborSection = grid[nx][ny].east;
                        } else {
                            candidateSection = t.east;
                            neighborSection = grid[nx][ny].west;
                        }

                        if (!section_types_match(candidateSection.type, neighborSection.type)) {
                            allMatch = false;
                        }
                    }
                }

                if (!hasNeighbor || !allMatch) {
                    validLocation = false;
                }
            }

            if (validLocation) {
                res[possibilities].x = cx;
                res[possibilities].y = cy;
                possibilities++;
            }
        }
    }

    *count = possibilities;
    if (possibilities == 0) {
        free(res);
        return NULL;
    }

    point* trimmed = realloc(res, sizeof(point) * possibilities);
    return trimmed ? trimmed : res;
}

void printPossibilities(point* possibilities, int size) {
    for (int i = 0 ; i < size ; i++) {
        printf("Tuile placable en : %d;%d.\n", possibilities[i].x, possibilities[i].y);
    }
}


int tile_is_empty(tile t) {
    return (t.center.type == empty && t.north.type == empty && t.south.type == empty && t.east.type == empty && t.west.type == empty);
}