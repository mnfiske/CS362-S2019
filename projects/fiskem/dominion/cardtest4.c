/**********************************************************************
Test of the Remodel card.
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
    printf("cardtest4: cardtest4.c:%d: main Assertion '%s' failed\n", line, test);
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
    int gainedCards = 0;
    int handPos = 5;
    int discardNum = 1;
    int play;
    int otherPlayer = 1;
    int orig2ndPlayerDeckCount;
    int orig2ndPlayerHandCount;
    char *test = malloc(256);
    int origVictoryCount = 0;
    int origKingdomCount = 0;
    int kingdomCount;
    int playedCount;
    int bonus = 0;
    int empty = 0;
    int numCards = 5;
    int handDiff = 0;
    int choice1 = 0;
    int choice2 = 0;
    int choice3 = 0;
    int origTreasureSupply;
    int origProvinceSupply;

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

    origTreasureSupply = initialG.supplyCount[copper] + initialG.supplyCount[silver] + initialG.supplyCount[gold];
    
    origProvinceSupply = initialG.supplyCount[province];
    
    //Create a copy of the gamestate, to avoid having to initialize again
    memcpy(&G, &initialG, sizeof(struct gameState));

    /******************************************************************************
    TEST1 positive exchange for card exactly two more expensive
    ******************************************************************************/

    printf("***************cardtest4.c TEST1 start***************\n");

    //Set the choice1
    G.hand[currentPlayer][0] = gold;
    choice1 = 0;

    //Set choice2
    choice2 = province;
    
    //Call the card
    play = cardEffect(remodel, choice1, choice2, choice3, &G, handPos, &bonus);
    //Verify the method returned 0
    test = "play == 0";
    assertTrue(play == 0, __LINE__, test);

    //Verify choice2 was added to discards
    test = "G.discard[currentPlayer][0] == province";
    assertTrue(G.discard[currentPlayer][0] == province, __LINE__, test);

    //Verify remodel card was added to played cards
    playedCount = G.playedCardCount;
    test = "playedCount == playedNum";
    assertTrue(playedCount == playedNum, __LINE__, test);

    //Verify other player's state did not change
    test = "orig2ndPlayerHandCount == G.handCount[otherPlayer]";
    assertTrue(orig2ndPlayerHandCount == G.handCount[otherPlayer], __LINE__, test);
    test = "orig2ndPlayerDeckCount == G.deckCount[otherPlayer]";
    assertTrue(orig2ndPlayerDeckCount == G.deckCount[otherPlayer], __LINE__, test);

    //Verify two card were removed from the hand, the trashed card and the card played
    test = "origHandCount - 2 == G.handCount[currentPlayer]";
    assertTrue(origHandCount - 2 == G.handCount[currentPlayer], __LINE__, test);

    //Verify no cards were removed from the player's deck
    deckCount = G.deckCount[currentPlayer];
    test = "deckCount == origDeckCount)";
    assertTrue(deckCount == origDeckCount, __LINE__, test);

    //Verify province was removed from supply
    test = "origProvinceSupply - 1 == G.supplyCount[province]";
    assertTrue(origProvinceSupply - 1 == G.supplyCount[province], __LINE__, test);

    //Verify no other change occurred for victory cards
    test = "origVictoryCount - 1 == G.supplyCount[estate] + G.supplyCount[duchy] + G.supplyCount[province]";
    assertTrue(origVictoryCount - 1 == G.supplyCount[estate] + G.supplyCount[duchy] + G.supplyCount[province], __LINE__, test);

    //Verify no change for kingdom cards
    test = "origKingdomCount == G.supplyCount[adventurer] + G.supplyCount[council_room] + G.supplyCount[feast] + G.supplyCount[gardens] + G.supplyCount[mine] + G.supplyCount[remodel] + G.supplyCount[smithy] + G.supplyCount[village] + G.supplyCount[baron] + G.supplyCount[great_hall]";
    kingdomCount = G.supplyCount[adventurer] + G.supplyCount[council_room] + G.supplyCount[feast] +
      G.supplyCount[gardens] + G.supplyCount[mine] + G.supplyCount[remodel] + G.supplyCount[smithy] + G.supplyCount[village] + G.supplyCount[baron] + 
      G.supplyCount[great_hall];
    assertTrue(origKingdomCount == kingdomCount, __LINE__, test);

    //Verify no change for treasure supply
    test = "origTreasureSupply == G.supplyCount[copper] + G.supplyCount[silver] + G.supplyCount[gold]";
    assertTrue(origTreasureSupply == G.supplyCount[copper] + G.supplyCount[silver] + G.supplyCount[gold], __LINE__, test);

   /******************************************************************************
   TEST2 positive exchange for a card cheaper than trashed card
   ******************************************************************************/

    printf("***************cardtest4.c TEST2 start***************\n");

    //Set the game back to its initial state
    memcpy(&G, &initialG, sizeof(struct gameState));

    //Set the choice1
    G.hand[currentPlayer][0] = gold;
    choice1 = 0;

    //Set choice2
    choice2 = copper;

    //Call the card
    play = cardEffect(remodel, choice1, choice2, choice3, &G, handPos, &bonus);
    //Verify the method returned 0
    test = "play == 0";
    assertTrue(play == 0, __LINE__, test);

    //Verify choice2 was added to discards
    test = "G.discard[currentPlayer][0] == copper";
    assertTrue(G.discard[currentPlayer][0] == copper, __LINE__, test);

    //Verify remodel card was added to played cards
    playedCount = G.playedCardCount;
    test = "playedCount == playedNum";
    assertTrue(playedCount == playedNum, __LINE__, test);

    //Verify other player's state did not change
    test = "orig2ndPlayerHandCount == G.handCount[otherPlayer]";
    assertTrue(orig2ndPlayerHandCount == G.handCount[otherPlayer], __LINE__, test);
    test = "orig2ndPlayerDeckCount == G.deckCount[otherPlayer]";
    assertTrue(orig2ndPlayerDeckCount == G.deckCount[otherPlayer], __LINE__, test);

    //Verify two card were removed from the hand, the trashed card and the card played
    test = "origHandCount - 2 == G.handCount[currentPlayer]";
    assertTrue(origHandCount - 2 == G.handCount[currentPlayer], __LINE__, test);

    //Verify no cards were removed from the player's deck
    deckCount = G.deckCount[currentPlayer];
    test = "deckCount == origDeckCount)";
    assertTrue(deckCount == origDeckCount, __LINE__, test);

    //Verify copper was removed from supply
    test = "origProvinceSupply - 1 == G.supplyCount[copper]";
    assertTrue(origProvinceSupply - 1 == G.supplyCount[copper], __LINE__, test);

    //Verify no change occurred for victory cards
    test = "origVictoryCount == G.supplyCount[estate] + G.supplyCount[duchy] + G.supplyCount[province]";
    assertTrue(origVictoryCount == G.supplyCount[estate] + G.supplyCount[duchy] + G.supplyCount[province], __LINE__, test);

    //Verify no change for kingdom cards
    test = "origKingdomCount == G.supplyCount[adventurer] + G.supplyCount[council_room] + G.supplyCount[feast] + G.supplyCount[gardens] + G.supplyCount[mine] + G.supplyCount[remodel] + G.supplyCount[smithy] + G.supplyCount[village] + G.supplyCount[baron] + G.supplyCount[great_hall]";
    kingdomCount = G.supplyCount[adventurer] + G.supplyCount[council_room] + G.supplyCount[feast] +
      G.supplyCount[gardens] + G.supplyCount[mine] + G.supplyCount[remodel] + G.supplyCount[smithy] + G.supplyCount[village] + G.supplyCount[baron] +
      G.supplyCount[great_hall];
    assertTrue(origKingdomCount == kingdomCount, __LINE__, test);

    //Verify no other change for treasure supply
    test = "origTreasureSupply - 1 == G.supplyCount[copper] + G.supplyCount[silver] + G.supplyCount[gold]";
    assertTrue(origTreasureSupply - 1 == G.supplyCount[copper] + G.supplyCount[silver] + G.supplyCount[gold], __LINE__, test);
    
    /******************************************************************************
    TEST3 boundary set choice2 to a card more than 2 greater than choice1
    ******************************************************************************/

    printf("***************cardtest4.c TEST3 start***************\n");

    //Set the game back to its initial state
    memcpy(&G, &initialG, sizeof(struct gameState));

    //Set the choice1 to a treasure card
    G.hand[currentPlayer][0] = copper;
    choice1 = 0;

    choice2 = gold;

    //Call the card
    play = cardEffect(mine, choice1, choice2, choice3, &G, handPos, &bonus);
    //Verify the method returned -1
    test = "play == -1";
    assertTrue(play == -1, __LINE__, test);

    //Verify no state was changed

    //Verify other player's state did not change
    test = "orig2ndPlayerHandCount == G.handCount[otherPlayer]";
    assertTrue(orig2ndPlayerHandCount == G.handCount[otherPlayer], __LINE__, test);
    test = "orig2ndPlayerDeckCount == G.deckCount[otherPlayer]";
    assertTrue(orig2ndPlayerDeckCount == G.deckCount[otherPlayer], __LINE__, test);

    //Verify no card discarded.
    playedCount = G.playedCardCount;
    test = "playedCount == empty";
    assertTrue(playedCount == empty, __LINE__, test);

    //Verify no cards were added to the hand
    test = "origHandCount == G.handCount[currentPlayer]";
    assertTrue(origHandCount == G.handCount[currentPlayer], __LINE__, test);

    //Verify no cards were removed from the player's deck
    deckCount = G.deckCount[currentPlayer];
    test = "deckCount == origDeckCount)";
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

    //Verify no change for treasure supply
    test = "origTreasureSupply == G.supplyCount[copper] + G.supplyCount[silver] + G.supplyCount[gold]";
    assertTrue(origTreasureSupply == G.supplyCount[copper] + G.supplyCount[silver] + G.supplyCount[gold], __LINE__, test);
    
    return 0;
}

