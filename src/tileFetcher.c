#include <stdio.h>
#include <string.h>
#include "tileFetcher.h"

int countLines(char* filename) {
    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        printf("le fichier n'a pas pu etre ouvert\n");
    }
    int res = 0;
    char c;

    while (!feof(f)) {
        c = fgetc(f);
        if (c == '\n') {
            res++;
        }
    }

    return res;
}

void parseFile(char* filename, tile* array)
{
    FILE *file = fopen(filename, "r");
    char* token;

    if (file == NULL) {
        printf("Impossible d'ouvrir le fichier\n");
        return;
    }

    char line[1024];
    int idTile = 0;

    while (fgets(line, sizeof(line), file)) {
        section sectionArray[5];

        line[strcspn(line, "\n")] = 0; // STRCPN: RENVOIE LA LONGUEUR DE LA PLUS GRANDE SOUS-CHAINE EN IGNORANT TOUS /n.
        // ICI ON S'ASSURE DONC QUE LA CHAINE EST BIEN TERMINEE PAR UN CARACTÈRE NUL.
        token = strtok(line, ","); // EXTRAIT UN TOKEN, DELIMITE PAR ,
        int i = 0;
        
        while (token != NULL && i < 5) {
            if (strcmp("route", token) == 0) {
                sectionArray[i].type = road;
                sectionArray[i].meepleId = -1;
            }
            else if (strcmp("ville", token) == 0) {
                sectionArray[i].type = town;
                sectionArray[i].meepleId = -1;
            }
            else if (strcmp("pre", token) == 0) {
                sectionArray[i].type = meadow;
                sectionArray[i].meepleId = -1;
            }
            else if (strcmp("blason", token) == 0) {
                sectionArray[i].type = aegis;
                sectionArray[i].meepleId = -1;
            }
            else if (strcmp("abbaye", token) == 0) {
                sectionArray[i].type = abbey;
                sectionArray[i].meepleId = -1;
            }

            i++;
            token = strtok(NULL, ","); // APRES LE PREMIER APPEL DE STRTOK, LE PREMIER ARGUMENT DOIT ETRE NULL,
            // ET NON PLUS LA CHAINE SOURCE.
        }

        array[idTile].north = sectionArray[0];
        array[idTile].east = sectionArray[1];
        array[idTile].south = sectionArray[2];
        array[idTile].west = sectionArray[3];
        array[idTile].center = sectionArray[4];

        idTile++;
    }

    fclose(file);
}

void printTiles(tile* array, int size) {
    char* nameArray[5] = {"Road", "Town", "Meadow", "Aegis", "Abbey"};
    for (int i = 0 ; i < size ; i++) {
        printf("Tile n°%d : N = %s, E = %s, S = %s, O = %s, C = %s.\n", i, nameArray[array[i].north.type], nameArray[array[i].east.type], nameArray[array[i].south.type], nameArray[array[i].west.type], nameArray[array[i].center.type]);
    }
}