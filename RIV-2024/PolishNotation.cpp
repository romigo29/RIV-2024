#include "PolishNotation.h"

map<char, int>Priorities = {
	{'(',0},
	{')',0},
	{'~',1},
	{'*',2},
	{'/',2},
	{'+',3},
	{'-',3},
	{'>',4},
	{'<',4},
	{'&',6},
	{'|',7},
	{',',8},
};


namespace PN {

	bool polishNotation(int i, LA::LEX& lex)
	{

		std::stack<LT::Entry> stack;
		std::queue<LT::Entry> queue;

		LT::Entry aggregate_symbol;
		aggregate_symbol.idxTI = -1;
		aggregate_symbol.lexema[0] = '#';
		aggregate_symbol.sn = lex.lexTable.table[i].sn;

		LT::Entry function_symbol;
		function_symbol.idxTI = LT_TI_NULLIDX;
		function_symbol.lexema[0] = '@';
		function_symbol.sn = lex.lexTable.table[i].sn;
		int idx;

		int lexem_counter = 0;
		int parm_counter = 0;
		int lexem_position = i;
		char* buf = new char[i];

		bool is_function = false;

		for (i; lex.lexTable.table[i].lexema[0] != LEX_SEMICOLON; i++, lexem_counter++) {
			switch (lex.lexTable.table[i].lexema[0]) {
			case LEX_ID:
			case LEX_LITERAL:
				if (lex.idTable.table[lex.lexTable.table[i].idxTI].idtype == IT::F) {
					is_function = true;
					idx = lex.lexTable.table[i].idxTI;
				}
				else {
					if (is_function)
						parm_counter++;
					queue.push(lex.lexTable.table[i]);
				}
				continue;

			case TILDE:
			case LEFTTHESIS:
				stack.push(lex.lexTable.table[i]);
				continue;

			case RIGHTTHESIS:
				while (stack.top().lexema[0] != LEFTTHESIS) {
					queue.push(stack.top());
					stack.pop();
					if (stack.empty())
						return false;
				}

				if (!is_function)
					stack.pop();
				else {
					function_symbol.idxTI = idx;
					idx = LT_TI_NULLIDX;
					lex.lexTable.table[i] = function_symbol;
					queue.push(lex.lexTable.table[i]);
					_itoa_s(parm_counter, buf, 2, 10);
					stack.top().lexema[0] = buf[0];
					stack.top().idxTI = LT_TI_NULLIDX;
					stack.top().sn = function_symbol.sn;
					queue.push(stack.top());
					stack.pop();
					parm_counter = 0;
					is_function = false;
				}
				continue;

			case LEX_OPERATION:

				while (!stack.empty() && Priorities[lex.lexTable.table[i].lexema[0]] > Priorities[stack.top().lexema[0]]) { //!!!!!!!!!!!!!!
					queue.push(stack.top());
					stack.pop();
				}
				stack.push(lex.lexTable.table[i]);
				continue;
			

			case LEX_REST:
			case LEX_MODULE:
				is_function = true;
				idx = lex.lexTable.table[i].idxTI;
				continue;
			}

		}

		while (!stack.empty()) {
			if (stack.top().lexema[0] == LEFTTHESIS || stack.top().lexema[0] == RIGHTTHESIS)
				return false;

			queue.push(stack.top());
			stack.pop();
		}

		while (lexem_counter != 0) {
			if (!queue.empty()) {
				lex.lexTable.table[lexem_position++] = queue.front();
				queue.pop();
			}
			else
				lex.lexTable.table[lexem_position++] = aggregate_symbol;

			lexem_counter--;
		}

		for (int i = 0; i < lexem_position; i++) {
			if (lex.lexTable.table[i].lexema[0] == LEX_OPERATION || lex.lexTable.table[i].lexema[0] == LEX_LITERAL)
				lex.idTable.table[lex.lexTable.table[i].idxTI].idxfirstLE = i;
		}

		return true;
	}
	
	bool startPolish(LA::LEX& lex)
	{
		bool result = false;
		for (int i = 0; i < lex.lexTable.size; i++) {
			if (lex.lexTable.table[i].lexema[0] == '=') {
				result = polishNotation(i + 1, lex);
				if (!result) {
					return false;
				}
			}
		}
		return true;
	}

}