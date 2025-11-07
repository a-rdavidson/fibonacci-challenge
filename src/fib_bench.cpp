#include <BigInt.hpp>
#include <FibImplementations.hpp>
#include <fib_bench.hpp>

int main(void) {
  /* for time limits */
  using namespace std::chrono_literals;
  const auto TIME_LIMIT = 1s;

  std::cout << "Starting benchmarks with a " << TIME_LIMIT.count() << "s time limit.\n";
  std::cout << "------------------------------------------\n"; 

  BenchmarkResult recursive_result = benchmark_fib(fib_recursive, TIME_LIMIT);

  std::cout << "Algorithm: Pure Recursive (O(Phi^N))\n";
  std::cout << "Max N calculated: **" << recursive_result.max_n << "**\n";
  std::cout << "Fib{" << recursive_result.max_n << "} value: " << 
    recursive_result.fib_value << "\n";
  std::cout << "------------------------------------------\n";


  BenchmarkResult memo_result = benchmark_fib(fib_memo, TIME_LIMIT);

  std::cout << "Algorithm: Recursive w/ Memoization (O(N^2))\n";
  std::cout << "Max N calculated: **" << memo_result.max_n << "**\n";
  std::cout << "Fib{" << memo_result.max_n << "} value: " << 
    memo_result.fib_value << "\n";
  std::cout << "------------------------------------------\n";


  BenchmarkResult bottom_up_result = benchmark_fib(fib_bottom_up, TIME_LIMIT);

  std::cout << "Algorithm: Iterative Bottom Up (O(N))\n";
  std::cout << "Max N calculated: **" << bottom_up_result.max_n << "**\n";
  std::cout << "Fib{" << bottom_up_result.max_n << "} value: " << 
    bottom_up_result.fib_value << "\n";
  std::cout << "------------------------------------------\n";

  BenchmarkResult better_bottom_up_result = benchmark_fib(fib_bottom_up_better, TIME_LIMIT);

  std::cout << "Algorithm: Iterative Bottom Up (O(N))\n";
  std::cout << "Max N calculated: **" << better_bottom_up_result.max_n << "**\n";
  std::cout << "Fib{" << better_bottom_up_result.max_n << "} value: " << 
    better_bottom_up_result.fib_value << "\n";
  std::cout << "------------------------------------------\n";

}

template<typename FibFunc, typename Duration>
BenchmarkResult benchmark_fib(FibFunc fib_func, Duration timeout) {
  size_t max_n = 0;
  size_t n = 0;
  BigInt last_fib_result((uint64_t) 0);

  auto start = std::chrono::steady_clock::now();

  while (true) {
    auto elapsed = std::chrono::steady_clock::now() - start;

    if (elapsed >= timeout) {
      break;
    }
    auto time_budget = timeout - elapsed; 

    n++;

    std::future<BigInt> future_fib = std::async(std::launch::async, fib_func, n);
    /* wait for a maximum of the remaining time alloted for this function */
    std::future_status status = future_fib.wait_for(time_budget);

    if (status == std::future_status::ready) {
      last_fib_result = future_fib.get();

      max_n = n;

    } else if (status == std::future_status::timeout) {
      break;
    }
  }
  return {max_n, last_fib_result};
}
