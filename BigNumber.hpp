#pragma once

#include <vector>
#include <string>

class BigNumber {
	static const int MAX_NUM = 9999;
	static const int POWER_OF_10 = 10000;
public:
	struct {
		std::vector<int> vec;
		std::string str;
	} integral, fractional;

	bool negative;

	BigNumber();
	BigNumber(int);
	BigNumber(double);
	BigNumber(std::string);
	~BigNumber();

	std::string asString() const;
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

private:
	void setNumber(const int&);
	void setNumber(const double&);
	void setNumber(const std::string&);
};