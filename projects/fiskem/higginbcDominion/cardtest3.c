/**********************************************************************
Test of the Mine card.
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
    printf("cardtest3: cardtest3.c:%d: main Assertion '%s' failed\n", line, test);
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
    int handPos = 5;
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
    int choice1 = 0;
    int choice2 = 0;
    int choice3 = 0;
    int origTreasureSupply;

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
    
    //Create a copy of the gamestate, to avoid having to initialize again
    memcpy(&G, &initialG, sizeof(struct gameState));

    /******************************************************************************
    TEST1 boundary set choice1 to a non-treasure card
    ******************************************************************************/

    printf("***************cardtest3.c TEST1 start***************\n");

    //Set the choice1 to a non-treasure card
    G.hand[currentPlayer][0] = great_hall;
    choice1 = 0;

    choice2 = silver;
    
    //Call the mine
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
    assertTrue(origVictoryCount == G.supplyCount[estate] + G.supplyCount[duchy] + G.supplyCount[province],  __LINE__, test);

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
   TEST2 boundry give a choice2 that is too expensive
   ******************************************************************************/

    printf("***************cardtest3.c TEST2 start***************\n");

    //Set the game back to its initial state
    memcpy(&G, &initialG, sizeof(struct gameState));

    //Set the choice1 to a treasure card
    G.hand[currentPlayer][0] = copper;
    choice1 = 0;

    //Set choice2 to a treasure card that's too expensive
    choice2 = gold;

    //Call the mine
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
    
    /******************************************************************************
    TEST3 boundary give a non-treasure choice2
    ******************************************************************************/

    printf("***************cardtest3.c TEST3 start***************\n");

    //Set the game back to its initial state
    memcpy(&G, &initialG, sizeof(struct gameState));

    //Set the choice1 to a treasure card
    G.hand[currentPlayer][0] = copper;
    choice1 = 0;

    choice2 = mine;

    //Call the mine
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

