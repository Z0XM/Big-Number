#include "BigNumber.hpp"

#include <iostream>
#include <algorithm>
#include <sstream>
#include <cmath>

int toDigit(char c) { return c - 48; }
char toChar(int c) { return c + 48; }
template <typename T = int> std::string toString(const T& x) {
	std::ostringstream out; out << x; return out.str(); 
}
template <typename begin_it, typename end_it>
std::string toString(begin_it begin, end_it end, bool fractional = false) {
	std::ostringstream out; 
	for (auto i = begin; i != end; i++) {
		if (i != begin || fractional)for (int ten = 1000; ten != 1 && *i < ten; ten /= 10)out << '0';
		out << *i;
	}
	return out.str();
}

std::string& trailZeroes(std::string&);
std::string& trailZeroesIntegral(std::string&);
std::string& trailZeroesFractional(std::string&);

BigNumber::BigNumber()
{
	integral.vec.push_back(0);
	integral.str = "0";
	fractional.vec.push_back(0);
	fractional.str = "0";
	negative = false;
}

BigNumber::BigNumber(int x)
{
	setNumber(x);
}

BigNumber::BigNumber(double x)
{
	setNumber(x);
}

BigNumber::BigNumber(std::string num)
{
	setNumber(trailZeroes(num));
}

