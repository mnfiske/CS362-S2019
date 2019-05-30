/**********************************************************************
Positive test of the Adventurer card.
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
    printf("unittest2: unittest2.c:%d: main Assertion '%s' failed\n", line, test);
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
    int play;
    int otherPlayer = 1;
    int orig2ndPlayerDeckCount;
    int orig2ndPlayerHandCount;
    char *test = malloc(256);
    int origVictoryCount = 0;
    int origKingdomCount = 0;
    int kingdomCount;
    int playedCount;
    int temphand[MAX_HAND];

    // initialize a new game
    initializeGame(numPlayer, k, seed, &initialG);
    //Track the  states for the players
    origHandCount = initialG.handCount[currentPlayer];
    origDeckCount = initialG.deckCount[currentPlayer];
    orig2ndPlayerHandCount = initialG.handCount[otherPlayer];
    orig2ndPlayerDeckCount = initialG.deckCount[otherPlayer];
    //Track the  states of the victory and kingdom cards
    origVictoryCount = initialG.supplyCount[estate] + initialG.supplyCount[duchy] + initialG.supplyCount[province];
    origKingdomCount = initialG.supplyCount[adventurer] + initialG.supplyCount[council_room] + initialG.supplyCount[feast] + initialG.supplyCount[gardens] +
      initialG.supplyCount[mine] + initialG.supplyCount[remodel] + initialG.supplyCount[smithy] + initialG.supplyCount[village] +
      initialG.supplyCount[baron] + initialG.supplyCount[great_hall];
    
    //Create a copy of the gamestate, to avoid having to initialize again
    memcpy(&G, &initialG, sizeof(struct gameState));

    /******************************************************************************
    TEST1 Verify Adventurer will pull two silver cards
    ******************************************************************************/

    printf("***************unittest2.c TEST1 start***************\n");

    //Set the all cards to silver
    for (int i = 0; i < origDeckCount; i++)
    {
      G.deck[currentPlayer][i] = silver;
    }
    
    //Call the playAdventurer method
    play = playAdventurer(currentPlayer, &G, handPos, temphand);
    //Verify the method returned 0
    test = "play == 0";
    assertTrue(play == 0, __LINE__, test);

    //Verify other player's state did not change
    test = "orig2ndPlayerHandCount == G.handCount[otherPlayer]";
    assertTrue(orig2ndPlayerHandCount == G.handCount[otherPlayer], __LINE__, test);
    test = "orig2ndPlayerDeckCount == G.deckCount[otherPlayer]";
    assertTrue(orig2ndPlayerDeckCount == G.deckCount[otherPlayer], __LINE__, test);

    //Verify only one card discarded.
    playedCount = G.playedCardCount;
    test = "playedCount == playedNum";
    assertTrue(playedCount == playedNum, __LINE__, test);

    //Verify two cards were added to the hand, and one was removed (b/c the Adventurer card was played and removed from the hand)
    test = "(origHandCount + gainedCards - playedNum) == G.handCount[currentPlayer]";
    assertTrue((origHandCount + gainedCards - playedNum) == G.handCount[currentPlayer], __LINE__, test);

    //Verify those cards are the silvers we set earlier
    test = "G.hand[currentPlayer][G.handCount[currentPlayer] - 1] == silver && G.hand[currentPlayer][G.handCount[currentPlayer] - 2] == silver";
    assertTrue(G.hand[currentPlayer][G.handCount[currentPlayer] - 1] == silver && G.hand[currentPlayer][G.handCount[currentPlayer] - 2] == silver, __LINE__, test);

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

    /******************************************************************************
    TEST2 Verify Adventurer will pull two gold cards
    ******************************************************************************/

    printf("***************unittest2.c TEST2 start***************\n");

    //Set the game back to its initial state
    memcpy(&G, &initialG, sizeof(struct gameState));

    //Set the all cards to gold
    for (int i = 0; i < origDeckCount; i++)
    {
      G.deck[currentPlayer][i] = gold;
    }

    //Call the playAdventurer method
    play = playAdventurer(currentPlayer, &G, handPos, temphand);
    //Verify the method returned 0
    test = "play == 0";
    assertTrue(play == 0, __LINE__, test);

    //Verify other player's state did not change
    test = "orig2ndPlayerHandCount == G.handCount[otherPlayer]";
    assertTrue(orig2ndPlayerHandCount == G.handCount[otherPlayer], __LINE__, test);
    test = "orig2ndPlayerDeckCount == G.deckCount[otherPlayer]";
    assertTrue(orig2ndPlayerDeckCount == G.deckCount[otherPlayer], __LINE__, test);

    //Verify only one card discarded.
    playedCount = G.playedCardCount;
    test = "playedCount == playedNum";
    assertTrue(playedCount == playedNum, __LINE__, test);

    //Verify two cards were added to the hand, and one was removed (b/c the Adventurer card was played and removed from the hand)
    test = "(origHandCount + gainedCards - playedNum) == G.handCount[currentPlayer]";
    assertTrue((origHandCount + gainedCards - playedNum) == G.handCount[currentPlayer], __LINE__, test);

    //Verify those cards are the golds we set earlier
    test = "G.hand[currentPlayer][G.handCount[currentPlayer] - 1] == gold && G.hand[currentPlayer][G.handCount[currentPlayer] - 2] == gold";
    assertTrue(G.hand[currentPlayer][G.handCount[currentPlayer] - 1] == gold && G.hand[currentPlayer][G.handCount[currentPlayer] - 2] == gold, __LINE__, test);

    //Verify the cards were removed from the player's deck
    deckCount = G.deckCount[currentPlayer];
    test = "deckCount == (origDeckCount - gainedCards)";
    assertTrue(deckCount == (origDeckCount - gainedCards), __LINE__, test);

    //Verify no state change occurred for victory cards
    test = "origVictoryCount = G.supplyCount[estate] + G.supplyCount[duchy] + G.supplyCount[province]";
    assertTrue(origVictoryCount == G.supplyCount[estate] + G.supplyCount[duchy] + G.supplyCount[province], __LINE__, test);

    //Verify no change for kingdom cards
    test = "origKingdomCount == G.supplyCount[adventurer] + G.supplyCount[council_room] + G.supplyCount[feast] + G.supplyCount[gardens] + G.supplyCount[mine] + G.supplyCount[remodel] + G.supplyCount[smithy] + G.supplyCount[village] + G.supplyCount[baron] + G.supplyCount[great_hall]";
    kingdomCount = G.supplyCount[adventurer] + G.supplyCount[council_room] + G.supplyCount[feast] +
      G.supplyCount[gardens] + G.supplyCount[mine] + G.supplyCount[remodel] + G.supplyCount[smithy] + G.supplyCount[village] + G.supplyCount[baron] +
      G.supplyCount[great_hall];
    assertTrue(origKingdomCount == kingdomCount, __LINE__, test);

    /******************************************************************************
    TEST3 Verify Adventurer will pull two copper cards
    ******************************************************************************/

    printf("***************unittest2.c TEST3 start***************\n");

    //Set the game back to its initial state
    memcpy(&G, &initialG, sizeof(struct gameState));

    //Set the all cards to copper
    for (int i = 0; i < origDeckCount; i++)
    {
      G.deck[currentPlayer][i] = copper;
    }

    //Call the playAdventurer method
    play = playAdventurer(currentPlayer, &G, handPos, temphand);
    //Verify the method returned 0
    test = "play == 0";
    assertTrue(play == 0, __LINE__, test);

    //Verify other player's state did not change
    test = "orig2ndPlayerHandCount == G.handCount[otherPlayer]";
    assertTrue(orig2ndPlayerHandCount == G.handCount[otherPlayer], __LINE__, test);
    test = "orig2ndPlayerDeckCount == G.deckCount[otherPlayer]";
    assertTrue(orig2ndPlayerDeckCount == G.deckCount[otherPlayer], __LINE__, test);

    //Verify only one card discarded.
    playedCount = G.playedCardCount;
    test = "playedCount == playedNum";
    assertTrue(playedCount == playedNum, __LINE__, test);

    //Verify two cards were added to the hand, and one was removed (b/c the Adventurer card was played and removed from the hand)
    test = "(origHandCount + gainedCards - playedNum) == G.handCount[currentPlayer]";
    assertTrue((origHandCount + gainedCards - playedNum) == G.handCount[currentPlayer], __LINE__, test);

    //Verify those cards are the silvers we set earlier
    test = "G.hand[currentPlayer][G.handCount[currentPlayer] - 1] == copper && G.hand[currentPlayer][G.handCount[currentPlayer] - 2] == copper";
    assertTrue(G.hand[currentPlayer][G.handCount[currentPlayer] - 1] == copper && G.hand[currentPlayer][G.handCount[currentPlayer] - 2] == copper, __LINE__, test);

    //Verify the cards were removed from the player's deck
    deckCount = G.deckCount[currentPlayer];
    test = "deckCount == (origDeckCount - gainedCards)";
    assertTrue(deckCount == (origDeckCount - gainedCards), __LINE__, test);

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

