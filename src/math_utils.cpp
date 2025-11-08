#include <math_utils.hpp>

FibonacciMatrix operator*(const FibonacciMatrix& A, const FibonacciMatrix& B) {
  FibonacciMatrix C;

  C.data[0][0] = (A.data[0][0] * B.data[0][0]) + 
                 (A.data[0][1] * B.data[1][0]);

  C.data[0][1] = (A.data[0][0] * B.data[0][1]) + 
                 (A.data[0][1] * B.data[1][1]);

  C.data[1][0] = (A.data[1][0] * B.data[0][0]) + 
                 (A.data[1][1] * B.data[1][0]);

  C.data[1][1] = (A.data[1][0] * B.data[0][1]) + 
                 (A.data[1][1] * B.data[1][1]);

  return C;
}

FibonacciMatrix matrix_pow(FibonacciMatrix A, size_t n) {
  if (n == 0) {
    return FibonacciMatrix::identity();
  } 

  FibonacciMatrix result = FibonacciMatrix::identity();
  
  /* iterates log(n) times
   * (divide n by 2 at end 
   * of loop
   */
  while (n > 0) {
    if (n % 2 == 1) {
      result = result * A;
    }
    A = A * A;
    n /= 2; 
  }
  return result; 
}
