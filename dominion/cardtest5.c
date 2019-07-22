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

    int kingdomCards1[10] = {adventurer, gardens, mine, remodel, smithy, village, baron, steward, ambassador, salvager};

    printf("\n");  
    printf("=======================================================================\n");
    printf("Card Test 5: int drawCard()\n");
    printf("=======================================================================\n");
    printf("\n");
    
    // Case #1: Deck and Discard Pile are Both Empty - Return -1
    printf("Test Case #1: Deck and Discard Pile are Both Empty\n");
    printf("=======================================================================\n");
    printf("\n");

    // initialize the game
    initializeGame(numPlayers, kingdomCards1, randomSeed, &state);
    // copy game state to testState
    memcpy(&testState, &state, sizeof(struct gameState));

    //set Player 0's discardCount and deckCount to 0
    testState.deckCount[0] = 0;
    testState.discardCount[0] = 0;

    printf("Test 1: ");
    assertTrue(-1, drawCard(0, &testState));
    printf("Should return -1\n");

    // Case #2: Deck is Empty
    printf("\n");
    printf("Test Case #2: Deck is Empty\n");
    printf("=======================================================================\n");
    printf("\n");

    memset(&state, 23, sizeof(struct gameState)); //clear game state
    // initialize the game
    initializeGame(numPlayers, kingdomCards1, randomSeed, &state);
    // copy game state to testState
    memcpy(&testState, &state, sizeof(struct gameState));

    //Move contents of deck to discard
    for (int i = 0; i < state.deckCount[0]; i++){
        testState.discard[0][i] = testState.deck[0][i];
        testState.deck[0][i] = -1;
        testState.discardCount[0]++;
        testState.deckCount[0]--;
    }
    //check to make sure discard count now has 5 cards
    printf("Check 1: ");
    assertTrue(testState.discardCount[0], 5);

    //call drawCard while deck is empty
    drawCard(0, &testState);
    //discard pile should have been shuffled and moved to draw
    //check discardCount - should be 0
    printf("Test 2: ");
    assertTrue(testState.discardCount[0], 0);
    printf("Discard Count of Player 0 - expected: %d, actual: %d\n", 0, testState.discardCount[0]);
    //new deck count is previous discard count - 1 (1 card has been drawn)
    printf("Test 3: ");
    assertTrue(testState.deckCount[0], 4);
    printf("Deck Count should be previous discard count - 1 card drawn: Expected: 4, Actual: %d\n", testState.deckCount[0]);

    // Case #3: Deck is Full
    printf("\n");
    printf("Test Case #3: Deck is Full\n");
    printf("=======================================================================\n");
    printf("\n");

    memset(&state, 23, sizeof(struct gameState)); //clear game state
    // initialize the game
    initializeGame(numPlayers, kingdomCards1, randomSeed, &state);
    // copy game state to testState
    memcpy(&testState, &state, sizeof(struct gameState));

    // calling drawCard
    drawCard(0, &testState);

    // Verifying Results
    printf("Test 4: ");
    assertTrue(testState.handCount[0], state.handCount[0] + 1);
    printf("Player 0 hand count - expected: %d, actual: %d\n", state.handCount[0] + 1, testState.handCount[0]);
    printf("Test 5: ");
    assertTrue(testState.deckCount[0], state.deckCount[0] - 1);
    printf("Player 0 deckCount - expected: %d, actual: %d\n", state.deckCount[0] - 1, testState.deckCount[0]);
    printf("Test 6: ");
    assertTrue(state.deck[0][state.deckCount[0]-1], testState.hand[0][testState.handCount[0]-1]);
    printf("Player 0's top deck card should become last hand card.");
    printDeck(0, &state);
    printHand(0, &testState);
}