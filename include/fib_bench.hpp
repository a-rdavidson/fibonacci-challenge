#pragma once
#include <iostream>
#include <thread> 
#include <chrono>
#include <atomic> 
#include <future> 
#include <string>
#include <BigInt.hpp>


/* benchmark the implementation function and return the 
 * nth fibonacci number calculated by the algorithm in the 
 * specified time frame
 *
 * fib_bench.cpp
 *
 */

struct BenchmarkResult {
  size_t max_n; 
  BigInt fib_value;
};
using FibFunc = BigInt(*)(size_t n);
template<typename FibFunc, typename Duration>
BenchmarkResult benchmark_fib(FibFunc fib_func, Duration timeout);
