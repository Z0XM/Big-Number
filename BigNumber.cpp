#include "BigNumber.hpp"

#include <iostream>
#include <algorithm>
#include <sstream>
#include <cmath>

int toDigit(char c) { return c - 48; }
char toChar(int c) { return c + 48; }
int pow10(int x) { int n = 1; for (int i = 0; i < x; i++)n *= 10; return n; }

std::string trailZeroesFront(const std::string& num)
{
	bool neg = num[0] == '-';
	int start = neg;
	for (; num[start] == '0' && start < num.size(); start++);
	if (start == num.size())return "0";
	return num.substr(start);
}

std::string trailZeroesBack(const std::string& num)
{
	int count;
	for (count = 0; count < num.size() && num[num.size() - count - 1] == '0'; count++);
	if (count == num.size())return "0";
	return num.substr(0, num.size() - count);
}

void fillVec(std::vector<BigNumber::Int>& vec, int x)
{
	vec.clear();
	while (x != 0) {
		vec.push_back(floor(x % BigNumber::EXP_OF_10));
		x /= BigNumber::EXP_OF_10;
	}
}

template<typename begin_it, typename end_it>
std::string toString(begin_it begin, end_it end, bool fractional = false) {
	std::ostringstream out;
	for (auto i = begin; i != end; i++) {
		if (i != begin || fractional)
			for (int ten = BigNumber::EXP_OF_10/10; ten != 1 && *i < ten; ten /= 10)
				out << '0';
		out << *i;
	}
	return out.str();
}

BigNumber::BigNumber()
{
}

BigNumber::BigNumber(int x) 
{
	negative = x < 0;
	x = abs(x);

	integral.str = std::to_string(x);
	fillVec(integral.vec, x);
}

BigNumber::BigNumber(double x)
{
	negative = x < 0;
	x = abs(x);
	int n = floor(x);

	integral.str = std::to_string(n);
	fillVec(integral.vec, n);

	fractional.str = trailZeroesBack(std::to_string(x - n));
	fractional.str.erase(0, 2);
	fillVec(fractional.vec, (x - n) * pow10(fractional.str.size()));

}

BigNumber::BigNumber(const char* num)
{
	std::string str = num;

	if (str[0] == '-') {
		str.erase(0, 1);
		negative = true;
	}

	int k = str.find('.');
	if (k != str.npos) {
		str[k] = ' ';
	}

	std::stringstream ss(str);
	ss >> integral.str >> fractional.str;

	integral.str = trailZeroesFront(integral.str);

	integral.vec.clear();
	for (int i = 0; i < integral.str.size(); i++) {
		if (i % POWER_OF_10 == 0)integral.vec.push_back(0);
		integral.vec.back() += toDigit(integral.str[integral.str.size() - 1 - i]) * (int)pow(10, i % POWER_OF_10);
	}

	fractional.str = trailZeroesBack(fractional.str);

	fractional.vec.clear();
	for (int i = 0; i < fractional.str.size(); i++) {
		if (i % POWER_OF_10 == 0)fractional.vec.push_back(0);
		fractional.vec.back() += toDigit(fractional.str[i]) * (int)pow(10, (POWER_OF_10 - i % POWER_OF_10 - 1));
	}
}

BigNumber::BigNumber(const std::string& num)
{
	std::string str = num;
	
	if (str[0] == '-') {
		str.erase(0, 1);
		negative = true;
	}

	int k = str.find('.');
	if (k != str.npos) {
		str[k] = ' ';
	}

	std::stringstream ss(str);
	ss >> integral.str >> fractional.str;

	integral.str = trailZeroesFront(integral.str);

	integral.vec.clear();
	for (int i = 0; i < integral.str.size(); i++) {
		if (i % POWER_OF_10 == 0)integral.vec.push_back(0);
		integral.vec.back() += toDigit(integral.str[integral.str.size() - 1 - i]) * (int)pow(10, i % POWER_OF_10);
	}

	fractional.str = trailZeroesBack(fractional.str);

	fractional.vec.clear();
	for (int i = 0; i < fractional.str.size(); i++) {
		if (i % POWER_OF_10 == 0)fractional.vec.push_back(0);
		fractional.vec.back() += toDigit(fractional.str[i]) * (int)pow(10, (POWER_OF_10 - i % POWER_OF_10 - 1));
	}
}

