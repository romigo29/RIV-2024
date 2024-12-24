#pragma once
#include <iostream>
#include "Error.h"
#include "MFST.h"
#include "Out.h"
#include "LA.h"
using namespace std;

namespace CodeGeneration {

	void GenerateCode(LA::LEX lex, Out::OUT out);
	void ExpressionHandler(std::ofstream* stream, LA::LEX lex, int startpos, int endpos);
}