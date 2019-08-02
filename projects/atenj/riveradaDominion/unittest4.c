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
    int nextPlayer = 1;
    int currentPlayer = 0;
    int tributeRevealedCards[2] = {-1, -1};
    struct gameState state, testState;

    int k[10] = {adventurer, gardens, mine, tribute, smithy, village, baron, steward, ambassador, salvager};
    int testHand1[5] = {copper, copper, tribute, mine, silver}; 
    int testDeck1[4] = {gardens, copper, copper, estate}; 
    int testDeck2[1] = {silver};   
    int testDeck3[2] = {estate, estate}; 
    int testDiscard1[1] = {baron};

    printf("\n");
    printf("=========================================\n");
    printf("Unit Test 4: Tribute - int callTribute()\n");
    printf("=========================================\n");
    printf("\n");

    //Case #1
    initializeGame(numPlayers, k, seed, &state);
    memcpy(state.hand[currentPlayer], testHand1, sizeof(testHand1));
    state.deckCount[nextPlayer] = 4; //set Player 1's deck count to size of test deck
    memcpy(state.deck[nextPlayer], testDeck1, sizeof(testDeck1)); //copy test deck to Player 1
    memcpy(&testState, &state, sizeof(struct gameState));
#if (NOISY_TEST == 1)
    printf("\n");
    printf("Case #1: nextPlayer Reveals copper and estate from deck\n");
    printf("Calling callTribute(&testState, nextPlayer, currentPlayer, tributeRevealedCards)\n");
#endif
    callTribute(currentPlayer, nextPlayer, &testState);
    //we expect tributeRevealedCards[0] == estate and tributeRevealedCards[1] == copper
    printf("Test 1: ");
    assertTrue(tributeRevealedCards[0], state.deck[nextPlayer][state.deckCount[nextPlayer] - 1]);
#if (NOISY_TEST == 1)
    printf("Check tributeRevealedCards[0]. Expected: %d, Actual: %d\n", state.deck[nextPlayer][state.deckCount[nextPlayer] - 1], tributeRevealedCards[0]);
#endif
    printf("Test 2: ");
    assertTrue(tributeRevealedCards[1], state.deck[nextPlayer][state.deckCount[nextPlayer] - 2]);
#if (NOISY_TEST == 1)
    printf("Check tributeRevealedCards[1]. Expected: %d, Actual: %d\n", state.deck[nextPlayer][state.deckCount[nextPlayer] - 2], tributeRevealedCards[1]);
#endif
    printf("Test 3: ");
    assertTrue(testState.deckCount[nextPlayer], state.deckCount[nextPlayer] - 2);
#if (NOISY_TEST == 1)
    printf("Player 1's deck count before: %d, after: %d\n",state.deckCount[1], testState.deckCount[1]);
#endif
    //we expect player 0's numCoins to increase by 2
    printf("Test 4: ");
    assertTrue(testState.coins, state.coins + 2);
#if (NOISY_TEST == 1)
    printf("Player 0's numCoins before: %d, after: %d\n", state.coins, testState.coins);
#endif
    //we expect player 0 deckCount to stay the same
    printf("Test 5: ");
    assertTrue(testState.deckCount[currentPlayer], state.deckCount[currentPlayer]);
#if (NOISY_TEST == 1)
    printf("Player 0's deckCount before: %d, after %d\n", state.deckCount[currentPlayer], testState.deckCount[currentPlayer]);
#endif
    //numActions should not change
    printf("Test 6: ");
    assertTrue(testState.numActions, state.numActions);
#if (NOISY_TEST == 1)
    printf("Player 0's numActions before: %d, after %d\n", state.numActions, testState.numActions);
#endif

    //Case #2:  nextPlayer only has 1 card to reveal (in discard pile), and it is an Action card
    memset(&state, 23, sizeof(struct gameState)); //clear game state
    initializeGame(numPlayers, k, seed, &state); 
    state.discardCount[nextPlayer] = 1; //1 card in discard pile
    state.deckCount[nextPlayer] = 0; //0 cards in deck
    memcpy(state.discard[nextPlayer], testDiscard1, sizeof(testDiscard1)); //copy discard pile over
    memcpy(state.hand[currentPlayer], testHand1, sizeof(testHand1));
    memcpy(&testState, &state, sizeof(struct gameState));
#if (NOISY_TEST == 1)
    printf("\n");
    printf("Case #2: nextPlayer has 1 action card in discard, 0 in deck\n");
    printf("Calling callTribute(&testState, nextPlayer, currentPlayer, tributeRevealedCards)\n");
#endif
    tributeRevealedCards[0] = -1; //reset revealed cards values
    tributeRevealedCards[1] = -1; //reset revealed cards values
    callTribute(currentPlayer, nextPlayer, &testState);
    printf("Test 7: ");
    assertTrue(tributeRevealedCards[0], state.discard[nextPlayer][state.discardCount[nextPlayer] - 1]);
#if (NOISY_TEST == 1)
    printf("Card in next player's discard pile is the tributeRevealedCards[0]. Expected: %d, Actual: %d\n",
        state.discard[nextPlayer][state.discardCount[nextPlayer]-1], tributeRevealedCards[0]);
#endif
    //numActions should increase by 2
    printf("Test 8: ");
    assertTrue(state.numActions + 2, testState.numActions);
