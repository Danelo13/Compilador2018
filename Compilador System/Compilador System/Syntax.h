#pragma once
#include "Errors.h"
#include "Lex.h"
#include "SymTab.h"

#define SYN_ERR_VAR_GLOBAL_YA_DEF "Var Global ya definida"
#define SYN_ERR_VAR_LOCAL_YA_DEF "Var Local ya definida"
#define SYN_ERR_VAR_SAME_PROC "Var no se puede llamar igual que proc"
#define SYN_ERR_PARAM_YA_DEF "Parametro ya Definido"
#define SYN_ERR_PROFUN_YA_DEF "Procedimiento/Funcion ya Definido"
#define SYN_ERR_VAR_LOCAL_SAME_PARAMA "Var Local no se puede llamar igual que procedimiento"
#define SYN_ERR_EXPECTED_VS_REALITY "Expected something else"



namespace CoSy {
	
	class CSyntax
	{
	private:
		msclr::gcroot<CErrors ^> refManagedErrors;
		CLex * lex;
		CSymTab SymbT;

		void addError(int lineNum, const char *desc) {
		
				String ^ strDesc = gcnew String(desc);
				String ^ strLine = gcnew String("");
				refManagedErrors->addError(CoSy::ERROR_PHASE::SYN_ANALYZER, lineNum, strDesc, strLine);
				
		}
		void addErrorExpected(const int line,const char *e,const char *f) {
			std::string se_esperaba("se esperaba: ");
			std::string expected(e);
			std::string se_encontro("se encontro: ");
			std::string found(f);
			std::string tError;

			tError += se_esperaba + expected + se_encontro + found;
			addError(line, tError.c_str());
		}

		void checkProgram();
		void checkVars();
		void checkType();
		void checkMain();

		void checkSwitch(); 
		void checkWhile();
		void checkFor();
		int checkDimension();
		void checkRead();
		void checkPrint();
		void checkIf();
		void checkParam();
		void checkAssign();
		void checkProc();
		void checkFunct();
		void checkBlock();
		void checkProc_Funct_Call();
		void checkReturn();
		void checkListaImp();
		void checkStatement();
		void checkEXPLOG();  // !
		void checkInc_Dec();

		bool isStatement();
	public:
		
		CSyntax(CErrors ^err, CLex *Lex);
		bool checkSyntax();
	};
}


