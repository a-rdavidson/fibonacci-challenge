#include <big_int.h>

big_int_t * fib_recursive(size_t n) { 
    if (n <= 1) {
        // Base cases F(0) = 0, F(1) = 1
        big_int_t *F_n = big_int_init(1);
        if (!F_n) return NULL;
        big_int_set_limb(F_n, (uint64_t)n);
        return F_n;
    }

    // F(n) = F(n-1) + F(n-2)

    // Recursively calculate F(n-1) and F(n-2)
    big_int_t *F_n_minus_1 = fib_recursive(n - 1);
    if (!F_n_minus_1) return NULL;
    
    big_int_t *F_n_minus_2 = fib_recursive(n - 2);
    if (!F_n_minus_2) {
        big_int_free(F_n_minus_1);
        return NULL;
    }

    // Calculate F(n) = F(n-1) + F(n-2)
    big_int_t *F_n = big_int_init(0); 
    if (!F_n) {
        big_int_free(F_n_minus_1);
        big_int_free(F_n_minus_2);
        return NULL;
    }

    if (big_int_add(F_n, F_n_minus_1, F_n_minus_2) != 0) {
        // Handle addition error (likely memory failure in ensure_capacity)
        big_int_free(F_n_minus_1);
        big_int_free(F_n_minus_2);
        big_int_free(F_n);
        return NULL;
    }

    big_int_free(F_n_minus_1);
    big_int_free(F_n_minus_2);

    return F_n;
}
