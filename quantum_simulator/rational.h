#pragma once
#include <iostream>
#include <ciso646>
class rational
{
    int num;
    int den;
    int gcd(int a, int b)
    {
        int tmp;
        while (b != 0)
        {
            tmp = b;
            a = a % b;
            b = a;
            a = tmp;
        }
        return (a >= 0) ? a : -a;
    }
public:
    rational();
    rational(int num_, int den_);
    rational(const int& a);
    rational(const rational& another);
    //
    friend rational operator+(const rational& a, const rational& b);
    friend rational& operator++(rational& a);
    friend rational operator++(rational& a, int);
    rational& operator+=(const rational& another);
    //
    friend rational operator-(const rational& a, const rational& b);
    friend rational& operator--(rational& a);
    friend rational operator--(rational& a, int);
    rational& operator-=(const rational& a);
    friend rational& operator-(rational& a);
    //
    friend rational operator*(const rational& a, const rational& b);
    rational& operator*=(const rational& another);
    //
    friend rational operator/(const rational& a, const rational& b);
    rational& operator/=(const rational& another);
    //
    rational& operator=(const rational& another);
    //
    friend bool operator==(const rational& lhs, const rational& rhs); // ==
    friend bool operator!=(const rational& lhs, const rational& rhs); // !=
    friend bool operator>(const rational& lhs, const rational& rhs); // >
    friend bool operator<(const rational& lhs, const rational& rhs); // <
    friend bool operator>=(const rational& lhs, const rational& rhs); // >=
    friend bool operator<=(const rational& lhs, const rational& rhs); // <=
    //
    friend std::ostream& operator<<(std::ostream& os, const rational& another);
};
