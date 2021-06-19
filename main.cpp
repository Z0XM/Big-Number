#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <chrono>
#include <conio.h>

#include "BigNumber.hpp"


std::string commandError, variableError[3];
std::map<std::string, BigNumber> variableMap;
bool timeDisplay = false;

void printVariable(std::string input);
void getVariableInput(std::string input);
void calculate(std::string input, std::string operand);

int main()
{
	std::cout << "##################  BigNumber Mathematics !  ##################\n";
	std::cout << "#### Type .help for a list of useful commands. ####\n";

	std::string helpStr =
		"#### Commands ####\n"
		"#### .help - show a list of commands.\n"
		"#### . [var] [number in string format] - declare a variable.\n"
		"#### . [var] mem - declare a variable and copy its value from memory.\n"
		"#### . [var] [another existing variable name] - declare a variable equal to another variable.\n"
		"#### .mem displays the last calculated result.\n"
		"#### .sum [var or num] [var or num] - add two variables/numbers.\n"
		"#### .sub [var or num] [var or num] - subtract two variables/numbers.\n"
		"#### .mul [var or num] [var or num] - multiply two variables/numbers.\n"
		"#### .time - display time taken after each solve.\n"
		"#### .clear - clear the memory.\n"
		"#### .exit - exit the program.\n\n";

	commandError = "\n#### Enter a valid command. Type `.help` for help. ####\n\n";
	variableError[0] = "#### Enter a valid variable name. ####\n";
	variableError[1] = "#### Such a variable does not exist. ####\n";
	variableError[2] = "#### Enter a valid value. ####\n";

	variableMap.insert(std::make_pair("mem", 0));

	bool running = true;
	while (running)
	{
		std::string input;
		std::cout << "## ";
		std::getline(std::cin, input);
		if (input[0] == '.')
		{
			if (input == ".help")
			{
				std::cout << helpStr;
			}
			else if (input == ".exit")
			{
				running = false;
			}
			else if (input == ".time")
			{
				timeDisplay = !timeDisplay;
			}
			else if (input == ".clear")
			{
				variableMap.clear();
				std::cout << "\n##################  BigNumber Mathematics !  ##################\n";
				std::cout << "#### Type .help for a list of useful commands. ####\n";
			}
			else if (input.substr(0,4) == ".sum" || input.substr(0, 4) == ".mul" || input.substr(0, 4) == ".sub")
			{
				calculate(input.substr(4, input.size()), input.substr(1, 3));
			}
			else if (input.find(' ') != input.npos)
			{
				getVariableInput(input.substr(1, input.size()));
			}
			else printVariable(input.substr(1, input.size()));
		}
		else std::cout << commandError;
	}
}

void printVariable(std::string input)
{
	if (variableMap.find(input) == variableMap.end())std::cout << variableError[1];
	else std::cout << "## " << input << " = " << variableMap[input].asString() << "\n";
}

void getVariableInput(std::string input)
{
	std::string inputs[2] = { "", "" };

	while (input[0] == ' ')input.erase(0, 1);

	if (!input.size()) { std::cout << commandError; return; }

	if (isdigit(input[0])) { std::cout << variableError[0]; return; }

	int index = input[0] == '-';
	while (input[index] != ' ' && index < input.size())index++;
	inputs[0] = input.substr(0, index);
	input.erase(0, index);

	if (!input.size()) { std::cout << commandError; return; }

	while (input[0] == ' ')input.erase(0, 1);

	if (!input.size()) { std::cout << commandError; return; }

	index = input[0] == '-';
	bool isVar = false;
	while (input[index] != ' ' && index < input.size()) {
		if (!isdigit(input[index]) && input[index] !='.')isVar = true;
		index++;
	}
	inputs[1] = input.substr(0, index);
	input.erase(0, index);

	while (input[0] == ' ')input.erase(0, 1);
	if (input.size()) { std::cout << commandError; return; }

	if (isVar && variableMap.find(inputs[1]) == variableMap.end()) { std::cout << variableError[1]; return; }
	else if (isVar) { variableMap[inputs[0]] = variableMap[inputs[1]]; }
	else variableMap[inputs[0]] = BigNumber(inputs[1]);
}

void calculate(std::string input, std::string operand)
{
	std::string inputs[2] = { "", "" };

	while (input[0] == ' ')input.erase(0, 1);

	if (!input.size()) { std::cout << commandError; return; }

	bool var[2] = { false, false };

	if (!isdigit(input[0]) && (input[0] != '-' || !isdigit(input[1]))) var[0] = true;

	int index = input[0] == '-';;
	while (input[index] != ' ' && index < input.size()) {
		if (!var && !isdigit(input[index]) && input[index] != '.') { std::cout << variableError[2]; return; }
		index++;
	}
	inputs[0] = input.substr(0, index);
	input.erase(0, index);

	if (!input.size()) { std::cout << commandError; return; }

	while (input[0] == ' ')input.erase(0, 1);

	if (!input.size()) { std::cout << commandError; return; }

	if (!isdigit(input[0]) && (input[0]!='-' || !isdigit(input[1]))) var[1] = true;

	index = input[0] == '-';
	while (input[index] != ' ' && index < input.size()) {
		if (!var[1] && !isdigit(input[index]) && input[index] != '.') { std::cout << variableError[2]; return; }
		index++;
	}
	inputs[1] = input.substr(0, index);
	input.erase(0, index);

	while (input[0] == ' ')input.erase(0, 1);
	if (input.size()) { std::cout << commandError; return; }

	if (var[0] && variableMap.find(inputs[0]) == variableMap.end()){std::cout << variableError[0]; return; }
	if (var[1] && variableMap.find(inputs[1]) == variableMap.end()){std::cout << variableError[0]; return; }

	BigNumber a = var[0] ? variableMap[inputs[0]] : BigNumber(inputs[0]);
	BigNumber b = var[1] ? variableMap[inputs[1]] : BigNumber(inputs[1]);

	std::cout << "## = ";
	auto started = std::chrono::high_resolution_clock::now();
	
	if (operand == "sum")variableMap["mem"] = a + b;
	if (operand == "sub")variableMap["mem"] = a - b;
	if (operand == "mul")variableMap["mem"] = a * b;
	std::cout << variableMap["mem"].asString();
	auto done = std::chrono::high_resolution_clock::now();
	if (timeDisplay) {
		std::cout << "\n Finished in " << std::chrono::duration_cast<std::chrono::nanoseconds>(done - started).count() << " nanoseconds";
		std::cout << "\n Finished in " << std::chrono::duration_cast<std::chrono::milliseconds>(done - started).count() << " milliseconds";
	}
	std::cout << "\n";
}

/*
 0.3443 4300
 0.3833 2340
                                | 0000 0000 0000 
 0.3833 * 0.3443 = 0.13197019   | 1319 7019 0000
 0.3833 * 0.00004300 = 0.16481900   | 0000 0232 2000
 0.00009*0.00009 = 0.0000000081 | 0000 0000 8100
 

 348 4848 48.48 4848
 0.0100
 0.0100
 








*/