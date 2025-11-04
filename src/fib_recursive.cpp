#include <BigInt.hpp>
#include <FibImplementations.hpp>

BigInt fib_recursive(size_t n) { 
  if (n == 0 || n == 1) {
    return BigInt((uint64_t) n);
  }
  
  return fib_recursive(n - 1) + fib_recursive(n - 2); 
}
