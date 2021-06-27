#pragma once

#include <vector>
#include <string>

class BigNumber {
	static const int MAX_NUM = 9999;
	static const int EXP_OF_10 = 10000;
	static const int POWER_OF_10 = 4;
public:
	typedef int Int;
	struct {
		std::vector<Int> vec = { 0 };
		std::string str = "0";
	} integral, fractional;

	bool negative = false;

	BigNumber();
	BigNumber(int);
	BigNumber(double);
	BigNumber(const std::string&);
	~BigNumber();

	std::string asString() const;
	friend void fillVec(std::vector<BigNumber::Int>& vec, int x);
	template<typename begin_it, typename end_it>
	friend std::string toString(begin_it begin, end_it end, bool fractional);
	friend std::ostream& operator<<(std::ostream&, const BigNumber&);

	friend bool operator==(const BigNumber&, const BigNumber&);
	friend bool operator!=(const BigNumber&, const BigNumber&);
	friend bool operator<(const BigNumber&, const BigNumber&);
	friend bool operator>(const BigNumber&, const BigNumber&);
	friend bool operator<=(const BigNumber&, const BigNumber&);
	friend bool operator>=(const BigNumber&, const BigNumber&);

	friend BigNumber operator+(const BigNumber&, const BigNumber&);
	friend BigNumber operator-(const BigNumber&, const BigNumber&);
	friend BigNumber operator*(const BigNumber&, const BigNumber&);
};