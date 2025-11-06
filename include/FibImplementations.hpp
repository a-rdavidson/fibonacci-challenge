#pragma once
#include <BigInt.hpp>

/* BEGIN fib_recursive.cpp functions */
BigInt fib_recursive(size_t n);

/* uses memoization to reduce redundant calculations
 * basic DSA optimization 
 */
BigInt fib_recursive_memo(size_t n);

/* END fib_recursive.cpp functions */

