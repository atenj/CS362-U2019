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

    int seed = 1000;
    int numPlayers = 2;
    int choice1= 0, choice2 = 0, handPos = 0, currentPlayer = 0;

    struct gameState state, testState;

    int k[10] = {adventurer, gardens, mine, remodel, smithy, village, baron, steward, ambassador, salvager};
    int testHand1[5] = {copper, silver, gold, mine, silver};
    int testHand2[5] = {province, silver, gold, mine, silver};
    int testHand3[5] = {copper, gardens, gold, mine, smithy};
    int testHand4[5] = {mine, gardens, gold, baron, copper};
  
    printf("\n");  
    printf("=========================================\n");
    printf("Unit Test 5: Mine - int callMine()\n");
    printf("=========================================\n");
    printf("\n");
    
    //Case #1: Trade a copper for a silver 
    initializeGame(numPlayers, k, seed, &state);
    memcpy(state.hand[0], testHand1, sizeof(testHand1));
    memcpy(&testState, &state, sizeof(struct gameState));
#if (NOISY_TEST == 1)
    printf("Case #1: Copper to Silver at Card 0\n");
    printHand(0, &state);
    printf("Calling callMine(0, 5, state, 3, 0)\n");
#endif
    choice1 = 0;
    choice2 = 5;
    handPos = 3;
    currentPlayer = 0;
    callMine(choice1, currentPlayer, choice2, &testState, handPos);
#if (NOISY_TEST == 1)
    printHand(0, &testState);
    printf("\n");
#endif
    //Card at hand position 0 should equal the choice2 parameter
    printf("Test 1: ");
    assertTrue(testState.hand[currentPlayer][choice1], choice2);
    //Card at hand position should be discarded and replaced with next card
    printf("Test 2: ");
    assertTrue(testState.hand[currentPlayer][handPos], state.hand[currentPlayer][handPos+1]);
    printf("Test 3: ");
    //Hand count of player 0 should be decremented by 1
    assertTrue(testState.handCount[currentPlayer], state.handCount[currentPlayer] - 1);
    printf("\n");

    //Case #2
    memset(&state, 23, sizeof(struct gameState)); //clear game state
    initializeGame(numPlayers, k, seed, &state); 
    memcpy(state.hand[0], testHand2, sizeof(testHand2));
    memcpy(&testState, &state, sizeof(struct gameState));
#if (NOISY_TEST == 1)
    printf("Case #2: Choice 1 is Not a Treasure\n");
    printHand(0, &testState);
    printf("Calling callMine(0, 5, state, 3, 0)\n");
#endif
    choice1 = 0;
    choice2 = 5;
    handPos = 3;
    currentPlayer = 0;
    callMine(choice1, currentPlayer, choice2, &testState, handPos);
#if (NOISY_TEST == 1)
    printHand(0, &testState);
    printf("\n");
#endif
    //Card at position 0 should not change
    printf("Test 4: ");
    assertTrue(state.hand[currentPlayer][choice1], testState.hand[currentPlayer][choice1]);
    //card at hand position should not change
    printf("Test 5: ");
    assertTrue(state.hand[currentPlayer][handPos], testState.hand[currentPlayer][handPos]);
    //hand count should be the same
    printf("Test 6: ");
    assertTrue(state.handCount[currentPlayer], testState.handCount[currentPlayer]);
    printf("\n");

    //Case #3
    memset(&state, 23, sizeof(struct gameState)); //clear game state
    initializeGame(numPlayers, k, seed, &state); 
    memcpy(state.hand[0], testHand3, sizeof(testHand3));
    memcpy(&testState, &state, sizeof(struct gameState));
#if (NOISY_TEST == 1)
    printf("Case #3: Choice 2 is Not a Valid Card, with both positive and negative integers\n");
    printHand(0, &state);
    printf("Calling callMine(0, 200, state, 3, 0)\n");
#endif
    choice1 = 0;
    choice2 = 200;
    handPos = 3;
    currentPlayer = 0;
    callMine(choice1, currentPlayer, choice2, &testState, handPos);
#if (NOISY_TEST == 1)
    printHand(0, &state);
    printf("\n");
#endif
    //No changes should be made in cards
    printf("Test 7: ");
    assertTrue(state.hand[currentPlayer][choice1], testState.hand[currentPlayer][choice1]);
    printf("Test 8: ");
    assertTrue(state.hand[currentPlayer][handPos], testState.hand[currentPlayer][handPos]);
    printf("Test 9: ");
    assertTrue(state.handCount[currentPlayer], testState.handCount[currentPlayer]);
    printf("\n");
#if (NOISY_TEXT ==1)
    printf("Calling callMine(0, -1, state, 3, 0)\n");
#endif
    choice1 = 0;
    choice2 = -1;
    handPos = 3;
    currentPlayer = 0;
    callMine(choice1, currentPlayer, choice2, &testState, handPos);
#if (NOISY_TEST == 1)
    printHand(0, &state);
    printf("\n");
#endif
    printf("Test 10: ");
    assertTrue(state.hand[currentPlayer][choice1], testState.hand[currentPlayer][choice1]);
    printf("Test 11: ");
    assertTrue(state.hand[currentPlayer][handPos], testState.hand[currentPlayer][handPos]);
    printf("Test 12: ");
    assertTrue(state.handCount[currentPlayer], testState.handCount[currentPlayer]);
    printf("\n");

    //Case 4:  Mine is in position 0, Copper in position 4, Try to exchange copper for gold
    memset(&state, 23, sizeof(struct gameState)); //clear game state
    initializeGame(numPlayers, k, seed, &state); 
    memcpy(state.hand[0], testHand4, sizeof(testHand4));
    memcpy(&testState, &state, sizeof(struct gameState));
#if (NOISY_TEST == 1)
    printf("Case #4: Choice 1 is Copper, Choice 2 is Gold.\n");
    printHand(0, &state);
    printf("Calling callMine(4, gold, state, 0, 0)\n");
#endif
    choice1 = 4;
    choice2 = 6;
    handPos = 0;
    currentPlayer = 0;
    printf("Test 13: ");
    assertTrue((callMine(choice1, currentPlayer, choice2, &testState, handPos)), -1);
    printf("\n");
    printf("\n");
    return 0;
}