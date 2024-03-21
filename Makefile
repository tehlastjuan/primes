.PHONY: clean tidy

CC=gcc
CFLAGS=-Wall -std=c11
src=$(wildcard *.c)
obj=$(src:.c=.o)

primes: $(obj)
	$(CC) $(CFLAGS) -o $@ $^
	rm -f *.o *~

clean :
	rm -f *.o *~
