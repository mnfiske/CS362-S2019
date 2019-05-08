CFLAGS= -Wall -fpic -coverage -lm -std=c99

testme: testme.c
	gcc -fprofile-arcs -ftest-coverage -Wall -lm -std=c99 -o testme -g testme.c dominion.o rngs.o
	gcov -b testme.c >> randtest.out
	cat testme.c.gcov >> randtest.out

all: testme 

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
