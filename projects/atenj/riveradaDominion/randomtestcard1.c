#include "dominion.h"
#include "dominion_helpers.h"
#include "interface.h"
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "rngs.h"
#include <math.h>

#define DEBUG 0
#define NOISY_TEST 0

int main () {

  //random seed for gamestate
  int seed = time(NULL);
  srand(seed);  
  int failedCheckCount1 = 0; //counter to see how many checks failed
  int failedCheckCount2 = 0;
  int failedCheckCount3 = 0;
  int failedCheckCount4 = 0;
  int failedCheckCount5 = 0;
  int failedCheckCount6 = 0;
  int numberOfTests = 1000; //number of random tests to generate

  int n, i, j, numPlayers, choice1, estateFlag;

  int k[10] = {adventurer, council_room, feast, gardens, mine,
    remodel, smithy, village, baron, great_hall};

    struct gameState state, testState;

    printf ("Testing callBaron\n");

    printf ("RANDOM TESTS.\n");


    for (n = 0; n < numberOfTests; n++) {

      choice1 = rand() % 2; //use this to randomly generate 0 or 1 for choice1
      numPlayers = (rand() % 3) + 2; //number of players between 2 and 4
      initializeGame(numPlayers, k, seed, &state); //initialize a game
      //deckCount, discardCount, and handCount are set to random numbers within domain
      for (i = 0; i < numPlayers; i++) {
        state.deckCount[i] = rand() % MAX_DECK; 
        state.discardCount[i] = rand() % MAX_DECK;
        state.handCount[i] = rand() % MAX_HAND;
      }
      //also want to randomize the supply of estates so we get full coverage
      if (numPlayers == 2) {
         state.supplyCount[estate] = rand() % 9; //supply count random between 0 and 8
      } else {
         state.supplyCount[estate] = rand() % 13; //supply count between 0 and 12
      }
      
#if (NOISY_TEST == 1)
      printf("\n");
      printf("====================================================\n");
      printf("Case %d\n", n);
      printf("Number of Estate Cards: %d\n", state.supplyCount[estate]);
      printf("numPlayers: %d\n", state.numPlayers);
      for (j = 0; j < numPlayers; j++) {
        printf("Player: %d -- Deck: %d, Discard: %d, Hand: %d, Choice1: %d\n", j, state.deckCount[j], state.discardCount[j], state.handCount[j], choice1);
      }
      //Check to see what happens when you call callBaron
#endif
      //End turn a random number of times so we aren't always testing Player 0
      for (int i = 0; i < rand() % 4; i++) {
        endTurn(&state);
      }
      //Now figure out whose turn it is
      int currentPlayer = state.whoseTurn;
      //Now save the game state to test state before testing
      memcpy(&testState, &state, sizeof(struct gameState));
      //In the test state, current player plays callBaron
      printf("Game State Before:\n");
      printState(&state);
      callBaron(choice1, currentPlayer,  &testState);
      printf("Game State After:\n");
      printState(&testState);
      //Now assert what should happen
      //We always know numBuys should increase by 1
      if ((state.numBuys+1) != testState.numBuys) {
#if (NOISY_TEST == 1)
        printf("numBuys - expected: %d, actual: %d\n", state.numBuys+1, testState.numBuys);
#endif
        failedCheckCount1++;
      }
      if (choice1 > 0) {
        estateFlag = 0; 
        for (int j = 0; j < state.handCount[currentPlayer]; j++) {
          if (state.hand[currentPlayer][j] == estate) {
            estateFlag = 1; //if an estate card is found in p's hand, set flag to 1
          }
        }
        if (estateFlag == 1) { //if estate card was found
          //coins increase by 4
          if (state.coins + 4 != testState.coins){
#if (NOISY_TEST == 1)
            printf("Coins - expected: %d, actual: %d\n", state.coins+4, testState.coins);
#endif
            failedCheckCount2++;
          }
          //discardCount[p]++
          if (testState.discardCount[currentPlayer] != state.discardCount[currentPlayer] + 1) {
#if (NOISY_TEST == 1)
            printf("discardCount - expected: %d, actual: %d\n", state.discardCount[currentPlayer] + 1, testState.discardCount[currentPlayer]);
#endif
            failedCheckCount3++;
          }
          //handCount[p]--
          if (state.handCount[currentPlayer] - 1 != testState.handCount[currentPlayer]) {
#if (NOISY_TEST == 1)
            printf("handCount - expected: %d, actual: %d\n", state.handCount[currentPlayer]-1, testState.handCount[currentPlayer]);
#endif
            failedCheckCount4++;
          }
        }
        else {
          //if estate card wasn't found
          //check if supplyCount[estate,state] > 0
          if (state.supplyCount[estate] > 0){
          //if yes, then supplyCount of estate--, discardCount[p]++, last discard should be an estate, and then check if supplycount is 0
            if((state.supplyCount[estate] -1) != testState.supplyCount[estate]){
#if (NOISY_TEST == 1)              
              printf("supplyCount - expected: %d, actual: %d\n", state.supplyCount[estate] - 1, testState.supplyCount[estate]);
#endif 
              failedCheckCount5++;
            }
            if((state.discardCount[currentPlayer] + 1) != testState.discardCount[currentPlayer]){
#if (NOISY_TEST == 1)
              printf("discardCount - expected: %d, actual: %d\n", state.discardCount[currentPlayer] + 1, testState.discardCount[currentPlayer]);
#endif
              failedCheckCount6++;
            }
          }
        }
      }


    }

    printf ("END TEST\n");

    printf("Failed Check 1: %d times\n", failedCheckCount1);
    printf("Failed Check 2: %d times\n", failedCheckCount2);
    printf("Failed Check 3: %d times\n", failedCheckCount3);
    printf("Failed Check 4: %d times\n", failedCheckCount4);
    printf("Failed Check 5: %d times\n", failedCheckCount5);
    printf("Failed Check 6: %d times\n", failedCheckCount6);
    return 0;
  }
