#include "dominion.h"
#include "dominion_helpers.h"
#include "interface.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 1

int assertTrue (int x, int y) {
    if (x == y) {
        printf("Passed\n");
        return 1;
    }
    else if (x != y) {
        printf("Failed\n");
        return 0;
    }
    else {
        printf("Error\n");
        return 1;
    }
}

int main() {

    // Parameters for Testing
    int i = 0, j = 0, changeCount = 0;
    int numPlayers = 3;
    int randomSeed = 1000;
    struct gameState state, testState;

    int kingdomCards1[10] = {adventurer, gardens, mine, remodel, smithy, village, baron, steward, ambassador, salvager};

    int copperCount = 0;
    int estateCount = 0;

    printf("\n");  
    printf("=======================================================================\n");
    printf("Card Test 2: int shuffle() - Turn Noisy Test on to Compare Decks\n");
    printf("=======================================================================\n");
    printf("\n");
    
    // Case #1: 3 Player Game Initialized, Turn Noisy Test on to Compare Decks
    printf("Test Case #1\n");
    printf("=======================================================================\n");
    printf("\n");

    // initialize the game
    initializeGame(numPlayers, kingdomCards1, randomSeed, &state);
    // copy game state to testState
    memcpy(&testState, &state, sizeof(struct gameState));

#if (NOISY_TEST == 1)
    printf("Before shuffle: ");
    printDeck(1, &testState);
#endif

    //call shuffle on player 1's deck
    shuffle(1, &testState);

#if (NOISY_TEST == 1)
    printf("After shuffle: ");
    printDeck(1, &testState);
#endif

    //verify that the number of cards in player 1's deck has not changed
    printf("Test 1: ");
    assertTrue(state.deckCount[1], testState.deckCount[1]);
    printf("Deck Count before shuffle: %d, after shuffle: %d\n", state.deckCount[1], testState.deckCount[1]);

    //verify that the count for estates and coppers has not changed

    for (i = 0; i < state.deckCount[1]; i++) {
        if (state.deck[1][i] == copper) {
            copperCount++;
        }
        if (state.deck[1][i] == estate) {
            estateCount++;
        }
    }

    //There should be 7 coppers in deck after shuffling
    printf("Test 2: ");
    assertTrue(7, copperCount);
    printf("The copper count should remain unchanged.\n");
    //There should be 3 estate in deck after shuffling
    printf("Test 3: ");
    assertTrue(3, estateCount);
    printf("The estate count should remain unchanged.\n");

    //See how many card positions have changed.  
    for (j = 0; j < testState.deckCount[1]; j++) {
        if (state.deck[1][j] != testState.deck[1][j]) {
            changeCount++;
        }
    }
    printf("Test 4: Number of card positions with change in value = %d\n", changeCount);

}