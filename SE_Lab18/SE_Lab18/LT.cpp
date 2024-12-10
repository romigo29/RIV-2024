#include"LT.h"
#include"stdafx.h"
#include<iostream>

namespace LT
{
	LexTable Create(int size)
	{
		if (size > LT_MAXSIZE)
		{
			throw ERROR_THROW(114);
		}
		LexTable lextable;
		lextable.maxsize = size;
		lextable.size = 0;
		lextable.table = new Entry[size];
		return lextable;
	}
	void Add(LexTable& lextable, Entry entry)
	{
		if (lextable.size > LT_MAXSIZE)
		{
			throw ERROR_THROW(115);
		}
		lextable.table[lextable.size] = entry;
		lextable.size++;
	}

	Entry GetEntry(LexTable& lextable, int n)
	{
		if (n > LT_MAXSIZE || n < 0)
		{
			throw ERROR_THROW(13);
		}
		return lextable.table[n];
	}
	void Delete(LexTable& lextable)
	{
		delete lextable.table;
		lextable.size = 0;
		lextable.maxsize = 0;
	}

	void PrintLT(LexTable& lexTable) {
		int currentLine = 1;
		LT::Entry current_entry_LT;

		ofstream LT_file_modified;
		LT_file_modified.open("LT_modified.txt");
		LT_file_modified << currentLine;
		LT_file_modified << '\t';
		for (int i = 0; i < lexTable.size; i++)
		{
			current_entry_LT = LT::GetEntry(lexTable, i);
			if (currentLine != current_entry_LT.sn)
			{
				currentLine = current_entry_LT.sn;
				LT_file_modified << '\n';
				LT_file_modified << currentLine;
				LT_file_modified << '\t';
			}

			LT_file_modified << current_entry_LT.lexema[0];
		}

		LT_file_modified.close();
	}
}