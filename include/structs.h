#pragma once

typedef enum {
    false, true
} bool;

typedef struct {
    int x;
    int y;
} point;

typedef enum {
    road, town, meadow, aegis, abbey, empty
} sectionType;

typedef struct {
    sectionType type;
    int meepleId; // ID DU JOUEUR POSSÉDANT UN MEEPLE SUR CETTE SECTION, -1 SI AUCUN
} section;

typedef struct {
    section north;
    section east;
    section south;
    section west;
    section center;
} tile;

typedef struct {
    int meeples;
    int score;
    int id;
} player;