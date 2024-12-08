#include"IT.h"
#include "Error.h"
#include"stdafx.h"


namespace IT
{
	IdTable Create(int size)
	{
		IdTable* tabl = new IdTable;
		if (size > TI_MAXSIZE)
		{
			throw ERROR_THROW(116);
		}
		tabl->maxsize = size;
		tabl->size = 0;
		tabl->table = new Entry[size];
		return *tabl;
	}

	void Add(IdTable& idtable, Entry entry)
	{
		bool ident = false;
		if (idtable.size + 1 > idtable.maxsize)
		{
			throw ERROR_THROW(117);
		}

		memcpy(&idtable.table[idtable.size], &entry, sizeof(Entry));
		idtable.size += 1;
	}
	Entry GetEntry(IdTable& idtable, int n)
	{
		return idtable.table[n];
	}

	int search(IdTable& idtable, Entry& entry)
	{
		for (int i = 0; i < idtable.size; i++)  
		{
			if (strcmp(entry.id, idtable.table[i].id) == 0 && entry.scope == idtable.table[i].scope)
			{
				return i;
			}
			else if (strcmp(entry.id, idtable.table[i].id) == 0 && idtable.table[i].idtype == IT::F)
			{
				return i;
			}
		}
		return -1;
	}
	int IsId(IdTable& idtable, char id[ID_MAXSIZE])
	{
		// Проход по всем элементам таблицы идентификаторов
		for (int i = 0; i < idtable.size; i++) {
			// Сравнение переданного идентификатора с текущим в таблице
			if (strcmp(idtable.table[i].id, id) == 0) {
				return idtable.table[i].idxfirstLE;  // Возврат номера строки, если идентификатор найден
			}
		}
		return TI_NULLIDX;  // Возвращаем специальную константу, если идентификатор не найден
	}
}