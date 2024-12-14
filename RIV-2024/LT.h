#pragma once
#define LEXEMA_FIXSIZE 2
#define	LT_MAXSIZE 4096
#define LT_TI_NULLIDX 0XFFFFFFFF
#define LEX_BYTE 't'
#define LEX_INT 't'
#define LEX_BOOL 't'
#define LEX_CH 't'
#define LEX_STRING 't'

#define LEX_ID 'i'
#define LEX_LITERAL 'l'
#define LEX_FUNCTION 'f'
#define LEX_DECLARE 'd'
#define LEX_RETURN 'r'
#define LEX_PRINT 'p'
#define LEX_MAIN 'm'
#define LEX_IF 's'
#define LEX_ELSE 'e'
#define LEX_SEMICOLON ';'
#define LEX_COMMA ','
#define LEX_LEFTBRACE '{'
#define LEX_RIGHTBRACE '}'
#define LEX_LEFTTHESIS '('
#define LEX_RIGHTTHESIS ')'
#define LEX_OPERATION 'v'
#define LEX_EQUAL '='
#define LEX_GREATER '>'
#define LEX_LESS '<'
#define LEX_GE 'g' //>=
#define LEX_LE 'u' //<=

namespace LT
{
	struct Entry
	{
		char lexema[LEXEMA_FIXSIZE];
		int sn;
		int idxTI;
	};
	struct lexTable
	{
		int maxsize;
		int size;
		Entry* table;
	};
	lexTable Create(int size);
	void Add(lexTable& lexTable, Entry entry);
	Entry GetEntry(lexTable& lexTable, int n);
	void Delete(lexTable& lexTable);
	void PrintLT(lexTable& lexTable);
}