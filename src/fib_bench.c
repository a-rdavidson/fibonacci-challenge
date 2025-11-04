#include <fib_implementations.h>
#include <big_int.h>


void fib_benchmark(fib_func_t func, const char * name, double max_time_seconds) {
  printf("\n--- Benchmark: %s (Max %.1f sec) ---\n", name, max_time_seconds);
    
    clock_t start_time = clock();
    unsigned int n = 0;
    big_int_t *result = NULL;
    
    // We start at n=1 and increment
    for (n = 1; ; n++) {
        // Free the result from the previous iteration
        if (result) {
            big_int_free(result);
            result = NULL;
        }

        // Run the function
        result = func(n);
        
        if (!result) {
            // Memory error or other failure
            printf("  Error: Function failed at F(%u) due to memory allocation failure.\n", n);
            break;
        }

        clock_t current_time = clock();
        double elapsed_time = (double)(current_time - start_time) / CLOCKS_PER_SEC;

        // If the time limit is exceeded, stop and report the *last successfully completed* N
        if (elapsed_time >= max_time_seconds) {
            
            printf("  Max N Reached: F(%u)\n", n);
            printf("  Result F(%u): ", n); big_int_print(result); printf("\n");
            printf("  Elapsed Time: %.2f seconds\n", elapsed_time);
            
            // Free the last result before exiting
            big_int_free(result);
            return;
        }

        // Safety break for extremely fast implementations, although the time limit should handle it.
        if (n > 5000000) { 
             printf("  Warning: Maximum iteration count reached (n > 5M).\n");
             break;
        }
    }
    
    if (result) big_int_free(result); // Final cleanup if loop exited early
}

int main(void) {
  fib_benchmark(fib_recursive, "Recursive", 1.0);

}
