#pragma once

namespace CoSy {

	public enum class ERROR_PHASE
	{
		GLOBAL,
		LEX_ANALYZER,
		SYN_ANALYZER,
		SEM_ANALYZER,
		CODE_GENERATION
	};

	public ref class CErrors
	{
	private:
		int numErrors;
		cli::array<System::String ^>^ errorsArray;
	public:
		CErrors();
		~CErrors();

		void clearErrors();
		bool addError(ERROR_PHASE errorPhase, int lineNumber, System::String ^ errorDesc, System::String ^errorLine);

		property int NumErrors
		{
			int get()
			{
				return numErrors;
			}
		}

		property int MaxErrors
		{
			int get()
			{
				return 100;
			}
		}

		property cli::array<System::String ^> ^ Errors
		{
			cli::array<System::String ^> ^ get()
			{
				return errorsArray;
			}
		}
	};
}
