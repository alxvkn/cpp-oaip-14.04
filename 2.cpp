#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

#include "M.hpp"

template <typename T>
class Fraction {
public:
    Fraction(T integer, T numerator, T denominator, bool negative = false)
        : integer(integer),
        numerator(numerator),
        denominator(denominator),
        negative(negative) {
        if (denominator == 0) throw std::runtime_error("zero denominator");
        elevate_sign();
    };

    Fraction(const Fraction<T>& other)
        : integer(other.integer),
        numerator(other.numerator),
        denominator(other.denominator),
        negative(other.negative) {
        if (denominator == 0) throw std::runtime_error("zero denominator");
        elevate_sign();
    };

    bool negative;

    T numerator;
    T denominator;
    T integer;

    std::string to_string() const {
        std::stringstream ss;
        if (negative)
            ss << '-';
        ss << '(' << integer;
        if (numerator)
            ss << ' ' << numerator << ':' << denominator;
        ss << ')';
        return ss.str();
    }

    void denormalize() {
        elevate_sign();
        numerator = numerator + integer * denominator;

        integer = 0;
    }

    void normalize() {
        elevate_sign();
        if (numerator > denominator) {
            integer += (numerator / denominator);
            numerator = (numerator % denominator);
        }
        if (numerator == denominator) {
            integer += 1;
            numerator = 0;
        }
    }

    void elevate_sign() {
        if (integer < 0) {
            negative = !negative;
            integer = -integer;
        }
        if (numerator < 0) {
            negative = !negative;
            numerator = -numerator;
        }
        if (denominator < 0) {
            negative = !negative;
            denominator = -denominator;
        }
    }

    void reduce() {
        T gcd = M<T>::gcd(numerator, denominator);
        numerator /= gcd;
        denominator /= gcd;
    }

    Fraction<T> operator+(const Fraction<T>& other) const {
        Fraction<T> a = Fraction<T>(*this);
        Fraction<T> b = Fraction<T>(other);

        a.denormalize();
        a.reduce();

        b.denormalize();
        b.reduce();

        std::cout << a.to_string() << std::endl;
        std::cout << b.to_string() << std::endl;

        T lcm = M<T>::lcm(a.denominator, b.denominator);
        T a_factor = lcm / a.denominator;
        T b_factor = lcm / b.denominator;

        a.numerator *= a_factor * (a.negative ? -1 : 1);
        a.denominator *= a_factor;

        b.numerator *= b_factor * (b.negative ? -1 : 1);
        b.denominator *= b_factor;

        Fraction<T> result = Fraction<T>(
            0,
            (a.numerator + b.numerator),
            a.denominator
        );

        result.normalize();
        result.reduce();

        return std::move(result);
    }

    // unary
    Fraction<T> operator-() const {
        Fraction<T> n(*this);
        n.negative = !negative;

        return std::move(n);
    }

    // binary
    Fraction<T> operator-(const Fraction<T>& other) const {
        return operator+(-other);
    }

    Fraction<T> operator*(const Fraction<T>& other) const {
        Fraction<T> a(*this);
        Fraction<T> b(other);

        a.denormalize();
        a.reduce();

        b.denormalize();
        b.reduce();

        Fraction<T> result = Fraction<T>(
            0,
            a.numerator * b.numerator,
            a.denominator * b.denominator
        );

        if (a.negative != b.negative)
            result = -result;

        result.normalize();
        result.reduce();

        return std::move(result);
    }

    Fraction<T> operator/(const Fraction<T>& other) const {
        Fraction<T> inv(other);

        inv.denormalize();
        std::swap(inv.numerator, inv.denominator);

        return operator*(inv);
    }
};

int main(int argc, char **argv) {
    auto a = Fraction<int>(0, 0, 1);
    auto b = Fraction<int>(0, 0, 1);

    const std::string prompt = "Введите через пробел (целую часть, числитель, знаменатель): ";

    std::cout << "a: " << prompt;
    std::cin >> a.integer >> a.numerator >> a.denominator;

    std::cout << "b: " << prompt;
    std::cin >> b.integer >> b.numerator >> b.denominator;

    char operation;
    std::cout << "Введите символ действия (+,-,*,/): ";
    std::cin >> operation;

    Fraction<int> result(0, 0, 1);

    switch (operation) {
        case '+':
            result = a + b;
            break;
        case '-':
            result = a - b;
            break;
        case '*':
            result = a * b;
            break;
        case '/':
            result = a / b;
            break;
    }

    std::cout << result.to_string() << std::endl;

    return 0;
}