#if (NOISY_TEST == 1)
    printf("numActions should increase by 2.  Expected: %d, Actual: %d\n", state.numActions + 2, testState.numActions);
#endif

    //Case #3:  Player 1 has 0 cards to reveal - no changes
    memset(&state, 23, sizeof(struct gameState)); //clear game state
    initializeGame(numPlayers, k, seed, &state); 
    state.discardCount[nextPlayer] = 0; //0 card in discard pile
    state.deckCount[nextPlayer] = 0; //0 cards in deck
    memcpy(state.hand[currentPlayer], testHand1, sizeof(testHand1));
    memcpy(&testState, &state, sizeof(struct gameState));
#if (NOISY_TEST == 1)
    printf("\n");
    printf("Case #3: nextPlayer has no cards to reveal\n");
    printf("Calling callTribute(&testState, nextPlayer, currentPlayer, tributeRevealedCards)\n");
#endif
    tributeRevealedCards[0] = -1; //reset revealed cards values
    tributeRevealedCards[1] = -1; //reset revealed cards values
    //expect no changes
    callTribute(currentPlayer, nextPlayer, &testState);
    printf("Test 9: ");
    assertTrue(state.deckCount[currentPlayer], testState.deckCount[currentPlayer]);
#if (NOISY_TEST == 1)
    printf("Deck Count of current player. Expected: %d, Actual: %d\n", state.deckCount[currentPlayer], testState.deckCount[currentPlayer]); 
#endif
    printf("Test 10: ");
    assertTrue(state.numActions, testState.numActions);
#if (NOISY_TEST == 1)
    printf("Number of Actions. Expected: %d, Actual: %d\n", state.numActions, testState.numActions); 
#endif
    printf("Test 11: ");
    assertTrue(state.coins, testState.coins);
#if (NOISY_TEST == 1)
    printf("Number of coins. Expected: %d, Actual: %d\n", state.coins, testState.coins); 
#endif
    printf("Test 12: ");
    assertTrue(state.deckCount[nextPlayer], testState.deckCount[nextPlayer]);
#if (NOISY_TEST == 1)
    printf("Deck Count of next player. Expected: %d, Actual: %d\n", state.deckCount[nextPlayer], testState.deckCount[nextPlayer]); 
#endif
    printf("Test 13: ");
    assertTrue(state.discardCount[nextPlayer], testState.discardCount[nextPlayer]);
#if (NOISY_TEST == 1)
    printf("Discard Count of next player. Expected: %d, Actual: %d\n", state.discardCount[nextPlayer], testState.discardCount[nextPlayer]); 
#endif

    //Case #4:  nextPlayer only has 1 card to reveal (in deck), and it is an treasure card
    memset(&state, 23, sizeof(struct gameState)); //clear game state
    initializeGame(numPlayers, k, seed, &state); 
    state.discardCount[nextPlayer] = 0; //0 card in discard pile
    state.deckCount[nextPlayer] = 1; //1 cards in deck
    memcpy(state.deck[nextPlayer], testDeck2, sizeof(testDeck2)); //copy deck over
    memcpy(state.hand[currentPlayer], testHand1, sizeof(testHand1));
    memcpy(&testState, &state, sizeof(struct gameState));
    #if (NOISY_TEST == 1)
    printf("\n");
    printf("Case #4: nextPlayer has 1 treasure card in deck, 0 in discard\n");
    printf("Calling callTribute(&testState, nextPlayer, currentPlayer, tributeRevealedCards)\n");
#endif
    tributeRevealedCards[0] = -1; //reset revealed cards values
    tributeRevealedCards[1] = -1; //reset revealed cards values
    callTribute(currentPlayer, nextPlayer, &testState);
    printf("Test 14: ");
    assertTrue(state.coins + 2, testState.coins);
#if (NOISY_TEST == 1)
    printf("Number of coins. Expected: %d, Actual: %d\n", state.coins, testState.coins); 
#endif

    //Case #5:  nextPlayer's cards to reveal are duplicates
    memset(&state, 23, sizeof(struct gameState)); //clear game state
    initializeGame(numPlayers, k, seed, &state); 
    state.deckCount[nextPlayer] = 2; //0 cards in deck
    memcpy(state.deck[nextPlayer], testDeck3, sizeof(testDeck3)); //copy test deck 3 to Player 1
    memcpy(state.hand[currentPlayer], testHand1, sizeof(testHand1));
    memcpy(&testState, &state, sizeof(struct gameState));
#if (NOISY_TEST == 1)
    printf("\n");
    printf("Case #5: nextPlayer's cards to reveal are duplicates, estates\n");
    printf("Calling callTribute(&testState, nextPlayer, currentPlayer, tributeRevealedCards)\n");
#endif
    callTribute(currentPlayer, nextPlayer, &testState);
    printf("Test 15: ");
    assertTrue(tributeRevealedCards[1], -1);
#if (NOISY_TEST == 1)
    printf("tributeRevealedCards[1] = Expected: %d, Actual: %d\n", -1, tributeRevealedCards[1]); 
#endif
    printf("Test 16: ");
    assertTrue(state.handCount[currentPlayer] + 1, testState.handCount[currentPlayer]);
#if (NOISY_TEST == 1)
    printf("handCount of current player. Expected: %d, Actual: %d\n", state.handCount[currentPlayer] + 1, testState.handCount[currentPlayer]); 
#endif
    printf("\n");
}