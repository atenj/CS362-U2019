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

    struct gameState state, testState;
    int choice1 = 0, choice2 = 0, handPos = 0, currentPlayer = 0;
    int twoCoins = 2;
    int cardsDrawn = 4;

    int k[10] = {adventurer, gardens, mine, remodel, smithy, minion, baron, steward, ambassador, salvager};
    int testHand1[5] = {copper, estate, gold, minion, silver}; // test hand for case 1, player 0, handPos = 3
    int testHand2[5] = {adventurer, silver, gold, smithy, silver}; // test hand for case 1, player 1
    int testHand3[3] = {silver, gold, smithy};  //test hand for case 3

    printf("\n");
    printf("=========================================\n");
    printf("Unit Test 2: Minion - int cardMinion()\n");
    printf("=========================================\n");
    printf("\n");

    //Case #1: Player 0 Choose choice1 = 1
    choice1 = 1;
    handPos = 3;
    initializeGame(numPlayers, k, seed, &state);
    state.handCount[1] = 5;     //change handCount of player1 to 5 for testing
    memcpy(state.hand[0], testHand1, sizeof(testHand1));
    memcpy(state.hand[1], testHand2, sizeof(testHand2));
    memcpy(&testState, &state, sizeof(struct gameState));
#if (NOISY_TEST == 1)
    printf("Case #1: Player Chooses +2 Coins (choice1 = 1)\n");
    printHand(0, &testState);
    printf("Calling cardMinion(1, 0, &testState, 3, 0)\n");
#endif
    cardMinion(choice1, choice2, &testState, 3, 0);
#if (NOISY_TEST == 1)
    printHand(0, &testState);
    printf("\n");
#endif
    //See if coins increased by 2
    printf("Test 1: ");
    assertTrue(testState.coins, state.coins + twoCoins);
#if (NOISY_TEST == 1)
    printf("Number of coins after test. Expected: %d, Actual: %d\n", state.coins + twoCoins, testState.coins);
#endif
    //See if numActions of Player 0 increased by 1
    printf("Test 2: ");
    assertTrue(testState.numActions, state.numActions + 1);
#if (NOISY_TEST == 1)  
    printf("Number of Actions. Expected: %d, Actual: %d\n", state.numActions + 1, testState.numActions);
#endif
    //See if player 0's hand is correct
    printf("Test 3: ");
    int errorFlag1 = 0;
    int errorFlag2 = 0;
    for (int i = 0; i < handPos; i++) {
        if (state.hand[currentPlayer][i] != testState.hand[currentPlayer][i]) {
            printf("Failed at position %d.\n", i);
            errorFlag1 = 1;
        }
    }
    for (int j = handPos; j < testState.handCount[currentPlayer]; j++) {
        if (testState.hand[currentPlayer][handPos] != state.hand[currentPlayer][handPos + 1]) {
            printf("Failed at position %d.\n", j);
            errorFlag2 = 1;
        }
    }
    if (errorFlag1 == 0 && errorFlag2 == 0) {
        printf("Passed.\n");
    }

    //Testing to look at Player 1's hand
    printf("Test 4: ");
    int errorFlag3 = 0;
    for (int i = 0; i < state.handCount[1]; i++) {
        if (state.hand[1][i] != testState.hand[1][i]) {
            printf("Failed at position %d.\n", i);
            errorFlag3 = 1;
        }
    }
    if (errorFlag3 == 0) {
        printf("Passed.\n");
    }
#if (NOISY_TEST == 1)  
    printf("Tested Player 1's Hand - Should not change.\n");
#endif

    //Case #2: Player 0 chooses choice2, Player 1 has five cards
    choice1 = 0;  //set choice1 to 0
    choice2 = 1;  //player chooses drawCard option
    handPos = 3;  //hand position of Minion in Player 0's hand
    initializeGame(numPlayers, k, seed, &state);
    state.handCount[1] = 5;     //change handCount of player1 to 5 for testing
    memcpy(state.hand[0], testHand1, sizeof(testHand1));
    memcpy(state.hand[1], testHand2, sizeof(testHand2));
    memcpy(&testState, &state, sizeof(struct gameState));
#if (NOISY_TEST == 1)
    printf("Case #2: Player Chooses drawCard Option (choice2 = 1)\n");
    printHand(0, &testState);
    printf("Calling cardMinion(0, 1, &testState, 3, 0)\n");
#endif
    cardMinion(choice1, choice2, &testState, 3, 0);
#if (NOISY_TEST == 1)
    printHand(0, &testState);
    printf("\n");
#endif
    //Make sure coins is the same 
    printf("Test 5: ");
    assertTrue(testState.coins, state.coins);