BigNumber::~BigNumber()
{
}

std::string BigNumber::asString() const
{
	std::string result;
	if (negative) result += '-';
	result += integral.str;
	if (fractional.str != "0")result += '.' + fractional.str;
	return result;
}

std::ostream& operator<<(std::ostream& out, const BigNumber& nBig)
{
	out << "\nNegative   => " << nBig.negative;
	out << "\nIntegral   => vec : ";
	for (auto i = nBig.integral.vec.crbegin(); i != nBig.integral.vec.crend(); i++)out << *i << " ";
	out << "\n           => str : " << nBig.integral.str;

	out << "\nFractional => vec : ";
	for (auto& i : nBig.fractional.vec)out << i << " ";
	out << "\n           => str : " << nBig.fractional.str;

	return out;
}

bool operator==(const BigNumber& nBig1, const BigNumber& nBig2)
{
	return nBig1.integral.str == nBig2.integral.str && nBig1.fractional.str == nBig2.fractional.str && nBig1.negative == nBig2.negative;
}

bool operator!=(const BigNumber& nBig1, const BigNumber& nBig2)
{
	return !(nBig1 == nBig2);
}

bool operator<(const BigNumber& nBig1, const BigNumber& nBig2)
{
	if (nBig1.negative > nBig2.negative)return true;
	if (nBig1.negative < nBig2.negative)return false;

	if (nBig1.integral.str.size() < nBig2.integral.str.size())return !nBig1.negative;
	if (nBig1.integral.str.size() > nBig2.integral.str.size())return nBig1.negative;

	for (int i = nBig1.integral.vec.size() - 1; i >= 0; i--)
	{
		if (nBig1.integral.vec[i] < nBig2.integral.vec[i])return !nBig1.negative;
		if (nBig1.integral.vec[i] > nBig2.integral.vec[i])return nBig1.negative;
	}

	for (int i = 0; i < nBig1.fractional.vec.size(); i++)
	{
		if (i == nBig2.fractional.vec.size())return nBig1.negative;
		if (nBig1.fractional.vec[i] < nBig2.fractional.vec[i])return !nBig1.negative;
		if (nBig1.fractional.vec[i] > nBig2.fractional.vec[i])return nBig1.negative;
	}

	return nBig1.negative;
}

bool operator>(const BigNumber& nBig1, const BigNumber& nBig2)
{
	return nBig2 < nBig1;
}

bool operator<=(const BigNumber& nBig1, const BigNumber& nBig2)
{
	return nBig1 < nBig2 || nBig1 == nBig2;
}

bool operator>=(const BigNumber& nBig1, const BigNumber& nBig2)
{
	return nBig1 > nBig2 || nBig1 == nBig2;
}

