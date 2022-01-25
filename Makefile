EXEC1 = encode
EXEC2 = decode
#SOURCES = $(wildcard *.c)
OBJECTS = encode.o decode.o node.o pq.o code.o io.o stack.o huffman.o

CC = clang
CFLAGS = -Wall -Wextra -Wpedantic -Werror -O2
LDFLAGS = -lm

all: encode decode

encode: encode.o node.o pq.o code.o io.o stack.o huffman.o header.h
	$(CC) encode.o node.o pq.o code.o io.o stack.o huffman.o -o encode $(LDFLAGS)

decode: decode.o node.o pq.o code.o io.o stack.o huffman.o header.h
	$(CC) decode.o node.o pq.o code.o io.o stack.o huffman.o -o decode $(LDFLAGS)

encode.o: encode.c node.o pq.o code.o io.o stack.o huffman.o
	$(CC) $(CFLAGS) -c encode.c

decode.o: decode.c node.o pq.o code.o io.o stack.o huffman.o
	$(CC) $(CFLAGS) -c decode.c

node.o: node.c node.h
	$(CC) $(CFLAGS) -c node.c

pq.o: pq.c pq.h node.c node.h
	$(CC) $(CFLAGS) -c pq.c

code.o: code.c code.h defines.h
	$(CC) $(CFLAGS) -c code.c

io.o: io.c io.h 
	$(CC) $(CFLAGS) -c io.c

stack.o: stack.c stack.h node.c node.h 
	$(CC) $(CFLAGS) -c stack.c

huffman.o: huffman.c huffman.h defines.h node.c pq.c code.c io.c stack.c
	$(CC) $(CFLAGS) -c huffman.c

clean:
	rm -rf $(EXEC1) $(EXEC2) $(OBJECTS)

format:
	clang-format -i -style=file *.[c,h]

