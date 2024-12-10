//#include "stdafx.h"
//#include "LA.h"
//#include <stack>
//#include <queue>
//#include <sstream>
//#include <string>
//#include <iostream>
//#include <fstream>
//#include <cstdlib>
//#include <cstring>
//
//using namespace std;
//
//bool PolishNotation(int i, LT::LexTable& lextable, IT::IdTable& idtable)
//{
//	stack<LT::Entry> stack;		// стек для операций
//	queue<LT::Entry> queue;		// очередь для операндов
//	LT::Entry temp;
//	temp.idxTI = -1;
//	temp.lexema[0] = '#';
//	temp.sn = -1;
//	LT::Entry func;
//	func.lexema[0] = '@';
//	LT::Entry commas;
//	int countComma = 0;			//подсчет количества запятых
//	char* comma = new char[1] { "" };
//	int countLex = 0;			// количество преобразованных лексем
//	int posLex = i;				// запоминаем номер лексемы перед преобразованием
//	bool findFunc = false;
//
//
//	for (i; lextable.table[i].lexema[0] != LEX_SEMICOLON; i++, countLex++)
//	{
//		switch (lextable.table[i].lexema[0])
//		{
//		case LEX_ID:	// если идентификатор
//		{
//			queue.push(lextable.table[i]);   // помещаем в очередь
//			continue;
//		}
//		case LEX_LITERAL:		// если литерал
//		{
//			queue.push(lextable.table[i]);	// помещаем в очередь
//			continue;
//		}
//		case LEX_LEFTTHESIS:		// если (
//		{
//			if (idtable.table[lextable.table[i - 1].idxTI].idtype == IT::F)
//				findFunc = true;
//			stack.push(lextable.table[i]); // помещаем ее в стек
//			continue;
//		}
//		case LEX_RIGHTTHESIS:	// если )
//		{
//			if (findFunc)
//			{
//				itoa(++countComma, comma, 10); //количество запятых
//				strcpy(commas.lexema, comma);
//				stack.push(commas);
//				stack.push(func);
//				findFunc = false;
//			}
//			while (stack.top().lexema[0] != LEX_LEFTTHESIS)	// пока не встретим (
//			{
//				queue.push(stack.top());	// выталкиваем из стека в очередь
//				stack.pop();
//
//				if (stack.empty())
//					return false;
//			}
//			stack.pop();	// уничтожаем (
//			continue;
//		}
//		case LEX_OPERATION:	// если знак оператора
//		{
//			while (!stack.empty() && lextable.table[i].priority <= stack.top().priority)
//				// пока приоритет текущего оператора 
//				//меньше или равен приоритету оператора в вершине стека
//			{
//				queue.push(stack.top());	// выталкиваем со стека в выходную строку
//				stack.pop();
//			}
//			stack.push(lextable.table[i]);
//			continue;
//		}
//		case LEX_COMMA:
//		{
//			countComma++;
//			continue;
//		}
//		}
//	}
//	while (!stack.empty())	// если стек не пустой
//	{
//		if (stack.top().lexema[0] == LEX_LEFTTHESIS || stack.top().lexema[0] == LEX_RIGHTTHESIS)
//			return false;
//		queue.push(stack.top());	// выталкиваем все в очередь
//		stack.pop();
//	}
//	while (countLex != 0)		// замена текущего выражения в таблице лексем на выражение в ПОЛИЗ
//	{
//		if (!queue.empty()) {
//			lextable.table[posLex++] = queue.front();
//			//cout << lex.idtable.table[queue.front().idxTI].id;	// вывод в консоль
//			queue.pop();
//		}
//		else
//		{
//			lextable.table[posLex++] = temp;
//		}
//		countLex--;
//	}
//
//	for (int i = 0; i < posLex; i++)		// восстановление индексов первого вхождения 
//		//в таблицу лексем у операторов из таблицы идентификаторов
//	{
//		if (lextable.table[i].lexema[0] == LEX_OPERATION || lextable.table[i].lexema[0] == LEX_LITERAL)
//			idtable.table[lextable.table[i].idxTI].idxfirstLE = i;
//	}
//	return true;
//}
//
//
//void WritePolishNotationToFile(const char* filename, LT::LexTable& lt) {
//	ofstream outfile(filename);
//
//	if (!outfile.is_open()) {
//		cout << "Unable to open file for writing." << endl;
//		return;
//	}
//
//	int line = 1;
//	outfile << "\n";
//	outfile << "Таблица лексем:\n\n";
//	outfile << line << "  ";
//	for (int i = 0; i < lt.size; i++)
//	{
//		if (lt.table[i].lexema[0] == ';' || lt.table[i].lexema[0] == '[')
//		{
//			outfile << lt.table[i].lexema[0];
//			line++;
//			outfile << '\n';
//			outfile << line << "  ";
//			continue;
//		}
//
//		outfile << lt.table[i].lexema[0];
//	}
//
//	outfile.close();
//}
