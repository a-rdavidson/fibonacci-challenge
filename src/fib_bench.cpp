#include <BigInt.hpp>
#include <FibImplementations.hpp>
#include <fib_bench.hpp>
#include <TimeGuard.hpp>

int main(void) {
  /* for time limits */
  using namespace std::chrono_literals;
  const auto TIME_LIMIT = 1s;

  std::cout << "Starting benchmarks with a " << TIME_LIMIT.count() << "s time limit.\n";
  std::cout << "------------------------------------------\n"; 

  BenchmarkResult fib_bottom_result = benchmark_fib(fib_bottom_up, TIME_LIMIT);

  std::cout << "Algorithm: Fib Bottom Up\n";
  std::cout << "Max N calculated: **" << fib_bottom_result.max_n << "**\n";
  std::cout << "Fib{" << fib_bottom_result.max_n << "} value: " << 
    fib_bottom_result.fib_value << "\n";
  std::cout << "------------------------------------------\n";

  BenchmarkResult fib_better_bottom_result = benchmark_fib(fib_bottom_up_better, TIME_LIMIT);

  std::cout << "Algorithm: Fib Better Bottom Up\n";
  std::cout << "Max N calculated: **" << fib_better_bottom_result.max_n << "**\n";
  std::cout << "Fib{" << fib_better_bottom_result.max_n << "} value: " << 
    fib_better_bottom_result.fib_value << "\n";
  std::cout << "------------------------------------------\n";

  BenchmarkResult fib_exp_result = benchmark_fib(fib_matrix_exp, TIME_LIMIT);

  std::cout << "Algorithm: Fib Matrix Exponentiation (O(logN))\n";
  std::cout << "Max N calculated: **" << fib_exp_result.max_n << "**\n";
  std::cout << "Fib{" << fib_exp_result.max_n << "} value: " << 
    fib_exp_result.fib_value << "\n";
  std::cout << "------------------------------------------\n";
}

template<typename FibFunc, typename Duration>
BenchmarkResult benchmark_fib(FibFunc fib_func, Duration timeout) {
  size_t n = 0;
  BigInt last_fib_result((uint64_t) 0);

  g_benchmark_guard.start(timeout);
  try {
    while (true) {
      n++;

      g_benchmark_guard.check_time();

      last_fib_result = fib_func(n);
    }
  } catch (const TimeExpiredException& e) {
    // stop on timeout
  }

  g_benchmark_guard.stop();

  return { 
    static_cast<size_t>(n > 0 ? n - 1 : 0),
    last_fib_result 
  };
}
