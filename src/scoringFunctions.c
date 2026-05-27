#include "scoringFunctions.h"
#include "structs.h"
#include "tileFunctions.h"
#include <stdlib.h>

bool in_bounds(int x, int y, int gridSize) {
    return x >= 0 && y >= 0 && x < gridSize && y < gridSize;
}

bool is_city_section(sectionType type) {
    return type == town || type == aegis;
}

bool is_road_section(sectionType type) {
    return type == road;
}

section* tile_section_by_direction(tile* t, int direction) {
    switch (direction) {
        case 0: return &t->north;
        case 1: return &t->east;
        case 2: return &t->south;
        case 3: return &t->west;
        default: return NULL;
    }
}

void scoreAbbeys(tile** gameGrid, int gridSize, point* tilePositions, int tilesSet, player* players, int playerCount) {
    for (int k = 0; k < tilesSet; k++) {
        int i = tilePositions[k].x;
        int j = tilePositions[k].y;
        tile* current = &gameGrid[i][j];
        bool isAbbey = current->center.type == abbey;
        if (isAbbey) {
            bool complete = true;
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if (complete) {
                        bool isCenter = (dx == 0 && dy == 0);
                        if (!isCenter) {
                            int ni = i + dx;
                            int nj = j + dy;
                            if (!in_bounds(ni, nj, gridSize) || tile_is_empty(gameGrid[ni][nj])) {
                                complete = false;
                            }
                        }
                    }
                }
            }

            if (complete) {
                int owner = current->center.meepleId;
                bool ownerValid = owner >= 0 && owner < playerCount;
                if (ownerValid) {
                    players[owner].meeples++;
                    players[owner].score += 8;
                    printf("Abbaye scoree !\n");
                }
                current->center.meepleId = -1;
            }
        }
    }
}

bool tile_has_road(tile t) {
    return is_road_section(t.north.type) || is_road_section(t.east.type) || is_road_section(t.south.type) || is_road_section(t.west.type);
}

void scoreRoads(tile** gameGrid, int gridSize, point* tilePositions, int tilesSet, player* players, int playerCount) {
    int maxCells = gridSize * gridSize;
    bool* visited = (bool*)calloc(maxCells, sizeof(bool));
    point* queue = (point*)malloc(sizeof(point) * maxCells);
    point* component = (point*)malloc(sizeof(point) * maxCells);
    int dx[4] = {-1, 0, 1, 0};
    int dy[4] = {0, 1, 0, -1};
    int opposite[4] = {2, 3, 0, 1};

    for (int k = 0; k < tilesSet; k++) {
        int i = tilePositions[k].x;
        int j = tilePositions[k].y;
        int index = i * gridSize + j;
        bool shouldProcess = !visited[index] && tile_has_road(gameGrid[i][j]);
        if (shouldProcess) {
            int head = 0;
            int tail = 0;
            int componentSize = 0;
            queue[tail++] = (point){i, j};
            visited[index] = true;

            while (head < tail) {
                point current = queue[head++];
                component[componentSize++] = current;
                tile* currentTile = &gameGrid[current.x][current.y];

                for (int direction = 0; direction < 4; direction++) {
                    section* current_section = tile_section_by_direction(currentTile, direction);
                    bool isRoad = is_road_section(current_section->type);
                    if (isRoad) {
                        int nx = current.x + dx[direction];
                        int ny = current.y + dy[direction];
                        bool inBounds = in_bounds(nx, ny, gridSize);
                        if (inBounds) {
                            section* neighborSection = tile_section_by_direction(&gameGrid[nx][ny], opposite[direction]);
                            bool neighborRoad = is_road_section(neighborSection->type);
                            if (neighborRoad) {
                                int neighborIndex = nx * gridSize + ny;
                                bool notVisited = !visited[neighborIndex];
                                if (notVisited) {
                                    visited[neighborIndex] = true;
                                    queue[tail++] = (point){nx, ny};
                                }
                            }
                        }
                    }
                }
            }

            bool hasOpenEndpoint = false;
            for (int m = 0; m < componentSize && !hasOpenEndpoint; m++) {
                tile* currentTile = &gameGrid[component[m].x][component[m].y];
                for (int direction = 0; direction < 4 && !hasOpenEndpoint; direction++) {
                    section* current_section = tile_section_by_direction(currentTile, direction);
                    bool isRoad = is_road_section(current_section->type);
                    if (isRoad) {
                        int nx = component[m].x + dx[direction];
                        int ny = component[m].y + dy[direction];
                        bool inBounds = in_bounds(nx, ny, gridSize);
                        bool neighborEmpty = true;
                        if (inBounds) {
                            neighborEmpty = tile_is_empty(gameGrid[nx][ny]);
                        }
                        if (!inBounds || neighborEmpty) {
                            hasOpenEndpoint = true;
                        } else {
                            section* neighborSection = tile_section_by_direction(&gameGrid[nx][ny], opposite[direction]);
                            bool neighborRoad = is_road_section(neighborSection->type);
                            bool centerRoad = gameGrid[nx][ny].center.type == road;
                            if (!neighborRoad && centerRoad) {
                                hasOpenEndpoint = true;
                            }
                        }
                    }
                }
            }

            if (!hasOpenEndpoint) {
                bool* scoredPlayer = (bool*)calloc(playerCount, sizeof(bool));
                for (int m = 0; m < componentSize; m++) {
                    tile* currentTile = &gameGrid[component[m].x][component[m].y];
                    for (int direction = 0; direction < 4; direction++) {
                        section* section = tile_section_by_direction(currentTile, direction);
                        bool isRoad = is_road_section(section->type);
                        if (isRoad) {
                            int owner = section->meepleId;
                            bool ownerValid = owner >= 0 && owner < playerCount;
                            if (ownerValid && !scoredPlayer[owner]) {
                                players[owner].score += componentSize;
                                players[owner].meeples++;
                                scoredPlayer[owner] = true;
                            }
                            section->meepleId = -1;
                        }
                    }
                }
                free(scoredPlayer);
                printf("Route scoree !\n");
            }
        }
    }

    free(visited);
    free(queue);
    free(component);
}

