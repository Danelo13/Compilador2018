#include "stdafx.h"
#include "ExpLogTree.h"


CExpLogTree::CExpLogTree()
{
}

bool CExpLogTree::Evaluate() {
	if (root != nullptr) {
		return root->Evaluate();
	}
	return false;
}
