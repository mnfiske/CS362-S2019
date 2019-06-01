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
    printf("randomtestadventurer: randomtestadventurer.c:%d: main Assertion '%s' failed\n", line, test);
  }
}

int checkAdventurer(int p, struct gameState *post, int handPos, int temphand[])
{
  char *test = malloc(256);
  int intendedGainedCards = 2;
  int playedNum = 1;

  struct gameState pre;
  pre.whoseTurn = p;
  memcpy(&pre, post, sizeof(struct gameState));
  int r;

  //Track the number of treasures in the deck
  int numTreasuresDeck = 0;
  for (int i = 0; i < pre.deckCount[p]; i++)
  {
    if (pre.deck[p][i] == copper || pre.deck[p][i] == silver || pre.deck[p][i] == gold)
    {
      numTreasuresDeck++;
    }
  }

  //If there are fewer than two treasures in the deck, track teasures in discards
  int numTreasuresDiscard = 0;
  if(numTreasuresDeck < 2)
  {
    numTreasuresDiscard = 0;
    for (int i = 0; i < pre.discardCount[p]; i++)
    {
      if (pre.discard[p][i] == copper || pre.discard[p][i] == silver || pre.discard[p][i] == gold)
      {
        numTreasuresDiscard++;
      }
    }
  }

  r = adventurerEffect(handPos, post);

  test = "r == 0";
  assertTrue(r == 0, __LINE__, test);

  test = "playedNum == post->playedCardCount";
  assertTrue(playedNum == post->playedCardCount, __LINE__, test);

  //To avoid having multiple tests fail due to a single bug that is explictly checked for (more than the intended
  //number of cards being drawn), set variable to the actual cards drawn for checking things like the cards being
  //drawn from the deck.
  int gainedCards = post->handCount[p] - pre.handCount[p];
  //test = "intendedGainedCards == gainedCards";
  //assertTrue(intendedGainedCards == gainedCards, __LINE__, test);
  
  //No treasures in deck/discards
  if (numTreasuresDeck + numTreasuresDiscard == 0)
  {
    //No changes to hand
    test = "pre.handCount[p] - playedNum == post->handCount[p]";
    assertTrue(pre.handCount[p] - playedNum == post->handCount[p], __LINE__, test);
    //All cards discarded
    test = "post->deckCount[p] == 0";
    assertTrue(post->deckCount[p] == 0, __LINE__, test);
    test = "post->discardCount[p] == pre.deckCount[p] + pre.discardCount[p]";
    assertTrue(post->discardCount[p] == pre.deckCount[p] + pre.discardCount[p], __LINE__, test);
  }
  else if (numTreasuresDeck + numTreasuresDiscard == 1)
  {
    //Player gained one card to their hand
    test = "pre.handCount[p] + 1 - playedNum == post->handCount[p]";
    assertTrue(pre.handCount[p] + 1 - playedNum == post->handCount[p], __LINE__, test);
    //That card is a treasure card
    test = "post->hand[p][post->handCount[p] - 1] == copper || post->hand[p][post->handCount[p] - 1] == silver || post->hand[p][post->handCount[p] - 1] == gold";
    assertTrue(post->hand[p][post->handCount[p] - 1] == copper || post->hand[p][post->handCount[p] - 1] == silver || post->hand[p][post->handCount[p] - 1] == gold, __LINE__, test);

    if (numTreasuresDeck == 1)
    {
      int cardsAddedToDiscards = post->discardCount[p] - pre.discardCount[p];
      //Cards were removed from the deck
      test = "pre.deckCount[p] - cardsAddedToDiscards - 1 == post->deckCount[p]";
      assertTrue(pre.deckCount[p] - cardsAddedToDiscards - 1 == post->deckCount[p], __LINE__, test);
      //If the top card of the deck wasn't treasures, the discard pile should have increased, otherwise should be the same
      int origDeckTopCardTreasure = pre.deck[p][pre.deckCount[p] - 1] == copper || pre.deck[p][pre.deckCount[p] - 1] == silver || pre.deck[p][pre.deckCount[p] - 1] == gold;
      if (!(origDeckTopCardTreasure))
      {
        test = "pre.discardCount[p] > post->discardCount[p]";
        assertTrue(pre.discardCount[p] > post->discardCount[p], __LINE__, test);
      }
      else
      {
        test = "pre.discardCount[p] == post->discardCount[p]";
        assertTrue(pre.discardCount[p] == post->discardCount[p], __LINE__, test);
      }
    }
    else
    {
      //One cards removed from combined deck and discards
      test = "pre.deckCount[p] + pre.discardCount[p] - 1 == post->deckCount[p] + post->discardCount[p]";
      assertTrue(pre.deckCount[p] + pre.discardCount[p] - 1 == post->deckCount[p] + post->discardCount[p], __LINE__, test);
    }
  }
  else
  {
    //Player gained two cards to their hand and discarded one card
    test = "pre.handCount[p] + intendedGainedCards - playedNum == post->handCount[p]";
    assertTrue(pre.handCount[p] + intendedGainedCards - playedNum == post->handCount[p], __LINE__, test);
    //Those cards are treasure cards
    int lastCardTreasure = post->hand[p][post->handCount[p] - 1] == copper || post->hand[p][post->handCount[p] - 1] == silver || post->hand[p][post->handCount[p] - 1] == gold;
    int secondLastCardTreasure = post->hand[p][post->handCount[p] - 2] == copper || post->hand[p][post->handCount[p] - 2] == silver || post->hand[p][post->handCount[p] - 2] == gold;
    test = "lastCardTreasure && secondLastCardTreasure";
    assertTrue(lastCardTreasure && secondLastCardTreasure, __LINE__, test);

    //The deck contained at least two treasures, treasures should be drawn from the deck
    if (numTreasuresDeck >= gainedCards)
    {
      int cardsAddedToDiscards = post->discardCount[p] - pre.discardCount[p];
      //Cards were removed from the deck
      test = "pre.deckCount[p] - cardsAddedToDiscards - gainedCards ==  post->deckCount[p]";
      assertTrue(pre.deckCount[p] - cardsAddedToDiscards - gainedCards ==  post->deckCount[p], __LINE__, test);
      //If the top two cards of the deck weren't treasures, the discard pile should have increased, otherwise should be the same
      int origDeckTopCardTreasure = pre.deck[p][pre.deckCount[p] - 1] == copper || pre.deck[p][pre.deckCount[p] - 1] == silver || pre.deck[p][pre.deckCount[p] - 1] == gold;
      int origDeck2ndCardTreasure = pre.deck[p][pre.deckCount[p] - 2] == copper || pre.deck[p][pre.deckCount[p] - 2] == silver || pre.deck[p][pre.deckCount[p] - 2] == gold;
      if (!(origDeckTopCardTreasure && origDeck2ndCardTreasure))
      {
        //If the top two weren't treasures, some number of cards should have been discarded
        test = "pre.discardCount[p] > post->discardCount[p]";
        assertTrue(pre.discardCount[p] < post->discardCount[p], __LINE__, test);
      }
      else
      {
        test = "pre.discardCount[p] == post->discardCount[p]";
        assertTrue(pre.discardCount[p] == post->discardCount[p], __LINE__, test);
      }
    }
    //The deck had less than two treasures, but the deck and discards combined have at least two treasures
    //The discards should have been shuffled into the deck and cards should have been removed
    else
    {
      //Two cards removed from combined deck and discards
      test = "pre.deckCount[p] + pre.discardCount[p] - gainedCards == post->deckCount[p] + post->discardCount[p]";
      assertTrue(pre.deckCount[p] + pre.discardCount[p] - gainedCards == post->deckCount[p] + post->discardCount[p], __LINE__, test);
    }
  }

  return 0;

}