BigNumber operator+(const BigNumber& nBig1, const BigNumber& nBig2)
{
	BigNumber nResult;

	if (nBig1.negative > nBig2.negative) {
		nResult = nBig1; nResult.negative = false;
		return nBig2 - nResult;
	}
	if (nBig2.negative > nBig1.negative) {
		nResult = nBig2; nResult.negative = false;
		return nBig1 - nResult;
	}

	nResult.negative = nBig1.negative;

	bool carry = 0;

	nResult.fractional.vec.clear();
	nResult.fractional.vec.reserve(std::max(nBig1.fractional.vec.size(), nBig2.fractional.vec.size()));
	for (int i = std::max(nBig1.fractional.vec.size(), nBig2.fractional.vec.size()) - 1; i >= 0; i--)
	{
		int num[2] = { 0, 0 };
		if (i < nBig1.fractional.vec.size())num[0] = nBig1.fractional.vec[i];
		if (i < nBig2.fractional.vec.size())num[1] = nBig2.fractional.vec[i];
		nResult.fractional.vec.insert(nResult.fractional.vec.begin(), (num[0] + num[1] + carry) % BigNumber::EXP_OF_10);
		carry = (num[0] + num[1] + carry) > BigNumber::MAX_NUM;
	}
	if (nResult.fractional.vec.empty()) {
		nResult.fractional.vec.push_back(0);
		nResult.fractional.str = "0";
	}
	else {
		nResult.fractional.str = toString(nResult.fractional.vec.cbegin(), nResult.fractional.vec.cend(), true);
		nResult.fractional.str = trailZeroesBack(nResult.fractional.str);
	}

	nResult.integral.vec.clear();
	for (int i = 0; i < nBig1.integral.vec.size() || i < nBig2.integral.vec.size(); i++)
	{
		int num[2] = { 0, 0 };
		if (i < nBig1.integral.vec.size())num[0] = nBig1.integral.vec[i];
		if (i < nBig2.integral.vec.size())num[1] = nBig2.integral.vec[i];
		nResult.integral.vec.push_back((num[0] + num[1] + carry) % BigNumber::EXP_OF_10);
		carry = (num[0] + num[1] + carry) > BigNumber::MAX_NUM;
	}
	if (carry)nResult.integral.vec.push_back(1);
	if (nResult.integral.vec.empty()) {
		nResult.integral.vec.push_back(0);
		nResult.integral.str = "0";
	}
	else nResult.integral.str = toString(nResult.integral.vec.crbegin(), nResult.integral.vec.crend());

	return nResult;
}

BigNumber operator-(const BigNumber& nBig1, const BigNumber& nBig2)
{
	BigNumber nResult;

	if (nBig1.negative > nBig2.negative) {
		nResult = nBig1; nResult.negative = false;//abs
		nResult = nBig2 + nResult; nResult.negative = true;
		return nResult;
	}
	if (nBig2.negative > nBig1.negative) {
		nResult = nBig2; nResult.negative = false;
		return nBig1 + nResult;
	}
	if (nBig1.negative) {
		nResult = nBig1; nResult.negative = false;
		BigNumber temp = nBig2; temp.negative = false;
		return temp - nResult;
	}
	if (nBig1 < nBig2) {
		nResult = nBig2 - nBig1;
		nResult.negative = true;
		return nResult;
	}
	if (nBig1 == nBig2)return nResult;

	bool carry = 0;

	nResult.fractional.vec.clear();
	

	for (int i = nBig1.fractional.vec.size() - 1; i >= 0; i--)
	{
		int num[2] = { nBig1.fractional.vec[i] - carry, 0 };
		if (i < nBig2.fractional.vec.size())num[1] = nBig2.fractional.vec[i];
		if (num[0] < num[1]) {
			carry = 1;
			num[0] += BigNumber::EXP_OF_10;
		}
		else carry = 0;
		nResult.fractional.vec.insert(nResult.fractional.vec.begin(), (num[0] - num[1]) % BigNumber::EXP_OF_10);
	}
	if (nResult.fractional.vec.empty()) {
		nResult.fractional.vec.push_back(0);
		nResult.fractional.str = "0";
	}
	else {
		nResult.fractional.str = toString(nResult.fractional.vec.cbegin(), nResult.fractional.vec.cend(), true);
		nResult.fractional.str = trailZeroesBack(nResult.fractional.str);
	}

	nResult.integral.vec.clear();

	for (int i = 0; i < nBig1.integral.vec.size(); i++)
	{
		int num[2] = { nBig1.integral.vec[i] - carry, 0 };
		if (i < nBig2.integral.vec.size())num[1] = nBig2.integral.vec[i];
		if (num[0] < num[1]) {
			carry = 1;
			num[0] += BigNumber::EXP_OF_10;
		}
		else carry = 0;
		nResult.integral.vec.push_back((num[0] - num[1]) % BigNumber::EXP_OF_10);
	}
	if (carry)nResult.integral.vec.push_back(1);
	if (nResult.integral.vec.empty()) {
		nResult.integral.vec.push_back(0);
		nResult.integral.str = "0";
	}
	else nResult.integral.str = toString(nResult.integral.vec.crbegin(), nResult.integral.vec.crend());

	return nResult;
}

