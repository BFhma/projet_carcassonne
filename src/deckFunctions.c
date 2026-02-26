#include "deckFunctions.h"
#include <stdlib.h>

void shuffleDeck(tile* deck, int size) {
    if (size > 1) 
    {
        for (int i = 0; i < size - 1; i++) 
        {
            int j = i + rand() / (RAND_MAX / (size - i) + 1);
            tile temp = deck[j];
            deck[j] = deck[i];
            deck[i] = temp;
        }
    }
}

tile draw(tile* deck, int size) {
    tile res;
    if (size == 1) {
        res = deck[0];
        return res;
    }
    else {
        res = deck[size - 1];
        realloc(deck, (size - 1) * sizeof(*deck));
        return res;
    }
}

void shiftLeft(tile* deck, int size) {
    tile first = deck[0];
    for (int i = 0 ; i < size - 1 ; i++) {
        deck[i] = deck[i + 1];
    }
    deck[size - 1] = first;
}