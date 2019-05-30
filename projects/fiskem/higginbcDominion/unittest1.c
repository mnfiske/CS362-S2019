/**********************************************************************
Positive test of the Smithy card.
***********************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include "stdlib.h"

void assertTrue(int result, int line, char test[])
{
  if (result == 0)
  {
    printf("unittest1: unittest1.c:%d: main Assertion '%s' failed\n", line, test);
  }
}

int main() {

    int seed = 1000;
    int numPlayer = 2;
    int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
    struct gameState G, initialG;
    int origHandCount;
    int currentPlayer = 0;
    int deckCount = 0;
    int origDeckCount;
    int playedNum = 1;
    int gainedCards = 3;
    int handPos = 0;
    int play;
    int otherPlayer = 1;
    int orig2ndPlayerDeckCount;
    int orig2ndPlayerHandCount;
    char *test = malloc(256);
    int origVictoryCount = 0;
    int origKingdomCount = 0;
    int origDiscards;
    int kingdomCount;
    int playedCount;
    int empty = 0;
    int test2DeckCount = 2;

    // initialize a new game
    initializeGame(numPlayer, k, seed, &initialG);

    //Create a copy of the gamestate, to avoid having to initialize again
    memcpy(&G, &initialG, sizeof(struct gameState));

    /*****************************************************************************
     TEST1 test Smithy with default beginning game deck/hand
    *****************************************************************************/

    printf("***************unittest1.c TEST1 start***************\n");

    //Track the  states for the players
    origHandCount = G.handCount[currentPlayer];
    origDeckCount = G.deckCount[currentPlayer];
    orig2ndPlayerHandCount = G.handCount[otherPlayer];
    orig2ndPlayerDeckCount = G.deckCount[otherPlayer];
    //Track the  states of the victory and kingdom cards
    origVictoryCount = G.supplyCount[estate] + G.supplyCount[duchy] + G.supplyCount[province];
    origKingdomCount = G.supplyCount[adventurer] + G.supplyCount[council_room] + G.supplyCount[feast] + G.supplyCount[gardens] +
                       G.supplyCount[mine] + G.supplyCount[remodel] + G.supplyCount[smithy] + G.supplyCount[village] +
                       G.supplyCount[baron] + G.supplyCount[great_hall];

    //Call the smithyEffect method
    play = smithyEffect(&G, handPos);
    //Verify the method returned 0
    test = "play == 0";
    assertTrue(play == 0, __LINE__, test);

    //Verify other player's state did not change
    test = "orig2ndPlayerHandCount == G.handCount[otherPlayer]";
    assertTrue(orig2ndPlayerHandCount == G.handCount[otherPlayer], __LINE__, test);
    test = "orig2ndPlayerDeckCount == G.deckCount[otherPlayer]";
    assertTrue(orig2ndPlayerDeckCount == G.deckCount[otherPlayer], __LINE__, test);

    //Verify one card discarded
    playedCount = G.playedCardCount;
    /*This test will fail due to a bug I introduced in assignment 2*/
    test = "playedCount == playedNum";
    assertTrue(playedCount == playedNum, __LINE__, test);

    //Verify three cards were added to the hand, and one was removed (b/c the Smithy card was played and removed from the hand)
    test = "(origHandCount + gainedCards - playedNum) == G.handCount[currentPlayer]";
    assertTrue((origHandCount + gainedCards - playedNum) == G.handCount[currentPlayer], __LINE__, test);

    //Verify the cards were removed from the player's deck
    deckCount = G.deckCount[currentPlayer];
    test = "deckCount == (origDeckCount - gainedCards)";
    assertTrue(deckCount == (origDeckCount - gainedCards), __LINE__, test);

    //Verify no state change occurred for victory cards
    test = "origVictoryCount = G.supplyCount[estate] + G.supplyCount[duchy] + G.supplyCount[province]";
    assertTrue(origVictoryCount == G.supplyCount[estate] + G.supplyCount[duchy] + G.supplyCount[province],  __LINE__, test);

    //Verify no change for kingdom cards
    test = "origKingdomCount == G.supplyCount[adventurer] + G.supplyCount[council_room] + G.supplyCount[feast] + G.supplyCount[gardens] + G.supplyCount[mine] + G.supplyCount[remodel] + G.supplyCount[smithy] + G.supplyCount[village] + G.supplyCount[baron] + G.supplyCount[great_hall]";
    kingdomCount = G.supplyCount[adventurer] + G.supplyCount[council_room] + G.supplyCount[feast] +
      G.supplyCount[gardens] + G.supplyCount[mine] + G.supplyCount[remodel] + G.supplyCount[smithy] + G.supplyCount[village] + G.supplyCount[baron] + 
      G.supplyCount[great_hall];
    assertTrue(origKingdomCount == kingdomCount, __LINE__, test);
    
    /******************************************************************************************************
     TEST2 Ensure Smithy gives three cards when there are only two cards in the deck, but discard pile has 
     cards to pull from
    ******************************************************************************************************/

    printf("***************unittest1.c TEST2 start***************\n");
    
    //Return to beginning state
    memcpy(&G, &initialG, sizeof(struct gameState));

    //Track the  states for the players
    origHandCount = G.handCount[currentPlayer];
    orig2ndPlayerHandCount = G.handCount[otherPlayer];
    orig2ndPlayerDeckCount = G.deckCount[otherPlayer];
    //Track the  states of the victory and kingdom cards
    origVictoryCount = G.supplyCount[estate] + G.supplyCount[duchy] + G.supplyCount[province];
    origKingdomCount = G.supplyCount[adventurer] + G.supplyCount[council_room] + G.supplyCount[feast] + G.supplyCount[gardens] +
      G.supplyCount[mine] + G.supplyCount[remodel] + G.supplyCount[smithy] + G.supplyCount[village] +
      G.supplyCount[baron] + G.supplyCount[great_hall];

    //Shift all but two of the card counts from the deck to the discard pile
    G.discardCount[currentPlayer] = G.deckCount[currentPlayer] - test2DeckCount;
    G.deckCount[currentPlayer] = test2DeckCount;
    //Track the state of the deck and discards before Smithy is played
    origDiscards = G.discardCount[currentPlayer];
    origDeckCount = G.deckCount[currentPlayer];

    //Call the smithyEffect method
    play = smithyEffect(&G, handPos);

    //Verify the method returned 0
    test = "play == 0";
    assertTrue(play == 0, __LINE__, test);

    //Verify other player's state did not change
    test = "orig2ndPlayerHandCount == G.handCount[otherPlayer]";
    assertTrue(orig2ndPlayerHandCount == G.handCount[otherPlayer], __LINE__, test);
    test = "orig2ndPlayerDeckCount == G.deckCount[otherPlayer]";
    assertTrue(orig2ndPlayerDeckCount == G.deckCount[otherPlayer], __LINE__, test);

    //Verify one card discarded
    playedCount = G.playedCardCount;
    /*This test will fail due to a bug I introduced in assignment 2*/
    test = "playedCount == playedNum";
    assertTrue(playedCount == playedNum, __LINE__, test);

    //Verify three cards were added to the hand, and one was removed (b/c the Smithy card was played and removed from the hand)
    test = "(origHandCount + gainedCards - playedNum) == G.handCount[currentPlayer]";
    assertTrue((origHandCount + gainedCards - playedNum) == G.handCount[currentPlayer], __LINE__, test);

    //Verify the discards were shuffled into the deck and three cards were removed from the player's deck
    deckCount = G.deckCount[currentPlayer];
    test = "deckCount == (origDeckCount - gainedCards + origDiscards)";
    assertTrue(deckCount == (origDeckCount - gainedCards + origDiscards), __LINE__, test);

    //Verify all discards were shuffled into the deck
    test = "G.discardCount[currentPlayer] == empty";
    assertTrue(G.discardCount[currentPlayer] == empty, __LINE__, test);

    //Verify no state change occurred for victory cards
    test = "origVictoryCount = G.supplyCount[estate] + G.supplyCount[duchy] + G.supplyCount[province]";
    assertTrue(origVictoryCount == G.supplyCount[estate] + G.supplyCount[duchy] + G.supplyCount[province], __LINE__, test);

    //Verify no change for kingdom cards
    test = "origKingdomCount == G.supplyCount[adventurer] + G.supplyCount[council_room] + G.supplyCount[feast] + G.supplyCount[gardens] + G.supplyCount[mine] + G.supplyCount[remodel] + G.supplyCount[smithy] + G.supplyCount[village] + G.supplyCount[baron] + G.supplyCount[great_hall]";
    kingdomCount = G.supplyCount[adventurer] + G.supplyCount[council_room] + G.supplyCount[feast] +
      G.supplyCount[gardens] + G.supplyCount[mine] + G.supplyCount[remodel] + G.supplyCount[smithy] + G.supplyCount[village] + G.supplyCount[baron] +
      G.supplyCount[great_hall];
    assertTrue(origKingdomCount == kingdomCount, __LINE__, test);

    /******************************************************************************************************
    TEST3 Ensure Smithy gives three cards when there are zero cards in the deck, but enough cards in discard
    ******************************************************************************************************/

    printf("***************unittest1.c TEST3 start***************\n");

    //Return to beginning state
    memcpy(&G, &initialG, sizeof(struct gameState));

    //Track the  states for the players
    origHandCount = G.handCount[currentPlayer];
    orig2ndPlayerHandCount = G.handCount[otherPlayer];
    orig2ndPlayerDeckCount = G.deckCount[otherPlayer];
    //Track the  states of the victory and kingdom cards
    origVictoryCount = G.supplyCount[estate] + G.supplyCount[duchy] + G.supplyCount[province];
    origKingdomCount = G.supplyCount[adventurer] + G.supplyCount[council_room] + G.supplyCount[feast] + G.supplyCount[gardens] +
      G.supplyCount[mine] + G.supplyCount[remodel] + G.supplyCount[smithy] + G.supplyCount[village] +
      G.supplyCount[baron] + G.supplyCount[great_hall];

    //Shift all but two of the card counts from the deck to the discard pile
    G.discardCount[currentPlayer] = G.deckCount[currentPlayer];
    G.deckCount[currentPlayer] = empty;
    //Track the state of the deck and discards before Smithy is played
    origDiscards = G.discardCount[currentPlayer];
    origDeckCount = G.deckCount[currentPlayer];

    //Call the smithyEffect method
    play = smithyEffect(&G, handPos);

    //Verify the method returned 0
    test = "play == 0";
    assertTrue(play == 0, __LINE__, test);

    //Verify other player's state did not change
    test = "orig2ndPlayerHandCount == G.handCount[otherPlayer]";
    assertTrue(orig2ndPlayerHandCount == G.handCount[otherPlayer], __LINE__, test);
    test = "orig2ndPlayerDeckCount == G.deckCount[otherPlayer]";
    assertTrue(orig2ndPlayerDeckCount == G.deckCount[otherPlayer], __LINE__, test);

    //Verify one card discarded
    playedCount = G.playedCardCount;
    /*This test will fail due to a bug I introduced in assignment 2*/
    test = "playedCount == playedNum";
    assertTrue(playedCount == playedNum, __LINE__, test);

    //Verify three cards were added to the hand, and one was removed (b/c the Smithy card was played and removed from the hand)
    test = "(origHandCount + gainedCards - playedNum) == G.handCount[currentPlayer]";
    assertTrue((origHandCount + gainedCards - playedNum) == G.handCount[currentPlayer], __LINE__, test);

    //Verify the discards were shuffled into the deck and three cards were removed from the player's deck
    deckCount = G.deckCount[currentPlayer];
    test = "deckCount == (origDeckCount - gainedCards + origDiscards)";
    assertTrue(deckCount == (origDeckCount - gainedCards + origDiscards), __LINE__, test);

    //Verify all discards were shuffled into the deck
    test = "G.discardCount[currentPlayer] == empty";
    assertTrue(G.discardCount[currentPlayer] == empty, __LINE__, test);

    //Verify no state change occurred for victory cards
    test = "origVictoryCount = G.supplyCount[estate] + G.supplyCount[duchy] + G.supplyCount[province]";
    assertTrue(origVictoryCount == G.supplyCount[estate] + G.supplyCount[duchy] + G.supplyCount[province], __LINE__, test);

    //Verify no change for kingdom cards
    test = "origKingdomCount == G.supplyCount[adventurer] + G.supplyCount[council_room] + G.supplyCount[feast] + G.supplyCount[gardens] + G.supplyCount[mine] + G.supplyCount[remodel] + G.supplyCount[smithy] + G.supplyCount[village] + G.supplyCount[baron] + G.supplyCount[great_hall]";
    kingdomCount = G.supplyCount[adventurer] + G.supplyCount[council_room] + G.supplyCount[feast] +
      G.supplyCount[gardens] + G.supplyCount[mine] + G.supplyCount[remodel] + G.supplyCount[smithy] + G.supplyCount[village] + G.supplyCount[baron] +
      G.supplyCount[great_hall];
    assertTrue(origKingdomCount == kingdomCount, __LINE__, test);
    
    return 0;
}
