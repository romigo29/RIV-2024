#pragma once
#include "stdafx.h"
#include "Out.h" 
#include "In.h" 
#include "Parm.h" 
#include "Error.h"

namespace Out     // Работа с выходным файлом
{
	struct OUT		// выходной файл
	{
		wchar_t outfile[PARM_MAX_SIZE]; // имя выходного файла 
		std::ofstream* stream;			// выходной поток выходного файла
	};

	static const OUT INITOUT{ L"", NULL }; 
	OUT getout(wchar_t outfile[]); 
	void WriteOut(In::IN in, wchar_t out[]); 
	void WriteError(OUT out, Error::ERROR error); 
	void CloseOut(OUT out); 
}