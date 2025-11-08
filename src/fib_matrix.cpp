#include <FibImplementations.hpp>

BigInt fib_matrix_exp(size_t n) {
  
  if (n == 0 || n == 1) {
    return BigInt((uint64_t) n);
  }

  FibonacciMatrix M;
  

  /* calculate n - 1 because M^(n-1) gives: 
  * [ Fn   , Fn-1 ]
  * [ Fn-1 , Fn-2 ]
  */ 
  FibonacciMatrix result_matrix = matrix_pow(M, n - 1);

  return result_matrix.data[0][0];
}
