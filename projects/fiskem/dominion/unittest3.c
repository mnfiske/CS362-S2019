/**********************************************************************
Positive test of the Council Room card.
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
    printf("unittest3: unittest3.c:%d: main Assertion '%s' failed\n", line, test);
  }
}

int main() {

    int seed = 1000;
    int numPlayer = 2;
    int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
    struct gameState G, initialG;
    int origHandCount;
    int player0 = 0;
    int player1 = 1;
    int player2 = 3;
    int player3 = 3;
    int deckCount = 0;
    int origDeckCount;
    int orig2ndPlayerDeckCount;
    int orig2ndPlayerHandCount;
    int orig3rdPlayerDeckCount;
    int orig3rdPlayerHandCount;
    int orig4thPlayerDeckCount;
    int orig4thPlayerHandCount;
    char *test = malloc(256);
    int origVictoryCount = 0;
    int origKingdomCount = 0;
    int origDiscards;
    int origBuys;
    int playedNum = 1;
    int gainedCards = 4;
    int otherPlayerGainedCards = 1;
    int addedBuys = 1;
    int handPos = 0;
    int discardNum = 1;
    int play;
    int kingdomCount;
    int playedCount;
    int empty = 0;
    //int test2DeckCount;

    // initialize a new game
    initializeGame(numPlayer, k, seed, &initialG);

    //Create a copy of the gamestate, to avoid having to initialize again
    memcpy(&G, &initialG, sizeof(struct gameState));

    /*****************************************************************************
     TEST1 test Council Room with default beginning game deck/hand, player 0, two players
    *****************************************************************************/

    printf("***************unittest3.c TEST1 start***************\n");

    //Track the  states for the players
    origHandCount = G.handCount[player0];
    origDeckCount = G.deckCount[player0];
    orig2ndPlayerHandCount = G.handCount[player1];
    orig2ndPlayerDeckCount = G.deckCount[player1];
    //Track the  states of the victory and kingdom cards
    origVictoryCount = G.supplyCount[estate] + G.supplyCount[duchy] + G.supplyCount[province];
    origKingdomCount = G.supplyCount[adventurer] + G.supplyCount[council_room] + G.supplyCount[feast] + G.supplyCount[gardens] +
                       G.supplyCount[mine] + G.supplyCount[remodel] + G.supplyCount[smithy] + G.supplyCount[village] +
                       G.supplyCount[baron] + G.supplyCount[great_hall];
    //Track player 0's buy count
    origBuys = G.numBuys;

    //Call the playCouncilRoom method
    play = playCouncilRoom(player0, &G, handPos);
    //Verify the method returned 0
    test = "play == 0";
    assertTrue(play == 0, __LINE__, test);

    //Verify the other player gained a card to their hand
    test = "G.handCount[player1] - otherPlayerGainedCards == orig2ndPlayerHandCount";
    assertTrue(G.handCount[player1] - otherPlayerGainedCards == orig2ndPlayerHandCount, __LINE__, test);

    //Verify the other player gained that card from their deck
    test = "orig2ndPlayerDeckCount - otherPlayerGainedCards == G.deckCount[player1]";
    assertTrue(orig2ndPlayerDeckCount - otherPlayerGainedCards == G.deckCount[player1], __LINE__, test);

    //Verify one card discarded
    playedCount = G.playedCardCount;
    test = "playedCount == playedNum";
    assertTrue(playedCount == playedNum, __LINE__, test);

    //Verify four cards were added to the hand, and one was removed (b/c the card was played and removed from the hand)
    test = "(origHandCount + gainedCards - playedNum) == G.handCount[player0]";
    assertTrue((origHandCount + gainedCards - playedNum) == G.handCount[player0], __LINE__, test);

    //Verify the cards were removed from the player's deck
    deckCount = G.deckCount[player0];
    test = "deckCount == (origDeckCount - gainedCards)";
    assertTrue(deckCount == (origDeckCount - gainedCards), __LINE__, test);

    //Verify the player had one buy added to their turn
    test = "origBuys + addedBuys == G.numBuys";
    assertTrue(origBuys + addedBuys == G.numBuys, __LINE__, test);

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
     TEST2 Ensure Council Room correctly gives all players an additional card when the player is not player 0
    ******************************************************************************************************/

    printf("***************unittest3.c TEST2 start***************\n");
    
    //Return to beginning state
    memcpy(&G, &initialG, sizeof(struct gameState));

    //move to the next player's turn
    endTurn(&G);

    //Track the  states for the players
    origHandCount = G.handCount[player1];
    orig2ndPlayerHandCount = G.handCount[player0];
    orig2ndPlayerDeckCount = G.deckCount[player0];
    //Track the  states of the victory and kingdom cards
    origVictoryCount = G.supplyCount[estate] + G.supplyCount[duchy] + G.supplyCount[province];
    origKingdomCount = G.supplyCount[adventurer] + G.supplyCount[council_room] + G.supplyCount[feast] + G.supplyCount[gardens] +
      G.supplyCount[mine] + G.supplyCount[remodel] + G.supplyCount[smithy] + G.supplyCount[village] +
      G.supplyCount[baron] + G.supplyCount[great_hall];

    //Track player 1's buy count
    origBuys = G.numBuys;

    //Call the playCouncilRoom method
    play = playCouncilRoom(player1, &G, handPos);
    //Verify the method returned 0
    test = "play == 0";
    assertTrue(play == 0, __LINE__, test);

    //Verify the other player gained a card to their hand
    test = "G.handCount[player0] - otherPlayerGainedCards == orig2ndPlayerHandCount";
    assertTrue(G.handCount[player0] - otherPlayerGainedCards == orig2ndPlayerHandCount, __LINE__, test);

    //Verify the other player gained that card from their deck
    test = "orig2ndPlayerDeckCount - otherPlayerGainedCards == G.deckCount[player0]";
    assertTrue(orig2ndPlayerDeckCount - otherPlayerGainedCards == G.deckCount[player0], __LINE__, test);

    //Verify one card discarded
    playedCount = G.playedCardCount;
    test = "playedCount == playedNum";
    assertTrue(playedCount == playedNum, __LINE__, test);

    //Verify four cards were added to the hand, and one was removed (b/c the card was played and removed from the hand)
    test = "(origHandCount + gainedCards - playedNum) == G.handCount[player1]";
    assertTrue((origHandCount + gainedCards - playedNum) == G.handCount[player1], __LINE__, test);

    //Verify the cards were removed from the player's deck
    deckCount = G.deckCount[player1];
    test = "deckCount == (origDeckCount - gainedCards)";
    assertTrue(deckCount == (origDeckCount - gainedCards), __LINE__, test);

    //Verify the player had one buy added to their turn
    test = "origBuys + addedBuys == G.numBuys";
    assertTrue(origBuys + addedBuys == G.numBuys, __LINE__, test);

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
    TEST3 Ensure Council Room correctly gives all players an additional card when the game has max players
          Note: If MAX_PLAYERS is increased beyond four, this test will need to be refactored.
    ******************************************************************************************************/

    printf("***************unittest3.c TEST3 start***************\n");

    //Validate MAX_PLAYERS is four, so devs are notified if this test needs to be modified
    test = "MAX_PLAYERS has increased beyond 4, unittest3.c test 3 needs to be refactored.";
    assertTrue(MAX_PLAYERS == 4, __LINE__, test);

    //Initialize a new game
    initializeGame(MAX_PLAYERS, k, seed, &G);

    //Track the original states for the players
    origHandCount = G.handCount[player0];
    orig2ndPlayerHandCount = G.handCount[player1];
    orig2ndPlayerDeckCount = G.deckCount[player1];
    orig3rdPlayerHandCount = G.handCount[player2];
    orig3rdPlayerDeckCount = G.deckCount[player2];
    orig4thPlayerHandCount = G.handCount[player3];
    orig4thPlayerDeckCount = G.deckCount[player3];
    //Track the  states of the victory and kingdom cards
    origVictoryCount = G.supplyCount[estate] + G.supplyCount[duchy] + G.supplyCount[province];
    origKingdomCount = G.supplyCount[adventurer] + G.supplyCount[council_room] + G.supplyCount[feast] + G.supplyCount[gardens] +
      G.supplyCount[mine] + G.supplyCount[remodel] + G.supplyCount[smithy] + G.supplyCount[village] +
      G.supplyCount[baron] + G.supplyCount[great_hall];

    //Track player 0's buy count
    origBuys = G.numBuys;

    //Call the playCouncilRoom method
    play = playCouncilRoom(player0, &G, handPos);
    //Verify the method returned 0
    test = "play == 0";
    assertTrue(play == 0, __LINE__, test);

    //Verify the other players gained a card to their hands
    test = "G.handCount[player1] - otherPlayerGainedCards == orig2ndPlayerHandCount";
    assertTrue(G.handCount[player1] - otherPlayerGainedCards == orig2ndPlayerHandCount, __LINE__, test);
    test = "G.handCount[player2] - otherPlayerGainedCards == orig3rdPlayerHandCount";
    assertTrue(G.handCount[player2] - otherPlayerGainedCards == orig3rdPlayerHandCount, __LINE__, test);
    test = "G.handCount[player3] - otherPlayerGainedCards == orig4thPlayerHandCount";
    assertTrue(G.handCount[player3] - otherPlayerGainedCards == orig4thPlayerHandCount, __LINE__, test);

    //Verify the other players gained those cards from their deck
    test = "orig2ndPlayerDeckCount - otherPlayerGainedCards == G.deckCount[player1]";
    assertTrue(orig2ndPlayerDeckCount - otherPlayerGainedCards == G.deckCount[player1], __LINE__, test);
    test = "orig3rdPlayerDeckCount - otherPlayerGainedCards == G.deckCount[player2]";
    assertTrue(orig3rdPlayerDeckCount - otherPlayerGainedCards == G.deckCount[player2], __LINE__, test);
    test = "orig4thPlayerDeckCount - otherPlayerGainedCards == G.deckCount[player3]";
    assertTrue(orig4thPlayerDeckCount - otherPlayerGainedCards == G.deckCount[player3], __LINE__, test);

    //Verify one card discarded
    playedCount = G.playedCardCount;
    test = "playedCount == playedNum";
    assertTrue(playedCount == playedNum, __LINE__, test);

    //Verify four cards were added to the hand, and one was removed (b/c the card was played and removed from the hand)
    test = "(origHandCount + gainedCards - playedNum) == G.handCount[player0]";
    assertTrue((origHandCount + gainedCards - playedNum) == G.handCount[player0], __LINE__, test);

    //Verify the cards were removed from the player's deck
    deckCount = G.deckCount[player0];
    test = "deckCount == (origDeckCount - gainedCards)";
    assertTrue(deckCount == (origDeckCount - gainedCards), __LINE__, test);

    //Verify the player had one buy added to their turn
    test = "origBuys + addedBuys == G.numBuys";
    assertTrue(origBuys + addedBuys == G.numBuys, __LINE__, test);

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
