#include "stdafx.h"
#include "Lex.h"


CoSy::CLex::CLex(CErrors ^ errors){
	refManagedErrors = errors;
	Keywords.insert(std::make_pair("var", ""));
	Keywords.insert(std::make_pair("int", ""));
	Keywords.insert(std::make_pair("float", ""));
	Keywords.insert(std::make_pair("string", ""));
	Keywords.insert(std::make_pair("bool", ""));
	Keywords.insert(std::make_pair("function", ""));
	Keywords.insert(std::make_pair("procedure", ""));
	Keywords.insert(std::make_pair("main", ""));
	Keywords.insert(std::make_pair("for", ""));
	Keywords.insert(std::make_pair("while", ""));
	Keywords.insert(std::make_pair("if", ""));
	Keywords.insert(std::make_pair("else", ""));
	Keywords.insert(std::make_pair("switch", ""));
	Keywords.insert(std::make_pair("default", ""));
	Keywords.insert(std::make_pair("print", ""));
	Keywords.insert(std::make_pair("read", ""));
	Keywords.insert(std::make_pair("return", ""));

}
CoSy::CLex::~CLex(){}

bool CoSy::CLex::parseCode(const char *src) {
	int currentLineNumber = 1;
	int numCharsInLine = 0;
	int firstLineComment = 0;
	const char *currentChar = src;
	const char *currentLine = src;
	std::string tokenBuffer;
	std::string lineBufferStr;
	std::string firstCommentLine;
	char lexSrcEof = '\0';
	bool stringNotClosed = false;
	TOKEN_TYPE tokenType = TOKEN_TYPE::UNDEFINED;


	Succeeded = true;
	LState = S_START;
	clearTokens();
	
	while (*currentChar != lexSrcEof) {
		switch (LState) {	
		case S_START:
			if (isAlpha(currentChar) || *currentChar == '_')
			{
				tokenBuffer.clear();
				tokenBuffer.append(currentChar, 1);
				LState = S_PARSING_ID;
				currentChar++;
			}
			else if (isDigit(currentChar)) {
				tokenBuffer.clear();
				tokenBuffer.append(currentChar, 1);
				LState = S_PARSING_INT;
				currentChar++;
			}
			else if (isSpace(currentChar)) 
			{	// it's space, Ignore
				//check if its a new line
				if (isNewLine(currentChar) && *currentChar == '\n')
				{
					currentLineNumber++;
					currentLine = currentChar + 1;
				}
				currentChar++;
			}
			else if (isArithmeticOp(currentChar)){
				tokenBuffer.clear();

				if (*currentChar == '/' && *currentChar + 1 == '/')
				{
					tokenBuffer.append(currentChar, 1);
					LState = S_PARSING_COMMENT;
					currentChar++;
				}
				else
				{
					tokenBuffer.append(currentChar, 1);
					LState = S_PARSING_ARITHMETICOP;
					currentChar++;
				}
			}
			else if (isRelationalOp(currentChar)) {
				tokenBuffer.clear();
				if (*currentChar == '<')
				{
					tokenBuffer.append(currentChar, 1);
					LState = S_PARSING_LESSTHAN;
					currentChar++;
				}
				else if (*currentChar == '>')
				{
					tokenBuffer.append(currentChar, 1);
					LState = S_PARSING_GREATERTHAN;
					currentChar++;
				}
				else if (*currentChar == '=')
				{
					tokenBuffer.append(currentChar, 1);
					LState = S_PARSING_ASSIGN;
					currentChar++;
				}
			}
			else if (isLogicalOp(currentChar))
			{
				tokenBuffer.clear();
				if (*currentChar == '&')
				{
					tokenBuffer.append(currentChar, 1);
					currentChar++;
					if (*currentChar == '&')
					{
						tokenBuffer.append(currentChar, 1);
						LState = S_PARSING_LOGICAL_OP_AND;
						currentChar++;
					}
					else
					{
						addError(currentLineNumber, "Operador AND invalido", currentLine);
					}
				}
				else if (*currentChar == '|')
				{
					tokenBuffer.append(currentChar, 1);
					currentChar++;
					if (*currentChar == '|')
					{
						tokenBuffer.append(currentChar, 1);
						LState = S_PARSING_LOGICAL_OP_OR;
						currentChar++;
					}
					else
					{
						addError(currentLineNumber, "Operador OR incorrecto", currentLine);
					}
				}
				else if (*currentChar == '!')
				{
					tokenBuffer.append(currentChar, 1);
					LState = S_PARSING_LOGICAL_OP_NOT;
					currentChar++;
				}
			}
			else if (isGroupingChar(currentChar))
			{
				tokenBuffer.clear();
				if (*currentChar == '(')
				{
					tokenBuffer.append(currentChar, 1);
					LState = S_PARSING_OPENPARENTHESIS;
					currentChar++;
				}
				else if (*currentChar == ')')
				{
					tokenBuffer.append(currentChar, 1);
					LState = S_PARSING_CLOSEPARENTHESIS;
					currentChar++;
				}
			}
			else if (isBlockChar(currentChar))
			{
				tokenBuffer.clear();
				if (*currentChar == '{')
				{
					tokenBuffer.append(currentChar, 1);
					LState = S_PARSING_OPENCURLYBRACKET;
					currentChar++;
				}
				else if (*currentChar == '}')
				{
					tokenBuffer.append(currentChar, 1);
					LState = S_PARSING_CLOSECURLYBRACKET;
					currentChar++;
				}
			}
			else if (isDimensionChar(currentChar))
			{
				tokenBuffer.clear();
				if (*currentChar == '[')
				{
					tokenBuffer.append(currentChar, 1);
					LState = S_PARSING_OPENBRACKET;
					currentChar++;
				}
				else if (*currentChar == ']')
				{
					tokenBuffer.append(currentChar, 1);
					LState = S_PARSING_CLOSEBRACKET;
					currentChar++;
				}
			}
			else if (isStringLiteral(currentChar))
			{
				tokenBuffer.append(currentChar, 1);
				LState = S_PARSING_STRING;
				currentChar++;
			}
			else if (*currentChar == '.')
			{
				addError(currentLineNumber, "Float invalido", currentLine);
				currentChar++;
			}
			break;
		case S_PARSING_INT:
			if (isDigit(currentChar)) {
				tokenBuffer.append(currentChar, 1);
				currentChar++;
				if (*currentChar == lexSrcEof) 
					addToken(tokenBuffer.c_str(), TOKEN_TYPE::INT, currentLineNumber);
				
			}
			else if (*currentChar == '.') {
				tokenBuffer.append(currentChar, 1);
				LState = S_PARSING_FLOAT;
				currentChar++;
			}
			else if (isSeparator(currentChar) || isNewLine(currentChar))
			{
				addToken(tokenBuffer.c_str(), TOKEN_TYPE::INT, currentLineNumber);
				LState = S_START;
			}
			else
			{
				addError(currentLineNumber, "Character invalid", currentLine);
			}
			break;
		case S_PARSING_FLOAT:
			if (isDigit(currentChar))
			{
				tokenBuffer.append(currentChar, 1);
				currentChar++;
				if (*currentChar == lexSrcEof)
				{
					if (Keywords.find(tokenBuffer) != Keywords.end())
					{
						addToken(tokenBuffer.c_str(), TOKEN_TYPE::FLOAT, currentLineNumber);
					}
				}
			}
			else
			{
				addToken(tokenBuffer.c_str(), TOKEN_TYPE::FLOAT, currentLineNumber);
				LState = S_START;
				if (!isDigit(currentChar - 1))
				{
					addError(currentLineNumber, "Character invalid", currentLine);
				}
			}
			break;
		case S_PARSING_STRING:
			if (!isStringLiteral(currentChar))
			{
				if (*currentChar == lexSrcEof)
				{
					addError(currentLineNumber, "String no Cerrado correctamente", currentLine);
				}
				tokenBuffer.append(currentChar, 1);
				currentChar++;
			}
			else
			{
				addToken(tokenBuffer.c_str(), TOKEN_TYPE::STRING, currentLineNumber);
				LState = S_START;
			}
			break;		
		case S_PARSING_GREATERTHAN:
			if (*currentChar == '=')
			{
				tokenBuffer.append(currentChar, 1);
				LState = S_PARSING_GREATERTHANOREQUAL;
				currentChar++;
			}
			else
			{
				addToken(tokenBuffer.c_str(), TOKEN_TYPE::RELATIONAL_OPERATOR, currentLineNumber);
				LState = S_START;
			}
			break;
		case S_PARSING_GREATERTHANOREQUAL:
			addToken(tokenBuffer.c_str(), TOKEN_TYPE::RELATIONAL_OPERATOR, currentLineNumber);
			LState = S_START;
			break;
		case S_PARSING_LESSTHAN :
			if (*currentChar == '=')
			{
				tokenBuffer.append(currentChar, 1);
				LState = S_PARSING_LESSTHANOREQUAL;
				currentChar++;
			}
			else
			{
				addToken(tokenBuffer.c_str(), TOKEN_TYPE::RELATIONAL_OPERATOR, currentLineNumber);
				LState = S_START;
			}
			break;
		case S_PARSING_LESSTHANOREQUAL:
			addToken(tokenBuffer.c_str(), TOKEN_TYPE::RELATIONAL_OPERATOR, currentLineNumber);
			LState = S_START;
			break;

		case S_PARSING_ASSIGN:
			if (*currentChar == '=')
			{
				tokenBuffer.append(currentChar, 1);
				LState = S_PARSING_EQUAL;
				currentChar++;
			}
			else
			{
				addToken(tokenBuffer.c_str(), TOKEN_TYPE::ASSIGN_OPERATOR, currentLineNumber);
				LState = S_START;
			}
			break;
		case S_PARSING_EQUAL:
			addToken(tokenBuffer.c_str(), TOKEN_TYPE::RELATIONAL_OPERATOR, currentLineNumber);
			LState = S_START;
			break;
		case S_PARSING_NOTEQUAL:
			addToken(tokenBuffer.c_str(), TOKEN_TYPE::RELATIONAL_OPERATOR, currentLineNumber);
			LState = S_START;
			break;
		case S_PARSING_OPENPARENTHESIS:
			addToken(tokenBuffer.c_str(), TOKEN_TYPE::GROUPING_OPERATOR, currentLineNumber);
			LState = S_START;
			break;
		case S_PARSING_CLOSEPARENTHESIS:
			addToken(tokenBuffer.c_str(), TOKEN_TYPE::GROUPING_OPERATOR, currentLineNumber);
			LState = S_START;
			break;
		case S_PARSING_ARITHMETICOP:
			if (currentChar - 1 == "/" && currentChar == "*")
			{
				tokenBuffer.append(currentChar, 1);
				currentChar++;
				LState = S_PARSING_COMMENT;
			}
			else
			{
				addToken(tokenBuffer.c_str(), TOKEN_TYPE::ARITHMETIC_OPERATOR, currentLineNumber);
				LState = S_START;
			}
			break;
		case S_PARSING_LOGICAL_OP_AND:
			addToken(tokenBuffer.c_str(), TOKEN_TYPE::LOGICAL_OPERATOR, currentLineNumber);
			LState = S_START;
			break;
		case S_PARSING_LOGICAL_OP_OR:
			addToken(tokenBuffer.c_str(), TOKEN_TYPE::LOGICAL_OPERATOR, currentLineNumber);
			LState = S_START;
			break;
		case S_PARSING_LOGICAL_OP_NOT:
			if (currentChar == "=")
			{
				tokenBuffer.append(currentChar, 1);
				currentChar++;
				LState = S_PARSING_NOTEQUAL;
			}
			else
			{
				addToken(tokenBuffer.c_str(), TOKEN_TYPE::LOGICAL_UNARY_OPERATOR, currentLineNumber);
				LState = S_START;
			}
			break;
		case S_PARSING_OPENBRACKET:
			addToken(tokenBuffer.c_str(), TOKEN_TYPE::DIMENSION_OPERATOR, currentLineNumber);
			LState = S_START;
			break;
		case S_PARSING_CLOSEBRACKET:
			addToken(tokenBuffer.c_str(), TOKEN_TYPE::DIMENSION_OPERATOR, currentLineNumber);
			LState = S_START;
			break;
		case S_PARSING_OPENCURLYBRACKET:
			addToken(tokenBuffer.c_str(), TOKEN_TYPE::GROUPING_OPERATOR, currentLineNumber);
			LState = S_START;
			break;
		case S_PARSING_CLOSECURLYBRACKET:
			addToken(tokenBuffer.c_str(), TOKEN_TYPE::GROUPING_OPERATOR, currentLineNumber);
			LState = S_START;
			break;
		case S_PARSING_SEMICOLON:
			addToken(tokenBuffer.c_str(), TOKEN_TYPE::SEPARATOR, currentLineNumber);
			LState = S_START;
			break;
		case S_PARSING_COMMA:
			addToken(tokenBuffer.c_str(), TOKEN_TYPE::SEPARATOR, currentLineNumber);
			LState = S_START;
			break;
		case S_PARSING_COLON:
			addToken(tokenBuffer.c_str(), TOKEN_TYPE::SEPARATOR, currentLineNumber);
			LState = S_START;
			break;
		case S_PARSING_COMMENT:
			break;
		}

	}
	return true;
}

