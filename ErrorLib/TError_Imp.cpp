#include "TError_Adv.h"


TError::TError(char* error_, char* function_, char* file_, int line_)
{
	err = move(error_);
	func = move(function_);
	file = move(file_);
	line = line_;
	//std::cout << "\nError: " << er << "Function: " << func << "File: " << fi <<"Line:"<<line<< endl;
}

TError::TError(const char* error_, const char* function_, const char* file_, int line_)
{
	err = move(error_);
	func = move(function_);
	file = move(file_);
	line = line_;
	//std::cout << "\nError: " << err << "Function: " << func << "File: " << fil <<"Line:"<<line<< endl;
}

void TError::what()
{
	std::cout << "\nError: " << err << "Function: " << func << "File: " << file << "Line:" << line << std::endl;
}