bool tile_has_city(tile t) {
    return is_city_section(t.north.type) || is_city_section(t.east.type) || is_city_section(t.south.type) || is_city_section(t.west.type) || t.center.type == aegis;
}

void scoreCities(tile** gameGrid, int gridSize, point* tilePositions, int tilesSet, player* players, int playerCount) {
    int maxCells = gridSize * gridSize;
    bool* visited = (bool*)calloc(maxCells, sizeof(bool));
    point* queue = (point*)malloc(sizeof(point) * maxCells);
    point* component = (point*)malloc(sizeof(point) * maxCells);
    int dx[4] = {-1, 0, 1, 0};
    int dy[4] = {0, 1, 0, -1};
    int opposite[4] = {2, 3, 0, 1};

    for (int k = 0; k < tilesSet; k++) {
        int i = tilePositions[k].x;
        int j = tilePositions[k].y;
        int index = i * gridSize + j;
        bool shouldProcess = !visited[index] && tile_has_city(gameGrid[i][j]);
        if (shouldProcess) {
            int head = 0;
            int tail = 0;
            int componentSize = 0;
            queue[tail++] = (point){i, j};
            visited[index] = true;

            while (head < tail) {
                point current = queue[head++];
                component[componentSize++] = current;
                tile* currentTile = &gameGrid[current.x][current.y];

                for (int direction = 0; direction < 4; direction++) {
                    section* current_section = tile_section_by_direction(currentTile, direction);
                    bool isCity = is_city_section(current_section->type);
                    if (isCity) {
                        int nx = current.x + dx[direction];
                        int ny = current.y + dy[direction];
                        bool inBounds = in_bounds(nx, ny, gridSize);
                        bool neighborExists = inBounds && !tile_is_empty(gameGrid[nx][ny]);
                        if (neighborExists) {
                            section* neighborSection = tile_section_by_direction(&gameGrid[nx][ny], opposite[direction]);
                            bool neighborCity = is_city_section(neighborSection->type);
                            if (neighborCity) {
                                int neighborIndex = nx * gridSize + ny;
                                bool notVisited = !visited[neighborIndex];
                                if (notVisited) {
                                    visited[neighborIndex] = true;
                                    queue[tail++] = (point){nx, ny};
                                }
                            }
                        }
                    }
                }
            }

            bool complete = true;
            int cityPoints = 0;
            for (int m = 0; m < componentSize && complete; m++) {
                tile* currentTile = &gameGrid[component[m].x][component[m].y];
                cityPoints += (currentTile->center.type == aegis) ? 2 : 1;
                for (int direction = 0; direction < 4 && complete; direction++) {
                    section* current_section = tile_section_by_direction(currentTile, direction);
                    bool isCity = is_city_section(current_section->type);
                    if (isCity) {
                        int nx = component[m].x + dx[direction];
                        int ny = component[m].y + dy[direction];
                        bool inBounds = in_bounds(nx, ny, gridSize);
                        bool emptyNeighbor = inBounds ? tile_is_empty(gameGrid[nx][ny]) : true;
                        if (!inBounds || emptyNeighbor) {
                            complete = false;
                        } else {
                            section* neighborSection = tile_section_by_direction(&gameGrid[nx][ny], opposite[direction]);
                            bool neighborCity = is_city_section(neighborSection->type);
                            if (!neighborCity) {
                                complete = false;
                            }
                        }
                    }
                }
            }

            if (complete) {
                bool* scoredPlayer = (bool*)calloc(playerCount, sizeof(bool));
                for (int m = 0; m < componentSize; m++) {
                    tile* currentTile = &gameGrid[component[m].x][component[m].y];
                    for (int direction = 0; direction < 4; direction++) {
                        section* current_section = tile_section_by_direction(currentTile, direction);
                        bool isCity = is_city_section(current_section->type);
                        if (isCity) {
                            int owner = current_section->meepleId;
                            bool ownerValid = owner >= 0 && owner < playerCount;
                            if (ownerValid && !scoredPlayer[owner]) {
                                players[owner].score += cityPoints;
                                players[owner].meeples++;
                                scoredPlayer[owner] = true;
                            }
                            current_section->meepleId = -1;
                        }
                    }
                    if (currentTile->center.type == aegis) {
                        int owner = currentTile->center.meepleId;
                        bool ownerValid = owner >= 0 && owner < playerCount;
                        if (ownerValid && !scoredPlayer[owner]) {
                            players[owner].score += cityPoints;
                            players[owner].meeples++;
                            scoredPlayer[owner] = true;
                        }
                        currentTile->center.meepleId = -1;
                    }
                }
                free(scoredPlayer);
                printf("Ville scoree !\n");
            }
        }
    }

    free(visited);
    free(queue);
    free(component);
}

void updateScoresAfterPlacement(tile** gameGrid, int gridSize, point* tilePositions, int tilesSet, player* players, int playerCount, int placedX, int placedY) {
    (void)placedX;
    (void)placedY;
    scoreAbbeys(gameGrid, gridSize, tilePositions, tilesSet, players, playerCount);
    scoreRoads(gameGrid, gridSize, tilePositions, tilesSet, players, playerCount);
    scoreCities(gameGrid, gridSize, tilePositions, tilesSet, players, playerCount);
}


