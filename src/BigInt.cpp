#include "BigInt.hpp"
#include <sstream>
#include <iomanip>
#include <stdexcept>

BigInt::BigInt() : digits(1, 0) {}
BigInt::BigInt(uint64_t value) : digits(1, value) { trim(); }
BigInt::BigInt(const std::string& str) {
    *this = BigInt(0);
    for (char c : str) {
        if (c < '0' || c > '9') continue;
        *this *= BigInt(10);
        *this += BigInt(static_cast<uint64_t>(c - '0'));
    }
}

BigInt::BigInt(BigInt&& other) noexcept : digits(std::move(other.digits)) {
    // other is now in a valid but unspecified state, maybe set its vector size to 0 if needed
}

// Move Assignment Operator
BigInt& BigInt::operator=(BigInt&& other) noexcept {
    if (this != &other) {
        digits = std::move(other.digits);
    }
    return *this;
}

void BigInt::trim() {
    while (digits.size() > 1 && digits.back() == 0)
        digits.pop_back();
}

BigInt BigInt::directMultiplication(const BigInt& rhs) const {
    BigInt result;
    result.digits.assign(digits.size() + rhs.digits.size(), 0);

    for (size_t i = 0; i < digits.size(); ++i) {
        unsigned __int128 carry = 0;
        for (size_t j = 0; j < rhs.digits.size() || carry; ++j) {
            unsigned __int128 cur = result.digits[i + j] +
                (unsigned __int128)digits[i] * (j < rhs.digits.size() ? rhs.digits[j] : 0) + carry;
            result.digits[i + j] = static_cast<uint64_t>(cur);
            carry = cur >> 64;
        }
    }

    result.trim();
    return result;
}

BigInt BigInt::operator+(const BigInt& rhs) const {
    BigInt result;
    result.digits.resize(std::max(digits.size(), rhs.digits.size()) + 1, 0);
    unsigned __int128 carry = 0;

    for (size_t i = 0; i < result.digits.size(); ++i) {
        unsigned __int128 a = (i < digits.size()) ? digits[i] : 0;
        unsigned __int128 b = (i < rhs.digits.size()) ? rhs.digits[i] : 0;
        unsigned __int128 sum = a + b + carry;
        result.digits[i] = static_cast<uint64_t>(sum);
        carry = sum >> 64;
    }
    result.trim();
    return result;
}

BigInt& BigInt::operator+=(const BigInt& rhs) {
    *this = *this + rhs;
    return *this;
}

BigInt BigInt::operator-(const BigInt& rhs) const {
    if (*this < rhs) throw std::underflow_error("Negative BigInt subtraction");

    BigInt result = *this;
    unsigned __int128 borrow = 0;

    for (size_t i = 0; i < rhs.digits.size() || borrow; ++i) {
        unsigned __int128 a = result.digits[i];
        unsigned __int128 b = (i < rhs.digits.size()) ? rhs.digits[i] : 0;
        unsigned __int128 diff = a - b - borrow;
        result.digits[i] = static_cast<uint64_t>(diff);
        borrow = (diff >> 64) & 1;
    }

    result.trim();
    return result;
}

BigInt& BigInt::operator-=(const BigInt& rhs) {
    *this = *this - rhs;
    return *this;
}

BigInt BigInt::operator*(const BigInt& rhs) const {
  if ( this->digits.size() < KARATSUBA_THRESH || rhs.digits.size() < KARATSUBA_THRESH) {
    return this->directMultiplication(rhs);
  }
  size_t n = std::max(this->digits.size(), rhs.digits.size());

  if ( n % 2 == 1) n++;
  size_t half_len = n / 2;
  BigInt x = * this;
  BigInt y = rhs;
  x.padToLength(n); 
  y.padToLength(n);

  std::pair<BigInt, BigInt> xParts = x.split(half_len);
  std::pair<BigInt, BigInt> yParts = y.split(half_len);

  BigInt x0 = xParts.first; 
  BigInt x1 = xParts.second;
  BigInt y0 = yParts.first;
  BigInt y1 = yParts.second;

  BigInt z2 = x1 * y1;
  BigInt z0 = x0 * y0;
  BigInt z1 = (x1 + x0) * (y1 + y0);
  z1 = z1 - z2 - z0;

  BigInt result = z2.shiftLeftByLimbs(n);
  result =result + z1.shiftLeftByLimbs(half_len);
  result = result + z0;

  result.trim();
  return result;
}

BigInt& BigInt::operator*=(const BigInt& rhs) {
    *this = *this * rhs;
    return *this;
}

bool BigInt::operator==(const BigInt& rhs) const {
    return digits == rhs.digits;
}
bool BigInt::operator!=(const BigInt& rhs) const { return !(*this == rhs); }

bool BigInt::operator<(const BigInt& rhs) const {
    if (digits.size() != rhs.digits.size())
        return digits.size() < rhs.digits.size();
    for (size_t i = digits.size(); i-- > 0;) {
        if (digits[i] != rhs.digits[i])
            return digits[i] < rhs.digits[i];
    }
    return false;
}

bool BigInt::operator>(const BigInt& rhs) const { return rhs < *this; }
bool BigInt::operator<=(const BigInt& rhs) const { return !(rhs < *this); }
bool BigInt::operator>=(const BigInt& rhs) const { return !(*this < rhs); }

BigInt BigInt::shiftLeftByLimbs(size_t num_limbs) const {
  BigInt result;
  result.digits.assign(num_limbs, 0);
  result.digits.insert(result.digits.end(), this->digits.begin(), this->digits.end()); 
  result.trim();
  return result;
}

void BigInt::padToLength(size_t new_length) {
  if (digits.size() < new_length) {
    digits.resize(new_length, 0);
  }
}

std::pair<BigInt, BigInt> BigInt::split(size_t half_len) const {
  BigInt low, high;

  if (half_len > 0) {
    low.digits.assign(digits.begin(), digits.begin() + std::min((size_t) digits.size(), half_len));     low.trim();
  } 
  if (digits.size() > half_len) {
    high.digits.assign(digits.begin() + half_len, digits.end());
    high.trim();
  }
  return {low, high};
}

std::string BigInt::to_string() const {
    if (*this == BigInt(0)) return "0";
    BigInt temp = *this;
    std::string out;
    while (temp > BigInt(0)) {
        BigInt div10;
        unsigned __int128 remainder = 0;
        div10.digits.resize(temp.digits.size(), 0);

        for (size_t i = temp.digits.size(); i-- > 0;) {
            unsigned __int128 cur = (remainder << 64) + temp.digits[i];
            div10.digits[i] = static_cast<uint64_t>(cur / 10);
            remainder = static_cast<uint64_t>(cur % 10);
        }

        div10.trim();
        out.push_back('0' + remainder);
        temp = div10;
    }
    std::reverse(out.begin(), out.end());
    return out;
}

std::ostream& operator<<(std::ostream& os, const BigInt& val) {
    os << val.to_string();
    return os;
}

