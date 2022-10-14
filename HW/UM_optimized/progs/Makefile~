#
# Makefile for the UM Test lab
#
CC = gcc

IFLAGS  = -I/comp/40/build/include -I/comp/40/bin/include -I/usr/sup/cii40/include/cii
CFLAGS  = -g -std=gnu99 -O2 -Wall -Wextra -Werror -pedantic $(IFLAGS)
LDFLAGS = -g -L/comp/40/build/lib -L/comp/40/bin/lib -L/usr/sup/cii40/lib64
LDLIBS  = -l40locality -lcii40 -lm

EXECS   = um

all: $(EXECS)


um: um-driver.o bitpack.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)
um-working: um-working.o bitpack.o um-working-instructions.o
um-test: um-test.o um.o bitpack.o um-instructions.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)
writetests: umlabwrite.o umlab.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

# To get *any* .o file, compile its .c file with the following rule.
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXECS)  *.o
