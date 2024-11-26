#pragma once
#include <vector>
#include <iostream>
#include "rational.h"
template <class T>
class matrix
{
public:
	int m; 
	int n; 
	std::vector<std::vector<T>> numbers;
public:
	matrix() : m(2), n(2)
	{
		for (int i = 0; i < m; ++i)
			numbers.push_back({ 0, 0 });
	}
	matrix(int m_, int n_) : m(m_), n(n_)
	{

		std::vector<T> str;
		for (int j = 0; j < n; ++j)
		{
			str.push_back(0);
		}
		for (int i = 0; i < m; ++i)
		{
			numbers.push_back(str);
		}
	}
	matrix(std::vector<std::vector<T>> numbers_) : numbers(numbers_), m(numbers_.size()), n(numbers_[0].size()) {}

	matrix(const matrix<T>& A) : m(A.m), n(A.n), numbers(A.numbers) {}
	void init_matrix(std::vector<std::vector<T>> numbers_)
	{
		m = numbers_.size();
		n = numbers_[0].size();
		numbers = numbers_;
	}
	T det() const
	{
		if (m != n or m < 1)
			throw 0;
		T determinant = 0;
		if (m > 1)
		{
			for (int i = 0; i < n; ++i)
			{
				matrix A(this->algebraic_complement(1, i + 1));
				determinant += pow(-1, i) * numbers[0][i] * A.det();

			}
		}
		if (m == 1)
			determinant = numbers[0][0];
		return determinant;
	}
	matrix algebraic_complement(int m_, int n_) const
	{
		matrix A(*this);
		A.numbers.erase(A.numbers.begin() + m_ - 1);
		A.m--;
		for (int i = 0; i < A.m; ++i)
		{
			A.numbers[i].erase(A.numbers[i].begin() + n_ - 1);
		}
		A.n--;
		return A;
	}
	matrix transposition() const
	{
		matrix A(n, m);
		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < n; j++)
			{
				A.numbers[j][i] = numbers[i][j];
			}
		}
		return A;

	}
	matrix inverse_matrix() const
	{
		if (m != n)
			throw 0;
		T determinant = this->det();
		matrix A(m, n);
		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < n; j++)
			{
				A.numbers[i][j] = T(pow(-1, i + j)) * this->algebraic_complement(i + 1, j + 1).det() / determinant;
			}
		}
		return A.transposition();
	}
	matrix operator+(const matrix& A) const
	{
		if (m != A.m or n != A.n)
			throw 0;
		matrix answer(m, n);
		for (int i = 0; i < m; ++i)
		{
			for (int j = 0; j < n; ++j)
			{
				answer.numbers[i][j] = numbers[i][j] + A.numbers[i][j];
			}
		}
		return answer;
	}
	matrix operator-(const matrix& A) const
	{
		if (m != A.m or n != A.n)
			throw 0;
		matrix answer(m, n);
		for (int i = 0; i < m; ++i)
		{
			for (int j = 0; j < n; ++j)
			{
				answer.numbers[i][j] = numbers[i][j] - A.numbers[i][j];
			}
		}
		return answer;
	}

	matrix operator*(const matrix& A) const
	{
		if (n != A.m)
			throw 0;
		matrix answer(m, A.n);
		for (int i = 0; i < answer.m; ++i)
		{
			for (int j = 0; j < answer.n; ++j)
			{
				T elem = 0;
				for (int k = 0; k < n; ++k)
				{

					elem += numbers[i][k] * A.numbers[k][j];

				}
				answer.numbers[i][j] = elem;
			}
		}
		return answer;
	}
	std::vector<T> operator*(const std::vector<T>& vec)
	{
		if (n != vec.size())
			throw 0;
		std::vector<T> result;
		for (int i = 0; i < m; ++i)
		{	
			T sum = 0;
			for (int j = 0; j < n; ++j)
			{	
				sum += numbers[i][j] * vec[j];
			}
			result.push_back(sum);
		}
		return result;
	}
	matrix operator&&(const matrix& A)
	{
		matrix answer(m * A.m, n * A.n);
		for (int i = 0; i < m; ++i)
		{
			for (int j = 0; j < n; ++j)
			{
				for (int p = 0; p < A.m; ++p)
				{
					for (int q = 0; q < A.n; ++q)
					{
						answer.numbers[i*A.m + p][j*A.n + q] = numbers[i][j] * A.numbers[p][q];
						//std::cout << "i = " << i << ", j = " << j << ", p = " << p << ", q = " << q << std::endl << answer << std::endl;
					}
				}
			}
		}
		return answer;
	}
	matrix& operator=(const matrix& A)
	{
		m = A.m;
		n = A.n;
		numbers = A.numbers;
		return *this;
	}
	friend std::ostream& operator<<(std::ostream& os, const matrix& A)
	{
		for (int i = 0; i < A.m; ++i)
		{
			for (int j = 0; j < A.n; ++j)
			{
				os << A.numbers[i][j] << " ";
			}
			os << std::endl;
		}
		return os;
	}
};