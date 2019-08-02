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
    int choice1 = 0, currPlayer = 0;
    int fourCoins = 4;

    struct gameState state, testState;

    int k[10] = {adventurer, gardens, mine, remodel, smithy, village, baron, steward, ambassador, salvager};
    int testHand1[5] = {copper, estate, gold, mine, silver}; // hand with an estate
    int testHand2[5] = {adventurer, silver, gold, mine, silver}; // hand without an estate
    
    printf("\n");
    printf("=========================================\n");
    printf("Unit Test 1: Baron - int callBaron()\n");
    printf("=========================================\n");
    printf("\n");

    //Case #1: Estate Card at Index 1, choice1 = 1
    initializeGame(numPlayers, k, seed, &state);
    memcpy(state.hand[0], testHand1, sizeof(testHand1));
    memcpy(&testState, &state, sizeof(struct gameState));
#if (NOISY_TEST == 1)
    printf("Case #1: Hand with Estate Card at Index 1\n");
    printHand(currPlayer, &testState);
    printf("Calling callBaron(1, &testState, 0)\n");
#endif
    choice1 = 1;
    callBaron(choice1, currPlayer, &testState);
#if (NOISY_TEST == 1)
    printHand(currPlayer, &testState);
    printf("\n");
#endif
    //See if coins increased by 4
    printf("Test 1: ");
    assertTrue(testState.coins, state.coins + fourCoins);
#if (NOISY_TEST == 1)
    printf("Number of coins after test. Expected: %d, Actual: %d\n", state.coins + fourCoins, testState.coins);
#endif
    //Card at index 1 in testState should be equal to card at index 2 in state
    printf("Test 2: ");
    assertTrue(testState.hand[currPlayer][choice1], state.hand[currPlayer][choice1 + 1]);
#if (NOISY_TEST == 1)  
    printf("Card at Index 1. Expected: %d, Actual: %d\n", state.hand[currPlayer][choice1 + 1], testState.hand[currPlayer][choice1]);
#endif
    printf("Test 3: ");
    assertTrue(testState.numBuys, state.numBuys + 1);
#if (NOISY_TEST == 1)  
    printf("Number of buys after. Expected: %d, Actual: %d\n", state.numBuys + 1, testState.numBuys);
#endif
    printf("Test 4: ");
    assertTrue(testState.handCount[currPlayer], state.handCount[currPlayer] - 1);
#if (NOISY_TEST == 1)  
    printf("Hand Count after. Expected: %d, Actual: %d\n", state.handCount[currPlayer]-1, testState.handCount[currPlayer]);
#endif

    // Case #2: No Estate Cards, Flag is 0
    memset(&state, 23, sizeof(struct gameState)); //clear game state
    initializeGame(numPlayers, k, seed, &state); 
    memcpy(state.hand[0], testHand2, sizeof(testHand2));
    memcpy(&testState, &state, sizeof(struct gameState));
#if (NOISY_TEST == 1)
    printf("\n");
    printf("Case #2: Hand Without Any Estate Cards, choice1 = 0\n");
    printHand(0, &state);
    printf("Calling callBaron(0, &testState, 0)\n");
#endif
    choice1 = 0;
    callBaron(choice1, currPlayer, &testState);
#if (NOISY_TEST == 1)
    printHand(currPlayer, &testState);
    printf("\n");
#endif
    //Estate card supply should be decremented by 1
    printf("Test 5: ");
    assertTrue(state.supplyCount[estate] - 1, testState.supplyCount[estate]);
#if (NOISY_TEST == 1)  
    printf("supplyCount of estates after. Expected: %d, Actual: %d\n", state.supplyCount[estate] - 1, testState.supplyCount[estate]);
#endif
    //The hand should be the same in state and testState
    printf("Test 6: Comparing Hands - no output = passed \n");
    for (int m = 0; m < testState.handCount[currPlayer]; m++) 
    {
        if (testState.hand[currPlayer][m] != state.hand[currPlayer][m]){
            printf("Hand has changed at position %d. Failed Test 6.\n", testState.hand[currPlayer][m]);
        }
    }

    // Case #3: No Estate Cards, but choice1 Flag is 1
    memset(&state, 23, sizeof(struct gameState)); //clear game state
    initializeGame(numPlayers, k, seed, &state); 
    memcpy(state.hand[0], testHand2, sizeof(testHand2));
    memcpy(&testState, &state, sizeof(struct gameState));
#if (NOISY_TEST == 1)
    printf("\n");
    printf("Case #3: Hand Without Any Estate Cards, choice1 = 1\n");
    printHand(currPlayer, &state);
    printf("Calling callBaron(1, &testState, 0)\n");
#endif
    choice1 = 1;
    callBaron(choice1, currPlayer, &testState);
#if (NOISY_TEST == 1)
    printHand(currPlayer, &testState);
    printf("\n");
#endif
    //Estate card supply should be decremented by 1
    printf("Test 7: ");
    assertTrue(state.supplyCount[estate] - 1, testState.supplyCount[estate]);
#if (NOISY_TEST == 1)  
    printf("supplyCount of estates after. Expected: %d, Actual: %d\n", state.supplyCount[estate] - 1, testState.supplyCount[estate]);
#endif
    //The hand should be the same in state and testState
    printf("Test 8: Comparing Hands - no output = passed \n");
    for (int m = 0; m < testState.handCount[currPlayer]; m++) 
    {
        if (testState.hand[currPlayer][m] != state.hand[currPlayer][m]){
            printf("Hand has changed at position %d. Failed Test 8.\n", testState.hand[currPlayer][m]);
        }
    }

    // Case #4: No Estate Cards, choice1 = 0, supply count of estates = 0
    memset(&state, 23, sizeof(struct gameState)); //clear game state
    initializeGame(numPlayers, k, seed, &state); 
    state.supplyCount[estate] = 0;
    memcpy(state.hand[0], testHand2, sizeof(testHand2));
    memcpy(&testState, &state, sizeof(struct gameState));
#if (NOISY_TEST == 1)
    printf("\n");
    printf("Case #4: Hand Without Any Estate Cards, choice1 = 0\n");
    printHand(currPlayer, &state);
    printf("Supply Count of estates is: %d\n", supplyCount(estate, &testState));
    printf("Calling callBaron(0, &testState, 0)\n");
#endif
    choice1 = 0;
    callBaron(choice1, currPlayer, &testState);
#if (NOISY_TEST == 1)
    printHand(currPlayer, &testState);
    printf("\n");
#endif
    int errorFlag = 0;
    for (int n = 0; n < testState.handCount[currPlayer]; n++) 
    {
        if (testState.hand[currPlayer][n] != state.hand[currPlayer][n]){
            printf("Hand has changed at position %d. Failed Test 9.\n", testState.hand[currPlayer][n]);
            errorFlag = 1;
        }
    }
    if (errorFlag == 1) {
        printf("Failed\n");
    }
    else {
        printf("Passed\n");

    }
    printf("\n");
    return 0;
}