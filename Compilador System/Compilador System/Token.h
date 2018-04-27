#pragma once
#include <string>

namespace CoSy {
	enum TOKEN_TYPE {
		UNDEFINED = 0,
		ID,
		INT,
		FLOAT,
		STRING,
		LOGICAL_CONST,
		KEYWORD,
		SEPARATOR,
		RELATIONAL_OPERATOR,
		ARITHMETIC_OPERATOR,
		LOGICAL_OPERATOR,
		LOGICAL_UNARY_OPERATOR,
		ASSIGN_OPERATOR,
		GROUPING_OPERATOR,
		DIMENSION_OPERATOR,
		UNARY_OP_NOT
	};

	class CToken
	{
	private:
		std::string mLex;
		TOKEN_TYPE mType;
		int mLineNumber;
		
	public:
		CToken(std::string lex, TOKEN_TYPE type, int lineNum);
		CToken();
		~CToken();

		
		std::string getLex() const { return mLex; }
		std::string getTypeStr();
		TOKEN_TYPE getType() const { return mType; }
		int getLineNumber() const { return mLineNumber; }
	};
}
