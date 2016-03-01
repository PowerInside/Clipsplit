CC=gcc
CFLAGS=-I.
DEPS = savesplit.h
OBJ = clipsplit.o splitfunc.o
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
make: $(OBJ)
	$(CC) $^ $(CFLAGS)
.PHONY: clean
clean: 
	rm -f *.o *~
