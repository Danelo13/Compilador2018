#include "stdafx.h"
#include "Syntax.h"


CoSy::CSyntax::CSyntax(CErrors ^err, CLex *Lex)
{
	refManagedErrors = err;
	lex = Lex;
}


bool CoSy::CSyntax::checkSyntax() {
	checkProgram();
	return (refManagedErrors->NumErrors == 0);

}
void CoSy::CSyntax::checkProgram() {
	
	bool varsProcessed = false;
	bool procfunctProcessed = false;
	bool mainProcessed = false;
	const CoSy::CToken * t = lex->getNextToken();
	while (t != NULL) {
		t = lex->getNextToken();
		if ((t->getLex()).compare("var")) {
			checkVars();
			varsProcessed = true;
			if (mainProcessed || procfunctProcessed) {
				addError(t->getLineNumber, "Error: Out of place, Variables are after Main");
			}
		}
		else if ((t->getLex().compare("procedure"))) {
			checkProc();
			procfunctProcessed = true;
			if (mainProcessed) {
				addError(t->getLineNumber, "Error: Out of place, Procedure is after main");
			}
		}
		else if ((t->getLex().compare("function"))) {
			checkFunct();
			procfunctProcessed = true;
			if (mainProcessed) {
				addError(t->getLineNumber, "Error: Out of place, Functions found after main");
			}
		}
		else if ((t->getLex().compare("main"))) {
			checkMain();
			mainProcessed = true;
		}
	}
}
//
void CoSy::CSyntax::checkVars()
{
	const CToken* t = lex->peekToken(0);
	vector <std::string> temp;
	while ((t != nullptr) && (!t->getLex().compare("var")))
	{
		do {
			t = lex->getNextToken();
			if (t->getType() == ID)
			{
				temp.push_back(t->getLex());
			}
			else
			{
				addErrorExpected(t->getLineNumber(), "id", t->getLex().c_str());
				//recoverFromError();
			}
			t = lex->getNextToken();
			if (!t->getLex().compare("["))
			{
				checkDimension();
				t = lex->getNextToken();
			}
		} while (!t->getLex().compare(","));
		if (t->getLex().compare(":"))
		{
			addErrorExpected(t->getLineNumber(), ":", t->getLex().c_str());
			//recoverFromError();
		}
		t = lex->getNextToken();
		checkType();
		t = lex->getNextToken();
		if (t->getLex().compare(";"))
		{
			addErrorExpected(t->getLineNumber(), ";", t->getLex().c_str());
			//recoverError();
		}
		t = lex->getNextToken();
	}
}
//
void CoSy::CSyntax::checkType() {
	// <tipo>:: = 'int' | 'float' | 'bool' | 'string'
	const CoSy::CToken * t = lex->peekToken(0);
	if (!t->getLex().compare("int") || !t->getLex().compare("float") || !t->getLex().compare("bool") || !t->getLex().compare("string")) {
		
		SymbT.addSymbol(t->getLex(), GLOBAL_VAR, 1, t->getType, t->getTypeStr);
		
	}
	else {
		addError(t->getLineNumber, "Not a Type?");
	}
} 
// 
void CoSy::CSyntax::checkMain() {
	const CoSy::CToken * t = lex->getNextToken();
	if (!t->getLex().compare("(")) {
		t = lex->getNextToken();
		if (!t->getLex().compare(")")) {
			checkBlock();
		}
		else
			addErrorExpected(t->getLineNumber, ")", t->getLex().c_str());
	}
}
//
int CoSy::CSyntax::checkDimension() {
	const CToken* t = lex->getNextToken();
	if (!t->getLex().compare("["))
	{
		t = lex->getNextToken();
		if (!t->getType() == INT || !t->getType() == DIMENSION_OPERATOR)
		{
			if (!t->getType() == INT)
			{
				return stoi(t->getLex());;
			}
		}
		else
		{
			addError(t->getLineNumber(), "Error de Dimension");
		}
	}
}
//
void CoSy::CSyntax::checkFunct() {
	const CToken* t = lex->getNextToken();
	if (t->getType() == ID)
	{
		t = lex->getNextToken();
		if (!t->getLex().compare("("))
		{
			checkParam();
			t = lex->getNextToken();
			if (!t->getLex().compare(":"))
			{
				t = lex->getNextToken();
				vector <std::string> temp;
				temp.push_back(t->getLex());
				checkType();
				t = lex->getNextToken();
				checkBlock();
			}
			else
			{
				addErrorExpected(t->getLineNumber(), ":", t->getLex().c_str());
				//recoverFromError();
			}
		}
		else
		{
			addError(t->getLineNumber(), "NO Parameters");
		}
	}
	else
	{
		addError(t->getLineNumber(), "No ID");
	}
}
//
void CoSy::CSyntax::checkProc() {
	const CToken* t = lex->getNextToken();
	if (t->getType() == ID)
	{
		t = lex->getNextToken();
		if (!t->getLex().compare("("))
		{
			checkParam();
			t = lex->getNextToken();
				checkBlock();
		}
		else
		{
			addError(t->getLineNumber(), "NO Parameters");
		}
	}
	else
	{
		addError(t->getLineNumber(), "No ID");
	}
}
//
void CoSy::CSyntax::checkBlock(){
	const CoSy::CToken *t = lex->getNextToken();
	if (!t->getLex().compare("{")) {
		t = lex->getNextToken();
		while (t->getLex().c_str() != "}") {
			t = lex->getNextToken();
			if (isStatement) {
				checkStatement();
			}
			if ((t->getLex().compare("var"))) {
				checkVars();
			}
		}
	}
}
//
void CoSy::CSyntax::checkRead() {
	const CToken* t = lex->getNextToken();
	int dimen = 0;
	if (!t->getLex().compare("("))
	{
		t = lex->getNextToken();
	}
	else
	{
		addErrorExpected(t->getLineNumber(), "(", t->getLex().c_str());
	}
	if (t->getType() == ID)
	{
		t = lex->getNextToken();
		if (!t->getLex().compare("["))
		{
			dimen = checkDimension();
		}
		t = lex->getNextToken();
	}
	if (!t->getLex().compare(")"))
	{
		t = lex->getNextToken();
	}
	else
	{
		addErrorExpected(t->getLineNumber(), ")", t->getLex().c_str());
	}
	if (t->getLex().compare(";"))
	{
		addErrorExpected(t->getLineNumber(), ";", t->getLex().c_str());
	}
}
//
void CoSy::CSyntax::checkPrint() {

}
void CoSy::CSyntax::checkIf() {
	const CToken* t = lex->getNextToken();
	if (!t->getLex().compare("("))
	{
		t = lex->getNextToken();
	}
	else {
		addErrorExpected(t->getLineNumber, "(", t->getLex().c_str());
	}
	// CHECK FOR EXPR
	if (!t->getLex().compare(")"))
	{
		t = lex->getNextToken();
	}
	else {
		addErrorExpected(t->getLineNumber, ")", t->getLex().c_str());
	}
	if (!t->getLex().compare("{"))
	{
		t = lex->getNextToken();
		checkBlock();
	}
	else {
		addErrorExpected(t->getLineNumber, "{", t->getLex().c_str());
	}
}
void CoSy::CSyntax::checkParam() {
	const CToken* t = lex->getNextToken;
	vector <std::string> temp;

	while (!t->getLex().compare(")"))
	{
		do {
			t = lex->getNextToken();
			if (t->getType() == ID)
			{
				temp.push_back(t->getLex());
			}
			else
			{
				addErrorExpected(t->getLineNumber(), "id", t->getLex().c_str());
				//recoverFromError();
			}
			t = lex->getNextToken();
		} while (!t->getLex().compare(","));
		if (t->getLex().compare(":"))
		{
			addErrorExpected(t->getLineNumber(), ":", t->getLex().c_str());
			//recoverFromError();
		}
		t = lex->getNextToken();
		checkType();
		t = lex->getNextToken();
		if (t->getLex().compare(";"))
		{
			addErrorExpected(t->getLineNumber(), ";", t->getLex().c_str());
			//recoverError();
		}
		t = lex->getNextToken();
	}
}
//
void CoSy::CSyntax::checkAssign(){
	const CToken* t = lex->getNextToken();
	int dimen = 0;
	if (t->getType() == ID)
	{
		t = lex->getNextToken();
		if (!t->getLex().compare("["))
		{
			dimen = checkDimension();
			t = lex->getNextToken();
		}
		if (t->getLex().compare("="))
		{
			addErrorExpected(t->getLineNumber(), "=", t->getLex().c_str());
		}
		else
		{
			t = lex ->getNextToken();
			if (t->getType() != ID || t->getType() != INT || t->getType() != FLOAT || t->getType() != STRING)
			{
				addError(t->getLineNumber(), "ERROR NO ID");
			}
		}
	}
}
//

