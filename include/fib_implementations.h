#include <time.h>
#include <big_int.h>


big_int_t * fib_recursive(size_t n);

typedef big_int_t *(*fib_func_t)(size_t n);

