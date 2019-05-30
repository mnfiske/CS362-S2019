/**********************************************************************
Test of the Smithy card.
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
    printf("randomcardtest1: randomcardtest1.c:%d: main Assertion '%s' failed\n", line, test);
  }
}

int checkSmithy(int p, struct gameState *post, int handPos)
{
  char *test = malloc(256);
  int intendedGainedCards = 3;

  struct gameState pre;
  memcpy(&pre, post, sizeof(struct gameState));
  int r;
  int playedNum = 1;

  int preDeck = pre.deckCount[p];
  int preHand = pre.handCount[p];

  r = smithyEffect(p, post, handPos);

  int postDeck = post->deckCount[p];
  int postHand = post->handCount[p];

  test = "r == 0";
  assertTrue(r == 0, __LINE__, test);

  //Smithy should have been discarded to played cards
  test = "post->playedCardCount == 1";
  assertTrue(post->playedCardCount == 1, __LINE__, test);

  //If deck had three or more cards, all cards should have come from the deck
  if (preDeck >= intendedGainedCards)
  {
    //Player gained three cards to their hand and discarded a card
    test = "preHand + intendedGainedCards - 1 == postHand";
    assertTrue(preHand + intendedGainedCards - 1 == postHand, __LINE__, test);
    
    //Those three cards came from the deck
    test = "pre.deckCount[p] - intendedGainedCards ==  postDeck";
    assertTrue(pre.deckCount[p] - intendedGainedCards ==  postDeck, __LINE__, test);
    
    //There shouldn't have been any discards
    test = "pre.discardCount[p] == post->discardCount[p]";
    assertTrue(pre.discardCount[p] == post->discardCount[p], __LINE__, test);
  }
  //The discards should have been shuffled into the deck
  else if (preDeck + pre.discardCount[p] >= intendedGainedCards)
  {
    //Player gained three cards to their hand and discarded a card
    test = "preHand + intendedGainedCards - 1 == postHand";
    assertTrue(preHand + intendedGainedCards - 1 == postHand, __LINE__, test);
    
    //Three cards removed from combined deck and discards
    test = "preDeck + pre.discardCount[p] - intendedGainedCards == postDeck + post->discardCount[p]";
    assertTrue(preDeck + pre.discardCount[p] - intendedGainedCards == postDeck + post->discardCount[p], __LINE__, test);
  }
  //There weren't three cards to draw
  else
  {
    int available = preDeck + pre.discardCount[p];
    //Player gained available cards to their hand and discarded one card
    test = "preHand + available - 1 == postHand";
    assertTrue(preHand + available - 1 == postHand, __LINE__, test);

    //Available cards removed from deck/discards
    test = "postDeck == 0 && post->discardCount[p] == 0";
    assertTrue(postDeck == 0 && post->discardCount[p] == 0, __LINE__, test);
  }

  return 0;
}

int main() {
  int n, p;

  int k[10] = { adventurer, council_room, feast, gardens, mine,
         remodel, smithy, village, baron, great_hall };
  int handPos = -1;
  int seed = rand() % 10000;

  struct gameState G;
  initializeGame(2, k, seed, &G);

  printf("Testing smithyEffect.\n");

  printf("RANDOM TESTS.\n");

  SelectStream(2);
  PutSeed(3);

  for (n = 0; n < 800; n++)
  {
    printf("Test %d\n", n);
    p = rand() % 2;
    G.whoseTurn = p;
    G.deckCount[p] = rand() % 20;
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
      G.hand[p][i] = rand() % treasure_map;
    }
    handPos = rand() % G.handCount[p];
    G.hand[p][handPos] = smithy;
    checkSmithy(p, &G, handPos);
  }
  return 0;
}

    


