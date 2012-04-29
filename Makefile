CC=gcc
CFLAGS=-Wall -g -Iinclude -std=gnu99

OBJNAMES=procinfo queue
OBJS=$(OBJNAMES:%=bin/%.o)

all: bin $(OBJS)
bin:
	mkdir bin

bin/%.o: src/%.c
	$(CC) -c $(CFLAGS) -o $@ $<

bin/procinfo.o bin/queue.o: include/procinfo.h
bin/queue.o: include/queue.h

clean:
	rm -f $(OBJS)

.PHONY: clean
