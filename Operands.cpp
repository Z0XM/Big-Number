#include "Operands.hpp"
#include <cstring>

OperandType getOp(const char* o)
{
	if (std::strcmp(o, "+") == 0) return PLUS;
	if (std::strcmp(o, "-") == 0) return MINUS;
	if (std::strcmp(o, "*") == 0) return MULTIPLY;
	if (std::strcmp(o, "/") == 0) return DIVIDE;
	if (std::strcmp(o, "%") == 0) return REMAINDER;
	if (std::strcmp(o, "<<") == 0) return LEFT_SHIFT;
	if (std::strcmp(o, ">>") == 0) return RIGHT_SHIFT;
	if (std::strcmp(o, "<") == 0) return LESSER;
	if (std::strcmp(o, "<=") == 0) return LESSER_EQ;
	if (std::strcmp(o, ">") == 0) return GREATER;
	if (std::strcmp(o, ">=") == 0) return GREATER_EQ;
	if (std::strcmp(o, "==") == 0) return EQUAL;
	if (std::strcmp(o, "!=") == 0) return NOT_EQUAL;
	if (std::strcmp(o, "&") == 0) return BIT_AND;
	if (std::strcmp(o, "^") == 0) return BIT_XOR;
	if (std::strcmp(o, "|") == 0) return BIT_OR;
	if (std::strcmp(o, "&&") == 0) return AND;
	if (std::strcmp(o, "||") == 0) return OR;

	if (std::strcmp(o, "(") == 0) return OPEN;
	if (std::strcmp(o, ")") == 0) return CLOSE;
	
	return NONE;
}

BigNumber operate(const BigNumber& a, const BigNumber& b, OperandType op)
{
	switch (op)
	{
	case PLUS:			return a + b;
	case MINUS:			return a - b;
	case MULTIPLY:		return a * b;
	//case DIVIDE:		return a / b;
	//case REMAINDER:		return (int)a % (int)b;
	//case LEFT_SHIFT:	return (unsigned long long)a << (int)b;
	//case RIGHT_SHIFT:	return (unsigned long long)a << (int)b;
	case LESSER:		return a < b;		
	case LESSER_EQ:		return a <= b;		
	case GREATER:		return a > b;	
	case GREATER_EQ:	return a >= b;		
	case EQUAL:			return a == b;
	case NOT_EQUAL:		return a != b;
	//case BIT_AND:		return (int)a & (int)b;
	//case BIT_XOR:		return (int)a ^ (int)b;		
	//case BIT_OR:		return (int)a | (int)b;		
	//case AND:			return a && b;
	//case OR:			return a || b;
	}
}