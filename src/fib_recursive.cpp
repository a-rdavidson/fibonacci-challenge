#include <BigInt.hpp>
#include <FibImplementations.hpp>

BigInt fib_recursive(size_t n) { 
  if (n == 0 || n == 1) {
    return BigInt((uint64_t) n);
  }
  
  return fib_recursive(n - 1) + fib_recursive(n - 2); 
}

BigInt fib_memo(size_t n) { 
  /* will be 0 initialized */
  std::unordered_map<size_t, BigInt> memo_table;
  return fib_recursive_memo(n, memo_table);
}

BigInt fib_recursive_memo(size_t n, std::unordered_map<size_t, BigInt>& memo_table) {

  if (n == 0 || n == 1) {
    return BigInt((uint64_t) n);
  }
  if (memo_table.find(n) != memo_table.end()) {
    return memo_table[n];
  }

  memo_table[n] = fib_recursive_memo(n - 1, memo_table) + 
    fib_recursive_memo(n - 2, memo_table);
  return memo_table[n];

}
