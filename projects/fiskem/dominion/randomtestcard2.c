/**********************************************************************
Test of the Embargo card.
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
    printf("randomcardtest2: randomcardtest2.c:%d: main Assertion '%s' failed\n", line, test);
  }
}

int checkEmbargo(int p, struct gameState *post, int handPos, int choice1)
{
  char *test = malloc(256);
  int intendedGainedCoins = 3;

  struct gameState pre;
  memcpy(&pre, post, sizeof(struct gameState));
  int r;

  int preDeck = pre.deckCount[p];
  int preHand = pre.handCount[p];

  r = playEmbargo(p, post, handPos, choice1);

  int postDeck = post->deckCount[p];
  int postHand = post->handCount[p];

  if (pre.supplyCount[choice1] != -1)
  {
    test = "r == 0";
    assertTrue(r == 0, __LINE__, test);

    //Verify coins increased by two
    test = "pre.coins + 2 == post->coins";
    assertTrue(pre.coins + 2 == post->coins, __LINE__, test);

    //Verify choice1's embargo tokens increased by one
    test = "pre.embargoTokens[choice1] + 1 == post->embargoTokens[choice1]";
    assertTrue(pre.embargoTokens[choice1] + 1 == post->embargoTokens[choice1], __LINE__, test);

    //Verify the card was trashed
      //removed from hand
    test = "preHand - 1 == postHand";
    assertTrue(preHand - 1 == postHand, __LINE__, test);
      //Not added to discards, deck, or played cards
    test = "preDeck == postDeck";
    assertTrue(preDeck == postDeck, __LINE__, test);
    test = "pre.discardCount[p] == post->discardCount[p]";
    assertTrue(pre.discardCount[p] == post->discardCount[p], __LINE__, test);
    test = "pre.playedCardCount == post->playedCardCount";
    assertTrue(pre.playedCardCount == post->playedCardCount, __LINE__, test);
  }
  else
  {
    test = "r == -1";
    assertTrue(r == -1, __LINE__, test);

    //Verify coins not increased
    test = "pre.coins == post->coins";
    assertTrue(pre.coins == post->coins, __LINE__, test);

    //Verify choice1's embargo tokens not increased
    test = "pre.embargoTokens[choice1] == post->embargoTokens[choice1]";
    assertTrue(pre.embargoTokens[choice1] == post->embargoTokens[choice1], __LINE__, test);

    //Verify no changes to hand/deck/discards/playedCards
    test = "preHand == postHand";
    assertTrue(preHand == postHand, __LINE__, test);
    test = "preDeck == postDeck";
    assertTrue(preDeck == postDeck, __LINE__, test);
    test = "pre.discardCount[p] == post->discardCount[p]";
    assertTrue(pre.discardCount[p] == post->discardCount[p], __LINE__, test);
    test = "pre.playedCardCount == post->playedCardCount";
    assertTrue(pre.playedCardCount == post->playedCardCount, __LINE__, test);
  }

  return 0;
}

int main() {
  int n, p;

  
  
  int temphand[MAX_HAND];
  int handPos = -1;
  int seed = rand() % 10000;

  printf("Testing playEmbargo.\n");

  printf("RANDOM TESTS.\n");

  SelectStream(2);
  PutSeed(3);

  for (n = 0; n < 800; n++)
  {
    printf("Test %d\n", n);

    int k[10];
    //Choose ten kingdom cards to add to k
    for (int i = 0; i < 10; i++)
    {
      //If the card has already been chosen, choose another card to add
      int notChosen = 1;
      while (notChosen)
      {
        //Choose the card, and set whether it is found in the supply already to 0
        int card = rand() % (treasure_map + 1);
        int found = 0;

        //Check if the card is already found in the supply
        for (int i = 0; i < 10; i++)
        {
          if (k[i] == card)
          {
            found = 1;
            break;
          }
        }
        //If not found, add it to the supply and set notChosen to false to break out of the while loop
        if (found == 0)
        {
          k[i] = card;
          notChosen = 0;
        }
      }
    }
    struct gameState G;
    initializeGame(2, k, seed, &G);

    //Set all kingdom supply counts to -1 (not in play)
    for (int i = adventurer; i <= treasure_map; i++)
    {
      G.supplyCount[i] = -1;
    }
    //Generate random kingdom supply pile numbers and embargo tokens
    for (int i = 0; i < 10; i++)
    {
      G.supplyCount[k[i]] = rand() % 11;
      G.embargoTokens[k[i]] = rand() % 3;
    }
    //Set curse supply count
    G.supplyCount[curse] = (rand() % 12) - 1; //for a range between -1 and 10
    //Set supply count for victory cards
    for (int i = 1; i < 4; i++)
    {
      G.supplyCount[i] = rand() % 9;
    }
    //Set treasure supply count
    G.supplyCount[copper] = rand() % 47;
    G.supplyCount[silver] = rand() % 41;
    G.supplyCount[gold] = rand() % 31;

    p = rand() % 2;
    G.whoseTurn = p;
    G.deckCount[p] = rand() % MAX_DECK;
    for (int i = 0; i < G.deckCount[p]; i++)
    {
      G.deck[p][i] = rand() % (treasure_map + 1);
    }
    G.discardCount[p] = rand() % (MAX_DECK - G.deckCount[p]);
    G.handCount[p] = rand() % MAX_HAND;
    if (G.handCount[p] < 5)
    {
      G.handCount[p] = 5;
    }
    for (int i = 0; i < G.handCount[p]; i++)
    {
      G.hand[p][i] = rand() % (treasure_map + 1);
    }
    handPos = rand() % G.handCount[p];
    G.hand[p][handPos] = smithy;
    G.coins = rand() % 30;
    int choice1 = rand() % (treasure_map + 1);

    checkEmbargo(p, &G, handPos, choice1);
  }
  return 0;
}

    


