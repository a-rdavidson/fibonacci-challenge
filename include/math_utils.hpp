#pragma once
#include <BigInt.hpp>

/* 2 x 2 matrix class, no need 
 * for further dimensions 
 */
struct FibonacciMatrix {
  BigInt data[2][2];
  
  /* Constructor defaults to base matrix 
   * [1, 1]
   * [1, 0]
   */

  FibonacciMatrix() {
    data[0][0] = BigInt((uint64_t) 1);
    data[0][1] = BigInt((uint64_t) 1);
    data[1][0] = BigInt((uint64_t) 1);
    data[1][1] = BigInt((uint64_t) 0);
  }


  /* Constructor for identity matrix 
   * [1, 0]
   * [0, 1]
   */
  static FibonacciMatrix identity() {
    FibonacciMatrix I; 

    I.data[0][0] = BigInt((uint64_t) 1);
    I.data[0][1] = BigInt((uint64_t) 0);
    I.data[1][0] = BigInt((uint64_t) 0);
    I.data[1][1] = BigInt((uint64_t) 1);
    return I;
  }

};

/* implements multiplication of 2 FibonacciMatrix structs & returns result 
 * to be used in the matrix_pow function
 * */
FibonacciMatrix operator*(const FibonacciMatrix& A, const FibonacciMatrix& B);

/* calculates A^n using fast exponentiation */
FibonacciMatrix matrix_pow(FibonacciMatrix A, size_t n);
FibonacciMatrix matrix_square(const FibonacciMatrix& A);
