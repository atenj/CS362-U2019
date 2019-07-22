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
    int numPlayers = 2;
    int randomSeed = 1000;
    struct gameState state, testState;

    int currentPlayer = 0;

    int kingdomCards1[10] = {adventurer, gardens, mine, remodel, smithy, village, baron, steward, ambassador, salvager};

    printf("\n");  
    printf("=======================================================================\n");
    printf("Card Test 3: int endTurn()\n");
    printf("=======================================================================\n");
    printf("\n");
    
    // Case #1: 2 Player Game Initialized
    printf("Test Case #1\n");
    printf("=======================================================================\n");
    printf("\n");

    // initialize the game
    initializeGame(numPlayers, kingdomCards1, randomSeed, &state);
    // copy game state to testState
    memcpy(&testState, &state, sizeof(struct gameState));

    //call endTurn on testState
    endTurn(&testState);

    printf("Test 1: ");
    if (whoseTurn(&state) < numPlayers - 1) {
        assertTrue(whoseTurn(&state) + 1, whoseTurn(&testState));
        printf("whoseTurn. Expected: %d, Actual: %d\n", whoseTurn(&state) + 1, whoseTurn(&testState));
    }
    else {
        assertTrue(whoseTurn(&testState), 0);
        printf("whoseTurn. Expected: %d, Actual %d\n", 0, whoseTurn(&testState));
    }

    //Verifying Reset
    printf("Test 2: ");
    assertTrue(testState.outpostPlayed, 0);
    printf("Test 3: ");
    assertTrue(testState.phase, 0);
    printf("Test 4: ");
    assertTrue(testState.numActions, 1);
    printf("Test 5: ");
    assertTrue(testState.numBuys, 1);
    printf("Test 6: ");
    assertTrue(testState.playedCardCount, 0);
    currentPlayer = whoseTurn(&testState);

    //player's handCount should be 5
    printf("Test 7: ");
    assertTrue(testState.deckCount[currentPlayer], 5);

    endTurn(&state); //end turn in test State to see what happens next
    endTurn(&testState);

    printf("Test 8: ");
    if (whoseTurn(&state) < numPlayers - 1) {
        assertTrue(whoseTurn(&state) + 1, whoseTurn(&testState));
        printf("whoseTurn. Expected: %d, Actual: %d\n", whoseTurn(&state) + 1, whoseTurn(&testState));
    }
    else {
        assertTrue(whoseTurn(&testState), 0);
        printf("whoseTurn. Expected: %d, Actual %d\n", 0, whoseTurn(&testState));
    }

}