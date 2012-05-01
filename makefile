#### CONFIGURATION/VARIABLES
# Compilation/linking parameters and programs
CC=gcc
CFLAGS=-Wall -g -Iinclude -std=gnu99

# Objects for the library itself
OBJNAMES=procinfo queue
OBJS=$(OBJNAMES:%=bin/%.o)



#### MAIN TARGETS
all: library

library: bin $(OBJS)

clean:
	rm -f bin/*

help:
	@echo
	@echo "The following main targets are available:"
	@echo "  all     - same as library"
	@echo "  library - compile the library (objects and static library)"
	@echo "  clean   - remove the generated files"
	@echo "  help    - display this text"
	@echo

.PHONY: library clean help

#### LIBRARY BINARIES
bin/%.o: src/%.c
	$(CC) -c $(CFLAGS) -o $@ $<

bin/procinfo.o bin/queue.o: include/procinfo.h
bin/procinfo.o bin/queue.o: include/queue.h

