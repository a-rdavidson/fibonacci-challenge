#pragma once
#include <BigInt.hpp>
#include <map>
#include <vector>

/* BEGIN fib_recursive.cpp functions */
BigInt fib_recursive(size_t n);

/* uses memoization to reduce redundant calculations
 * basic DSA optimization 
 */
BigInt fib_memo(size_t n);
BigInt fib_recursive_memo(size_t n, std::unordered_map<size_t, BigInt>&);

/* END fib_recursive.cpp functions */

/* BEGIN fib_iterative.cpp functions */
BigInt fib_bottom_up(size_t n);
BigInt fib_bottom_up_better(size_t n);