void CoSy::CLex::clearTokens()
{
	if (Tokens.size() > 0)
	{
		for (int i = 0; i < (int)Tokens.size(); i++)
		{
			if (Tokens[i] != NULL)
			{
				delete Tokens[i];
				Tokens[i] = NULL;
			}
		}
		Tokens.clear();
	}
}
int CoSy::CLex::getNumTokens() const {
	return Tokens.size();
}
void CoSy::CLex::getTokens(std::vector<CToken *>* tokenVec) const {
	std::copy(Tokens.begin(), Tokens.end(), std::back_inserter(*tokenVec));
}
void CoSy::CLex::addError(int lineNum, const char *desc, const char *line)
{
	String ^ strDesc = gcnew String(desc);
	String ^ strLine = gcnew String(line);
	refManagedErrors->addError(CoSy::ERROR_PHASE::LEX_ANALYZER, lineNum, strDesc, strLine);
	Succeeded = false;
}
void CoSy::CLex::addToken(const char * lex, TOKEN_TYPE type, int lineNum)
{
	Tokens.push_back(new CToken(std::string(lex), type, lineNum));
}
bool CoSy::CLex::isAlpha(const char * c) const
{
	if ((*c >= 'a' && *c <= 'z') || (*c >= 'A' && *c <= 'Z'))
	{
		return true;
	}

	return false;
}
bool CoSy::CLex::isDigit(const char * c) const
{
	if (*c >= '0' && *c <= '9')
	{
		return true;
	}

	return false;
}
bool CoSy::CLex::isSpace(const char * c) const
{
	if (*c == ' ' || *c == '\t' || isNewLine(c))
	{
		return true;
	}

	return false;
}
bool CoSy::CLex::isNewLine(const char * c) const
{
	if (*c == '\n' || *c == '\r')
	{
		return true;
	}

	return false;
}
bool CoSy::CLex::isArithmeticOp(const char * c) const
{
	if (*c == '+' || *c == '-' || *c == '*' || *c == '/' || *c == '%' || *c == '^')
	{
		return true;
	}

	return false;
}
bool CoSy::CLex::isRelationalOp(const char * c) const
{
	if (*c == '<' || *c == '>' || *c == '=')
	{
		return true;
	}

	return false;
}
bool CoSy::CLex::isLogicalOp(const char * c) const
{
	if (*c == '&' || *c == '|' || *c == '!')
	{
		return true;
	}

	return false;
}
bool CoSy::CLex::isSeparator(const char * c) const
{
	if (*c == ';' || *c == ',' || *c == ':')
	{
		return true;
	}

	return false;
}
bool CoSy::CLex::isGroupingChar(const char * c) const
{
	if (*c == '(' || *c == ')')
	{
		return true;
	}

	return false;
}
bool CoSy::CLex::isBlockChar(const char * c) const
{
	if (*c == '{' || *c == '}')
	{
		return true;
	}

	return false;
}
bool CoSy::CLex::isDimensionChar(const char * c) const
{
	if (*c == '[' || *c == ']')
	{
		return true;
	}

	return false;
}
bool CoSy::CLex::isStringLiteral(const char * c) const
{
	if (*c == '\"')
	{
		return true;
	}

	return false;
}