//complementary to checkBlock
bool CoSy::CSyntax::isStatement()
{
	const CToken* t = lex->peekToken(0);
	if (!t->getLex().compare("if") || t->getLex().compare("while") || t->getLex().compare("for") || t->getLex().compare("switch") || t->getLex().compare("read") || t->getLex().compare("print") || t->getLex().compare("return") || t->getLex().compare("procedure") || t->getLex().compare("function"))
		return true;
	return false;
}


void CoSy::CSyntax::checkProc_Funct_Call(){}
void CoSy::CSyntax::checkReturn(){}

void CoSy::CSyntax::checkStatement()
{
	const CToken* t = lex->peekToken(0);
	if (!t->getLex().compare("if"))
	{
		checkIf();
	}
	if (t->getLex().compare("while"))
	{
		checkWhile();
	}
	if (!t->getLex().compare("for"))
	{
		checkFor();
	}
	if (!t->getLex().compare("switch"))
	{
		checkSwitch();
	}
	if (!t->getLex().compare("read"))
	{
		checkRead();
	}
	if (!t->getLex().compare("print"))
	{
		checkPrint();
	}
	if (!t->getLex().compare("return"))
	{
		t = lex->getNextToken();
		if (t->getType() != ID)
		{
			addErrorExpected(t->getLineNumber(), "id", t->getLex().c_str());
		}
		else
		{
			t = lex->getNextToken();
		}
		t = lex->getNextToken();
		if (t->getLex().compare(";"))
		{
			addErrorExpected(t->getLineNumber(), ";", t->getLex().c_str());
		}
	}
	if (!t->getLex().compare("procedure"))
	{
		checkProc_Funct_Call();
	}
	
}
//