#if (NOISY_TEST == 1)
    printf("Number of coins after test. Expected: %d, Actual: %d\n", state.coins, testState.coins);
#endif
    //See if numActions of Player 0 increased by 1
    printf("Test 6: ");
    assertTrue(testState.numActions, state.numActions + 1);
#if (NOISY_TEST == 1)  
    printf("Number of Actions. Expected: %d, Actual: %d\n", state.numActions + 1, testState.numActions);
#endif
    //Compare hand count before and after.  Expect it to be 1 less because we drew 4 cards
    printf("Test 7: ");
    assertTrue(testState.handCount[0], state.handCount[0] - 1);
#if (NOISY_TEST == 1)  
    printf("Number of cards in Player 0's hand after test.  Expected: %d, Actual: %d\n", state.handCount[0] - 1, testState.handCount[0]);
#endif
    //Compare deck count for Player 0 before and after.  Should be 4 less because drew 4 cards
    printf("Test 8: ");
    assertTrue(state.deckCount[0] - cardsDrawn, testState.deckCount[0]);
#if (NOISY_TEST == 1)  
    printf("Number of cards in Player 0's deck after test. Expected: %d, Actual %d\n", state.deckCount[0] - cardsDrawn, testState.deckCount[0]);
#endif   
    //Testing to look at Player 1's hand
    printf("Test 9: ");
    assertTrue(testState.handCount[1], state.handCount[1] - 1);
#if (NOISY_TEST == 1)  
    printf("Number of cards in Player 1's hand after test.  Expected: %d, Actual: %d\n", state.handCount[1] - 1, testState.handCount[1]);
#endif
    //Compare deck count for Player 1 before and after.  Should be 4 less because drew 4 cards
    printf("Test 10: ");
    assertTrue(state.deckCount[1] - cardsDrawn, testState.deckCount[1]);
#if (NOISY_TEST == 1)  
    printf("Number of cards in Player 1's deck after test. Expected: %d, Actual %d\n", state.deckCount[1] - cardsDrawn, testState.deckCount[1]);
 #endif   

    //Case #3: Player 0 chooses choice2, Player 1 has three cards
    choice1 = 0;  //set choice1 to 0
    choice2 = 1;  //player chooses drawCard option
    handPos = 3;  //hand position of Minion in Player 0's hand
    initializeGame(numPlayers, k, seed, &state);
    state.handCount[1] = 3;     //change handCount of player1 to 5 for testing
    memcpy(state.hand[0], testHand1, sizeof(testHand1));
    memcpy(state.hand[1], testHand3, sizeof(testHand3));
    memcpy(&testState, &state, sizeof(struct gameState));
#if (NOISY_TEST == 1)
    printf("Case #3: Player Chooses drawCard Option, Player 1 only has 3 cards in hand (choice2 = 1)\n");
    printHand(0, &testState);
    printf("Calling cardMinion(0, 1, &testState, 3, 0)\n");
#endif
    cardMinion(choice1, choice2, &testState, 3, 0);
#if (NOISY_TEST == 1)
    printHand(0, &testState);
    printf("\n");
#endif
    //Compare hand count of Player 0 before and after.  Expect it to be 1 less because we drew 4 cards
    printf("Test 11: ");
    assertTrue(testState.handCount[0], state.handCount[0] - 1);
#if (NOISY_TEST == 1)  
    printf("Number of cards in Player 0's hand after test.  Expected: %d, Actual: %d\n", state.handCount[0] - 1, testState.handCount[0]);
#endif
    //Compare deck count for Player 0 before and after.  Should be 4 less because drew 4 cards
    printf("Test 12: ");
    assertTrue(state.deckCount[0] - cardsDrawn, testState.deckCount[0]);
#if (NOISY_TEST == 1)  
    printf("Number of cards in Player 0's deck after test. Expected: %d, Actual %d\n", state.deckCount[0] - cardsDrawn, testState.deckCount[0]);
#endif   
    //Player 1 only had 3 cards, should not change
    printf("Test 13: ");
    assertTrue(state.handCount[1], testState.handCount[1]);
#if (NOISY_TEST == 1)  
    printf("Number of cards in Player 1's deck after test. Expected: %d, Actual %d\n", state.handCount[1], testState.handCount[1]);
#endif  
    printf("Test 14: ");
    int errorFlag4 = 0;
    for (int m = 0; m < state.deckCount[1]; m++) {
        if (state.hand[1][m] != testState.hand[1][m]) {
            printf("Failed at position %d.\n", m);
            errorFlag4 = 1;
        }
    }
    if (errorFlag4 == 0) {
        printf("Passed\n");
    }
    printf("\n");
}