#### CONFIGURATION/VARIABLES
# Compilation/linking parameters and programs
CC=gcc
CFLAGS=-Wall -g -Iinclude -std=gnu99
AR=ar
ARFLAGS=rcs


# Objects for the library itself
OBJNAMES=procinfo queue unucleo
OBJS=$(OBJNAMES:%=bin/%.o)



#### MAIN TARGETS
all: library

library: bin lib lib/libsisop.a

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



#### DIRECTORIES
bin:
	mkdir bin

lib:
	mkdir lib



#### LIBRARY BINARIES
lib/libsisop.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

bin/%.o: src/%.c
	$(CC) -c $(CFLAGS) -o $@ $<

bin/procinfo.o bin/queue.o bin/unucleo.o: include/procinfo.h
bin/procinfo.o bin/queue.o bin/unucleo.o: include/queue.h
bin/unucleo.o: include/unucleo.h