BigNumber::~BigNumber()
{
	
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

std::string BigNumber::asString() const
{
	std::string result;
	if (negative) result += '-';
	result += integral.str;
	if(fractional.str != "0")result += '.' + fractional.str;
	return result;
}

void BigNumber::setNumber(const int& x)
{
	integral.str = toString(abs(x));
	integral.vec.clear();
	integral.vec.push_back(abs(x) % POWER_OF_10);
	if (abs(x) > MAX_NUM) {
		integral.vec.push_back(floor(abs(x) / POWER_OF_10));
	}
	
	fractional.vec.clear();
	fractional.vec.push_back(0);
	fractional.str = "0";

	negative = x < 0;
}

void BigNumber::setNumber(const double& x)
{
	integral.str = toString(floor(abs(x)));
	integral.vec.clear();
	integral.vec.push_back((int)(abs(x)) % POWER_OF_10);
	if (x > MAX_NUM) {
		integral.vec.push_back(floor(abs(x) / POWER_OF_10));
	}

	fractional.str = toString(abs(x) - floor(abs(x)));
	fractional.str.erase(0, 2);
	fractional.vec.clear();
	fractional.vec.push_back((abs(x) - floor(abs(x))) * (int)pow(10, fractional.str.size()));

	negative = x < 0;
}

void BigNumber::setNumber(const std::string& num)
{
	negative = num[0] == '-';

	bool isDecimal = num.find('.') != num.npos;
	integral.str = isDecimal ? num.substr(negative, num.find('.') - negative) : num.substr(negative, num.size() - negative);

	integral.vec.clear();
	for (int i = 0; i < integral.str.size(); i++) {
		if (i % 4 == 0)integral.vec.push_back(0);
		integral.vec.back() += toDigit(integral.str[integral.str.size() - 1 - i]) * (int)pow(10, i % 4);
	}
	if (integral.vec.empty()) {
		integral.vec.push_back(0);
		integral.str = "0";
	}

	fractional.str = isDecimal ? num.substr(num.find('.') + 1, num.size() - negative) : "0";
	
	fractional.vec.clear();
	for (int i = 0; i < fractional.str.size(); i++) {
		if (i % 4 == 0)fractional.vec.push_back(0);
		fractional.vec.back() += toDigit(fractional.str[i]) * (int)pow(10, (4 - i%4 - 1));
	}
	if (fractional.vec.empty()) {
		fractional.vec.push_back(0);
		fractional.str = "0";
	}
}

std::string& trailZeroes(std::string& num)
{
	trailZeroesIntegral(num);

	bool isDecimal = num.find('.') != num.npos;

	if (!isDecimal)return num;

	trailZeroesFractional(num);
	
	return num;
}

std::string& trailZeroesIntegral(std::string& num)
{
	bool neg = num[0] == '-';
	int count = neg;
	for (; num[count] == '0'; count++);
	num.erase(neg, count - neg);
	return num;
}

std::string& trailZeroesFractional(std::string& num)
{
	int index;
	for (index = num.size() - 1; index >=0 && num[index] == '0'; index--);
	num.erase(index + 1);
	return num;
}

bool operator==(const BigNumber& nBig1, const BigNumber& nBig2)
{
	return nBig1.integral.str == nBig2.integral.str && nBig1.fractional.str == nBig2.fractional.str && nBig1.negative == nBig2.negative;
}

bool operator!=(const BigNumber& nBig1, const BigNumber& nBig2)
{
	return nBig1.integral.str != nBig2.integral.str && nBig1.fractional.str != nBig2.fractional.str && nBig1.negative != nBig2.negative;
}
bool operator<(const BigNumber& nBig1, const BigNumber& nBig2)
{
	if (nBig1.negative > nBig2.negative)return true;
	if (nBig1.negative < nBig2.negative)return false;

	if (nBig1.integral.vec.size() < nBig2.integral.vec.size())return true;
	if (nBig1.integral.vec.size() > nBig2.integral.vec.size())return false;

	for (int i = nBig1.integral.vec.size() - 1; i >= 0; i--)
	{
		if (nBig1.integral.vec[i] < nBig2.integral.vec[i])return true;
		if (nBig1.integral.vec[i] > nBig2.integral.vec[i])return false;
	}

	for (int i = 0; i < nBig1.fractional.vec.size(); i++)
	{
		if (i == nBig2.fractional.vec.size())return false;
		if (nBig1.fractional.vec[i] < nBig2.fractional.vec[i])return true;
		if (nBig1.fractional.vec[i] > nBig2.fractional.vec[i])return false;
	}

	return false;
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
	return nBig2 < nBig1 || nBig1 == nBig2;
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
		nResult.fractional.vec.insert(nResult.fractional.vec.begin(), (num[0] + num[1] + carry) % BigNumber::POWER_OF_10);
		carry = (num[0] + num[1] + carry) > BigNumber::MAX_NUM;
	}
	if (nResult.fractional.vec.empty()) {
		nResult.fractional.vec.push_back(0);
		nResult.fractional.str = "0";
	}
	else {
		nResult.fractional.str = toString(nResult.fractional.vec.cbegin(), nResult.fractional.vec.cend());
		trailZeroesFractional(nResult.fractional.str);
	}

	nResult.integral.vec.clear();

	for (int i = 0; i < nBig1.integral.vec.size() || i < nBig2.integral.vec.size(); i++)
	{
		int num[2] = { 0, 0 };
		if (i < nBig1.integral.vec.size())num[0] = nBig1.integral.vec[i];
		if (i < nBig2.integral.vec.size())num[1] = nBig2.integral.vec[i];
		nResult.integral.vec.push_back((num[0] + num[1] + carry) % BigNumber::POWER_OF_10);
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
			num[0] += BigNumber::POWER_OF_10;
		}
		nResult.fractional.vec.insert(nResult.fractional.vec.begin(), (num[0] - num[1]) % BigNumber::POWER_OF_10);
	}
	if (nResult.fractional.vec.empty()) {
		nResult.fractional.vec.push_back(0);
		nResult.fractional.str = "0";
	}
	else {
		nResult.fractional.str = toString(nResult.fractional.vec.cbegin(), nResult.fractional.vec.cend(), true);
		trailZeroesFractional(nResult.fractional.str);
	}

	nResult.integral.vec.clear();

	for (int i = 0; i < nBig1.integral.vec.size(); i++)
	{
		int num[2] = { nBig1.integral.vec[i], 0 };
		if (i < nBig2.integral.vec.size())num[1] = nBig2.integral.vec[i];
		if (num[0] < num[1]) {
			carry = 1;
			num[0] += BigNumber::POWER_OF_10;
		}
		else carry = 0;
		nResult.integral.vec.push_back((num[0] - num[1]) % BigNumber::POWER_OF_10);
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

		for (int i = 0; i < nBig1.fractional.vec.size(); i++){
			for (int j = 0; j < nBig2.fractional.vec.size(); j++){
				int cell = i + j;

				carry = nBig1.fractional.vec[i] * nBig2.fractional.vec[j];
				while (carry % BigNumber::POWER_OF_10 == 0)carry /= BigNumber::POWER_OF_10;

				if ((carry / BigNumber::POWER_OF_10) != 0)cell++;

				do {
					int sum = nResult.fractional.vec[cell] + carry;
					nResult.fractional.vec[cell] = sum % BigNumber::POWER_OF_10;
					carry = sum / BigNumber::POWER_OF_10;
					cell--;
				} while (carry && cell >= 0);
			}
		}
		while (nResult.fractional.vec.back() == 0)nResult.fractional.vec.pop_back();
		nResult.fractional.str = toString(nResult.fractional.vec.cbegin(), nResult.fractional.vec.cend(), true);
		trailZeroesFractional(nResult.fractional.str);
	}

	auto integralFractionalMultiply = [&nResult](const std::string& first, const std::string& second) {
		std::string num = (BigNumber(first) * BigNumber(second)).asString();
		while (second.size() > num.size())num += '0';
		if (second.size() == num.size())num += "0.";
		else num.insert(num.cend() - second.size(), '.');
		nResult = nResult + BigNumber(num);
	};

	if (nBig1.integral.str != "0" && nBig2.fractional.str != "0")integralFractionalMultiply(nBig1.integral.str, nBig2.fractional.str);
	if (nBig2.integral.str != "0" && nBig1.fractional.str != "0")integralFractionalMultiply(nBig2.integral.str, nBig1.fractional.str);

	if (nBig1.integral.str != "0" && nBig2.integral.str != "0") {
		if (nBig1.integral.str == "0" || nBig2.integral.str == "0")return nResult;

		for (int i = 0; i < nBig1.integral.vec.size(); i++){
			for (int j = 0; j < nBig2.integral.vec.size(); j++) {
				int cell = i + j;

				while (cell >= nResult.integral.vec.size())nResult.integral.vec.push_back(0);
				carry = nBig2.integral.vec[j] * nBig1.integral.vec[i];

				do {
					if (cell >= nResult.integral.vec.size())nResult.integral.vec.push_back(0);
					int sum = nResult.integral.vec[cell] + carry;
					nResult.integral.vec[cell] = sum % BigNumber::POWER_OF_10;
					carry = sum / BigNumber::POWER_OF_10;
					cell++;
				} while (carry);
			}
		}

		nResult.integral.str = toString(nResult.integral.vec.crbegin(), nResult.integral.vec.crend());
	}
	nResult.negative = nBig1.negative != nBig2.negative;

	//std::cout << nBig1.asString() << " : " << nBig2.asString() << " : " << nResult.asString() << '\n';

	return nResult;
}

