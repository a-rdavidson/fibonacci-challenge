#include <big_int.h>
#include <math.h>
 
int big_int_add(big_int_t * result, const big_int_t * A, const big_int_t * B) {
    size_t max_len = (A->num_digits > B->num_digits) ? A->num_digits : B->num_digits;
    size_t new_len = max_len;

    // Ensure capacity for max_len digits plus one potential carry-out limb
    if (ensure_capacity(result, max_len + 1) != 0) {
        return -1; // Propagate memory error
    }

    uint64_t carry = 0; // Carry is 0 or 1
    size_t i;
    
    for (i = 0; i < max_len; i++) {
        uint64_t a_limb = (i < A->num_digits) ? A->digits[i] : 0;
        uint64_t b_limb = (i < B->num_digits) ? B->digits[i] : 0;

        uint64_t sum_ab = a_limb + b_limb;
        
        uint64_t carry1 = (sum_ab < a_limb) ? 1 : 0; 
        
        uint64_t final_sum = sum_ab + carry;
        
        uint64_t carry2 = (final_sum < sum_ab) ? 1 : 0;

        result->digits[i] = final_sum;

        carry = carry1 + carry2; 
    }

    // Handle final carry-out
    if (carry > 0) {
        result->digits[i] = carry;
        new_len++;
    }

    // Set the final number of used digits
    result->num_digits = new_len;

    return 0;
}

int ensure_capacity(big_int_t * big, size_t required_limbs) {
   if (big->num_limbs < required_limbs) {
        // Reallocate for enough space
        uint64_t *new_digits = (uint64_t *)realloc(big->digits, required_limbs * sizeof(uint64_t));
        
        if (!new_digits) {
            return -1; // Failure
        }
        
        // Zero out the newly allocated space (important to prevent garbage data)
        size_t old_limbs = big->num_limbs;
        for (size_t i = old_limbs; i < required_limbs; i++) {
            new_digits[i] = 0;
        }

        big->digits = new_digits;
        big->num_limbs = required_limbs;
    }
    
    return 0;
}

big_int_t *big_int_init(size_t initial_capacity) {
    big_int_t *big = (big_int_t *)malloc(sizeof(big_int_t));
    if (!big) return NULL;

    big->num_digits = 0;
    big->num_limbs = initial_capacity;

    if (initial_capacity > 0) {
        big->digits = (uint64_t *)calloc(initial_capacity, sizeof(uint64_t));
        if (!big->digits) {
            free(big);
            return NULL;
        }
    } else {
        big->digits = NULL;
    }
    return big;
}

void big_int_free(big_int_t *big) {
    if (big) {
        free(big->digits);
        free(big);
    }
}

void big_int_set_limb(big_int_t *big, uint64_t value) {
    // Check if initialization is necessary or size is 0
    if (big->num_limbs == 0 && value > 0) {
        // Since ensure_capacity needs a pre-existing big_int_t, we use the
        // initialization logic directly if big->num_limbs is 0.
        big->num_limbs = 1;
        big->digits = (uint64_t *)calloc(1, sizeof(uint64_t));
        if (!big->digits) {
            fprintf(stderr, "Error: Failed to allocate memory for set_limb.\n");
            big->num_limbs = 0;
            return;
        }
    }
    
    if (value == 0) {
        big->num_digits = 0;
        return;
    }

    // Call ensure_capacity if capacity is less than required (1 limb)
    if (big->num_limbs < 1) {
        if (ensure_capacity(big, 1) == -1) {
            fprintf(stderr, "Error: Failed to ensure capacity for set_limb.\n");
            return;
        }
    }
    
    big->digits[0] = value;
    big->num_digits = 1;

    // Zero out subsequent limbs in case of a smaller previous number
    for (size_t i = 1; i < big->num_limbs; i++) {
        big->digits[i] = 0;
    }
}

void big_int_print(const big_int_t *big) {
    if (big->num_digits == 0) {
        printf("0");
        return;
    }
    // Print from most significant limb to least significant limb
    for (size_t i = big->num_digits; i > 0; i--) {
        printf("%016llX", big->digits[i - 1]);
    }
}

bool big_int_equal(const big_int_t *A, const big_int_t *B) {
    if (A->num_digits != B->num_digits) {
        return false;
    }
    for (size_t i = 0; i < A->num_digits; i++) {
        if (A->digits[i] != B->digits[i]) {
            return false;
        }
    }
    return true;
}
