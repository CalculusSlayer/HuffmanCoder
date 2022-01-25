#include "io.h"
#include <unistd.h>
#include <fcntl.h>
#include <inttypes.h>
#include <string.h>
#include <math.h>

uint64_t bytes_read = 0;
uint64_t bytes_written = 0;
static uint8_t write_bit_buffer[BLOCK] = { 0 };
static uint32_t write_bit_top = 0;

static int read_index = 0;
static uint8_t read_buffer[BLOCK];
// Function to read n number of bytes from infile.
// Cite: TA Christian
int read_bytes(int infile, uint8_t *buf, int nbytes) {
    /*
	int bytes = -1;
	int total_bytes = 0;
	while ((total_bytes != nbytes) && (bytes != 0)) {
		bytes = read(infile, buf, nbytes);
		total_bytes += bytes;
		buf += bytes;
	}
	bytes_read += total_bytes;
	return total_bytes;
	*/
    if (nbytes == 0) {
        return 0;
    }
    int temp = 0;
    int bytes_read_function = 0;
    while ((temp = read(infile, buf + bytes_read_function, nbytes - bytes_read_function)) > 0) {
        bytes_read_function += temp;
        if (bytes_read_function == nbytes) {
            break;
        }
    }
    bytes_read += bytes_read_function;
    return bytes_read_function;
}

// Function to write n number of bytes to outfile.
// Cite: TA Christian
int write_bytes(int outfile, uint8_t *buf, int nbytes) {
    /*
	int bytes = -1;
	int total_bytes = 0;
	while ((total_bytes != nbytes) && (bytes != 0)) {
		bytes = write(outfile, buf, nbytes);
		total_bytes += bytes;
		buf += bytes;
	}
	bytes_written += total_bytes;
	return total_bytes;
	*/
    if (nbytes == 0) {
        return 0;
    }
    int temp = 0;
    int bytes_write_function = 0;
    while ((temp = write(outfile, buf + bytes_write_function, nbytes - bytes_write_function)) > 0) {
        bytes_write_function += temp;
        if (bytes_write_function == nbytes) {
            break;
        }
    }
    bytes_written += bytes_write_function;
    return bytes_write_function;
}

// Function to read a single bit using bitwise operations.
// Cite: TA Eugene
bool read_bit(int infile, uint8_t *bit) {
    static int end = -1;
    if (read_index == 0) {
        int bytes = read_bytes(infile, read_buffer, BLOCK);
        if (bytes < BLOCK) {
            end = 8 * bytes + 1;
        }
    }

    // Do bit operation such as shifts and bitwise and to
    // get a certain bit.
    uint8_t bit_vector = 1 << (read_index % 8);
    bit_vector &= read_buffer[read_index / 8];
    bit_vector = bit_vector >> (read_index % 8);
    *bit = bit_vector;
    read_index += 1;
    if (read_index == 8 * BLOCK) {
        read_index = 0;
    }
    return read_index != (uint8_t) end;
}

// Function to write bits from code to outfile
// Cite: TA Eugene
void write_code(int outfile, Code *c) {

    uint8_t bit = 0;
    for (uint32_t i = 0; i < code_size(c); i++) {
        bit = 1 << (i % 8);
        bit &= c->bits[i / 8];
        bit = bit >> (i % 8);
        if (bit == 1) {
            write_bit_buffer[write_bit_top / 8]
                = write_bit_buffer[write_bit_top / 8] | (1 << (write_bit_top % 8));
        } else {
            write_bit_buffer[write_bit_top / 8]
                = write_bit_buffer[write_bit_top / 8] & ~(1 << (write_bit_top % 8));
        }

        write_bit_top += 1;

        if (write_bit_top == 8 * BLOCK) {
            flush_codes(outfile);
        }
    }
}

// Flush function is used to empty buffer whenever needed.
void flush_codes(int outfile) {
    if (write_bit_top > 0) {
        uint64_t flush_bytes = ceil(write_bit_top / 8.0);
        write_bytes(outfile, write_bit_buffer, flush_bytes);
        write_bit_top = 0;
    }
}
