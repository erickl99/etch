CC = gcc
exec = etch
sources = $(wildcard src/*.c)
objects = $(sources:.c=.o)
flags = -Wall

$(exec): $(objects)
	cc $(objects) -o build/etch -lcrypto

%.o: %.c
	cc -c $(flags) $< -o $@ -lcrypto -Iinclude

.PHONY: clean
clean:
	rm build/etch src/*.o
	rm -rf .etch
