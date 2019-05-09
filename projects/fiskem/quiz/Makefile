CFLAGS= -Wall -fpic -coverage -lm -std=c99

testme: testme.c
	echo "results" >> tests.out
	gcc -fprofile-arcs -ftest-coverage -Wall -lm -std=c99 -o testme -g testme.c
	./testme
	gcov -b testme.c >> tests.out
	cat testme.c.gcov >> tests.out

all: testme 

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testme
