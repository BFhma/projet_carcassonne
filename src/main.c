#include "structs.h"
#include "tileFetcher.h"
#include "deckFunctions.h"
#include "tileFunctions.h"
#include "positionFunctions.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char* argv[]) {
    argc--;
    argv++;

    srand(time(NULL));
    int nbLines = countLines(*argv);

    // CHARGEMENT DE LA CONFIGURATION VIA LE FICHIER CSV ET CREATION DU 'DECK' DE TUILES
    tile* tileArray = (tile*)malloc(sizeof(tile) * nbLines);
    parseFile(*argv, tileArray);
    shuffleDeck(tileArray, nbLines);
    int deckHeight = nbLines;

    //FONCTION DE TEST :
    //printTiles(tileArray, deckHeight);
    
    // INITIALISATION DES JOUEURS
    int nbPlayers;
    printf("Veuillez entrer le nombre de joueurs (2-5) : ");
    scanf("%d", &nbPlayers);
    printf("\n");

    player* playerArray = (player*)malloc(sizeof(player) * nbPlayers);

    for (int i = 0 ; i < nbPlayers ; i++) {
        playerArray[i].id = i;
        playerArray[i].meeples = 8;
        playerArray[i].score = 0;
    }

    // INITIALISATION DE LA GRILLE
    tile** gameGrid = (tile**)malloc(sizeof(tile*) * (nbLines * 2 + 2));
    for (int i = 0 ; i < nbLines * 2 + 2 ; i++) {
        gameGrid[i] = (tile*)malloc(sizeof(tile) *(nbLines * 2 + 2));
    }

    // REMPLISSAGE DE LA GRILLE AVEC LES VALEURS PAR DÉFAUT
    for (int i = 0 ; i < (nbLines * 2) + 2 ; i++) {
        for (int j = 0 ; j < (nbLines * 2) + 2 ; j++) {
            gameGrid[i][j].center.meepleId = -1;
            gameGrid[i][j].center.type = empty;
            gameGrid[i][j].east.meepleId = -1;
            gameGrid[i][j].east.type = empty;
            gameGrid[i][j].west.meepleId = -1;
            gameGrid[i][j].west.type = empty;
            gameGrid[i][j].north.meepleId = -1;
            gameGrid[i][j].north.type = empty;
            gameGrid[i][j].south.meepleId = -1;
            gameGrid[i][j].south.type = empty;
        }
    }

    // INITIALISATION DU TABLEAU DE POSITIONS DE TUILES PLACÉES
    point* tilePositions = (point*)malloc(sizeof(point) * nbLines);
    for (int i = 0 ; i < nbLines ; i++) {
        tilePositions[i].x = -1;
        tilePositions[i].y = -1;
    }

    // LANCEMENT DE LA MAIN LOOP, AVEC SES DEUX POTENTIELLES CONDITIONS D'ARRÊT :
    int deckShifts = 0;
    bool isFirstTurn = true;
    int tilesSet = 0;
    int playerTurn = 0;

    while (deckShifts != deckHeight && deckHeight > 0) {
        tile currentTile;
        if (isFirstTurn == true) {
            currentTile = draw(tileArray, deckHeight);
            //printTiles(&currentTile, 1);
            deckHeight--;
            gameGrid[nbLines][nbLines] = currentTile;
            addTilePosition(tilePositions, nbLines, nbLines, tilesSet);
            tilesSet++;
            /*
            printf("Joueur %d, veux-tu placer un meeple sur ta tuile ? o/n\n", playerTurn + 1);
            char choice;
            scanf(" %c", &choice);
            if (choice == 'o') {
                // TODO
            }
            */
            isFirstTurn = false;
        }
        else {
            if (isPlaceable(tileArray[deckHeight - 1], gameGrid, tilePositions, tilesSet) == true) {
                printf("La tuile piochee est placable !\n");
                currentTile = draw(tileArray, deckHeight);
                //printTiles(&currentTile, 1);
                deckHeight--;
                deckShifts = 0;
            }
            else { // LA TUILE PIOCHÉE N'EST PAS PLACABLE
                printf("Tuile piochee non placable, on en pioche une autre.\n");
                shiftLeft(tileArray, deckHeight);
                deckShifts++;
            }
        }

    }

    // LE JEU EST TERMINÉ
    if (deckShifts == deckHeight) {
        printf("Fin du jeu : plus aucune tuile n'est posable !\n");
    }
    else if (deckHeight == 0) {
        printf("Fin du jeu : toutes les tuiles ont ete posees !\n");
    }

    //LIBERATION DE LA MEMOIRE ALLOUEE.
    free(playerArray);
    free(tileArray);
    free(tilePositions);

    return 0;
}