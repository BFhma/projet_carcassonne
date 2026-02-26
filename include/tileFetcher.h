#pragma once
#include "structs.h"

int countLines(char* filename); // Ouvre un fichier avec un nom passé en paramètres, retourne son nombre de ligne.
void parseFile(char* filename, tile* array); // Ouvre un fichier avec un nom passé en paramètres, remplit le tableau de tuiles
// également passé en paramètres.
void printTiles(tile* array, int size); // Affiche les tuiles de la liste de tuiles.