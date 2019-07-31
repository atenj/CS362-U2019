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
  int numberOfTests = 100; //number of random tests to generate

  int n, j, numPlayers, choice2, handPos, currentPlayer;

  int k[10] = {adventurer, council_room, feast, gardens, minion,
    remodel, smithy, village, baron, great_hall};

    struct gameState state, testState;

    printf ("Testing cardMinion\n");

    printf ("RANDOM TESTS.\n");


    for (n = 0; n < numberOfTests; n++) {

      int choice1 = rand() % 2; //use this to randomly generate 0 or 1 for choice1
      //set choice2 = to opposite of choice1
      if (choice1 == 0) {
        choice2 = 1;
      } else if (choice1 == 1) {
        choice2 = 0;
      }

      numPlayers = (rand() % 3) + 2; //number of players between 2 and 4
      initializeGame(numPlayers, k, seed, &state); //initialize a game
      //deckCount, discardCount, and handCount are set to random numbers within domain
      for (int i = 0; i < numPlayers; i++) {
        state.deckCount[i] = rand() % MAX_DECK; 
        state.discardCount[i] = rand() % MAX_DECK;
        state.handCount[i] = rand() % MAX_HAND;
      }

#if (NOISY_TEST == 1)
      printf("\n");
      printf("====================================================\n");
      printf("Case %d\n", n);
      printf("numPlayers: %d\n", state.numPlayers);
      for (j = 0; j < numPlayers; j++) {
        printf("Player: %d -- Deck: %d, Discard: %d, Hand: %d, Choice1: %d, Choice2: %d\n", j, state.deckCount[j], state.discardCount[j], state.handCount[j], choice1, choice2);
      }
      //Check to see what happens when you call cardMinion
#endif
      //End turn a random number of times so we aren't always testing Player 0
      int turns = rand() % 4;

      for (int m = 0; m < turns; m++) {
        endTurn(&state);
      }
      //Now figure out whose turn it is
      currentPlayer = state.whoseTurn;

      //Now pick a random card in the currentPlayer's hand (from 0 to handCount[current] - 1)
       int count = state.handCount[currentPlayer];
       handPos = rand() % count; 

       //Now save the game state to test state before testing
       memcpy(&testState, &state, sizeof(struct gameState));
       //In the test state, current player plays cardBaron
       cardMinion(choice1, choice2, &testState, handPos, currentPlayer);

       //Now assert what should happen
       //We always know numActions should increase by 1
       if ((state.numActions+1) != testState.numActions) {
#if (NOISY_TEST == 1)
         printf("numActions - expected: %d, actual: %d\n", state.numActions+1, testState.numActions);
 #endif
         failedCheckCount1++;
       }

       //Checking choice1
       if (choice1 == 1) {
          if (testState.coins != state.coins+2) {
#if (NOISY_TEST == 1)
            printf("Number of coins - expected: %d, actual: %d\n", state.coins + 2, testState.coins);
#endif
            failedCheckCount2++;
          }
       }
       
       if (choice2 == 1) {
          if (testState.handCount[currentPlayer] != 4) {
#if (NOISY_TEST == 1)
          printf("Hand Count - expected: 4, actual: %d\n", testState.handCount[currentPlayer]);
#endif
          failedCheckCount3++;
        } 
      }

      if (choice2 == 1) {
        //loop over the number of players
        for (int p = 0; p < testState.numPlayers; p++) {
          //verify that if the player originally had 5 or more cards, they now have 4 cards
          if (state.handCount[p] > 4) {
            if (testState.handCount[p] != 4) {
#if (NOISY_TEST == 1)
              printf("Hand Count of player %d - expected: 4, actual: %d\n", p, testState.handCount[p]);
#endif
              failedCheckCount4++;
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

    return 0;
  }
