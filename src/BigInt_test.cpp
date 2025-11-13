#include "BigInt.hpp"
#include "math_utils.hpp"

#include <random>
#include <algorithm>
#include <iostream>
#include <iomanip>

using namespace std::chrono;

// --- UTILITY: Random BigInt Generation ---

BigInt generate_random_bigint(size_t num_limbs) {
    if (num_limbs == 0) return BigInt(0);

    BigInt result;
    result.digits.clear(); // Clear the default [0]

    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> distrib;

    result.digits.resize(num_limbs);
    for (size_t i = 0; i < num_limbs; ++i) {
        result.digits[i] = distrib(gen);
    }
    result.trim();
    return result;
}

// --- BENCHMARK 1: O(N) Addition/Subtraction Stress ---
void test_add_sub_stress(int iterations, size_t num_limbs) {
    std::cout << "\n--- 1. Addition/Subtraction Stress (O(N), In-Place) ---\n";
    std::cout << "Testing " << iterations << " iterations on " << num_limbs << " limbs.\n";

    BigInt A = generate_random_bigint(num_limbs);
    BigInt B = generate_random_bigint(num_limbs);
    
    // Ensure A >= B for safe subtraction
    if (A < B) std::swap(A, B);

    auto start = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        // Test in-place operators
        A += B;
        A -= B; // This should bring A back to its initial value (if memory is stable)
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    std::cout << "A/S Stress Time: " << duration.count() << " ms.\n";
    // Profile: Look for low BigInt::operator+= and BigInt::operator-= time, and minimal malloc/free.
}

// --- BENCHMARK 2: O(N^2) Base Case Speed Test ---
void test_direct_multiplication_stress(int iterations) {
    std::cout << "\n--- 2. Direct Multiplication Stress (O(N^2) Base Case) ---\n";
    std::cout << "Testing " << iterations << " iterations on " << KARATSUBA_THRESH << " limbs.\n";

    size_t size = KARATSUBA_THRESH;
    BigInt A = generate_random_bigint(size);
    BigInt B = generate_random_bigint(size);
    
    // Note: Calling directMultiplication() directly bypasses the operator* fallback logic.
    auto start = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        BigInt C = A.directMultiplication(B);
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    std::cout << "Direct Multiplication Time: " << duration.count() << " ms.\n";
    // Profile: Time should be dominated by BigInt::directMultiplication. This confirms its efficiency.
}

// --- BENCHMARK 3: Karatsuba O(N^1.585) Scaling Check ---
void test_karatsuba_scaling(size_t base_limbs, int iterations) {
    std::cout << "\n--- 3. Karatsuba Scaling Check (O(N^1.585)) ---\n";
    std::cout << "Running " << iterations << " times with base limbs: " << base_limbs << "\n";

    // Test N limbs
    BigInt N1 = generate_random_bigint(base_limbs);
    BigInt N2 = generate_random_bigint(base_limbs);

    auto start_N = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        volatile BigInt Result_N = N1 * N2; // Use volatile to prevent optimization
    }
    auto stop_N = high_resolution_clock::now();
    auto duration_N = duration_cast<microseconds>(stop_N - start_N);

    std::cout << "Size " << base_limbs << " limbs (T_N): " << duration_N.count() << " us.\n";

    // Test 2N limbs
    BigInt N_2_1 = generate_random_bigint(base_limbs * 2);
    BigInt N_2_2 = generate_random_bigint(base_limbs * 2);

    auto start_2N = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        volatile BigInt Result_2N = N_2_1 * N_2_2; // Use volatile to prevent optimization
    }
    auto stop_2N = high_resolution_clock::now();
    auto duration_2N = duration_cast<microseconds>(stop_2N - start_2N);

    std::cout << "Size " << base_limbs * 2 << " limbs (T_2N): " << duration_2N.count() << " us.\n";

    if (duration_N.count() > 0) {
        double ratio = (double)duration_2N.count() / duration_N.count();
        std::cout << "Time Ratio (T_2N / T_N): " << std::fixed << std::setprecision(2) << ratio << "\n";
        // Target ratio for Karatsuba should be close to 2^1.585 ≈ 3.0.
    }
    // Profile: Time should be dominated by BigInt::operator* and recursive calls.
}

// --- TEST 4: Algebraic Identity (Correctness and Stability) ---
void test_algebraic_identity() {
    std::cout << "\n--- 4. Algebraic Identity Check (A^2 - B^2 = (A+B)(A-B)) ---\n";
    
    // Choose sizes well above the Karatsuba threshold
    BigInt A = generate_random_bigint(KARATSUBA_THRESH * 2 + 1);
    BigInt B = generate_random_bigint(KARATSUBA_THRESH + 1); 

    // Calculate R1 = A^2 - B^2
    BigInt R1_A2 = A * A;
    BigInt R1_B2 = B * B;
    BigInt R1 = R1_A2 - R1_B2; // Ensure A^2 > B^2 (A is larger size)

    // Calculate R2 = (A + B) * (A - B)
    BigInt A_plus_B = A + B;
    BigInt A_minus_B = A - B;
    BigInt R2 = A_plus_B * A_minus_B;

    std::cout << "R1 Size: " << R1.size() << " limbs. R2 Size: " << R2.size() << " limbs.\n";

    if (R1 == R2) {
        std::cout << "Identity Test PASSED. (R1 == R2).\n";
    } else {
        std::cout << "Identity Test FAILED. Result Mismatch.\n";
        // If FAILED, a core bug exists in +, -, or * operators.
    }
}

// --- BENCHMARK 5: Final Fibonacci System Test ---
void test_fibonacci_performance(size_t n, int iterations) {
    std::cout << "\n--- 5. Fibonacci Matrix Exponentiation (System Test) ---\n";
    std::cout << "Calculating F(" << n << ") " << iterations << " times...\n";

    FibonacciMatrix F_base; // Assumes default constructor is the base matrix
    F_base.data[0][0] = BigInt(1);
    F_base.data[0][1] = BigInt(1);
    F_base.data[1][0] = BigInt(1);
    F_base.data[1][1] = BigInt(0);

    auto start = high_resolution_clock::now();
    FibonacciMatrix Result;
    for (int i = 0; i < iterations; ++i) {
        Result = matrix_pow(F_base, n);
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    
    // The nth Fibonacci number is F_n = Result.data[0][1]
    std::cout << "Total F(" << n << ") Time: " << duration.count() << " ms.\n";
    std::cout << "Size of F(" << n << "): " << Result.data[0][1].size() << " limbs.\n";
    
    // Profile: This shows the total time spent in all components (matrix_square, operator*, memory).
}

int main() {
    // Correctness Test
    //test_algebraic_identity();

    // Efficiency Benchmarks (Increased iterations for sampling)
    // Runs for ~5-10 seconds
    //test_add_sub_stress(500000000, 100); 
    
    // Runs for ~5-10 seconds
    //test_direct_multiplication_stress(125000000); 
    
    // Runs for ~10-20 seconds
    test_karatsuba_scaling(KARATSUBA_THRESH * 8, 1000); 

    // Runs for ~5-10 seconds
    //test_fibonacci_performance(11000, 100); 

    return 0;
}
