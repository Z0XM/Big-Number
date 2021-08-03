#pragma once
#pragma once

#include <array>
#include "BigNumber.hpp"

///////////////////////////////////////////////////////////////
/// \brief Structure to store properties of an operand.
/// 
///////////////////////////////////////////////////////////////
struct Operand {
	const char* sym;					/// < character symbol
	int lvl;							/// < level of precedence
	bool aso;							/// < associativity 0->left to right, 1->right to left
};

enum Associativity {
	LEFT_TO_RIGHT,
	RIGHT_TO_LEFT
};

enum OperandType {
	NONE = 0,
	OPEN, CLOSE,
	/// associativity -> left to right
	PLUS, MINUS,								/// < level 1
	MULTIPLY, DIVIDE, REMAINDER,				/// < level 2
	LEFT_SHIFT, RIGHT_SHIFT,					/// < level 3
	LESSER, LESSER_EQ, GREATER, GREATER_EQ,		/// < level 4
	EQUAL, NOT_EQUAL,							/// < level 5
	BIT_AND,									/// < level 6
	BIT_XOR,									/// < level 7
	BIT_OR,										/// < level 8
	/// associativity -> right to left
	AND,										/// < level 9
	OR											/// < level 10
};

extern constexpr std::array<Operand, 21> Operands = {
	" ", 0, 0,
	"(", 0, 0,		")", 0, 0,
	/// associativity -> left to right
	"+", 1, 0,		"-", 1, 0,										/// < level 1
	"*", 2, 0,		"/", 2, 0,		"%", 2, 0,						/// < level 2
	"<<", 3, 0,		">>", 3, 0,										/// < level 3
	"<", 4, 0,		"<=", 4, 0,		">", 4, 0,		">=", 4, 0,		/// < level 4
	"==", 5, 0,		"!=", 5, 0,										/// < level 5
	"&", 6, 0,														/// < level 6
	"^", 7, 0,														/// < level 7
	"|", 8, 0,														/// < level 8
	/// associativity -> right to left
	"&&", 9, 1,														/// < level 9
	"||", 9, 1														/// < level 10
};

OperandType getOp(const char* o);
BigNumber operate(const BigNumber& a, const BigNumber& b, OperandType op);