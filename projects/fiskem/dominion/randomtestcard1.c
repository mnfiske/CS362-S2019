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
    printf("cardtest2: cardtest2.c:%d: main Assertion '%s' failed\n", line, test);
  }
}

int checkAdventurer(int p, struct gameState *post, int handPos)
{
  char *test = malloc(256);
  int intendedGainedCards = 3;

  struct gameState pre;
  memcpy(&pre, post, sizeof(struct gameState));
  int r;

  int preDeck = pre.deckCount[p];
  int preHand = pre.handCount[p];

  r = playSmithy(p, post, handPos);

  int postDeck = post->deckCount[p];
  int postHand = post->handCount[p];

  test = "r == 0";
  assertTrue(r == 0, __LINE__, test);

  //To avoid having multiple tests fail due to a single bug that is explictly checked for (more than the intended
  //number of cards being drawn), set variable to the actual cards drawn for checking things like the cards being
  //drawn from the deck.
  int gainedCards = postHand - preHand;
  
  //If deck had three or more cards, all cards should have come from the deck
  if (preDeck >= gainedCards)
  {
    //Player gained three cards to their hand
    test = "preHand + intendedGainedCards == postHand";
    assertTrue(preHand + intendedGainedCards == postHand, __LINE__, test);
    
    test = "pre.deckCount[p] - gainedCards ==  postDeck";
    assertTrue(pre.deckCount[p] - gainedCards ==  postDeck, __LINE__, test);
    
    //There shouldn't have been any discards
    test = "pre.discardCount[p] == post->discardCount[p]";
     assertTrue(pre.discardCount[p] == post->discardCount[p], __LINE__, test);
    
  }
  //The discards should have been shuffled into the deck
  else if (preDeck + pre.discardCount[p] >= gainedCards)
  {
    //Player gained three cards to their hand
    test = "preHand + intendedGainedCards == postHand";
    assertTrue(preHand + intendedGainedCards == postHand, __LINE__, test);
    
    //Two cards removed from combined deck and discards
    test = "preDeck + pre.discardCount[p] - gainedCards == postDeck + post->discardCount[p]";
    assertTrue(preDeck + pre.discardCount[p] - gainedCards == postDeck + post->discardCount[p], __LINE__, test);
  }
  //There weren't three cards to draw
  else 
  {
    int available = preDeck + pre.discardCount[p];
    //Player gained available cards to their hand
    test = "preHand + available == postHand";
    assertTrue(preHand + available == postHand, __LINE__, test);
    
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
  int temphand[MAX_HAND];
  int handPos = -1;
  int seed = rand() % 10000;

  struct gameState G;
  initializeGame(2, k, seed, &G);

  printf("Testing playAdventurer.\n");

  printf("RANDOM TESTS.\n");

  SelectStream(2);
  PutSeed(3);

  for (n = 0; n < 800; n++)
  {
    printf("Test %d", n);
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
      G.hand[p][i] = rand() % treasure_map;
    }
    handPos = rand() % G.handCount[p];
    G.hand[p][handPos] = smithy;
    checkAdventurer(p, &G, handPos);
  }
  return 0;
}

    


