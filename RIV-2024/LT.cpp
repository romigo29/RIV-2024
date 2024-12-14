#include"LT.h"
#include"stdafx.h"
#include<iostream>

namespace LT
{
	lexTable Create(int size)
	{
		if (size > LT_MAXSIZE)
		{
			throw ERROR_THROW(114);
		}
		lexTable lexTable;
		lexTable.maxsize = size;
		lexTable.size = 0;
		lexTable.table = new Entry[size];
		return lexTable;
	}
	void Add(lexTable& lexTable, Entry entry)
	{
		if (lexTable.size > LT_MAXSIZE)
		{
			throw ERROR_THROW(115);
		}
		lexTable.table[lexTable.size] = entry;
		lexTable.size++;
	}

	Entry GetEntry(lexTable& lexTable, int n)
	{
		if (n > LT_MAXSIZE || n < 0)
		{
			throw ERROR_THROW(13);
		}
		return lexTable.table[n];
	}
	void Delete(lexTable& lexTable)
	{
		delete lexTable.table;
		lexTable.size = 0;
		lexTable.maxsize = 0;
	}

	void PrintLT(lexTable& lexTable) {
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