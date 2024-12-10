#pragma once
#include "stdafx.h"
#include "LA.h"
bool PolishNotation(int i, LT::LexTable& lextable, IT::IdTable& idtable);
void PolishStart(LA::LEX lex);
void WritePolishNotationToFile(const char* filename, LT::LexTable& lextable);