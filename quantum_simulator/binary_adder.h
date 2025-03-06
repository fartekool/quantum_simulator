#pragma once
#include <vector>
#include <string>
using namespace std;


vector<bool> Convert_string_to_vector_of_bool(string num)
{
	vector<bool> res;
	for (int i = num.length(); i > 0; --i)
	{
		bool b = (num[i - 1] == '1');
		res.push_back(b);
	}
	return res;
}

string Convert_vector_of_bool_to_string(vector<bool> num)
{
	string res = "";
	for (size_t i = num.size(); i > 0; --i)
	{
		if (num[i-1])
			res += '1';
		else
			res += '0';
	}
	return res;
}
string Adder(string num1, string num2)
{
	size_t n = max(num1.size(), num2.size());

	vector<bool> A = Convert_string_to_vector_of_bool(num1);
	vector<bool> B = Convert_string_to_vector_of_bool(num2);
	vector<bool> sum;

	if (A.size() <= B.size())
	{
		while (A.size() != B.size())
			A.push_back(false);
	}
	else
	{
		while (A.size() != B.size())
			B.push_back(false);
	}


	bool carry = 0;

	size_t i;
	for (i = 0; i < n; ++i)
	{
		bool sum_of_bits = A[i] ^ B[i] ^ carry;
		carry = (A[i] & B[i]) | (carry & (A[i] ^ B[i]));
		sum.push_back(sum_of_bits);
	}
	if (carry)
		sum.push_back(carry);
	return Convert_vector_of_bool_to_string(sum);
}


