#pragma once
#include <map>
#include <string>

namespace CoSy {
	enum ENODE_CLASS
	{
		UNIDENTIFIED = 0,
		GLOBAL_VAR,
		PROC,
		FUNC,
		PARAM,
		LOCAL_VAR
	};

	class CNodeVal
	{
	public:
		CNodeVal() {};
		~CNodeVal() {};
	};
	class CLocalNode
	{
	private:
		ENODE_CLASS nClass;
		std::string typ;
		int dimen;
		std::string nproc_func;
		CNodeVal * val;
		CLocalNode *next;
	public:
		CLocalNode(ENODE_CLASS _nClass, std::string _typ, int _dimen, std::string _nproc_func);
		void setNext(CLocalNode *n);

		ENODE_CLASS getNodeClass() {
			return nClass;
		}
		std::string getNprocFunc() {
			return nproc_func;
		}
		CLocalNode* const GetNext() {
			return next;
		}


		~CLocalNode();
	};

	class CGlobalNode 
	{
	private:
		std::string Symbol;
		ENODE_CLASS nClass;
		CNodeVal *val;
		int Dimen;
		std::string typ;
		CLocalNode *localNode;

	public:
		 CGlobalNode(std::string _symbol,ENODE_CLASS _nClass, int _dimen, std::string _typ);
		 void setLocalNode(CLocalNode *n);

		 ENODE_CLASS getNodeClass() {
			 return nClass;}
		 std::string getSymbol() {
			 return Symbol;}
		 CLocalNode *const getLocalNode() {
			 return localNode;}

		 ~CGlobalNode();
	};

	


	 class CSymTab
	{
	private:
		std::map < std::string, CGlobalNode* >mNodes;

	public:
		bool symbolExists(std::string Symbol, ENODE_CLASS nClass, std::string nproc_func);
		bool addSymbol(std::string Symbol, ENODE_CLASS nClass, int dimen, std::string typ, std::string nproc_func);
		CSymTab();
		~CSymTab();
	};




}
