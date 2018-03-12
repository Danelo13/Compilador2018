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
	
	while (*currentChar != lexSrcEof) 
	{
		switch (LState) {	
		case S_START:
			if (isAlpha(currentChar) || *currentChar == '_')
			{
				tokenBuffer.clear();
				
				LState = S_PARSING_ID;
			
			}
			else if (isDigit(currentChar)) {
				tokenBuffer.clear();
				
				LState = S_PARSING_INT;
				
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
			else if (isArithmeticOp(currentChar))
			{
				

				if (  *currentChar == '/' && *(currentChar + 1) == '*')
				{
					
					LState = S_PARSING_COMMENT;
					
					break;
				}
				else
				{
					tokenBuffer.clear();
					LState = S_PARSING_ARITHMETICOP;

				}
			}
			else if (isRelationalOp(currentChar)) {
				tokenBuffer.clear();
				if (*currentChar == '<')
				{
				
					LState = S_PARSING_LESSTHAN;
			
				}
				else if (*currentChar == '>')
				{
		
					LState = S_PARSING_GREATERTHAN;
			
				}
				else if (*currentChar == '=')
				{
		
					LState = S_PARSING_ASSIGN;
			
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
						LState = S_PARSING_LOGICAL_OP_AND;
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
					
						LState = S_PARSING_LOGICAL_OP_OR;
				
					}
					else
					{
						addError(currentLineNumber, "Operador OR incorrecto", currentLine);
					}
				}
				else if (*currentChar == '!')
				{
			
					LState = S_PARSING_LOGICAL_OP_NOT;
				
				}
			}
			else if (isSeparator(currentChar))
			{
				tokenBuffer.clear();
				if (*currentChar == ',')
				{
					LState = S_PARSING_COMMA;
				}
				else if (*currentChar == ':')
				{
					LState = S_PARSING_COLON;
				}
				else if (*currentChar == ';')
				{
					LState = S_PARSING_SEMICOLON;
				}
			}
			else if (isGroupingChar(currentChar))
			{
				tokenBuffer.clear();
				if (*currentChar == '(')
				{
			
					LState = S_PARSING_OPENPARENTHESIS;
		
				}
				else if (*currentChar == ')')
				{
		
					LState = S_PARSING_CLOSEPARENTHESIS;
		
				}
			}
			else if (isBlockChar(currentChar))
			{
				tokenBuffer.clear();
				if (*currentChar == '{')
				{
				
					LState = S_PARSING_OPENCURLYBRACKET;
			
				}
				else if (*currentChar == '}')
				{
			
					LState = S_PARSING_CLOSECURLYBRACKET;
				
				}
			}
			else if (isDimensionChar(currentChar))
			{
				tokenBuffer.clear();
				if (*currentChar == '[')
				{
				
					LState = S_PARSING_OPENBRACKET;
	
				}
				else if (*currentChar == ']')
				{
			
					LState = S_PARSING_CLOSEBRACKET;
		
				}
			}
			else if (isStringLiteral(currentChar))
			{
				tokenBuffer.clear();
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
				{
					addToken(tokenBuffer.c_str(), TOKEN_TYPE::INT, currentLineNumber);
				}
			}
			else if (*currentChar == '.') {
				tokenBuffer.append(currentChar, 1);
				currentChar++;
				if (isDigit(currentChar))
				{
					LState = S_PARSING_FLOAT;
				}
				else
				{
					addError(currentLineNumber, "FLOAT INVALIDO", currentLine);
					LState = S_START;
				}
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
			}
			else 
			{
				addToken(tokenBuffer.c_str(), TOKEN_TYPE::FLOAT, currentLineNumber);
				LState = S_START;
			}
			break;
		case S_PARSING_STRING:
			tokenBuffer.append(currentChar, 1);
			currentChar++;
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
			tokenBuffer.append(currentChar, 1);
			currentChar++;
			if (*currentChar == '=')
			{
				LState = S_PARSING_GREATERTHANOREQUAL;
			}
			else
			{
				addToken(tokenBuffer.c_str(), TOKEN_TYPE::RELATIONAL_OPERATOR, currentLineNumber);
				LState = S_START;
			}
			break;
		case S_PARSING_GREATERTHANOREQUAL:
			tokenBuffer.append(currentChar, 1);
			currentChar++;
			addToken(tokenBuffer.c_str(), TOKEN_TYPE::RELATIONAL_OPERATOR, currentLineNumber);
			LState = S_START;
			break;
			
		case S_PARSING_LESSTHAN:
			tokenBuffer.append(currentChar, 1);
			currentChar++;
			if(*currentChar == '=')
			{
				LState = S_PARSING_LESSTHANOREQUAL;
			}
			else
			{
				addToken(tokenBuffer.c_str(), TOKEN_TYPE::RELATIONAL_OPERATOR, currentLineNumber);
				LState = S_START;
			}
			break;
		case S_PARSING_LESSTHANOREQUAL:
			tokenBuffer.append(currentChar, 1);
			currentChar++;
			addToken(tokenBuffer.c_str(), TOKEN_TYPE::RELATIONAL_OPERATOR, currentLineNumber);
			LState = S_START;
			break;

		case S_PARSING_ASSIGN:
			tokenBuffer.append(currentChar, 1);
			currentChar++;
			if (*currentChar == '=')
			{
				LState = S_PARSING_EQUAL;
			}
			else
			{
				addToken(tokenBuffer.c_str(), TOKEN_TYPE::ASSIGN_OPERATOR, currentLineNumber);
				LState = S_START;
			}
			break;
		case S_PARSING_EQUAL:
			tokenBuffer.append(currentChar, 1);
			currentChar++;
			addToken(tokenBuffer.c_str(), TOKEN_TYPE::RELATIONAL_OPERATOR, currentLineNumber);
			LState = S_START;
			break;
		case S_PARSING_NOTEQUAL:
			tokenBuffer.append(currentChar, 1);
			currentChar++;
			addToken(tokenBuffer.c_str(), TOKEN_TYPE::RELATIONAL_OPERATOR, currentLineNumber);
			LState = S_START;
			break;
		case S_PARSING_OPENPARENTHESIS:
			tokenBuffer.append(currentChar, 1);
			currentChar++;
			addToken(tokenBuffer.c_str(), TOKEN_TYPE::GROUPING_OPERATOR, currentLineNumber);
			LState = S_START;
			break;
		case S_PARSING_CLOSEPARENTHESIS:
			tokenBuffer.append(currentChar, 1);
			currentChar++;
			addToken(tokenBuffer.c_str(), TOKEN_TYPE::GROUPING_OPERATOR, currentLineNumber);
			LState = S_START;
			break;
		case S_PARSING_ARITHMETICOP:
			tokenBuffer.append(currentChar, 1);
			currentChar++;
			addToken(tokenBuffer.c_str(), TOKEN_TYPE::ARITHMETIC_OPERATOR, currentLineNumber);
			LState = S_START;
			break;
		case S_PARSING_LOGICAL_OP_AND:
			tokenBuffer.append(currentChar, 1);
			currentChar++;
			addToken(tokenBuffer.c_str(), TOKEN_TYPE::LOGICAL_OPERATOR, currentLineNumber);
			LState = S_START;
			break;
		case S_PARSING_LOGICAL_OP_OR:
			tokenBuffer.append(currentChar, 1);
			currentChar++;
			addToken(tokenBuffer.c_str(), TOKEN_TYPE::LOGICAL_OPERATOR, currentLineNumber);
			LState = S_START;
			break;
		case S_PARSING_LOGICAL_OP_NOT:
			tokenBuffer.append(currentChar, 1);
			currentChar++;
			if (*currentChar == '=')
			{
				LState = S_PARSING_NOTEQUAL;
			}
			else
			{
				addToken(tokenBuffer.c_str(), TOKEN_TYPE::LOGICAL_UNARY_OPERATOR, currentLineNumber);
				LState = S_START;
			}
			break;
		case S_PARSING_OPENBRACKET:
			tokenBuffer.append(currentChar, 1);
			currentChar++;
			addToken(tokenBuffer.c_str(), TOKEN_TYPE::DIMENSION_OPERATOR, currentLineNumber);
			LState = S_START;
			break;
		case S_PARSING_CLOSEBRACKET:
			tokenBuffer.append(currentChar, 1);
			currentChar++;
			addToken(tokenBuffer.c_str(), TOKEN_TYPE::DIMENSION_OPERATOR, currentLineNumber);
			LState = S_START;
			break;
		case S_PARSING_OPENCURLYBRACKET:
			tokenBuffer.append(currentChar, 1);
			currentChar++;
			addToken(tokenBuffer.c_str(), TOKEN_TYPE::GROUPING_OPERATOR, currentLineNumber);
			LState = S_START;
			break;
		case S_PARSING_CLOSECURLYBRACKET:
			tokenBuffer.append(currentChar, 1);
			currentChar++;
			addToken(tokenBuffer.c_str(), TOKEN_TYPE::GROUPING_OPERATOR, currentLineNumber);
			LState = S_START;
			break;
		case S_PARSING_SEMICOLON:
			tokenBuffer.append(currentChar, 1);
			currentChar++;
			addToken(tokenBuffer.c_str(), TOKEN_TYPE::SEPARATOR, currentLineNumber);
			LState = S_START;
			break;
		case S_PARSING_COMMA:
			tokenBuffer.append(currentChar, 1);
			currentChar++;
			addToken(tokenBuffer.c_str(), TOKEN_TYPE::SEPARATOR, currentLineNumber);
			LState = S_START;
			break;
		case S_PARSING_COLON:
			tokenBuffer.append(currentChar, 1);
			currentChar++;
			addToken(tokenBuffer.c_str(), TOKEN_TYPE::SEPARATOR, currentLineNumber);
			LState = S_START;
			break;
		case S_PARSING_COMMENT:
			currentChar++;
			while (*currentChar != lexSrcEof)
			{
				if (*currentChar == '*')
				{
					currentChar++;
					if (*currentChar == '/')
					{
						LState = S_START;
						break;
					}
				}
				else
				{
					currentChar++;
				}
			}
			if (*currentChar == lexSrcEof)
			{
			
				addError(currentLineNumber, "Commentario no cerrado", currentLine);
				break;
			}
			currentChar++;
			break;
		default:
			break;
		case S_PARSING_ID:
			if (isAlpha(currentChar) || *currentChar == '_' || isDigit(currentChar))
			{
				tokenBuffer.append(currentChar, 1);
				currentChar++;
				if (*currentChar == lexSrcEof)
				{
					if (Keywords.find(tokenBuffer) != Keywords.end())
					{
						addToken(tokenBuffer.c_str(), TOKEN_TYPE::KEYWORD, currentLineNumber);
					}
					else if (tokenBuffer.compare("true") == 0 || tokenBuffer.compare("false") == 0)
					{
						addToken(tokenBuffer.c_str(), TOKEN_TYPE::LOGICAL_CONST, currentLineNumber);
					}
					else
					{
						addToken(tokenBuffer.c_str(), TOKEN_TYPE::ID, currentLineNumber);
					}
				}
			}
			else
			{
				if (Keywords.find(tokenBuffer) != Keywords.end())
				{
					addToken(tokenBuffer.c_str(), TOKEN_TYPE::KEYWORD, currentLineNumber);
				}
				else if (tokenBuffer.compare("true") == 0 || tokenBuffer.compare("false") == 0)
				{
					addToken(tokenBuffer.c_str(), TOKEN_TYPE::LOGICAL_CONST, currentLineNumber);
				}
				else
				{
					addToken(tokenBuffer.c_str(), TOKEN_TYPE::ID, currentLineNumber);
				}
				LState = S_START;
			}
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
	currentToken = 0;
}
int CoSy::CLex::getNumTokens() const {
	return Tokens.size();
}
void CoSy::CLex::getTokens(std::vector<CToken *>* tokenVec) const 
{
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


const CoSy::CToken* CoSy::CLex::getNextToken() {
	if (currentToken < Tokens.size()) {
		return Tokens[currentToken++];
	}
	return nullptr;
}
const CoSy::CToken* CoSy::CLex::peekToken(int num) {
	if (((currentToken + num) < Tokens.size()) && (currentToken + num >= 0))
		return Tokens[currentToken + num];
	return nullptr;
}