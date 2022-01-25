# Huffman Coding

This project implements the huffman coding algorithms. It
is used for the purposes of file compression. There are two
separate programs that can either encode or decode an input file.
The encoding executable takes the input file and encodes it using a binary
tree and various other techniques. The decoding executable can take an
encoded file and turn it back into the original text.

## Building 

Build both programs with:

```
make
```

Build just encode with:

```
make encode
```

Build just decode with:

```
make decode
```

Note: The makefile assumes the user has a clang compiler
installed on their system. In the makefile, one can change
the compiler type where it says "CC = clang" to 
"CC = gcc" if they have gcc installed on their system.

## Running

Run encode with:

```
$ ./encode [-h] [-i infile] [-o outfile] [-v]
```

Run decode with:

```
$ ./decode [-h] [-i infile] [-o outfile] [-v]
```