void CoSy::CSyntax::checkListaImp() {}

void CoSy::CSyntax::checkSwitch(){
	const CToken* t = lex->getNextToken();
	if (!t->getLex().compare("("))
	{
		t = lex->getNextToken();
	}
	else
	{
		addErrorExpected(t->getLineNumber(), "(", t->getLex().c_str());
	}
	if (t->getType() == ID)
	{
		t = lex->getNextToken();
	}
	else
	{
		addError(t->getLineNumber(), "ERROR NO ID");
	}
	if (!t->getLex().compare(")"))
	{
		t = lex->getNextToken();
	}
	else
	{
		addErrorExpected(t->getLineNumber(), ")", t->getLex().c_str());
	}
	if (!t->getLex().compare("{"))
	{
		while (t->getLex().compare("}"))
		{
			t = lex->getNextToken();
			if (!t->getLex().compare("case"))
			{
				checkBlock();
			}
			if (!t->getLex().compare("default"))
			{
				checkBlock();
			}
		}
	}
}
//

void CoSy::CSyntax::checkWhile(){
	const CToken* t = lex->getNextToken();
	if (!t->getLex().compare("("))
	{
		t = lex->getNextToken();
	}
	else {
		addErrorExpected(t->getLineNumber, "(", t->getLex().c_str());
	}
	// CHECK FOR EXPR
	if (!t->getLex().compare(")"))
	{
		t = lex->getNextToken();
	}
	else {
		addErrorExpected(t->getLineNumber, ")", t->getLex().c_str());
	}
	if (!t->getLex().compare("{"))
	{
		t = lex->getNextToken();
		checkBlock();
	}
	else {
		addErrorExpected(t->getLineNumber, "{", t->getLex().c_str());
	}
}
void CoSy::CSyntax::checkFor(){}