// This is the main DLL file.

#include "stdafx.h"

#include "Compilador System.h"

CoSy::cManager::cManager() 
{
	err = gcnew CErrors;
	lex = new CLex(err);
	synt = new CSyntax(err, lex);

};
CoSy::cManager::~cManager() 
{
	delete lex;
	delete synt;
};

cli::array<System::String^>^ CoSy::cManager::Compile(System::String ^program)
{
	int PhaseFail = 0;
	cli::array<System::String^>^ compilationDetails;

	if (err && lex != NULL) {
		err->clearErrors();
	}
	else if (!err) {
		compilationDetails = gcnew cli::array<System::String ^>(1);
		compilationDetails[0] = gcnew System::String("===== Compiler: fail Error module");
		return compilationDetails;	
	}
	else if (lex == NULL) {
		compilationDetails = gcnew cli::array<System::String ^>(1);
		compilationDetails[0] = gcnew System::String("===== Compiler: fail Lex module");
		return compilationDetails;
	}
	

	lex->parseCode((const char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(program).ToPointer());

	// synt->checkSyntax(())




	return compilationDetails;
	
}

cli::array<System::String^>^ CoSy::cManager::GetAllTokens() {
	cli::array<System::String ^>^ allTokens;
	if (/*lex->succeded() &&*/ lex->getNumTokens() > 0) {
		allTokens = gcnew cli::array<System::String^> (lex->getNumTokens());
		std::vector<CToken *> allTokensVec;
		lex->getTokens(&allTokensVec);

		for (int i = 0; i < (int)allTokensVec.size(); i++) {
			System::String ^ LEX = gcnew System::String(allTokensVec[i]->getLex().c_str());
			System::String ^ typ = gcnew System::String(allTokensVec[i]->getTypeStr().c_str());

			allTokens[i] = gcnew System::String("");
			allTokens[i] = System::String::Format("{0}\t\t{1}",LEX,typ);
		}

	}
	else
	{
		allTokens = gcnew cli::array<System::String ^>(1);
		allTokens[0] = gcnew System::String("NO TOKENS FOUND");
	}
	return allTokens;
}

cli::array<System::String^>^ CoSy::cManager::getCompilationDetails()
{
	int numErrorLines = 0;
	cli::array<System::String^>^ compilationDetails;

	if (lex->succeeded() /**/) {
		compilationDetails = gcnew cli::array<System::String^>(1);
		compilationDetails[0] = gcnew System::String("================== Compilation Succeded ================");
		return compilationDetails;
	}
	else
	{
		numErrorLines = err->NumErrors + 3;
		compilationDetails = gcnew cli::array<System::String^>(numErrorLines);
		for (int i = 0; i < numErrorLines; i++) {
			compilationDetails[i] = gcnew System::String("");
		}
		compilationDetails[0] = System::String::Format("Compilation Phase \tLine Number \tDescription \t\t\tLine");
		compilationDetails[1] = System::String::Format("----------------------------------------------------------------");

		cli::array<System::String ^>^ allErrors = err->Errors;
		for (int i = 2; i < numErrorLines - 1; i++) {
			compilationDetails[i] = allErrors[i - 2];
		}
		compilationDetails[numErrorLines - 1] = System::String::Format("================ Compile: Failed {0} errors ==========", err->NumErrors);
		return compilationDetails;
	}

}