int main() {
  int n, p;

  int k[10] = { adventurer, council_room, feast, gardens, mine,
         remodel, smithy, village, baron, great_hall };
  int temphand[MAX_HAND];
  int handPos = -1;
  int seed = rand() % 10000;

  struct gameState G;
  initializeGame(2, k, seed, &G);

  printf("Testing adventurerEffect.\n");

  printf("RANDOM TESTS.\n");

  SelectStream(2);
  PutSeed(3);

  for (n = 0; n < 800; n++)
  {
    printf("Test %d\n", n);
    p = rand() % 2;
    G.whoseTurn = p;
    G.deckCount[p] = rand() % MAX_DECK;
    for (int i = 0; i < G.deckCount[p]; i++)
    {
      G.deck[p][i] = rand() % treasure_map;
    }
    G.discardCount[p] = rand() % (MAX_DECK - G.deckCount[p]);
    G.handCount[p] = rand() % MAX_HAND;
    if (G.handCount[p] < 5)
    {
      G.handCount[p] = 5;
    }
    for (int i = 0; i < G.handCount[p]; i++)
    {
      //If taking too long to run, bump treasure_map down
      G.hand[p][i] = rand() % treasure_map;
    }
    handPos = rand() % G.handCount[p];
    G.hand[p][handPos] = adventurer;
    checkAdventurer(p, &G, handPos, temphand);
  }
  return 0;
}

    


