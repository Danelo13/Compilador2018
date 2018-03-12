#pragma once
#include "windows.h"
#include "Token.h"
#include "Errors.h"
#include "vcclr.h"

using namespace System;

#include <map>
#include <vector>
#include <iterator>
using namespace std;

namespace CoSy {
	enum LEX_STATE
	{
		S_START = 0,
		S_PARSING_ID,
		S_PARSING_INT,
		S_PARSING_FLOAT,
		S_PARSING_STRING,
		S_PARSING_LESSTHAN,
		S_PARSING_LESSTHANOREQUAL,
		S_PARSING_GREATERTHAN,
		S_PARSING_GREATERTHANOREQUAL,
		S_PARSING_ASSIGN,
		S_PARSING_EQUAL,
		S_PARSING_NOTEQUAL,
		S_PARSING_OPENPARENTHESIS,
		S_PARSING_CLOSEPARENTHESIS,
		S_PARSING_ARITHMETICOP,
		S_PARSING_LOGICAL_OP_AND,
		S_PARSING_LOGICAL_OP_OR,
		S_PARSING_LOGICAL_OP_NOT,
		S_PARSING_OPENBRACKET,
		S_PARSING_CLOSEBRACKET,
		S_PARSING_OPENCURLYBRACKET,
		S_PARSING_CLOSECURLYBRACKET,
		S_PARSING_SEMICOLON,
		S_PARSING_COMMA,
		S_PARSING_COLON,
		S_PARSING_COMMENT
	};
	public class CLex
	{
	private:
		LEX_STATE LState;
		bool Succeeded;

		msclr::gcroot<CErrors ^> refManagedErrors;

		map<string, string> Keywords;
		vector<CToken *> Tokens;
		int currentToken = 0;

		void addError(int line, const char *desc, const char *nline);
		void addToken(const char *lex, TOKEN_TYPE type, int lineN);
		void clearTokens();

		bool isAlpha(const char *c) const;
		bool isDigit(const char *c) const;
		bool isSpace(const char *c) const;
		bool isNewLine(const char *c) const;
		bool isArithmeticOp(const char *c) const;
		bool isRelationalOp(const char *c) const;
		bool isLogicalOp(const char *c) const;
		bool isSeparator(const char *c) const;
		bool isGroupingChar(const char *c) const;
		bool isBlockChar(const char *c) const;
		bool isDimensionChar(const char *c) const;
		bool isStringLiteral(const char *c) const;
	public:
		CLex(CErrors ^ errors);
		~CLex();

		const CToken* getNextToken();
		const CToken* peekToken(int num);
		bool parseCode(const char * src);
		int getNumTokens() const;
		void getTokens(std::vector<CToken *> *tokenVec) const;

		bool succeeded() const
		{
			return Succeeded;
		}
		
	};

}