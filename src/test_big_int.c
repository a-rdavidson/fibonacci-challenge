#include <stdio.h>
#include <big_int.h>

#define MAX_U64 0xFFFFFFFFFFFFFFFFULL // The largest 64-bit integer
#define HALF_U64 0x8000000000000000ULL // 2^63

void run_test(const char *name, big_int_t *A, big_int_t *B, big_int_t *expected) {
    big_int_t *R = big_int_init(0);
    
    printf("--- Test: %s ---\n", name);
    printf("  A: "); big_int_print(A);
    printf("\n  B: "); big_int_print(B);
    
    if (big_int_add(R, A, B) != 0) {
        printf("\n  Result: FAILED (Memory Allocation Error)\n");
    } else {
        printf("\n  Expected: "); big_int_print(expected);
        printf("\n  Actual:   "); big_int_print(R);
        
        if (big_int_equal(R, expected)) {
            printf("\n  Result: PASSED\n");
        } else {
            printf("\n  Result: FAILED (Incorrect Sum)\n");
        }
    }
    big_int_free(R);
    printf("\n");
}

/*
int main(void) {
    // Test Case 1: Simple Addition (No Carry-out)
    big_int_t *A1 = big_int_init(1);
    big_int_set_limb(A1, 100);
    big_int_t *B1 = big_int_init(1);
    big_int_set_limb(B1, 200);
    big_int_t *E1 = big_int_init(1);
    big_int_set_limb(E1, 300);
    run_test("Simple Addition (No Carry)", A1, B1, E1);

    // Test Case 2: Addition with a Carry-in/Carry-out within the first limb
    // A = 2^64 - 2 (0xFF..FE)
    // B = 5 (0x00..05)
    // Sum = 2^64 + 3 (0x100..03)
    // R = [3, 1]
    big_int_t *A2 = big_int_init(1);
    big_int_set_limb(A2, MAX_U64 - 1); 
    big_int_t *B2 = big_int_init(1);
    big_int_set_limb(B2, 5);
    big_int_t *E2 = big_int_init(2);
    E2->digits[0] = 3; // Should be (MAX_U64 - 1 + 5) % 2^64 = 4
    E2->digits[1] = 1; // Carry
    E2->num_digits = 2;
    run_test("Single Limb Carry-out (MAX_U64 - 1 + 5)", A2, B2, E2);

    // Test Case 3: Maximum Carry-out (Forces two limbs)
    // A = MAX_U64
    // B = 1
    // Sum = 2^64
    // R = [0, 1]
    big_int_t *A3 = big_int_init(1);
    big_int_set_limb(A3, MAX_U64);
    big_int_t *B3 = big_int_init(1);
    big_int_set_limb(B3, 1);
    big_int_t *E3 = big_int_init(2); 
    E3->digits[0] = 0;
    E3->digits[1] = 1;
    E3->num_digits = 2;
    run_test("Maximum Carry-out (MAX_U64 + 1)", A3, B3, E3);

    // Test Case 4: Addition of Unequal Lengths (and more intermediate carry)
    // A = [MAX_U64, 1]
    // B = [1]
    // R = [0, 2]
    big_int_t *A4 = big_int_init(2);
    A4->digits[0] = MAX_U64;
    A4->digits[1] = 1;
    A4->num_digits = 2;
    big_int_t *B4 = big_int_init(1);
    big_int_set_limb(B4, 1);
    big_int_t *E4 = big_int_init(2);
    E4->digits[0] = 0; 
    E4->digits[1] = 2; 
    E4->num_digits = 2;
    run_test("Unequal Lengths & Intermediate Carry", A4, B4, E4);

    // Test Case 5: Two Limb Overflow
    // A = [HALF_U64, HALF_U64]
    // B = [HALF_U64, HALF_U64]
    // Limb 0: HALF_U64 + HALF_U64 = 0 (CARRY 1)
    // Limb 1: HALF_U64 + HALF_U64 + CARRY(1) = 1 (CARRY 1)
    // Final R = [0, 1, 1]
    big_int_t *A5 = big_int_init(2);
    A5->digits[0] = HALF_U64;
    A5->digits[1] = HALF_U64;
    A5->num_digits = 2;
    big_int_t *B5 = big_int_init(2);
    B5->digits[0] = HALF_U64;
    B5->digits[1] = HALF_U64;
    B5->num_digits = 2;
    big_int_t *E5 = big_int_init(3);
    E5->digits[0] = 0; 
    E5->digits[1] = 1; 
    E5->digits[2] = 1; 
    E5->num_digits = 3;
    run_test("Two Limb Overflow", A5, B5, E5);


    // Clean up
    big_int_free(A1); big_int_free(B1); big_int_free(E1);
    big_int_free(A2); big_int_free(B2); big_int_free(E2);
    big_int_free(A3); big_int_free(B3); big_int_free(E3);
    big_int_free(A4); big_int_free(B4); big_int_free(E4);
    big_int_free(A5); big_int_free(B5); big_int_free(E5);

    return 0;
}*/
