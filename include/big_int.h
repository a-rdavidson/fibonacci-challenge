#include <stdint.h> 
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#define NUM_BITS_PER_LIMB   64

#ifndef BIGINT_H
#define BIGINT_H


typedef struct {
  uint64_t * digits;   // Array of 'limbs'
  size_t num_digits;   // number of decimal digits in number 
  size_t num_limbs;    // total size of limbs array
} big_int_t;

/* Adds A & B storing result in result, result big_int_t 
 * must already be allocated and can be resized during operation
 * A & B assumed to be valid, non-negative big_int_t's 
 */
int big_int_add(big_int_t * result, const big_int_t * A, const big_int_t * B);

/* 
 * checks if a given big_int has enough memory to hold a specified 
 * number of digits. Will handle reallocation if necessary. returns
 * 0 on success
 */
int ensure_capacity(big_int_t * big, size_t D);

big_int_t * big_int_init(size_t init_capacity);
void big_int_free(big_int_t * big);
void big_int_set_limb(big_int_t *big, uint64_t value);
void big_int_print(const big_int_t * big);
bool big_int_equal(const big_int_t *A, const big_int_t *B);

#endif
