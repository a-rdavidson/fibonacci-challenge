#pragma once 

#include <chrono> 
#include <stdexcept>
#include <memory>

/* Exception Class 
 * Thrown when current operation exceeds time alloted 
 */
class TimeExpiredException : public std::runtime_error {
public: 
  TimeExpiredException() : std::runtime_error("Benchmark time expired") {}
};

/* manages global time budget for a single threaded benchmark */
struct TimeGuard {
private:
  std::chrono::steady_clock::time_point start_time_;
  std::chrono::milliseconds timeout_duration_;
  bool is_active_ = false;

public: 
  TimeGuard() = default;

  template<typename Duration>
  void start(Duration timeout) {
    start_time_ = std::chrono::steady_clock::now();
    timeout_duration_ = std::chrono::duration_cast<std::chrono::milliseconds>(timeout);
    is_active_ = true;
  }

  void stop() {
    is_active_ = false;
  }

  void check_time() const {
    if (is_active_ && (std::chrono::steady_clock::now() - start_time_) > timeout_duration_) {
      throw TimeExpiredException();
    }
  }
};

/* external declaration of global instance defined in TimeGuard.cpp*/

extern TimeGuard g_benchmark_guard;
