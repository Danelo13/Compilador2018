#include "stdafx.h"
#include "SymTab.h"


CoSy::CSymTab::CSymTab()
{
}
CoSy::CSymTab::~CSymTab()
{
}

bool CoSy::CSymTab::symbolExists(std::string Symbol, ENODE_CLASS nClass, std::string nproc_func) {
	if (nClass == GLOBAL_VAR || nClass == PROC || nClass == FUNC) {
		if (mNodes.find(Symbol) != mNodes.end())
			return true;
	}
	else if (nClass == PARAM || nClass == LOCAL_VAR) {
		auto it = mNodes.find(Symbol);
		if (it != mNodes.end()) {
			CGlobalNode *gnode = it->second;
			if (gnode != nullptr) {
				CLocalNode *lnode = gnode->getLocalNode();
				while (lnode != nullptr) {
					if (lnode->getNodeClass() == nClass && lnode->getNprocFunc().compare(nproc_func) == 0) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool CoSy::CSymTab::addSymbol(std::string Symbol, ENODE_CLASS nClass, int dimen, std::string typ, std::string nproc_func){
	if (!symbolExists(Symbol, nClass, nproc_func)) {
		if (nClass == GLOBAL_VAR || nClass == PROC || nClass == FUNC) {
			mNodes.insert(std::make_pair(Symbol, new CGlobalNode(Symbol, nClass, dimen, typ)));
		}
		else if (nClass == PARAM || nClass == LOCAL_VAR) {
			CLocalNode *lNode = new CLocalNode(nClass, typ, dimen, nproc_func);
			auto it = mNodes.find(Symbol);
			if (it != mNodes.end()) {
				CGlobalNode *gNode = it->second;
				gNode->setLocalNode(lNode);
			}
			else {
				CGlobalNode * gNode = new CGlobalNode(Symbol, UNIDENTIFIED, dimen, typ);
				gNode->setLocalNode(lNode);
			}
		}
		return true;
	}
	return false;
}

CoSy::CGlobalNode::CGlobalNode(std::string _symbol, ENODE_CLASS _nClass, int _dimen, std::string _typ) {
	Symbol = _symbol;
	nClass = _nClass;
	Dimen = _dimen;
	typ = _typ;
	val = nullptr;
	localNode = nullptr;
}

void CoSy::CGlobalNode::setLocalNode(CLocalNode *n) {
	if (localNode == nullptr)
		localNode = n;
	else {
		CLocalNode *tmp = localNode;
		localNode = n;
		n->setNext(tmp);
	}
}

CoSy::CLocalNode::CLocalNode(ENODE_CLASS _nClass, std::string _typ, int _dimen, std::string _nproc_func) {
	nClass = _nClass;
	typ = _typ;
	dimen = _dimen;
	nproc_func = _nproc_func;
	val = nullptr;
	next = nullptr;
}

void CoSy::CLocalNode::setNext(CLocalNode *n) {
	next = n;
}