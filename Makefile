CC=gcc
CFLAGS=-std=c89 -pedantic -Wall -W


all: asm emu

asm: asm.c
	$(CC) $(CFLAGS) asm.c -o asm

emu: emu.c
	$(CC) $(CFLAGS) emu.c -lm -o emu

clean:
	rm -f asm emu
