#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Simple BigInt class for arbitrary-precision arithmetic
class BigInt {
public:
    // Store digits in reverse order for easy calculation
    vector<int> digits;

    // Constructors
    BigInt() {
        digits.push_back(0);
    }

    BigInt(int num) {
        if (num == 0) {
            digits.push_back(0);
        } else {
            while (num > 0) {
                digits.push_back(num % 10);
                num /= 10;
            }
        }
    }

    // Addition operator
    BigInt operator+(const BigInt& b) const {
        BigInt result;
        result.digits.clear();

        int carry = 0;
        size_t i = 0;
        while (i < digits.size() || i < b.digits.size() || carry) {
            int sum = carry;
            if (i < digits.size()) sum += digits[i];
            if (i < b.digits.size()) sum += b.digits[i];
            result.digits.push_back(sum % 10);
            carry = sum / 10;
            i++;
        }
        return result;
    }

    // Multiplication operator (by int)
    BigInt operator*(int num) const {
        BigInt result;
        result.digits.clear();

        int carry = 0;
        for (size_t i = 0; i < digits.size() || carry; i++) {
            int prod = carry;
            if (i < digits.size()) prod += digits[i] * num;
            result.digits.push_back(prod % 10);
            carry = prod / 10;
        }
        return result;
    }

    // Print the BigInt number
    void print() const {
        for (vector<int>::const_reverse_iterator it = digits.rbegin(); it != digits.rend(); ++it)
            cout << *it;
        cout << endl;
    }
};

int main() {
    int N, A;
    cin >> N >> A;

    BigInt sum;
    BigInt B(A); // Initialize B as BigInt with value A

    for (int i = 1; i <= N; i++) {
        // sum = sum + i * B
        BigInt term = B * i;
        sum = sum + term;

        // B = B * A
        B = B * A;
    }

    sum.print();

    return 0;
}