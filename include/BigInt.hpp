#pragma once
#include <vector>
#include <cstdint>
#include <string>
#include <iostream>

/* length (in 8 byte words) if numbers are greater than to use 
 * karatsuba multiplication algorithm. Chosen arbitrarily, need
 * to tune for better performance
 */
#define KARATSUBA_THRESH 4 

class BigInt {
private:
    std::vector<uint64_t> digits;  // Little-endian limbs (least significant first)
    //static const uint64_t BASE = (1ULL << 64);
    BigInt directMultiplication(const BigInt& rhs) const;
    void trim(); // remove leading zeros

public:
    // Constructors
    BigInt();
    BigInt(uint64_t value);
    BigInt(const std::string& str);

    // Copy and move semantics
    BigInt(const BigInt& other) = default;
    BigInt(BigInt&& other) noexcept = default;
    BigInt& operator=(const BigInt& other) = default;
    BigInt& operator=(BigInt&& other) noexcept = default;

    // Arithmetic operators
    BigInt operator+(const BigInt& rhs) const;
    BigInt& operator+=(const BigInt& rhs);
    BigInt operator-(const BigInt& rhs) const;
    BigInt& operator-=(const BigInt& rhs);
    BigInt operator*(const BigInt& rhs) const;
    BigInt& operator*=(const BigInt& rhs);

    // Comparison operators
    bool operator==(const BigInt& rhs) const;
    bool operator!=(const BigInt& rhs) const;
    bool operator<(const BigInt& rhs) const;
    bool operator>(const BigInt& rhs) const;
    bool operator<=(const BigInt& rhs) const;
    bool operator>=(const BigInt& rhs) const;

    // Utility
    std::string to_string() const;

    // Stream output
    friend std::ostream& operator<<(std::ostream& os, const BigInt& val);
};


