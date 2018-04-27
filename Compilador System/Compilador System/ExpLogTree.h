#pragma once
#include "ExpLogNodee.h"
ref class CExpLogTree
{
private:
	CExpLogNodee * root;

public:
	bool Evaluate();
	CExpLogTree();
	~CExpLogTree();

};

