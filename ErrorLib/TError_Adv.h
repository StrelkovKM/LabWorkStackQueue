#pragma once
#include <iostream>
#include <string>
#include <utility>
#include "TString_Adv.h"


class TError
{
protected:
  TString err;
  TString func;
  TString file;
  int line;
public:
  TError(char* error_, char* function_, char* file_, int line_);
  TError(const char* error_, const char* function_, const char* file_, int line_);
  void what();
};