#pragma once
#include <iostream>
#include <string>
#include <utility>

class TError
{
protected:
  std::string error;
  std::string function;
  std::string file;
  int line;
public:
  TError(const std::string& error_, const std::string& function_, const std::string& file_, int line_);
  void what();
};

TError::TError(const std::string& error_, const std::string& function_, const std::string& file_, int line_)
  : error(error_), function(function_), file(file_), line(line_)
{
  std::cout << "\nError: " << error << " Function: " << function << " File: " << file << " Line:" << line << std::endl;
}

void TError::what()
{
	std::cout << "\nError: " << error << "Function: " << function << "File: " << file << "Line:" << line << std::endl;
}

#define THROW_ERROR(err,...) throw TError(err, __func__, __FILE__, __LINE__);