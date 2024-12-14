#pragma once
#include "GRB.h"
#include"Error.h"
#define GRB_ERROR_SERIES 600
#define NS(n) GRB::Rule::Chain::N(n)
#define TS(n) GRB::Rule::Chain::T(n)
#define ISNS(n)	GRB::Rule::Chain::isN(n)

namespace GRB
{
	Greibach greibach(NS('S'), TS('$'),				//стартовый символ, дно стека
		6,											//количество правил
		Rule(NS('S'), GRB_ERROR_SERIES + 0,
			3,
			Rule::Chain(4, TS('m'), TS('{'), NS('N'), TS('}')),                        // Основной случай
			Rule::Chain(6, TS('m'), TS('{'), NS('N'), TS('}'), TS(';'), NS('S')),      // Часто встречающаяся структура
			Rule::Chain(11, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('}'), TS(';'), NS('S')) // Сложная структура
		),
		Rule(NS('N'), GRB_ERROR_SERIES + 1,
			16,
			Rule::Chain(4, TS('d'), TS('t'), TS('i'), TS(';')),
			Rule::Chain(5, TS('d'), TS('t'), TS('i'), TS(';'), NS('N')),                         // Объявление переменной
			Rule::Chain(7, TS('d'), TS('t'), TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(3, TS('r'), NS('E'), TS(';')),
			Rule::Chain(4, TS('r'), NS('E'), TS(';'), NS('N')),                                  // Возврат
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),                         // Присваивание
			Rule::Chain(4, TS('i'), TS('v'), TS('i'), NS('N')),
			Rule::Chain(3, TS('p'), NS('E'), TS(';')),                                  // Печать
			Rule::Chain(8, TS('d'), TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS(';')), // Объявление функции
			Rule::Chain(5, TS('d'), TS('t'), TS('i'), TS(';'), NS('N')),                // Объявление с продолжением                        // Возврат с продолжением
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),                // Присваивание с продолжением
			Rule::Chain(4, TS('p'), NS('E'), TS(';'), NS('N')),                         // Печать с продолжением
			Rule::Chain(7, TS('s'), TS('('), NS('E'), TS(')'), TS('{'), NS('N'), TS('}')), // Условие
			Rule::Chain(8, TS('s'), TS('('), NS('E'), TS(')'), TS('{'), NS('N'), TS('}'), NS('N')), // Условие с продолжением
			Rule::Chain(11, TS('s'), TS('('), NS('E'), TS(')'), TS('{'), NS('N'), TS('}'), TS('e'), TS('{'), NS('N'), TS('}')), // Условие с else
			Rule::Chain(13, TS('s'), TS('('), NS('E'), TS(')'), TS('{'), NS('N'), TS('}'), NS('N'), TS('e'), TS('{'), NS('N'), TS('}'), NS('N')) // Сложное условие
		),
		Rule(NS('E'), GRB_ERROR_SERIES + 2,
			13,
			Rule::Chain(1, TS('i')),                                                   // Переменная
			Rule::Chain(1, TS('l')),                                                   // Литерал
			Rule::Chain(3, TS('('), NS('E'), TS(')')),                                 // Выражение в скобках
			Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')')),                        // Вызов функции
			Rule::Chain(3, TS('i'), TS('('), TS(')')),                                 // Вызов функции без параметров
			Rule::Chain(2, TS('i'), NS('M')),                                          // Операция с переменной
			Rule::Chain(2, TS('l'), NS('M')),                                          // Операция с литералом
			Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')),                        // Операция с выражением
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), NS('M')),               // Вызов функции с операцией
			Rule::Chain(3, TS('i'), TS('v'), TS('i')),
			Rule::Chain(2, TS('~'), NS('E')),                                          // Логическое отрицание
			Rule::Chain(4, NS('E'), TS('v'), NS('E')),                                 // Логическая операция
			Rule::Chain(5, TS('('), NS('E'), TS(')'), TS('v'), NS('E'))                // Операция между выражениями
		),
		Rule(NS('W'), GRB_ERROR_SERIES + 3,			//Подвыражение
			4,		//W	→i|l|i,W|l,W
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('i'), TS(','), NS('W')),
			Rule::Chain(3, TS('l'), TS(','), NS('W'))
		),
		Rule(NS('F'), GRB_ERROR_SERIES + 4,			//Параметры функции
			2,		//F	→ti|ti,F
			Rule::Chain(2, TS('t'), TS('i')),
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('F'))
		),
		Rule(NS('M'), GRB_ERROR_SERIES + 2,			//Арифметические и логические операции
			6,		//M →vE|vEM
			Rule::Chain(2, TS('>'), NS('E')),
			Rule::Chain(2, TS('<'), NS('E')),
			Rule::Chain(2, TS('v'), NS('E')),
			Rule::Chain(3, TS('v'), NS('E'), NS('M')),
			Rule::Chain(4, TS('('), NS('E'), TS(')'), TS('v')),
			Rule::Chain(6, TS('('), NS('E'), TS(')'), TS('v'), TS('('), NS('E'), TS(')'))
		)
	);
}
