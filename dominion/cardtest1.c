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
    int numPlayers;
    int randomSeed;
    struct gameState state;

    // Kingdom Cards for Testing
    // kingdomCards1: all unique cards, no errors
    int kingdomCards1[10] = {adventurer, gardens, mine, remodel, smithy, village, baron, steward, ambassador, salvager};
    //kingdomCards2: smithy is used twice
    int kingdomCards2[10] = {adventurer, gardens, mine, remodel, smithy, smithy, baron, steward, ambassador, salvager};

    int curseCount3Players = 20;
    int curseCount2Players = 10;
    int curseCount4Players = 30;
    int victoryCount3Players = 12;
    int victoryCount2Players = 8;
    int copperCount = 39;
    int silverCount = 40;
    int goldCount = 30;
    int kingdomSupply = 10;
    int noSupply = -1;
    int playerDeckCount = 10;

    printf("\n");  
    printf("=========================================\n");
    printf("Card Test 1: int initializeGame()\n");
    printf("=========================================\n");
    printf("\n");
    
    // Case #1: 3 Players, Parameters Within Bounds
    printf("Test Case #1: 3 Players, Parameters Within Bounds\n");
    printf("=========================================\n");
    printf("\n");
    
    numPlayers = 3;
    randomSeed = 1000;

    initializeGame(numPlayers, kingdomCards1, randomSeed, &state);

    //verify that numPlayers is correct
    printf("Test 1: ");
    assertTrue(numPlayers, state.numPlayers);
    printf("numPlayers - expected: %d, actual: %d\n", numPlayers, state.numPlayers);
    //Curse Count is correct
    printf("Test 2: ");
    assertTrue(curseCount3Players, state.supplyCount[curse]);
    printf("Supply Count of Curse Cards - expected: %d, actual: %d\n", curseCount3Players, state.supplyCount[curse]);
    //Victory Cards are Correct
    printf("Test 3: ");
    assertTrue(victoryCount3Players, state.supplyCount[estate]);
    printf("Supply Count of Estate Cards - expected: %d, actual %d\n", victoryCount3Players, state.supplyCount[estate]);
    printf("Test 4: ");
    assertTrue(victoryCount3Players, state.supplyCount[estate]);
    printf("Supply Count of Duchy Cards - expected: %d, actual %d\n", victoryCount3Players, state.supplyCount[duchy]);
    printf("Test 5: ");
    assertTrue(victoryCount3Players, state.supplyCount[estate]);
    printf("Supply Count of Province Cards - expected: %d, actual %d\n", victoryCount3Players, state.supplyCount[province]);
    //Treasure Cards are Correct
    printf("Test 6: ");
    assertTrue(copperCount, state.supplyCount[copper]);
    printf("Supply Count of Copper Cards - expected: %d, actual %d\n", copperCount, state.supplyCount[copper]);
    printf("Test 7: ");
    assertTrue(silverCount, state.supplyCount[silver]);
    printf("Supply Count of Silver Cards - expected: %d, actual %d\n", silverCount, state.supplyCount[silver]);
    printf("Test 8: ");
    assertTrue(goldCount, state.supplyCount[gold]);
    printf("Supply Count of Gold Cards - expected: %d, actual %d\n", goldCount, state.supplyCount[gold]);
    //Kingdom Cards - supplyCount should be 12 for victory cards, 10 for all else
    printf("Test 9: ");
    assertTrue(kingdomSupply, state.supplyCount[mine]);
    printf("Supply Count of Mine Cards - expected: %d, actual %d\n", kingdomSupply, state.supplyCount[mine]);
    printf("Test 10: ");
    assertTrue(victoryCount3Players, state.supplyCount[gardens]);
    printf("Supply Count of Garden Cards - expected: %d, actual %d\n", victoryCount3Players, state.supplyCount[gardens]);
    //Check Supply of a Card Not Used in the Game
    printf("Test 11: ");
    assertTrue(noSupply, state.supplyCount[tribute]);
    printf("Supply Count of Tribute Cards - expected: %d, actual %d\n", noSupply, state.supplyCount[tribute]);
    //Deck Count of Player1 Should be 5 (5 cards in hand)
    printf("Test 12: ");
    assertTrue(playerDeckCount - 5, state.deckCount[0]);
    printf("Deck Count of Player 0 - expected: %d, actual %d\n", playerDeckCount - 5, state.deckCount[0]);
    //Deck Count of Other Players should be 10
    printf("Test 13: ");
    assertTrue(playerDeckCount, state.deckCount[1]);
    printf("Deck Count of Player 1 - expected: %d, actual %d\n", playerDeckCount, state.deckCount[1]);
    printf("Test 14: ");
    assertTrue(playerDeckCount, state.deckCount[2]);
    printf("Deck Count of Player 2 - expected: %d, actual %d\n", playerDeckCount, state.deckCount[2]);
    // Check handCount and discardCount of players 1 and 2
    printf("Test 15: ");
    assertTrue(0, state.handCount[1]);
    printf("Hand Count of Player 1 - expected: %d, actual %d\n", 0, state.handCount[1]);
    printf("Test 16: ");
    assertTrue(0, state.discardCount[2]);
    printf("Discard Count of Player 2 - expected: %d, actual %d\n", 0, state.discardCount[2]);
    // Check embargo tokens of a random supply pile
    printf("Test 16: ");
    assertTrue(0, state.embargoTokens[steward]);
    printf("Embargo Tokens Count of Steward - expected: %d, actual %d\n", 0, state.embargoTokens[steward]);
    // Check First Player's Turn Initialized
    printf("Test 17: ");
    assertTrue(0, state.outpostPlayed);
    printf("outpostPlayed - expected: %d, actual: %d\n", 0, state.outpostPlayed);
    printf("Test 18: ");
    assertTrue(0, state.outpostPlayed);
    printf("phase - expected: %d, actual: %d\n", 0, state.phase);
    printf("Test 19: ");
    assertTrue(1, state.numActions);
    printf("numActions - expected: %d, actual: %d\n", 1, state.numActions);   
    printf("Test 20: ");
    assertTrue(1, state.numBuys);
    printf("numBuys - expected: %d, actual: %d\n", 1, state.numBuys);
    printf("Test 21: ");
    assertTrue(0, state.playedCardCount);
    printf("numBuys - expected: %d, actual: %d\n", 0, state.playedCardCount);
    printf("Test 22: ");
    assertTrue(0, state.whoseTurn);
    printf("numBuys - expected: %d, actual: %d\n", 0, state.whoseTurn);
    //Check First Player's Cards Drawn
    printf("Test 23: ");
    assertTrue(5, state.handCount[0]);
    printf("Player 0 hand count - expected: %d, actual: %d\n", 5, state.handCount[0]);

    // Case #2: Invalid Number of Players
    printf("\n");
    printf("=========================================\n");
    printf("Test Case #2: Invalid Number of Players\n");
    printf("=========================================\n");
    printf("\n");

    //Number of Players Has to be At Least 2
    numPlayers = 1;

    memset(&state, 23, sizeof(struct gameState)); //clear game state
    printf("Test 24: ");
    assertTrue(initializeGame(numPlayers, kingdomCards1, randomSeed, &state), -1);
    printf("Invalid Number of Players. Should return -1\n");

    // Case #3: There is a Duplicate in Kingdom Cards
    printf("\n");
    printf("=========================================\n");
    printf("Test Case #3: Duplicate in Kingdom Cards\n");
    printf("=========================================\n");
    printf("\n");

    numPlayers = 2;

    memset(&state, 23, sizeof(struct gameState)); //clear game state
    printf("Test 25: ");
    assertTrue(initializeGame(numPlayers, kingdomCards2, randomSeed, &state), -1);
    printf("Duplicate in Kingdom Cards. Should return -1\n");

    // Case #4: Two Player Game - Checking What Is Different
    printf("\n");
    printf("=========================================\n");
    printf("Test Case #4: Two Player Game\n");
    printf("=========================================\n");
    printf("\n");

    numPlayers = 2;

    memset(&state, 23, sizeof(struct gameState)); //clear game state
    initializeGame(numPlayers, kingdomCards1, randomSeed, &state);

    printf("Test 26: ");
    assertTrue(curseCount2Players, state.supplyCount[curse]);
    printf("Supply Count of Curse Cards - expected: %d, actual: %d\n", curseCount2Players, state.supplyCount[curse]);
    printf("Test 27: ");
    assertTrue(victoryCount2Players, state.supplyCount[estate]);
    printf("Supply Count of Estate Cards - expected: %d, actual %d\n", victoryCount2Players, state.supplyCount[estate]);
    printf("Test 28: ");
    assertTrue(victoryCount2Players, state.supplyCount[gardens]);
    printf("Supply Count of Garden Cards - expected: %d, actual %d\n", victoryCount2Players, state.supplyCount[gardens]);

    // Case #5: Four Player Game - Checking What Is Different
    printf("\n");
    printf("=========================================\n");
    printf("Test Case #4: Four Player Game\n");
    printf("=========================================\n");
    printf("\n");

    numPlayers = 4;

    memset(&state, 23, sizeof(struct gameState)); //clear game state
    initializeGame(numPlayers, kingdomCards1, randomSeed, &state);

    printf("Test 29: ");
    assertTrue(curseCount4Players, state.supplyCount[curse]);
    printf("Supply Count of Curse Cards - expected: %d, actual: %d\n", curseCount4Players, state.supplyCount[curse]);

}