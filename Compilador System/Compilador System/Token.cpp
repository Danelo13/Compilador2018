#include "stdafx.h"
#include "Token.h"


CoSy::CToken::CToken(std::string lex, TOKEN_TYPE type, int lineNum) : mLex(lex), mType(type), mLineNumber(lineNum)
{

}

CoSy::CToken::~CToken()
{

}

std::string CoSy::CToken::getTypeStr() {
	std::string typeStr = "";
	static const char * EnumStrings[] = {
		"UNDEFINED",
		"ID",
		"INT",
		"FLOAT",
		"STRING",
		"LOGICAL_CONST",
		"KEYWORD",
		"SEPARATOR",
		"RELATIONAL_OPERATOR",
		"ARITHMETIC_OPERATOR",
		"LOGICAL_OPERATOR",
		"LOGICAL_UNARY_OPERATOR",
		"ASSIGN_OPERATOR",
		"GROUPING_OPERATOR",
		"DIMENSION_OPERATOR"
	};
	typeStr = EnumStrings[mType];
	return typeStr;
}
