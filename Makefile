CC=gcc
CFLAGS=-I.
DEPS = savesplit.h
OBJ = clipsplit.o splitfunc.o
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
make: $(OBJ)
	$(CC) $^ $(CFLAGS)
debug: *.c
	$(CC) $^ -g $(CFLAGS)
test:
	cd tests; make

.PHONY: clean
clean: 
	find . -name "*.o" -delete -o -name "a.out" -delete -o -name "*~" -delete
