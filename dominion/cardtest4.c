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
    int players[MAX_PLAYERS];

    int testHand1[5] = {province, province, province, province, province};

    int kingdomCards1[10] = {adventurer, gardens, mine, remodel, smithy, village, baron, steward, ambassador, salvager};

    printf("\n");  
    printf("=======================================================================\n");
    printf("Card Test 4: int getWinners()\n");
    printf("=======================================================================\n");
    printf("\n");
    
    // Case #1: 2 Player Game Initialized - Player 0's Score is Higher
    printf("Test Case #1\n");
    printf("=======================================================================\n");
    printf("\n");

    // initialize the game
    initializeGame(numPlayers, kingdomCards1, randomSeed, &state);
    // Make changes to testState to force winner
    // Scores should be equal because game was just initialized
    // Change Player 0's hand so their score is higher
    memcpy(state.hand[0], testHand1, sizeof(testHand1));
    // copy game state to testState
    memcpy(&testState, &state, sizeof(struct gameState));

    //call getWinners on testState
    printf("Test 1: ");
    assertTrue(getWinners(players, &testState), 0);
    printf("Winner - Expected: 0, Actual: %d\n", getWinners(players, &testState));

    // Case #2: 2 Player Game Initialized - Scores are Tied But Player 0 has had 1 more turn
    printf("Test Case #2: Tied but Player 0 had 1 More Turn\n");
    printf("=======================================================================\n");
    printf("\n");

    memset(&state, 23, sizeof(struct gameState)); //clear game state
    // initialize the game
    initializeGame(numPlayers, kingdomCards1, randomSeed, &state);
    // Scores should be equal because game was just initialized
    // copy game state to testState
    memcpy(&testState, &state, sizeof(struct gameState));
    //endTurn is called so that it is player 1's Turn
    endTurn(&testState);
    //call getWinners on testState
    printf("Test 2: ");
    assertTrue(getWinners(players, &testState), 1);
    printf("Winner - Expected: 1, Actual: %d\n", getWinners(players, &testState));
    
}