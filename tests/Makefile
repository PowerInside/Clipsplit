CC=gcc
CFLAGS = -lcriterion
make: default.c
	$(CC) ../splitfunc.c $^ $(CFLAGS) -o test

.PHONY: clean
clean: 
	find . -name "*.o" -delete -o -name "*~" -delete -o -name "test" -delete
