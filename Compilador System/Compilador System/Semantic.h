#pragma once
#include"SymTab.h"
#include <vector>
#include "Lex.h"


struct Vester_Level {
	int numOperandsd = 0;
	int numOperators = 0;


};

namespace CoSy {
	 class CSemantic
	{
	private:
		std::map<std::string, std::vector<CToken>> MapExpLog;
		CLex * lex;
	public:
		CSemantic();
		~CSemantic();
		void addExpLog();
		std::string infix2postfix(std::vector<CToken>);
		void evaluateExperessions();
		bool ProcessExpression();
		
		
		void EvalExpr(std::vector<Vester_Level> *Nested_lvlzz, int *currentNest, int *numErrors);
		void EvalTerm(std::vector<Vester_Level> *Nested_lvlzz, int *currentNest, int *numErrors);
		bool checkForNot();
	};

}