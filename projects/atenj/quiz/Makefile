CFLAGS= -Wall -fpic -coverage -lm -std=c99

testme:
	-gcc -o testme -g testme.c $(CFLAGS)
	-testme >> testresults.out
	-gcov testme.c -b -f >> testresults.out 
clean:
	-rm -f testme *.gcov *.gcda *.gcno *.o