// Compilador System.h
#pragma once
#include <string>
#include "Lex.h"
#include "Errors.h"
#include "Token.h"



namespace CoSy {

	public ref class cManager
	{
		// TODO: Add your methods for this class here.
	private:
		CLex * lex;
		CErrors ^ err;

	public:
		cManager();
		~cManager();

		cli::array<System::String^>^ Compile(System::String ^program);
		cli::array<System::String^>^ GetAllTokens();
		cli::array<System::String^>^ getCompilationDetails();
		
		
	};
	
}
