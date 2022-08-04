EXEC1 = encode
EXEC2 = decode

OBJECTS1 = encode.o node.o pq.o code.o io.o stack.o huffman.o
OBJECTS2 = decode.o node.o pq.o code.o io.o stack.o huffman.o

CC = clang
CFLAGS = -Wall -Wextra -Wpedantic -Werror -O2
LDFLAGS = -lm

all: $(EXEC1) $(EXEC2)

$(EXEC1): encode.o node.o pq.o code.o io.o stack.o huffman.o
	$(CC) $(OBJECTS1) -o $(EXEC1) $(LDFLAGS)

$(EXEC2): decode.o node.o pq.o code.o io.o stack.o huffman.o
	$(CC) $(OBJECTS2) -o $(EXEC2) $(LDFLAGS)

encode.o: encode.c defines.h stack.h io.h pq.h node.h huffman.h header.h
	$(CC) $(CFLAGS) -c encode.c

decode.o: decode.c defines.h stack.h io.h pq.h node.h huffman.h header.h
	$(CC) $(CFLAGS) -c decode.c

node.o: node.c node.h 
	$(CC) $(CFLAGS) -c node.c

pq.o: pq.c pq.h node.h
	$(CC) $(CFLAGS) -c pq.c

code.o: code.c code.h defines.h
	$(CC) $(CFLAGS) -c code.c

io.o: io.c io.h code.h 
	$(CC) $(CFLAGS) -c io.c

stack.o: stack.c stack.h node.h 
	$(CC) $(CFLAGS) -c stack.c

huffman.o: huffman.c huffman.h defines.h pq.h node.h io.h stack.h code.h
	$(CC) $(CFLAGS) -c huffman.c

clean:
	rm -rf $(EXEC1) $(EXEC2) $(OBJECTS1) $(EXEC2).o 

format:
	clang-format -i -style=file *.[c,h]

