#include <BigInt.hpp>
#include <FibImplementations.hpp>


BigInt fib_bottom_up(size_t n) {
  
  std::vector<BigInt> vec(n + 1);

  vec[0] = 0; 
  vec[1] = 1;

  for (size_t i = 2; i <= n; i++) {
    vec[i] = vec[i - 1] + vec[i - 2];
  } 
  return vec[n];
  

}


BigInt fib_bottom_up_better(size_t n) {
  if (n <= 1) {
    return BigInt((uint64_t) n);
  }
  BigInt curr((uint64_t) 0);
  BigInt prev1((uint64_t) 1);
  BigInt prev2((uint64_t) 0);

  for (size_t i = 2; i <= n; i++) {
    curr = prev1 + prev2;
    
    prev2 = prev1; 

    prev1 = curr;
  } 
  return curr; 
}
