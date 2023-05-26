CC = gcc
CFLAGS = -O0 -lssl -lcrypto
SRC = *.c

all: $(patsubst %.c, %.out, $(wildcard *.c))

%.out: %.c Makefile
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.out
