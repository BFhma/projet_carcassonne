#include "printing.h"
#include "positionFunctions.h"
#include "structs.h"
#include "tileFunctions.h"
#include <stdio.h>

char* bg_for(sectionType t) {
    switch (t) {
        case meadow:  return "\033[30;42m";   
        case road:    return "\033[30;103m";  
        case town:    return "\033[30;46m";   
        case aegis:   return "\033[30;47m";   
        case abbey:   return "\033[30;44m";   
        case village: return "\033[30;45m";   
        default:      return NULL;
    }
}

char char_for(section s) {
    switch (s.type) {
        case meadow:  return 'P';
        case road:    return 'R';
        case town:    return 'V';
        case aegis:   return 'B';
        case abbey:   return 'A';
        case village: return 'L';
        default:      return ' ';
    }
}

void write_section_with_highlight(section s, int highlight) {
    char *bg = bg_for(s.type);
    if (s.meepleId >= 0 && highlight) {
        int pid = s.meepleId + 1; 
        char mch = (pid >= 0 && pid <= 9) ? ('0' + pid) : '?';
        if (bg) printf("%s\033[31m%c\033[0m", bg, mch);
        else printf("\033[31m%c\033[0m", mch);
        return;
    }
    char c = char_for(s);
    if (c == ' ') {
        putchar(' ');
        return;
    }
    if (bg) printf("%s%c\033[0m", bg, c);
    else putchar(c);
}

void write_section(section s) {
    write_section_with_highlight(s, 1);
}

section effective_center_section(tile t) {
    section center = t.center;
    if (center.type == meadow || center.type == empty) {
        bool hasRoadSide = t.north.type == road || t.east.type == road || t.south.type == road || t.west.type == road;
        if (hasRoadSide) {
            center.type = road;
        }
    }
    return center;
}

section corner_section(section primary, section secondary, bool primaryRoad, bool secondaryRoad) {
    if (primaryRoad && secondaryRoad) {
        return (section){meadow, -1};
    }
    if (primaryRoad) {
        return secondary;
    }
    return primary;
}

void print_tile_row(tile t, int line) {
    if (tile_is_empty(t)) {
        putchar(' '); putchar(' '); putchar(' ');
        return;
    }

    bool northRoad = t.north.type == road;
    bool eastRoad = t.east.type == road;
    bool southRoad = t.south.type == road;
    bool westRoad = t.west.type == road;

    if (line == 0) {
        write_section(corner_section(t.north, t.west, northRoad, westRoad));
        write_section(t.north);
        write_section(corner_section(t.north, t.east, northRoad, eastRoad));
    } else if (line == 1) {
        write_section(t.west);
        write_section(effective_center_section(t));
        write_section(t.east);
    } else {
        write_section(corner_section(t.south, t.west, southRoad, westRoad));
        write_section(t.south);
        write_section(corner_section(t.south, t.east, southRoad, eastRoad));
    }
}

void printTile(tile t) {
    for (int line = 0; line < 3; line++) {
        print_tile_row(t, line);
        putchar('\n');
    }
}

void printBoard(tile** gameGrid, int gridSize, point* tilePositions, int tilesSet) {
    if (tilesSet <= 0) {
        return;
    }

    int min_i = gridSize, max_i = -1, min_j = gridSize, max_j = -1;
    for (int k = 0; k < tilesSet; k++) {
        int i = tilePositions[k].x;
        int j = tilePositions[k].y;
        if (i < min_i) min_i = i;
        if (i > max_i) max_i = i;
        if (j < min_j) min_j = j;
        if (j > max_j) max_j = j;
    }

    if (max_i == -1) {
        return;
    }

    int margin = 1;
    int vis_min_i = (min_i - margin >= 0) ? (min_i - margin) : 0;
    int vis_min_j = (min_j - margin >= 0) ? (min_j - margin) : 0;
    int vis_max_i = (max_i + margin < gridSize) ? (max_i + margin) : (gridSize - 1);
    int vis_max_j = (max_j + margin < gridSize) ? (max_j + margin) : (gridSize - 1);

    for (int i = vis_min_i; i <= vis_max_i; i++) {
        for (int line = 0; line < 3; line++) {
            for (int j = vis_min_j; j <= vis_max_j; j++) {
                tile t = gameGrid[i][j];
                print_tile_row(t, line);
            }
            putchar('\n');
        }
    }
}


void print_tile_row_with_possibilities(tile t, int line, int x, int y, point* possibilities, int possibilitiesCount) {
    bool is_possible = isPositionInArray((point){x, y}, possibilities, possibilitiesCount);
    
    if (is_possible) {
        
        if (line == 0) {
            printf("%03d", x);  
        } else if (line == 1) {
            printf("===");
        } else {
            printf("%03d", y);  
        }
    } else if (tile_is_empty(t)) {
        printf("   ");  
    } else {
        
        print_tile_row(t, line);
    }
}

void printBoardWithPossibilities(tile** gameGrid, int gridSize, point* tilePositions, int tilesSet, point* possibilities, int possibilitiesCount) {
    int min_i = gridSize, max_i = -1, min_j = gridSize, max_j = -1;
    for (int k = 0; k < tilesSet; k++) {
        int i = tilePositions[k].x;
        int j = tilePositions[k].y;
        if (i < min_i) min_i = i;
        if (i > max_i) max_i = i;
        if (j < min_j) min_j = j;
        if (j > max_j) max_j = j;
    }

    for (int i = 0; i < possibilitiesCount; i++) {
        int x = possibilities[i].x;
        int y = possibilities[i].y;
        if (x < min_i) min_i = x;
        if (x > max_i) max_i = x;
        if (y < min_j) min_j = y;
        if (y > max_j) max_j = y;
    }

    if (max_i == -1) {
        return;
    }

    int margin = 1;
    int vis_min_i = (min_i - margin >= 0) ? (min_i - margin) : 0;
    int vis_min_j = (min_j - margin >= 0) ? (min_j - margin) : 0;
    int vis_max_i = (max_i + margin < gridSize) ? (max_i + margin) : (gridSize - 1);
    int vis_max_j = (max_j + margin < gridSize) ? (max_j + margin) : (gridSize - 1);

    for (int i = vis_min_i; i <= vis_max_i; i++) {
        for (int line = 0; line < 3; line++) {
            for (int j = vis_min_j; j <= vis_max_j; j++) {
                tile t = gameGrid[i][j];
                print_tile_row_with_possibilities(t, line, i, j, possibilities, possibilitiesCount);
            }
            putchar('\n');
        }
    }
}


