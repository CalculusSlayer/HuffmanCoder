#include "code.h"
#include "defines.h"

// From TA Christian
Code code_init(void) {
    Code c = { .top = 0, .bits = { 0 } };
    return c;
}

// This function return the size of the
// code structure. Value of c->top is
// the exact same as the size of code
// structure.
uint32_t code_size(Code *c) {
    return c->top;
}

// This function checks if code structure is
// empty or not. It does that by seeing if
// top is equal to zero.
bool code_empty(Code *c) {
    if (c->top == 0) {
        return true;
    }
    return false;
}

// This function checks if code structure is full.
// It does this by seeing if c->top equals to
// 256.
bool code_full(Code *c) {
    if (c->top == ALPHABET) {
        return true;
    }
    return false;
}

// This function sets the ith bit to 1. There are
// 256 bits to account for as there are 32
// bytes in total in the code structure. You
// have to use the bitwise operations left shift
// and 'bitwise or' to set the bit to 1.
bool code_set_bit(Code *c, uint32_t i) {
    if (i < ALPHABET) {
        c->bits[i / 8] |= 1 << (i % 8);
        return true;
    }
    return false;
}

// This function does the exact same thing as
// code_set_byte, except it sets the ith bit to
// 0. You have to use the bitwise operations
// left shift, bitwise not, and 'bitwise and'
// to clear the ith bit.
bool code_clr_bit(Code *c, uint32_t i) {
    if (i < ALPHABET) {
        c->bits[i / 8] &= ~(1 << (i % 8));
        return true;
    }
    return false;
}

// This function gets the ith bit. It does not change
// any bits in the code structure at all. It just
// retrieves information about whether the ith bit
// is 1 or 0. If the result of bitwise operations
// is 1, function returns true. If the result of
// the bitwise operations is 0 or i is great than or equal
// to ALPHABET, then false is returned.
bool code_get_bit(Code *c, uint32_t i) {
    uint8_t something;
    if (i >= ALPHABET) {
        return false;
    } else {
        something = 1 << (i % 8);
        something &= c->bits[i / 8];
        something = something >> (i % 8);
        if (something == 1) {
            return true;
        }
        return false;
    }
}

// This function pushes a bit into the code
// structure unless code structure is full.
bool code_push_bit(Code *c, uint8_t bit) {
    if (!code_full(c) && bit == 1) {
        code_set_bit(c, c->top);
        c->top += 1;
        return true;
    } else if (!code_full(c) && bit == 0) {
        code_clr_bit(c, c->top);
        c->top += 1;
        return true;
    } else
        return false;
}

// This function pops a bit from the code
// structure unless code structure is empty.
bool code_pop_bit(Code *c, uint8_t *bit) {
    if (code_empty(c)) {
        return false;
    }
    c->top -= 1;
    if (code_get_bit(c, c->top)) {
        *bit = (uint8_t) 1;
    } else {
        *bit = (uint8_t) 0;
    }
    return true;
}
