# Assignment 5 - Huffman Coding

This assignment implements the huffman coding algorithms. It
is used for the purposes of file compression. There are two
separate programs that can either encode or decode an input file.
The encoding file takes the input file and encodes it using a binary
tree and various other techniques. The decoding file can take an
encoded file and turn it back into the same text.

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

## Running

Run encode with:

```
$ ./encode [-h] [-i infile] [-o outfile] [-v]
```

Run decode with:

```
$ ./decode [-h] [-i infile] [-o outfile] [-v]
```

