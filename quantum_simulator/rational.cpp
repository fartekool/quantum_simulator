#include "rational.h"
#include <iostream>

// ������������
rational::rational() : num(0), den(1) {}; // ��-���������
rational::rational(int num_, int den_) // ���������������
{
	int gcd_ = gcd(num_, den_);
	if (den_ < 0)
	{
		num_ = -num_;
		den_ = -den_;
	}
	num_ /= gcd_;
	den_ /= gcd_;
	num = num_;
	den = den_;
}
rational::rational(const int& a) : num(a), den(1) {}; // ��������������
rational::rational(const rational& another) // �����������
{
	num = another.num;
	den = another.den;
}
//
// ��������
rational operator+(const rational& a, const rational& b) // ������� ��������
{
	rational right(b);
	return rational(a.num * right.den + right.num * a.den, a.den * right.den);
}
rational& operator++(rational& a) // ���������� ���������
{
	a.num += a.den;
	return a;
}
rational operator++(rational& a, int) // ����������� ���������
{
	auto old_value = a;
	a.num += a.den;
	return old_value;
}
rational& rational::operator+=(const rational& another)
{
	num = num * another.den + another.num * den;
	den *= another.den;
	int gcd_ = gcd(num, den);
	num /= gcd_;
	den /= gcd_;
	return *this;
}
//
// ���������
rational operator-(const rational& a, const rational& b) // ������� ���������
{
	rational right(b);
	return rational(a.num * right.den - right.num * a.den, a.den * right.den);
}
rational& operator--(rational& a) // ���������� ���������
{
	a.num -= a.den;
	return a;
}
rational operator--(rational& a, int) // ����������� ���������
{
	auto old_value = a;
	a.num -= a.den;
	return old_value;
}
rational& rational::operator-=(const rational& a)
{
	num = num * a.den - a.num * den;
	den *= a.den;
	int gcd_ = gcd(num, den);
	num /= gcd_;
	den /= gcd_;
	return *this;
}
rational& operator-(rational& a) // �����
{
	a.num *= -1;
	return a;
}
//
// ���������
rational operator*(const rational& a, const rational& b) // ������� ���������
{
	rational right(b);
	return rational(a.num * right.num, a.den * right.den);
}
rational& rational::operator*=(const rational& a)
{
	num *= a.num;
	den *= a.den;
	int gcd_ = gcd(num, den);
	num /= gcd_;
	den /= gcd_;
	return *this;
}
//
// �������
rational operator/(const rational& a, const rational& b) // ������� �������
{
	rational right(b);
	return rational(a.num * right.den, a.den * right.num);
}
rational& rational::operator/=(const rational& a)
{
	num *= a.den;
	den *= a.num;
	int gcd_ = gcd(num, den);
	num /= gcd_;
	den /= gcd_;
	return *this;
}
//
// ������������
rational& rational::operator=(const rational& another)
{
	num = another.num;
	den = another.den;
	return *this;
}
//
// ���������
bool operator==(const rational& lhs, const rational& rhs) // ==
{
	if (lhs.num == rhs.num and lhs.den == rhs.den)
		return true;
	else
		return false;
}
bool operator!=(const rational& lhs, const rational& rhs) // !=
{
	return !(operator==(lhs, rhs));
}
bool operator>(const rational& lhs, const rational& rhs) // >
{
	if (lhs.num * rhs.den > rhs.num * lhs.den)
		return true;
	else
		return false;
}
bool operator<(const rational& lhs, const rational& rhs) // <
{
	if (lhs.num * rhs.den < rhs.num * lhs.den)
		return true;
	else
		return false;
}
bool operator>=(const rational& lhs, const rational& rhs) // >=
{
	return !(operator<(lhs, rhs));
}
bool operator<=(const rational& lhs, const rational& rhs) // <=
{
	return !(operator>(lhs, rhs));
}
//
// ���������� ���������� ������ (����� � �������)
std::ostream& operator<<(std::ostream& os, const rational& another)
{
	if (another.den == 1)
		os << another.num;
	else
		os << another.num << "/" << another.den;
	return os;
}
//

