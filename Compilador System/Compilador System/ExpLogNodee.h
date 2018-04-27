#pragma once
#include "Token.h"
enum Res_ExpLogNode {
	RES_ARIT,
	RES_BOOL
};
enum OP_ExpLogNode {
	OP_ARIT,
	OP_REL,
	OP_LOG
};
 class CExpLogNodee
{
	CoSy::CToken * operand;
	CExpLogNodee * node1;
	CExpLogNodee * node2;
	Res_ExpLogNode ExpectedResNode1, ExpectedResNode2, CurrentNodeRes;
	OP_ExpLogNode NodeOperatorType;
	CoSy::CToken * nodeOperator;
	
public:
	bool Evaluate();
	CExpLogNodee();
};

