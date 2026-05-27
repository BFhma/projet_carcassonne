#include "structs.h"
#include "tileFetcher.h"
#include "deckFunctions.h"
#include "tileFunctions.h"
#include "positionFunctions.h"
#include "printing.h"
#include "scoringFunctions.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

int main(int argc, char* argv[]) {
    argc--;
    argv++;

    srand(time(NULL));

    char path[256];
    char *filename = *argv;

    strcpy(path, "../data/");
    strcat(path, filename);

    int nbLines = countLines(path);
    if (nbLines <= 0) {
        printf("Erreur: impossible de charger le fichier\n");
        return 1;
    }

    // CHARGEMENT DE LA CONFIGURATION VIA LE FICHIER CSV ET CREATION DU 'DECK' DE TUILES
    tile* tileArray = (tile*)malloc(sizeof(tile) * nbLines);
    parseFile(path, tileArray);
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
    int gridSize = (nbLines * 2) + 2; // On double le nombre de lignes pour avoir assez de place pour poser les tuiles, et on ajoute 2 pour la tuile de départ et une marge
    int center = gridSize / 2; /* index central pour la première tuile */
    tile** gameGrid = (tile**)malloc(sizeof(tile*) * gridSize);
    for (int i = 0 ; i < gridSize ; i++) {
        gameGrid[i] = (tile*)malloc(sizeof(tile) * gridSize);
    }

    // REMPLISSAGE DE LA GRILLE AVEC LES VALEURS PAR DÉFAUT
    for (int i = 0 ; i < gridSize ; i++) {
        for (int j = 0 ; j < gridSize ; j++) {
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
            currentTile = draw(&tileArray, &deckHeight);
            printTileContent(currentTile);
            /* Placer la première tuile au centre de la grille */
            gameGrid[center][center] = currentTile;
            addTilePosition(tilePositions, center, center, tilesSet);
            tilesSet++;
            printBoard(gameGrid, gridSize, tilePositions, tilesSet);
            printf("Joueur %d, veux-tu placer un meeple sur ta tuile ? o/n\n", playerTurn + 1);
            char choice;
            scanf(" %c", &choice);
            if (choice == 'o') {
                printf("Sur quelle section ? nord/sud/est/ouest/centre\n");
                char sectionChoice[6];
                do {
                    scanf("%s", sectionChoice);
                    if (strcmp(sectionChoice, "nord") == 0) {
                        currentTile.north.meepleId = playerTurn;
                        playerArray[playerTurn].meeples--;
                    }
                    else if (strcmp(sectionChoice, "sud") == 0) {
                        currentTile.south.meepleId = playerTurn;
                        playerArray[playerTurn].meeples--;
                    }
                    else if (strcmp(sectionChoice, "est") == 0) {
                        currentTile.east.meepleId = playerTurn;
                        playerArray[playerTurn].meeples--;
                    }
                    else if (strcmp(sectionChoice, "ouest") == 0) {
                        currentTile.west.meepleId = playerTurn;
                        playerArray[playerTurn].meeples--;
                    }
                    else if (strcmp(sectionChoice, "centre") == 0) {
                        currentTile.center.meepleId = playerTurn;
                        playerArray[playerTurn].meeples--;
                    }
                    else {
                        printf("Veuillez entrer un choix valide.\n");
                    }
                } while (strcmp(sectionChoice, "nord") != 0 && strcmp(sectionChoice,"sud") != 0 && strcmp(sectionChoice, "ouest") != 0 && strcmp(sectionChoice, "est") != 0 && strcmp(sectionChoice, "centre") != 0);
            }
            /* enregistrer les modifications (meeple) dans la grille */
            gameGrid[center][center] = currentTile;
            isFirstTurn = false;
        }
        else {
            if (isPlaceable(tileArray[deckHeight - 1], gameGrid, tilePositions, tilesSet) == true) {
                printf("La tuile piochee est placable !\n");
                currentTile = draw(&tileArray, &deckHeight);
                //printTiles(&currentTile, 1);
                printTileContent(currentTile);
                int possibilitiesCount = 0;
                point* possibilities = returnPossibilities(currentTile, gameGrid, gridSize, tilePositions, tilesSet, &possibilitiesCount);
                point whereToPlace;
                printBoardWithPossibilities(gameGrid, gridSize, tilePositions, tilesSet, possibilities, possibilitiesCount);
                printPossibilities(possibilities, possibilitiesCount);
                printTile(currentTile);
                bool doWantRotate = false;
                do {
                    printf("Voulez-vous faire pivoter la tuile ? o/n\n");
                    char rotateChoice;
                    scanf(" %c", &rotateChoice);
                    if (rotateChoice == 'o') {
                        doWantRotate = true;
                        rotateRight(&currentTile);
                        free(possibilities);
                        possibilities = returnPossibilities(currentTile, gameGrid, gridSize, tilePositions, tilesSet, &possibilitiesCount);
                        printBoardWithPossibilities(gameGrid, gridSize, tilePositions, tilesSet, possibilities, possibilitiesCount);
                        printPossibilities(possibilities, possibilitiesCount);
                        printTile(currentTile);
                    }
                    else if (rotateChoice == 'n') {
                        doWantRotate = false;
                    }
                    else {
                        printf("Veuillez entrer un choix valide.\n");
                    }
                } while (doWantRotate);

                do {
                    printf("Entrez la position x : ");
                    scanf("%d", &whereToPlace.x);
                    printf("Entrez la position y : ");
                    scanf("%d", &whereToPlace.y);
                    if (isPositionInArray(whereToPlace, possibilities, possibilitiesCount) == false) {
                        printf("Veuillez entrer une position valide.\n");
                    }
                } while (isPositionInArray(whereToPlace, possibilities, possibilitiesCount) == false);
                gameGrid[whereToPlace.x][whereToPlace.y] = currentTile;
                addTilePosition(tilePositions, whereToPlace.x, whereToPlace.y, tilesSet);
                tilesSet++;
                if (playerArray[playerTurn].meeples > 0) {
                    printf("Joueur %d, veux-tu placer un meeple sur ta tuile ? o/n\n", playerTurn + 1);
                    char choice;
                    scanf(" %c", &choice);
                    if (choice == 'o') {
                        printf("Sur quelle section ? nord/sud/est/ouest/centre\n");
                        char sectionChoice[6];
                        do {
                            scanf("%s", sectionChoice);
                            if (strcmp(sectionChoice, "nord") == 0) {
                                currentTile.north.meepleId = playerTurn;
                                playerArray[playerTurn].meeples--;
                            }
                            else if (strcmp(sectionChoice, "sud") == 0) {
                                currentTile.south.meepleId = playerTurn;
                                playerArray[playerTurn].meeples--;
                            }
                            else if (strcmp(sectionChoice, "est") == 0) {
                                currentTile.east.meepleId = playerTurn;
                                playerArray[playerTurn].meeples--;
                            }
                            else if (strcmp(sectionChoice, "ouest") == 0) {
                                currentTile.west.meepleId = playerTurn;
                                playerArray[playerTurn].meeples--;
                            }
                            else if (strcmp(sectionChoice, "centre") == 0) {
                                currentTile.center.meepleId = playerTurn;
                                playerArray[playerTurn].meeples--;
                            }
                            else {
                                printf("Veuillez entrer un choix valide.\n");
                            }
                        } while (strcmp(sectionChoice, "nord") != 0 && strcmp(sectionChoice,"sud") != 0 && strcmp(sectionChoice, "ouest") != 0 && strcmp(sectionChoice, "est") != 0 && strcmp(sectionChoice, "centre") != 0);
                        /* enregistrer les modifications (meeple) dans la grille */
                        gameGrid[whereToPlace.x][whereToPlace.y] = currentTile;
                        printf("Tuile placee !\n");
                    }
                }
                updateScoresAfterPlacement(gameGrid, gridSize, tilePositions, tilesSet, playerArray, nbPlayers, whereToPlace.x, whereToPlace.y);
                deckShifts = 0;
                free(possibilities);
            }
            else { // LA TUILE PIOCHÉE N'EST PAS PLACABLE
                printf("Tuile piochee non placable, on en pioche une autre.\n");
                shiftLeft(tileArray, deckHeight);
                deckShifts++;
            }
        }

        playerTurn = (playerTurn + 1) % nbPlayers;
    }

    // LE JEU EST TERMINÉ
    if (deckShifts == deckHeight) {
        printf("Fin du jeu : plus aucune tuile n'est posable !\n");
    }
    else if (deckHeight == 0) {
        printf("Fin du jeu : toutes les tuiles ont ete posees !\n");
    }

    //ANNONCE DU SCORE FINAL
    printf("Scores finaux :\n");
    for (int i = 0 ; i < nbPlayers ; i++) {
        printf("Joueur %d : %d points\n", playerArray[i].id + 1, playerArray[i].score);
    }

    //LIBERATION DE LA MEMOIRE ALLOUEE.
    free(playerArray);
    free(tileArray);
    free(tilePositions);
    return 0;
}