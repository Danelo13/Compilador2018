#include "stdafx.h"
#include "Errors.h"


CoSy::CErrors::CErrors(){
	numErrors = 0;

	errorsArray = gcnew cli::array<System::String ^>(100);

	for (int i = 0; i < 100; i++)
	{
		errorsArray[i] = gcnew System::String("");
	}
}
CoSy::CErrors::~CErrors(){}

void CoSy::CErrors::clearErrors() {
	numErrors = 0;
}

bool CoSy::CErrors::addError(ERROR_PHASE errorPhase, int lineNumber, System::String ^ errorDesc, System::String ^ errorLine) {
	if (numErrors < 100)
	{
		System::String ^ errorLine = gcnew System::String("");
		if (errorLine->Length > 20)
		{
			errorLine = errorLine->Substring(0, 20);
		}
		else
		{
			errorLine = errorLine;
		}

		errorsArray->SetValue(System::String::Format("<{0}>: \t{1} \t\t{2} \t\t{3}", errorPhase.ToString(), lineNumber, errorDesc, errorLine), numErrors);
		numErrors++;

		return true;
	}

	return false;
}