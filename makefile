#### CONFIGURATION/VARIABLES
# Compilation/linking parameters and programs
CC=gcc
CFLAGS=-Wall -g -Iinclude -std=gnu99
AR=ar
ARFLAGS=rcs
LD=gcc
LDFLAGS=-Llib


# Objects for the library itself
OBJNAMES=procinfo queue unucleo
OBJS=$(OBJNAMES:%=bin/%.o)

# Tests
TESTNAMES=maxprocs yield-oneproc yield-twoprocs join
TESTS=$(TESTNAMES:%=bin/%.test)
TESTOBJS=$(TESTNAMES:%=bin/test-%.o)

vpath %.c src testes
vpath %.h include
vpath %.o bin
vpath %.a lib

#### MAIN TARGETS
all: library

library: bin lib lib/libsisop.a

tests: bin library $(TESTS)

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



#### DIRECTORIES
bin:
	mkdir bin

lib:
	mkdir lib



#### LIBRARY BINARIES
lib/libsisop.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

bin/%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

bin/procinfo.o bin/queue.o bin/unucleo.o: include/procinfo.h
bin/procinfo.o bin/queue.o bin/unucleo.o: include/queue.h
bin/unucleo.o: include/unucleo.h



#### TEST BINARIES
bin/%.test: test-%.o libsisop.a
	$(LD) $(LDFLAGS) -o $@ $< -lsisop

bin/test-%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(TESTOBJS): unucleo.h


#### TEST RUNNING
run_tests: tests
	@echo
	@for test in $(TESTNAMES);			\
	do									\
	    echo "Running test $$test...";	\
		bin/$$test.test || exit 1;		\
		echo;							\
	done

bin/%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<