BigNumber operator*(const BigNumber& nBig1, const BigNumber& nBig2)
{
	BigNumber nResult;

	int carry = 0;

	if ((nBig1.fractional.str == "0" && nBig1.integral.str == "0") || (nBig2.fractional.str == "0" && nBig2.integral.str == "0"))return nResult;

	if (nBig1.fractional.str != "0" && nBig2.fractional.str != "0") {

		while (nResult.fractional.vec.size() != nBig1.fractional.vec.size() + nBig2.fractional.vec.size())nResult.fractional.vec.push_back(0);

		for (int i = 0; i < nBig1.fractional.vec.size(); i++) {
			for (int j = 0; j < nBig2.fractional.vec.size(); j++) {
				int cell = i + j + 1;

				carry = nBig1.fractional.vec[i] * nBig2.fractional.vec[j];
				do {
					int sum = nResult.fractional.vec[cell] + carry;
					nResult.fractional.vec[cell] = sum % BigNumber::EXP_OF_10;
					carry = sum / BigNumber::EXP_OF_10;
					cell--;
				} while (carry && cell >= 0);
			}
		}
		while (nResult.fractional.vec.back() == 0)nResult.fractional.vec.pop_back();
		nResult.fractional.str = toString(nResult.fractional.vec.cbegin(), nResult.fractional.vec.cend(), true);
		nResult.fractional.str = trailZeroesBack(nResult.fractional.str);
	}
	
	auto integralFractionalMultiply = [&nResult](const std::string& first, const std::string& second) {
		std::string num = (BigNumber(first) * BigNumber(second)).asString();
		while (second.size() > num.size())num = "0" + num;
		if (second.size() == num.size())num = "0." + num;
		else num.insert(num.cend() - second.size(), '.');
		nResult = nResult + BigNumber(num);
	};

	if (nBig1.integral.str != "0" && nBig2.fractional.str != "0")integralFractionalMultiply(nBig1.integral.str, nBig2.fractional.str);
	if (nBig2.integral.str != "0" && nBig1.fractional.str != "0")integralFractionalMultiply(nBig2.integral.str, nBig1.fractional.str);

	if (nBig1.integral.str != "0" && nBig2.integral.str != "0") {
		if (nBig1.integral.str == "0" || nBig2.integral.str == "0")return nResult;

		for (int i = 0; i < nBig1.integral.vec.size(); i++) {
			for (int j = 0; j < nBig2.integral.vec.size(); j++) {
				int cell = i + j;

				while (cell >= nResult.integral.vec.size())nResult.integral.vec.push_back(0);
				carry = nBig2.integral.vec[j] * nBig1.integral.vec[i];

				do {
					if (cell >= nResult.integral.vec.size())nResult.integral.vec.push_back(0);
					int sum = nResult.integral.vec[cell] + carry;
					nResult.integral.vec[cell] = sum % BigNumber::EXP_OF_10;
					carry = sum / BigNumber::EXP_OF_10;
					cell++;
				} while (carry);
			}
		}

		nResult.integral.str = toString(nResult.integral.vec.crbegin(), nResult.integral.vec.crend());
	}
	nResult.negative = nBig1.negative != nBig2.negative;
	return nResult;
}

BigNumber operator+(const BigNumber& nBig)
{
	return nBig;
}

BigNumber operator-(const BigNumber& nBig)
{
	BigNumber nResult = nBig;
	nResult.negative = !nResult.negative;
	return nResult;
}

/*

0.9000 0000 0010
0.9000 0000 0010

0.8100 0000
0.0000 0000
0.0000 0000 0009 0000
0.0000 0000
0.0000 0000 0000
0.0000 0000 0009 0000
0.0000 0000 0000 0000
0.0000 0000 0000 0000 0000 0100


9000 0 0010
9000 0 0010

8100 0 18 0 100 0

*/