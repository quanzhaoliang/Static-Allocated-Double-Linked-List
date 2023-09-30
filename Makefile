test: test.c list.c
	gcc -Wall -std=c99 -o test test.c list.c

clean:
	rm -f test *.o