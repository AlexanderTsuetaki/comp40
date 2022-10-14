#                                                                               
# Makefile for the UM Test lab                                                  
#                                                                               
CC = gcc

IFLAGS  = -I/comp/40/build/include -I/comp/40/bin/include -I/usr/sup/cii40/include/cii
CFLAGS  = -g -std=gnu99 -O2 -Wall -Wextra -Werror -pedantic $(IFLAGS)
LDFLAGS = -g -L/comp/40/build/lib -L/comp/40/bin/lib -L/usr/sup/cii40/lib64
LDLIBS  = -l40locality -lcii40 -lm

EXECS   = calc40
all: calc40

##compile step
%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -c $< -o $@


## Linking step
calc40: calc40.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

clean:
	rm -f bitpack *.o
