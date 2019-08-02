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
#define NOISY_TEST 1

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
  int failedCheckCount7 = 0;
  int failedCheckCount8 = 0;
  int failedCheckCount9 = 0;
  int failedCheckCount10 = 0;


  int numberOfTests = 1000; //number of random tests to generate

  int n, j, numPlayers, currentPlayer, nextPlayer;
  int tributeRevealedCards[2] = {-1,-1};

  int addCoins = 0; //tracks number of coins to add
  int addCards = 0; //tracks number of cards to add
  int addActions = 0; //tracks number of actions to add

  int k[10] = {adventurer, council_room, feast, gardens, tribute,
    remodel, smithy, village, baron, great_hall};

    struct gameState state, testState;

    printf ("Testing cardTribute\n");

    printf ("RANDOM TESTS.\n");


    for (n = 0; n < numberOfTests; n++) {

      numPlayers = (rand() % 3) + 2; //number of players between 2 and 4
      addCoins = 0; //reset the counters for coins 
      addActions = 0; //actions
      addCards = 0;   //and cards
      initializeGame(numPlayers, k, seed, &state); //initialize a game

      //deckCount, discardCount are set to random numbers within domain
      //for this test, we want some instances where there might be 0 cards in the 
      //discard or deck, so we are going to limit the range for the deckCount
      //and the discardCounts so there's a better probability of getting a 0
      for (int i = 0; i < numPlayers; i++) {
        state.deckCount[i] = rand() % 11; 
        for (int j = 0; j < state.deckCount[i]; j++) {
          state.deck[i][j] = rand() % 27; //set to random value between 0 and 26
        }
        state.discardCount[i] = rand() % 6;
        for (int k = 0; k < state.discardCount[i]; k++) {
          state.discard[i][k] = rand() % 27; //set to random value between 0 and 26
        }
      }

      //we want the cards in the decks and discards to be set to a random mix of cards


#if (NOISY_TEST == 1)
      printf("\n");
      printf("====================================================\n");
      printf("Case %d\n", n);
      printf("numPlayers: %d\n", state.numPlayers);
      for (j = 0; j < numPlayers; j++) {
        printf("Player: %d -- Deck: %d, Discard: %d, Hand: %d\n", j, state.deckCount[j], state.discardCount[j], state.handCount[j]);
        printDeck(j, &state);
        printDiscard(j, &state);
      }
      //Check to see what happens when you call cardTribute
#endif
      //End turn a random number of times so we aren't always testing Player 0
      int turns = rand() % 4;

      for (int m = 0; m < turns; m++) {
        endTurn(&state);
      }

      //Now figure out whose turn it is
      currentPlayer = state.whoseTurn;
      //if the current player is the last player, nextPlayer is 0
      if (currentPlayer == numPlayers - 1) {
        nextPlayer = 0;
      } else { //otherwise nextPlayer is currentPlayer index + 1
        nextPlayer = currentPlayer + 1;
      }

       //Now save the game state to test state before testing
      memcpy(&testState, &state, sizeof(struct gameState));
       //In the test state, current player plays cardTribute
      cardTribute(&testState, nextPlayer, currentPlayer, tributeRevealedCards);

       //Now assert what should happen
      // if the discard count plus deck count of the next player contain 1 or less cards
      if ((testState.discardCount[nextPlayer] + testState.deckCount[nextPlayer]) <= 1) {
        //if the deck has the only card, its the first revealed card and deck is decremented
        if (testState.deckCount[nextPlayer] > 0) {
          if (tributeRevealedCards[0] != state.deck[nextPlayer][state.deckCount[nextPlayer]-1]) {
#if (NOISY_TEST == 1)
            printf("tributeRevealedCards[0] - expected: %d, actual: %d\n", state.deck[nextPlayer][state.deckCount[nextPlayer]-1], tributeRevealedCards[0]);
#endif
            failedCheckCount1++;
          }
          if (testState.deckCount[nextPlayer] != state.deckCount[nextPlayer] - 1){
#if (NOISY_TEST == 1)
            printf("deckCount of nextPlayer - expected: %d, actual: %d\n", state.deckCount[nextPlayer] - 1, testState.deckCount[nextPlayer]);
#endif
            failedCheckCount2++;
          }
        }
        //if the discard has the only card, its the first revealed card and discard is decremented
        else if (testState.discardCount[nextPlayer] > 0) {
          if (tributeRevealedCards[0] != state.discard[nextPlayer][state.discardCount[nextPlayer]-1]){
#if (NOISY_TEST == 1)
            printf("tributeRevealedCards[0] - expected: %d, actual: %d\n", state.discard[nextPlayer][state.discardCount[nextPlayer]-1], tributeRevealedCards[0]);
#endif
            failedCheckCount3++;
          }
          if (testState.discardCount[nextPlayer] != state.discardCount[nextPlayer] - 1){
#if (NOISY_TEST == 1)
            printf("discardCount of nextPlayer - expected: %d, actual: %d\n", state.discardCount[nextPlayer] - 1, testState.discardCount[nextPlayer]);
#endif
            failedCheckCount4++;
          }
        }
        else {
#if (NOISY_TEST == 1)
          printf("No cards revealed.");
#endif
        }
      } else {
        if (state.deckCount[nextPlayer] == 0) {
            //this means deck is empty but there are at least 2 cards in discard
            //then we expect discard was shuffled and moved to deck
            //in the test state, the discard should be empty and
            // the deck count should be state's discard - 2
          if (testState.discardCount[nextPlayer] != 0) {
#if (NOISY_TEST == 1)
            printf("discardCount of nextPlayer - expected: %d, actual %d\n", 0, testState.discardCount[nextPlayer]);
#endif
            failedCheckCount5++;
          }
          if (testState.deckCount[nextPlayer] != state.discardCount[nextPlayer] - 2) {
#if (NOISY_TEST == 1)
            printf("deckCount of nextPlayer - expected:%d, actual %d\n", state.discardCount[nextPlayer]-2, testState.deckCount[nextPlayer]);
#endif
            failedCheckCount6++;
          }
        }
      }

        if (tributeRevealedCards[1] == -1) {
          //we had a duplicate card
          //we expect the playedCardCount to be incremented
          if (testState.playedCardCount != state.playedCardCount + 1) {
#if (NOISY_TEST == 1)
            printf("testState playedCardCount - expected: %d, actual: %d\n", state.playedCardCount + 1, testState.playedCardCount);
#endif 
            failedCheckCount7++;
          }
        }

        for (int i = 0; i < 2; i++) {
          if (tributeRevealedCards[i] == copper || tributeRevealedCards[i] == silver || tributeRevealedCards[i] == gold){
            addCoins += 2;
          }
          else if (tributeRevealedCards[i] == estate || tributeRevealedCards[i] == duchy || tributeRevealedCards[i] == province || tributeRevealedCards[i] == gardens || tributeRevealedCards[i] == great_hall){
            addCards += 2;
          }
          else if (tributeRevealedCards[i] != -1) {
            addActions += 2;
          }
        }

        if (testState.coins != state.coins + addCoins) {
#if (NOISY_TEST == 1)
          printf("testState coins - expected: %d, actual: %d\n", state.coins + addCoins, testState.coins);
#endif
          failedCheckCount8++;
        }
        if (testState.deckCount[currentPlayer] != state.deckCount[currentPlayer] + addCards) {
#if (NOISY_TEST == 1)
          printf("testState deckCount[currentPlayer] - expected: %d, actual: %d\n", state.deckCount[currentPlayer] + addCards, testState.deckCount[currentPlayer]);
#endif
          failedCheckCount9++;
        }
        if (testState.numActions != state.numActions + addActions) {
#if (NOISY_TEST == 1)
          printf("testState numActions - expected: %d, actual: %d\n", state.numActions + addActions, testState.numActions);
#endif
          failedCheckCount10++;
        }

      //
    }

    printf ("END TEST\n");

    printf("Failed Check 1: %d times\n", failedCheckCount1);
    printf("Failed Check 2: %d times\n", failedCheckCount2);
    printf("Failed Check 3: %d times\n", failedCheckCount3);
    printf("Failed Check 4: %d times\n", failedCheckCount4);
    printf("Failed Check 5: %d times\n", failedCheckCount5);
    printf("Failed Check 6: %d times\n", failedCheckCount6);  
    printf("Failed Check 7: %d times\n", failedCheckCount7);  
    printf("Failed Check 8: %d times\n", failedCheckCount8);  
    printf("Failed Check 9: %d times\n", failedCheckCount9);  
    printf("Failed Check 10: %d times\n", failedCheckCount10);  

    return 0;
  }
