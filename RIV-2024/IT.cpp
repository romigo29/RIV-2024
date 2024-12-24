#include"IT.h"
#include "Error.h"
#include"stdafx.h"

namespace IT
{
	idTable Create(int size)
	{
		idTable* tabl = new idTable;
		if (size > TI_MAXSIZE)
		{
			throw ERROR_THROW(116);
		}
		tabl->maxsize = size;
		tabl->size = 0;
		tabl->table = new Entry[size];
		return *tabl;
	}

	void Add(idTable& idTable, Entry entry)
	{
		bool ident = false;
		if (idTable.size + 1 > idTable.maxsize)
		{
			throw ERROR_THROW(117);
		}

		memcpy(&idTable.table[idTable.size], &entry, sizeof(Entry));
		idTable.size += 1;
	}
	Entry GetEntry(idTable& idTable, int n)
	{
		return idTable.table[n];
	}

	int search(idTable& idTable, Entry& entry)
	{
		for (int i = 0; i < idTable.size; i++)  
		{
			if (strcmp(entry.id, idTable.table[i].id) == 0 && entry.scope == idTable.table[i].scope)
			{
				return i;
			}
			else if (strcmp(entry.id, idTable.table[i].id) == 0 && idTable.table[i].idtype == IT::F)
			{
				return i;
			}
		}
		return -1;
	}
	int IsId(idTable& idTable, char id[ID_MAXSIZE])
	{

		for (int i = 0; i < idTable.size; i++) {
			if (strcmp(idTable.table[i].id, id) == 0) {
				return idTable.table[i].idxfirstLE;  
			}
		}
		return TI_NULLIDX; 
	}

	void Delete(idTable& idtable) {
		if (idtable.table != nullptr) {
			delete[] idtable.table;
			idtable.table = nullptr;
			idtable.size = 0;
			idtable.maxsize = 0;
		}
	}
}