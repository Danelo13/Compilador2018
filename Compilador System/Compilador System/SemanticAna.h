#pragma once
#include "ExpLogTree.h"
#include "Token.h"
#include <vector>
#include <map>
#include <string>


ref class CSemanticAna
{
private:
	std::map<int, CExpLogTree *> if_for_while_Expressions;
	std::map<std::string, CExpLogTree *> return_expressions;
	std::map<std::string, CExpLogTree *> assign_expressions;
public:
	CSemanticAna();
	
	bool ProcessExpression();
	void EvaluateExpression();

};

