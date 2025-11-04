#include <iostream>
#include <BigInt.hpp>

using namespace std;

void test_constructor_and_to_string() {
    cout << "=== Constructor & to_string Test ===" << endl;
    BigInt a(12345);
    BigInt b("987654321");
    cout << "a = " << a.to_string() << " (Expected: 12345)" << endl;
    cout << "b = " << b.to_string() << " (Expected: 987654321)" << endl;
    cout << endl;
}

void test_copy_and_move() {
    cout << "=== Copy & Move Semantics Test ===" << endl;
    BigInt a("9999");
    BigInt b = a; // Copy constructor
    cout << "b (copy of a): " << b.to_string() << " (Expected: 9999)" << endl;

    BigInt c("1234");
    c = a; // Copy assignment
    cout << "c (after copy assignment from a): " << c.to_string() << " (Expected: 9999)" << endl;

    BigInt d = std::move(a); // Move constructor
    cout << "d (after move): " << d.to_string() << " (Expected: 9999)" << endl;
    cout << "a (moved-from): " << a.to_string() << " (Expected: 0 or empty)" << endl;

    BigInt e("8888");
    e = std::move(b); // Move assignment
    cout << "e (after move assignment): " << e.to_string() << " (Expected: 9999)" << endl;
    cout << "b (moved-from): " << b.to_string() << " (Expected: 0 or empty)" << endl;
    cout << endl;
}

void test_addition() {
    cout << "=== Addition Test ===" << endl;
    BigInt a("999");
    BigInt b("1");
    BigInt c = a + b;
    cout << "999 + 1 = " << c.to_string() << " (Expected: 1000)" << endl;

    a += b;
    cout << "a += b -> " << a.to_string() << " (Expected: 1000)" << endl;

    BigInt d("123456789");
    BigInt e("987654321");
    BigInt f = d + e;
    cout << "123456789 + 987654321 = " << f.to_string() << " (Expected: 1111111110)" << endl;
    cout << endl;
}

void test_comparisons() {
    cout << "=== Comparison Test ===" << endl;
    BigInt a("100");
    BigInt b("200");
    BigInt c("100");

    cout << "a == b? " << (a == b) << " (Expected: 0)" << endl;
    cout << "a == c? " << (a == c) << " (Expected: 1)" << endl;
    cout << "a < b? " << (a < b) << " (Expected: 1)" << endl;
    cout << "b > a? " << (b > a) << " (Expected: 1)" << endl;
    cout << endl;
}

void test_subtraction() {
    cout << "=== Subtraction Test ===" << endl;
    BigInt a("1000");
    BigInt b("1");
    BigInt c = a - b;
    cout << "1000 - 1 = " << c.to_string() << " (Expected: 999)" << endl;

    a -= b;
    cout << "a -= b -> " << a.to_string() << " (Expected: 999)" << endl;
    cout << endl;
}

void test_multiplication() {
    cout << "=== Multiplication Test ===" << endl;
    BigInt a("123");
    BigInt b("456");
    BigInt c = a * b;
    cout << "123 * 456 = " << c.to_string() << " (Expected: 56088)" << endl;
    cout << endl;
}

void test_edge_cases() {
    cout << "=== Edge Cases Test ===" << endl;
    BigInt zero("0");
    BigInt one("1");

    cout << "0 + 1 = " << (zero + one).to_string() << " (Expected: 1)" << endl;
    cout << "0 * 12345 = " << (zero * BigInt("12345")).to_string() << " (Expected: 0)" << endl;

    BigInt large1("999999999999999999999999999999");
    BigInt large2("1");
    cout << "Large + 1 = " << (large1 + large2).to_string()
         << " (Expected: 1000000000000000000000000000000)" << endl;
    cout << endl;
}

int main() {
    cout << "===== BigInt Unit Tests =====" << endl << endl;
    test_constructor_and_to_string();
    test_copy_and_move();
    test_addition();
    test_subtraction();
    test_multiplication();
    test_comparisons();
    test_edge_cases();
    cout << "===== All Tests Complete =====" << endl;
    return 0;
}

