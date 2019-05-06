/**********************************************************************
Test of the Adventurer card.
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
    printf("cardtest2: cardtest2.c:%d: main Assertion '%s' failed\n", line, test);
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
    int bonus = 0;
    int empty = 0;
    int numCards = 5;
    int handDiff = 0;

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
    TEST1 positive verify Adventurer draws two treasure cards when they are at the 
    bottom of the deck
    ******************************************************************************/

    printf("***************cardtest2.c TEST1 start***************\n");

    //Set the top of the deck to non-treasure cards
    for (int i = 2; i < origDeckCount; i++)
    {
      G.deck[currentPlayer][i] = curse;
    }

    //Set the bottom two cards of the deck to silver
    G.deck[currentPlayer][1] = silver;
    G.deck[currentPlayer][0] = silver;
    
    //Call the playAdventurer method
    play = cardEffect(adventurer, empty, empty, empty, &G, handPos, &bonus);
    //Verify the method returned 0
    test = "play == 0";
    assertTrue(play == 0, __LINE__, test);

    //Verify other player's state did not change
    test = "orig2ndPlayerHandCount == G.handCount[otherPlayer]";
    assertTrue(orig2ndPlayerHandCount == G.handCount[otherPlayer], __LINE__, test);
    test = "orig2ndPlayerDeckCount == G.deckCount[otherPlayer]";
    assertTrue(orig2ndPlayerDeckCount == G.deckCount[otherPlayer], __LINE__, test);

    //Verify only one card added to played cards.
    playedCount = G.playedCardCount;
    test = "playedCount == playedNum";
    assertTrue(playedCount == playedNum, __LINE__, test);

    //Verify non-treasure cards in the deck were discarded
    test = "(origDeckCount - (G.handCount[currentPlayer] - origHandCount)) == G.discardCount[currentPlayer]";
    assertTrue((origDeckCount - (G.handCount[currentPlayer] - origHandCount)) == G.discardCount[currentPlayer], __LINE__, test);

    //Verify two cards were added to the hand, and one was removed (b/c the Adventurer card was played and removed from the hand)
    test = "(origHandCount + gainedCards - playedNum) == G.handCount[currentPlayer]";
    assertTrue((origHandCount + gainedCards - playedNum) == G.handCount[currentPlayer], __LINE__, test);

    //Verify those cards are the silvers we set earlier
    test = "G.hand[currentPlayer][G.handCount[currentPlayer] - 1] == silver && G.hand[currentPlayer][G.handCount[currentPlayer] - 2] == silver";
    assertTrue(G.hand[currentPlayer][G.handCount[currentPlayer] - 1] == silver && G.hand[currentPlayer][G.handCount[currentPlayer] - 2] == silver, __LINE__, test);

    //Verify the cards were removed from the player's deck
    deckCount = G.deckCount[currentPlayer];
    test = "deckCount == empty";
    assertTrue(deckCount == empty, __LINE__, test);

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
    TEST2 positive Verify Adventurer will pull two treasure cards when the deck has
    no treasure cards but the discard pile does
    ******************************************************************************/

    printf("***************cardtest2.c TEST2 start***************\n");

    //Set the game back to its initial state
    memcpy(&G, &initialG, sizeof(struct gameState));

    //Set the deck cards to curse
    for (int i = 0; i < origDeckCount; i++)
    {
      G.deck[currentPlayer][i] = curse;
    }

    //Set discard pile to have 5 gold cards
    G.discardCount[currentPlayer] = numCards;
    for (int i = 0; i < numCards; i++)
    {
      G.deck[currentPlayer][i] = gold;
    }

    //Call the playAdventurer method
    play = cardEffect(adventurer, empty, empty, empty, &G, handPos, &bonus);
    //Verify the method returned 0
    test = "play == 0";
    assertTrue(play == 0, __LINE__, test);

    //Verify other player's state did not change
    test = "orig2ndPlayerHandCount == G.handCount[otherPlayer]";
    assertTrue(orig2ndPlayerHandCount == G.handCount[otherPlayer], __LINE__, test);
    test = "orig2ndPlayerDeckCount == G.deckCount[otherPlayer]";
    assertTrue(orig2ndPlayerDeckCount == G.deckCount[otherPlayer], __LINE__, test);

    //Verify only one card added to played cards.
    playedCount = G.playedCardCount;
    test = "playedCount == playedNum";
    assertTrue(playedCount == playedNum, __LINE__, test);

    //Verify two cards were added to the hand, and one was removed (b/c the Adventurer card was played and removed from the hand)
    test = "(origHandCount + gainedCards - playedNum) == G.handCount[currentPlayer]";
    assertTrue((origHandCount + gainedCards - playedNum) == G.handCount[currentPlayer], __LINE__, test);

    //Verify those cards are the gold we set earlier
    test = "G.hand[currentPlayer][G.handCount[currentPlayer] - 1] == gold && G.hand[currentPlayer][G.handCount[currentPlayer] - 2] == gold";
    assertTrue(G.hand[currentPlayer][G.handCount[currentPlayer] - 1] == gold && G.hand[currentPlayer][G.handCount[currentPlayer] - 2] == gold, __LINE__, test);

    //Verify the cards from the original deck were discarded
    test = "G.discardCount[currentPlayer] == origHandCount";
    assertTrue(G.discardCount[currentPlayer] == origHandCount, __LINE__, test);

    //Verify the deck is now the original discards minus the two treasure cards
    test = "G.deckCount[currentPlayer] == numCards - gainedCards";
    assertTrue(G.deckCount[currentPlayer] == numCards - gainedCards, __LINE__, test);

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
    TEST3 boundry verify Adventurer will only pull one treasure card if there is only
    one in the deck/discards
    ******************************************************************************/

    printf("***************cardtest2.c TEST3 start***************\n");

    //Set the game back to its initial state
    memcpy(&G, &initialG, sizeof(struct gameState));

    //Set gained cards to 1
    gainedCards = 1;

    //Set the all cards but one to curse
    for (int i = 1; i < origDeckCount; i++)
    {
      G.deck[currentPlayer][i] = curse;
    }

    //Set one card to silver
    G.deck[currentPlayer][0] = silver;

    //Call the playAdventurer method
    play = cardEffect(adventurer, empty, empty, empty, &G, handPos, &bonus);
    //Verify the method returned 0
    test = "play == 0";
    assertTrue(play == 0, __LINE__, test);

    //Verify other player's state did not change
    test = "orig2ndPlayerHandCount == G.handCount[otherPlayer]";
    assertTrue(orig2ndPlayerHandCount == G.handCount[otherPlayer], __LINE__, test);
    test = "orig2ndPlayerDeckCount == G.deckCount[otherPlayer]";
    assertTrue(orig2ndPlayerDeckCount == G.deckCount[otherPlayer], __LINE__, test);

    //Verify only one card added to played cards.
    playedCount = G.playedCardCount;
    test = "playedCount == playedNum";
    assertTrue(playedCount == playedNum, __LINE__, test);

    //Verify one card was added to the hand, and one was removed (b/c the Adventurer card was played and removed from the hand)
    test = "(origHandCount + gainedCards - playedNum) == G.handCount[currentPlayer]";
    assertTrue((origHandCount + gainedCards - playedNum) == G.handCount[currentPlayer], __LINE__, test);

    //Verify that card is the silver we set earlier
    test = "G.hand[currentPlayer][G.handCount[currentPlayer] - 1] == silver";
    assertTrue(G.hand[currentPlayer][G.handCount[currentPlayer] - 1] == silver, __LINE__, test);

    //Verify the card was removed from the player's deck
    deckCount = G.deckCount[currentPlayer];
    test = "deckCount == empty";
    assertTrue(deckCount == empty, __LINE__, test);

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

