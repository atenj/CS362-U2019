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

int main(){

    int seed = 1000;
    int numPlayers = 2;
    int choice1 = 0, choice2 = 0, currentPlayer = 0, handPos = 0;

    struct gameState state, testState;

    int k[10] = {adventurer, gardens, mine, remodel, smithy, village, baron, steward, ambassador, salvager};
    int testHand1[5] = {copper, copper, ambassador, mine, silver}; 
    int testHand2[5] = {ambassador, silver, gold, ambassador, silver}; 
 
    printf("\n");
    printf("=========================================\n");   
    printf("Unit Test 3: Ambassador - int callAmbassador()\n");
    printf("=========================================\n");
    printf("\n");

    //Case #1: Choice2 is not valid
    initializeGame(numPlayers, k, seed, &state);
    memcpy(state.hand[0], testHand1, sizeof(testHand1));
    memcpy(&testState, &state, sizeof(struct gameState));
#if (NOISY_TEST == 1)
    printf("Case #1: Choice2 is Out of Bounds\n");
    printf("Calling callAmbassador(choice1, choice2, &testState, handPos, currentPlayer)\n");
#endif
    choice1 = 0; //reveal copper at position 0
    choice2 = 5; //this is incorrect on purpose, can't be >2
    handPos = 2; //position of Ambassador card
    printf("Test 1: ");
    assertTrue(callAmbassador(currentPlayer, choice1, choice2, &testState, handPos), -1);
#if (NOISY_TEST == 1)    
    printf("Choice2 is out of bounds. Expected outcome: return -1.\n");
#endif
    printf("\n");

   //Case #2: choice1 == handPos
    memset(&state, 23, sizeof(struct gameState)); //clear game state
    initializeGame(numPlayers, k, seed, &state);
    memcpy(state.hand[0], testHand2, sizeof(testHand2));
    memcpy(&testState, &state, sizeof(struct gameState));
#if (NOISY_TEST == 1)
    printf("Case #2: Choice 1 == handPos\n");
    printf("Calling callAmbassador(choice1, choice2, &testState, handPos, currentPlayer)\n");
#endif
    choice1 = 0; //reveal ambassador at position 0
    choice2 = 2; //return 2 copies to supply
    handPos = 0; //position of Ambassador card we want to play
    printf("Test 2: ");
    assertTrue(callAmbassador(currentPlayer, choice1, choice2, &testState, handPos), -1);
#if (NOISY_TEST == 1)
    printf("Choice1 cannot equal handPos. Expected outcome: return -1.\n");
#endif
    printf("\n");

    //Case #3: choice2 = 2 But there aren't 2 copies of card in hand
    memset(&state, 23, sizeof(struct gameState)); //clear game state
    initializeGame(numPlayers, k, seed, &state);
    memcpy(state.hand[0], testHand1, sizeof(testHand1));
    memcpy(&testState, &state, sizeof(struct gameState));
#if (NOISY_TEST == 1)
    printf("Case #3: choice2 = 2 but there aren't 2 copies of card in hand\n");
    printf("Calling callAmbassador(choice1, choice2, &testState, handPos, currentPlayer)\n");
#endif
    choice1 = 4; //reveal silver at position 4
    choice2 = 2; //return 2 copies to supply
    handPos = 2; //position of Ambassador card we want to play
    printf("Test 3: ");
    assertTrue(callAmbassador(currentPlayer, choice1, choice2, &testState, handPos), -1);
#if (NOISY_TEST == 1)
    printf("Do not have 2 copies of card in hand. Expected outcome: return -1.\n");
#endif
//Case #4: Inputs Within Bounds, playing ambassador at 2, revealing copper at 0
    memset(&state, 23, sizeof(struct gameState)); //clear game state
    initializeGame(numPlayers, k, seed, &state);
    memcpy(state.hand[0], testHand1, sizeof(testHand1));
    memcpy(&testState, &state, sizeof(struct gameState));
#if (NOISY_TEST == 1)
    printf("Case #4: Inputs Within Bounds, playing ambassador at 2, revealing copper at 0\n");
    printf("Calling callAmbassador(choice1, choice2, &testState, handPos, currentPlayer)\n");
#endif
    choice1 = 0; //reveal copper at position 0
    choice2 = 2; //return 2 copies to supply
    handPos = 2; //position of Ambassador card we want to play
    callAmbassador(currentPlayer, choice1, choice2, &testState, handPos);
    printf("Test 4: ");
    //supply count of copper increases by 2
    assertTrue(testState.supplyCount[copper], state.supplyCount[copper] + 2);
#if (NOISY_TEST == 1)
    printf("copper supply test: %d. copper supply original: %d\n", testState.supplyCount[copper], state.supplyCount[copper]);
#endif
    printf("Test 5: ");
    //player 1's deck count is +1
    assertTrue(testState.deckCount[1], state.deckCount[1] + 1);
#if (NOISY_TEST == 1)
    printf("test deck count: %d, original deck count: %d\n", testState.deckCount[currentPlayer], state.deckCount[currentPlayer]);
#endif
    printf("Test 6: ");
    //player 0's hand count should be 2 (played 1 card, returned 2)
    assertTrue(testState.handCount[currentPlayer], state.handCount[currentPlayer] - 3);
#if (NOISY_TEST == 1)
    printf("test hand count: %d, original hand count: %d\n", testState.handCount[currentPlayer], state.handCount[currentPlayer]);
#endif
    printf("\n");
}