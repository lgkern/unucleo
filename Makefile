CC=gcc
CFLAGS=-Wall -g -Iinclude -std=c99

OBJNAMES=queue
OBJS=$(OBJNAMES:%=bin/%.o)

all: bin $(OBJS)
bin:
	mkdir bin

bin/%.o: src/%.c
	$(CC) -c $(CFLAGS) -o $@ $<

bin/queue.o: include/queue.h