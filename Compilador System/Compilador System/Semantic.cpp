#include "stdafx.h"
#include "Semantic.h"


CoSy::CSemantic::CSemantic()
{
	
}
CoSy::CSemantic::~CSemantic() {

}
void CoSy::CSemantic::evaluateExperessions() {
	for (auto it = MapExpLog.begin();
		it != MapExpLog.end();
		it++) {
		//t* = ExpLogTree BuildExpLogTree(it->second);
		
		/*if (!t->evaluate()) {
			addError();
		}
*/
	}
}

bool CoSy::CSemantic::ProcessExpression() {
	std::vector <Vester_Level> NestedLevels;
	int currentNest = 0;
	int numErrors = 0;
	Vester_Level lvl0;
	NestedLevels.push_back(lvl0);

	EvalExpr(&NestedLevels, &currentNest, &numErrors);
	// check errors
	//REEEEEeeeeeeeeee

}

void CoSy::CSemantic::EvalExpr(std::vector<Vester_Level> *Nested_lvlzz, int *currentNest, int *numErrors) {
	EvalTerm(Nested_lvlzz, currentNest, numErrors);
	const CoSy::CToken *t= lex->getNextToken();
	if (t == nullptr) {
		return;
	}if (isOperand(t->getLex())) {
			Nested_lvlzz->at(*currentNest(), numOperators++);
			t = lex->getNextToken();
			if (t == nullprt) {
				return;
			}
			EvalExpr(Nested_lvlzz, currentNest, numErrors);
		}
}
void CoSy::CSemantic::EvalTerm(std::vector<Vester_Level> *Nested_lvlzz, int *currentNest, int *numErrors) {
	bool negatedTerm = checkForNot();
	const CToken *t = lex->peekToken(0);
	if (!t->getLex.Compare("(")) {
		Nested_lvlzz->at(*currentNest).numOperandsd++;
		(*currentNest)++;
		Vester_Level nextLvl;
		Nested_lvlzz->push_back(nextLvl);
		t = lex->getNextToken();
		if (t == nullptr) {
			return;
		}
		EvalExpr(Nested_lvlzz, currentNest, numErrors);
		if (!t->getLex().compare(")")) {
			// #1 
			// #2
			(*currentNest)--;

		}
	}
}
bool CoSy::CSemantic::checkForNot() {
	const CToken *t = lex->getNextToken();
	if (t->getType == UNARY_OP_NOT)
	{
		t = lex->getNextToken();
		return true;

	}
	return false;
}