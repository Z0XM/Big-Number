#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stack>
#include <algorithm>
#include <map>
#include <chrono>
#include "BigNumber.hpp"
#include "Operands.hpp"

std::stringstream& getInput(std::stringstream& ss);
void format(std::stringstream& eq);
BigNumber solveInfix(std::stringstream& ss);

bool doesVariableExist(const std::string& var);

std::map<std::string, BigNumber> variableMap;

bool timeDisplay = false;

int main()
{
	variableMap.insert(std::make_pair("mem", 0));

	while (true) {
		std::stringstream eq;
		getInput(eq);

		if (eq.str() == "end")break;
		else if (eq.str() == "time")timeDisplay = !timeDisplay;
		else if (eq.str() == "mem")std::cout << "-> " << variableMap["mem"].asString() << '\n';
		else if(doesVariableExist(eq.str()))std::cout << "-> " << variableMap[eq.str()].asString() << '\n';
		
		else {
			std::string resultVar("mem");
			std::string equalSym;
			if (eq.str().find('=') != std::string::npos) {
				eq >> resultVar;
				eq >> equalSym;
			}

			format(eq);

			auto started = std::chrono::high_resolution_clock::now();

			variableMap[resultVar] = solveInfix(eq);

			auto done = std::chrono::high_resolution_clock::now();
			if (timeDisplay) {
				std::cout << "Finished in " << std::chrono::duration_cast<std::chrono::nanoseconds>(done - started).count() << " nanoseconds\n";
				std::cout << "Finished in " << std::chrono::duration_cast<std::chrono::milliseconds>(done - started).count() << " milliseconds\n";
			}

			std::cout << "-> " << variableMap[resultVar].asString() << '\n';
		}
	}
	return 0;
}

std::stringstream& getInput(std::stringstream& eq)
{
	std::cout << "-> ";

	std::string input;
	std::getline(std::cin, input);

	eq << input;

	return eq;
}

void format(std::stringstream& eq)
{
}

BigNumber solveInfix(std::stringstream& postfix)
{
	std::stringstream eq;
	postfix.swap(eq);

	std::stack<BigNumber> values;
	std::stack<OperandType> stack;

	std::string str;
	while (eq >> str) {
		OperandType op = getOp(str.c_str());
		if (op == NONE) {
			if (doesVariableExist(str))values.push(variableMap[str]);
			else values.push(str);
		}
		else if (op == OPEN) {
			stack.push(op);
		}
		else if (op == CLOSE) {
			BigNumber b(values.top()); values.pop();
			BigNumber a(values.top()); values.pop();

			values.push(operate(a, b, stack.top()));

			stack.pop();
			stack.pop();
		}
		else {
			while (!stack.empty()) {
				if (Operands[op].lvl > Operands[stack.top()].lvl) break;

				if (Operands[op].lvl < Operands[stack.top()].lvl || Operands[stack.top()].aso == LEFT_TO_RIGHT) {
					BigNumber b(values.top()); values.pop();
					BigNumber a(values.top()); values.pop();

					values.push(operate(a, b, stack.top()));

					stack.pop();
				}
			}
			stack.push(op);
		}
	}

	while (!stack.empty()) {
		BigNumber b(values.top()); values.pop();
		BigNumber a(values.top()); values.pop();

		values.push(operate(a, b, stack.top()));

		stack.pop();
	}

	return values.top();
}

bool doesVariableExist(const std::string& var)
{
	return variableMap.find(var) != variableMap.end();
}