#### CONFIGURATION/VARIABLES
# Compilation/linking parameters and programs
CC=gcc
CFLAGS=-Wall -g -Iinclude -std=gnu99

# Objects for the library itself
OBJNAMES=procinfo queue
OBJS=$(OBJNAMES:%=bin/%.o)

# Tests
TESTNAMES=maxprocs yield-oneproc yield-twoprocs join
TESTS=$(TESTNAMES:%=bin/%.test)
TESTOBJS=$(TESTNAMES:%=bin/test-%.o)

vpath %.c src testes
vpath %.h include
vpath %.o bin

#### MAIN TARGETS
all: library

library: bin $(OBJS)

tests: bin library $(TESTS)
	@echo
	@echo "Linking tests is currently not supported..."

clean:
	rm -f $(OBJS)
	rm -f $(TESTS)
	rm -f $(TESTOBJS)

help:
	@echo
	@echo "The following main targets are available:"
	@echo "  all       - library and tests"
	@echo "  library   - compile the library (objects and static library)"
	@echo "  tests     - compile the tests"
	@echo "  run_tests - runs the tests (currently not supported)"
	@echo "  clean     - remove the generated files"
	@echo "  help      - display this text"
	@echo

.PHONY: library tests run_tests clean help
.SUFFIXES = .c .h .o .a .test

#### DIRECTORIES
bin:
	mkdir bin

bin/tests:
	mkdir -p bin/tests



#### LIBRARY BINARIES
bin/%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

bin/procinfo.o bin/queue.o: procinfo.h
bin/procinfo.o bin/queue.o: queue.h



#### TEST BINARIES
bin/%.test: test-%.o
	@echo $@

bin/test-%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(TESTOBJS): unucleo.h


#### TEST RUNNING
run_tests:
	@echo
	@echo "Running tests is currently not supported..."

bin/%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<