/**********************************************************************
One positive, one boundary, and one negative test of the Steward card.
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
    printf("unittest4: unittest4.c:%d: main Assertion '%s' failed\n", line, test);
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
    int gainedCards = 2;
    int handPos = 0;
    int discardNum = 1;
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
    int test2DeckCount;
    int choice1 = 0;
    int choice2 = 0;
    int choice3 = 0;
    int smallDeck = 2;
    int insufficientDeck = 1;

    // initialize a new game
    initializeGame(numPlayer, k, seed, &initialG);

    //Create a copy of the gamestate, to avoid having to initialize again
    memcpy(&G, &initialG, sizeof(struct gameState));

    /*****************************************************************************
     TEST1 test Steward with choice1 == 1 and exactly two cards in deck, no discards
    *****************************************************************************/

    printf("***************unittest4.c TEST1 start***************\n");

    //Set deck size to 2
    G.deckCount[currentPlayer] = smallDeck;

    //Verify discards are empty
    test = "G.discard[currentPlayer] == empty";
    assertTrue(G.discardCount[currentPlayer] == empty, __LINE__, test);

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

    //Set choice to draw two cards
    choice1 = 1;

    //Call the playSteward method
    play = playSteward(currentPlayer, &G, handPos, choice1, choice2, choice3);
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
    test = "playedCount == playedNum";
    assertTrue(playedCount == playedNum, __LINE__, test);

    //Verify two cards were added to the hand, and one was removed
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
     TEST2 test Steward with choice1 == 1 and exactly only card in deck, no discards
    ******************************************************************************************************/

    printf("***************unittest4.c TEST2 start***************\n");
    
    //Return to beginning state
    memcpy(&G, &initialG, sizeof(struct gameState));

    //Set deck size to 1
    G.deckCount[currentPlayer] = insufficientDeck;

    //Verify discards are empty
    test = "G.discard[currentPlayer] == empty";
    assertTrue(G.discardCount[currentPlayer] == empty, __LINE__, test);

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

    //Set choice to draw two cards
    choice1 = 1;

    //Call the playSteward method
    play = playSteward(currentPlayer, &G, handPos, choice1, choice2, choice3);
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
    test = "playedCount == playedNum";
    assertTrue(playedCount == playedNum, __LINE__, test);

    //Verify only one card was added to the hand since there was only one to draw, and one was removed
    test = "(origHandCount + insufficientDeck - playedNum) == G.handCount[currentPlayer]";
    assertTrue((origHandCount + insufficientDeck - playedNum) == G.handCount[currentPlayer], __LINE__, test);

    //Verify the cards were removed from the player's deck
    deckCount = G.deckCount[currentPlayer];
    test = "deckCount == (origDeckCount - insufficientDeck)";
    assertTrue(deckCount == (origDeckCount - insufficientDeck), __LINE__, test);

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
    TEST3 Ensure Steward gracefully handles invalid input of choice1 == 3 (trash two cards) and choices 2 
    and 3 with values outside the hand range.
    ******************************************************************************************************/

    printf("***************unittest4.c TEST3 start***************\n");

    //Return to beginning state
    memcpy(&G, &initialG, sizeof(struct gameState));

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

    //Set choice to draw trash cards
    choice1 = 3;
    choice2 = MAX_HAND + 1;
    choice3 = MAX_HAND + 2;

    //Call the playSteward method
    play = playSteward(currentPlayer, &G, handPos, choice1, choice2, choice3);
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
    test = "playedCount == playedNum";
    assertTrue(playedCount == playedNum, __LINE__, test);

    //Verify no cards were added to the hand, and one was removed
    test = "(origHandCount - playedNum) == G.handCount[currentPlayer]";
    assertTrue((origHandCount - playedNum) == G.handCount[currentPlayer], __LINE__, test);

    //Verify no cards were removed from the player's deck
    deckCount = G.deckCount[currentPlayer];
    test = "deckCount == origDeckCount";
    assertTrue(deckCount == origDeckCount, __LINE__, test);

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
