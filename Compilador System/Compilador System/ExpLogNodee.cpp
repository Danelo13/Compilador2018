#include "stdafx.h"
#include "ExpLogNodee.h"


CExpLogNodee::CExpLogNodee()
{
}

bool CExpLogNodee::Evaluate() {
	if (node1 != nullptr) 
		node1->Evaluate();
	if (node2 != nullptr)
		node2->Evaluate();
	if (node1 == nullptr && node2 == nullptr) {
		CurrentNodeRes = 0;
	}
	else {
		Res_ExpLogNode r1 = node1->getResult();
		Res_ExpLogNode r2 = node2->getResult();
		if (NodeOperatorType == OP_ARIT) {
				
		}
		else if (NodeOperatorType == OP_LOG) {

		}
		else if (NodeOperatorType == OP_REL) {

		}
	}

	
}
