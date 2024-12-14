#pragma once
#include <iostream>
#include "Error.h"
#include "MFST.h"
#include "Out.h"
#include "LA.h"
using namespace std;

namespace CodeGeneration {

	void GenerateCode(LA::LEX lex, Out::OUT out);
}