#pragma once

#include "structs.h"

bool in_bounds(int x, int y, int gridSize);

bool is_city_section(sectionType type);

bool is_road_section(sectionType type);

section* tile_section_by_direction(tile* t, int direction);

void scoreAbbeys(tile** gameGrid, int gridSize, point* tilePositions, int tilesSet, player* players, int playerCount);

bool tile_has_road(tile t);

void scoreRoads(tile** gameGrid, int gridSize, point* tilePositions, int tilesSet, player* players, int playerCount);

bool tile_has_city(tile t);

void scoreCities(tile** gameGrid, int gridSize, point* tilePositions, int tilesSet, player* players, int playerCount);

void updateScoresAfterPlacement(tile** gameGrid, int gridSize, point* tilePositions, int tilesSet, player* players, int playerCount, int placedX, int placedY);
