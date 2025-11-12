#include <math_utils.hpp>
#include <TimeGuard.hpp>

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

FibonacciMatrix matrix_square(const FibonacciMatrix& A) {
  g_benchmark_guard.check_time();

  const BigInt&a = A.data[0][0];
  const BigInt&b = A.data[0][1]; //assumes b = A.data[1][0]
  const BigInt&d = A.data[1][1];

  BigInt b_sq = b * b;
  BigInt a_sq = a * a;
  BigInt d_sq = d * d;

  BigInt a_d = a + d;
  BigInt b_times_sum = b * a_d;
  FibonacciMatrix C;

  C.data[0][0] = a_sq + b_sq;
  C.data[0][1] = b_times_sum;
  C.data[1][0] = b_times_sum;
  C.data[1][1] = b_sq + d_sq;
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
    g_benchmark_guard.check_time();
    if (n % 2 == 1) {
      result = result * A;
    }
    A = matrix_square(A);
    n /= 2; 
  }
  return result; 
}
