#include <iostream>
#include <BigInt.hpp>
#include <FibImplementations.hpp>
int main(void) {
  BigInt fib30 = fib_recursive(30);
  std::cout << "fib30: " << fib30.to_string() << std::endl;
  return 0;
}
