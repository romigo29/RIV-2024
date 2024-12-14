#include "CodeGeneration.h"

namespace CodeGeneration {

	static int ifs = 1;

	void Head(Out::OUT out) {
		*out.stream << ".586P\n";
		*out.stream << ".model flat, stdcall\n";
		*out.stream << "includelib libucrt.lib\n";
		*out.stream << "includelib kernel32.lib\n";
		*out.stream << "includelib ../Debug/LIB.lib\n";
		*out.stream << "ExitProcess PROTO : DWORD\n\n";
		*out.stream << "SetConsoleCP PROTO : DWORD\n\n";
		*out.stream << "SetConsoleOutputCP PROTO : DWORD\n\n";
		*out.stream << "Pow PROTO : BYTE, : BYTE \n\n";
		*out.stream << "Sum PROTO : BYTE, : BYTE \n\n";
		*out.stream << "writestr PROTO : DWORD \n\n";
		*out.stream << "writeint PROTO : BYTE \n\n";
		*out.stream << "writebool PROTO : BYTE \n\n";
		*out.stream << "writechar PROTO : BYTE \n\n";
		*out.stream << ".stack 4096\n\n";
	}

	void Const(Out::OUT out, LA::LEX lex) {
		*out.stream << ".const\n";
		for (int i = 0; i < lex.idTable.size; i++) {
			if (lex.idTable.table[i].idtype == IT::L) {
				*out.stream << lex.idTable.table[i].id;
				switch (lex.idTable.table[i].iddatatype)
				{
				case IT::BYTE: {
					*out.stream << " SBYTE " << (int)lex.idTable.table[i].value.vbyte << " ; byte";
					break;
				}
				case IT::CH: {
					*out.stream << " BYTE " << (int)lex.idTable.table[i].value.vchar << " ; symbol";
					break;
				}
				case IT::STR: {
					*out.stream << " DB " << lex.idTable.table[i].value.vstr->str << ", 0 ; text";
					break;
				}
				case IT::BOOL: {
					*out.stream << " BYTE " << lex.idTable.table[i].value.vbool << " ; boolean";
				}
				}
				*out.stream << "\n";
			}
		}
		*out.stream << "\n";
	}

	void Data(Out::OUT out, LA::LEX lex) {
		*out.stream << ".data\n";
		for (int i = 0; i < lex.idTable.size; i++) {
			if (lex.idTable.table[i].idtype == IT::V) {
				*out.stream << lex.idTable.table[i].id;
				switch (lex.idTable.table[i].iddatatype) {
				case IT::BYTE: {
					*out.stream << " SBYTE 0 ; byte";
					break;
				}

				case IT::INT: {
					*out.stream << " SDWORD 0 ; int";
					break;

				}
				case IT::STR: {
					*out.stream << " DWORD 0 ; str";
					break;
				}
				case IT::CH: {
					*out.stream << " BYTE 0 ; ch";
					break;
				}
				case IT::BOOL: {
					*out.stream << " BYTE 0 ; boolean";
					break;
				}
				}
				*out.stream << "\n";
			}
		}
		*out.stream << "\n";
	}

	bool compareVectorToCString(const std::vector<char>& vec, const char* cstr) {
		if (vec.size() != std::strlen(cstr)) {
			return false;
		}

		for (size_t i = 0; i < vec.size(); ++i) {
			if (vec[i] != cstr[i]) {
				return false;
			}
		}

		return true;
	}

	void Expression(Out::OUT out, LA::LEX lex, int startPos, int endPos) {
		for (int i = startPos; i < endPos; i++) {
			switch (lex.lexTable.table[i].lexema[0]) {
			case LEX_OPERATION: {
				char operation = lex.lexTable.table[i].lexema[1];
				IT::Entry* leftOperand = &lex.idTable.table[lex.lexTable.table[i - 2].idxTI];
				IT::Entry* rightOperand = &lex.idTable.table[lex.lexTable.table[i - 1].idxTI];
				IT::Entry* result = &lex.idTable.table[lex.lexTable.table[i - 4].idxTI];

				switch (operation) {
				case PLUS: { // Сложение
					*out.stream << "; Addition\n";

					if (leftOperand->iddatatype == IT::BYTE && rightOperand->iddatatype == IT::BYTE) {
						*out.stream << "mov al, " << leftOperand->id << "\n";  // Загружаем первый операнд (BYTE)
						*out.stream << "add al, " << rightOperand->id << "\n"; // Складываем с правым операндом
						*out.stream << "mov " << result->id << ", al\n";       // Сохраняем результат
					}
					else if (leftOperand->iddatatype == IT::INT && rightOperand->iddatatype == IT::INT) {
						*out.stream << "mov eax, " << leftOperand->id << "\n"; // Загружаем первый операнд (INT)
						*out.stream << "add eax, " << rightOperand->id << "\n"; // Складываем с правым операндом
						*out.stream << "mov " << result->id << ", eax\n";      // Сохраняем результат
					}
					break;
				}
				case MINUS: { // Вычитание
					*out.stream << "; Subtraction\n";

					if (leftOperand->iddatatype == IT::BYTE && rightOperand->iddatatype == IT::BYTE) {
						*out.stream << "mov al, " << leftOperand->id << "\n";  // Загружаем первый операнд (BYTE)
						*out.stream << "sub al, " << rightOperand->id << "\n"; // Вычитаем правый операнд
						*out.stream << "mov " << result->id << ", al\n";       // Сохраняем результат
					}
					else if (leftOperand->iddatatype == IT::INT && rightOperand->iddatatype == IT::INT) {
						*out.stream << "mov eax, " << leftOperand->id << "\n"; // Загружаем первый операнд (INT)
						*out.stream << "sub eax, " << rightOperand->id << "\n"; // Вычитаем правый операнд
						*out.stream << "mov " << result->id << ", eax\n";      // Сохраняем результат
					}
					break;
				}
				case STAR: { // Умножение
					*out.stream << "; Multiplication\n";

					if (leftOperand->iddatatype == IT::BYTE && rightOperand->iddatatype == IT::BYTE) {
						*out.stream << "movsx ax, " << leftOperand->id << "\n"; // Расширение num1 -> ax
						*out.stream << "movsx bx, " << rightOperand->id << "\n"; // Расширение num2 -> bx
						*out.stream << "imul ax, bx\n";                         // Умножение со знаком в 16 битах

						if (result->iddatatype == IT::BYTE) {
							*out.stream << "mov " << result->id << ", al\n"; // Сохраняем младший байт результата
						}
						else if (result->iddatatype == IT::INT) {
							*out.stream << "mov " << result->id << ", eax\n"; // Сохраняем полный результат
						}
					}
					else if (leftOperand->iddatatype == IT::INT && rightOperand->iddatatype == IT::INT) {
						*out.stream << "mov eax, " << leftOperand->id << "\n"; // Загрузка num1 в eax
						*out.stream << "imul eax, " << rightOperand->id << "\n"; // Умножение int
						*out.stream << "mov " << result->id << ", eax\n"; // Сохранение результата
					}
					break;
				}
				case DIRSLASH: { // Деление
					*out.stream << "; Division\n";

					if (leftOperand->iddatatype == IT::BYTE && rightOperand->iddatatype == IT::BYTE) {
						*out.stream << "movsx ax, " << leftOperand->id << "\n"; // Расширение num1 -> ax
						*out.stream << "movsx bx, " << rightOperand->id << "\n"; // Расширение num2 -> bx

						// Деление без проверки на ноль
						*out.stream << "idiv bl\n"; // Деление со знаком (8-бит)

						if (result->iddatatype == IT::BYTE) {
							*out.stream << "mov " << result->id << ", al\n"; // Сохраняем результат (младший байт)
						}
						else if (result->iddatatype == IT::INT) {
							*out.stream << "mov " << result->id << ", eax\n"; // Сохраняем полный результат
						}
					}
					else if (leftOperand->iddatatype == IT::INT && rightOperand->iddatatype == IT::INT) {
						*out.stream << "mov eax, " << leftOperand->id << "\n"; // Загрузка num1 в eax
						*out.stream << "cdq\n";                              // Расширение знака в edx:eax
						*out.stream << "mov ecx, " << rightOperand->id << "\n"; // Загрузка num2 в ecx

						// Деление без проверки на ноль
						*out.stream << "idiv ecx\n"; // Деление со знаком
						*out.stream << "mov " << result->id << ", eax\n"; // Сохранение результата
					}
					break;
				}
				default:
					*out.stream << "; Unsupported operation\n";
					break;
				}
				break;
			}

			case LEX_EQUAL: {
				*out.stream << "\n" << "; string #" << lex.lexTable.table[i].sn << " : ";
				for (int j = -1; lex.lexTable.table[i + j].lexema[0] != LEX_SEMICOLON; j++) {
					*out.stream << lex.lexTable.table[i + j].lexema[0];
				}
				*out.stream << "\n";
				bool isArgs = false;
				IT::Entry* func, * save = nullptr;
				IT::Entry* recipent = &lex.idTable.table[lex.lexTable.table[i - 1].idxTI];
				IT::Entry* sender = &lex.idTable.table[lex.lexTable.table[i + 3].idxTI];
				if (sender->idtype != IT::F && sender->idtype != IT::SF) {
					switch (recipent->iddatatype) {
					case IT::BYTE:
					case IT::CH:
					case IT::BOOL:
					{
						*out.stream << "mov al, " << sender->id << "\n";
						*out.stream << "mov " << recipent->id << ", al \n";
						break;
					}

					case IT::INT: {

						if (sender->iddatatype == IT::BYTE) {
							*out.stream << "movzx eax, " << sender->id << "\n"; // Расширение BYTE до INT (незнаковое)
						}
						else {
							*out.stream << "mov eax, " << sender->id << "\n"; // Обычное присваивание
						}
						*out.stream << "mov " << recipent->id << ", eax\n";
						break;
					}

					case IT::STR: { // для текста разное присваение
						if (sender->idtype == IT::L) {
							*out.stream << "push offset " << sender->id << '\n';
							*out.stream << "pop eax\n";
							*out.stream << "mov " << recipent->id << ", eax \n";
						}
						else {
							*out.stream << "mov esi, " << "offset " << sender->id << "\n";
							*out.stream << "mov edi, " << "offset " << recipent->id << "\n\n";
							*out.stream << "copy_loop: \n";
							*out.stream << "lodsd \n";
							*out.stream << "stosd \n";
							*out.stream << "cmp eax, 0 \n";
							*out.stream << "jnz copy_loop \n";
						}
						break;
					}
					}
				}
				else {
					stack<IT::Entry> stackForParams;
					int iterator = 1;
					while (lex.lexTable.table[i + iterator].lexema[0] != '@') {
						stackForParams.push(lex.idTable.table[lex.lexTable.table[i + iterator].idxTI]);
						iterator += 1;
					}
					while (!stackForParams.empty()) {
						switch (stackForParams.top().iddatatype) {
						case IT::BYTE:
						case IT::BOOL:
						case IT::CH: {
							*out.stream << "movsx eax, " << stackForParams.top().id << "\n";
							*out.stream << "push eax\n";
							break;
						}

						case IT::INT: {
							*out.stream << "mov eax, " << stackForParams.top().id << "\n";
							*out.stream << "push eax\n";
							break;
						}
						case IT::STR: {
							if (stackForParams.top().idtype == IT::L) {
								*out.stream << "push OFFSET " << stackForParams.top().id << "\n";
							}
							else {
								*out.stream << "push " << stackForParams.top().id << "\n";
							}
							break;
						}
						}
						stackForParams.pop();
					}
					if (sender->idtype == IT::F) {
						*out.stream << "CALL F" << sender->id << "\n";
					}
					else {
						if (strcmp(sender->id, "Pow") == 0) {
							*out.stream << "CALL Pow" << "\n";
						}
						if (strcmp(sender->id, "Sum") == 0) {
							*out.stream << "CALL Sum" << "\n";
						}
					}
					if (sender->iddatatype == IT::BYTE || sender->iddatatype == IT::BOOL || sender->iddatatype == IT::CH) {
						*out.stream << "mov " << recipent->id << ", al\n";
					}
					else {
						*out.stream << "mov " << recipent->id << ", eax\n";
					}
				}
				break;
			}
			case LEX_IF: {
				int cur = 1;
				int startPos;
				bool isEnd = false;
				IT::Entry* first = nullptr, * second = nullptr;
				std::string op = "";
				int currentIf = ifs;
				ifs++;
				*out.stream << "\nIf_Begin" << currentIf << ": \n";

				while (true) {
					switch (lex.lexTable.table[i + cur].lexema[0]) {
					case LEX_RIGHTTHESIS: {
						startPos = i + 1;
						break;
					}
					case LEX_ID: {
						if (first == nullptr) {
							first = &lex.idTable.table[lex.lexTable.table[i + cur].idxTI];
						}
						else {
							second = &lex.idTable.table[lex.lexTable.table[i + cur].idxTI];
						}
						break;
					}
					case LEX_LITERAL: {
						if (first == nullptr) {
							first = &lex.idTable.table[lex.lexTable.table[i + cur].idxTI];
						}
						else {
							second = &lex.idTable.table[lex.lexTable.table[i + cur].idxTI];
						}
						break;
					}
					case LEX_GREATER: {
						std::string data = lex.lexTable.table[i + cur].lexema;

						if (data == "==") op = "je";
						else if (data == "!=") op = "jne";
						else if (data == "<=") op = "jle";
						else if (data == ">=") op = "jge";
						else if (data == ">") op = "jg";
						else if (data == "<") op = "jl";

						break;
					}
					case LEX_LEFTBRACE: {
						if (second == nullptr) {
							second = new IT::Entry;
							if (first->iddatatype == IT::CH || first->iddatatype == IT::BOOL || first->iddatatype == IT::BYTE) {
								*out.stream << "movzx eax, " << first->id << "\n";
							}
							else if (first->iddatatype == IT::INT) {
								*out.stream << "mov eax, " << first->id << "\n";
							}
							*out.stream << "mov ebx, 1\n";
							*out.stream << "cmp eax, ebx\n";
							*out.stream << "je If_End" << currentIf << "\n";
							*out.stream << "jmp If_End" << ifs << "\n";
							break;
						}

						if (first->iddatatype == IT::CH || first->iddatatype == IT::BOOL || first->iddatatype == IT::BYTE) {
							*out.stream << "movzx eax, " << first->id << "\n";
						}
						else if (first->iddatatype == IT::INT) {
							*out.stream << "mov eax, " << first->id << "\n";
						}
						if (second->iddatatype == IT::CH || second->iddatatype == IT::BOOL || second->iddatatype == IT::BYTE) {
							*out.stream << "movzx ebx, " << second->id << "\n";
						}
						else if (second->iddatatype == IT::INT) {
							*out.stream << "mov ebx, " << second->id << "\n";
						}
						*out.stream << "cmp eax, ebx\n";
						*out.stream << op.c_str() << " If_End" << currentIf << "\n";
						*out.stream << "jmp If_End" << ifs << "\n";
						break;
					}
					case LEX_RIGHTBRACE: {
						isEnd = true;
						break;
					}
					}
					if (isEnd) {
						*out.stream << "If_End" << currentIf << ":\n";
						ifs += 2;
						Expression(out, lex, startPos, i + cur);
						i = i + cur;
						break;
					}
					cur++;
				}
				*out.stream << "If_End" << currentIf + 1 << ":\n";
				break;
			}


			case LEX_RETURN: {
				*out.stream << "\n; return\n";
				IT::Entry* returnValue = &lex.idTable.table[lex.lexTable.table[i + 1].idxTI];

				if (returnValue->iddatatype == IT::BYTE) {
					// Если тип BYTE
					if (returnValue->idtype == IT::L) {
						*out.stream << "movsx eax, " << returnValue->id << "\n"; // Знаковое расширение BYTE в EAX
					}
					else {
						*out.stream << "movsx eax, " << returnValue->id << "\n";
						*out.stream << "mov " << returnValue->id << ", al\n"; // Сохраняем в младший байт AL
					}
				}
				else if (returnValue->iddatatype == IT::INT) {
					// Если тип INT
					*out.stream << "mov eax, " << returnValue->id << "\n"; // Просто загружаем значение в EAX
				}
				else if (returnValue->iddatatype == IT::STR) {
					// Если тип STR
					if (returnValue->idtype == IT::L) {
						*out.stream << "mov eax, OFFSET " << returnValue->id << "\n"; // Указатель на строку
					}
					else {
						*out.stream << "mov eax, " << returnValue->id << "\n"; // Сохраняем адрес строки
					}
				}
				else {
					// Для остальных типов: предполагаем unsigned
					*out.stream << "movzx eax, " << returnValue->id << "\n"; // Беззнаковое расширение
				}
				break;
			}
			case LEX_PRINT: {
				switch (lex.idTable.table[lex.lexTable.table[i + 2].idxTI].iddatatype)
				{
				case (IT::CH): {
					*out.stream << "push eax\n";
					*out.stream << "movzx eax, " << lex.idTable.table[lex.lexTable.table[i + 2].idxTI].id << '\n';
					*out.stream << "push eax\n";
					*out.stream << "CALL writechar" << '\n';
					*out.stream << "pop eax\n" << '\n';
					break;
				}
				case (IT::STR): {
					*out.stream << "\npush ";
					if (lex.idTable.table[lex.lexTable.table[i + 2].idxTI].idtype == IT::L)
						*out.stream << "offset " << lex.idTable.table[lex.lexTable.table[i + 2].idxTI].id << '\n';
					else
						*out.stream << lex.idTable.table[lex.lexTable.table[i + 2].idxTI].id << '\n';

					*out.stream << "CALL writestr" << '\n';
					break;
				}
				case (IT::INT): {
					*out.stream << "\nmov eax, " << lex.idTable.table[lex.lexTable.table[i + 2].idxTI].id << "\n"; // Прямое перемещение
					*out.stream << "push eax\n";
					*out.stream << "CALL writeint" << '\n';
					break;
				}
				case (IT::BYTE): {
					*out.stream << "\nmovsx eax, " << lex.idTable.table[lex.lexTable.table[i + 2].idxTI].id << "\n";
					*out.stream << "push eax\n";
					*out.stream << "CALL writeint" << '\n';
					break;
				}
				case (IT::BOOL): {
					*out.stream << "push eax\n";
					*out.stream << "movzx eax, " << lex.idTable.table[lex.lexTable.table[i + 2].idxTI].id << '\n';
					*out.stream << "push eax\n";
					*out.stream << "CALL writebool" << '\n';
					*out.stream << "pop eax\n" << '\n';
					break;
				}
				}
				break;
			}
			case LEX_ID: {
				if ((lex.lexTable.table[i - 1].lexema[0] == LEX_SEMICOLON || lex.lexTable.table[i - 1].lexema[0] == LEX_LEFTBRACE || lex.lexTable.table[i - 1].lexema[0] == LEX_RIGHTBRACE) &&
					lex.lexTable.table[i + 1].lexema[0] != LEX_EQUAL) {
					stack<IT::Entry> stackForParams;
					IT::Entry* function = &lex.idTable.table[lex.lexTable.table[i].idxTI];
					int iterator = 1;
					while (lex.lexTable.table[i + iterator].lexema[0] != '@') {
						stackForParams.push(lex.idTable.table[lex.lexTable.table[i + iterator].idxTI]);
						iterator += 1;
					}
					while (!stackForParams.empty()) {
						switch (stackForParams.top().iddatatype) {
						case IT::BYTE:
						case IT::BOOL:
						case IT::CH: {
							*out.stream << "movsx eax, " << stackForParams.top().id << "\n";
							*out.stream << "push eax\n";
							break;
						}
						case IT::STR: {
							if (stackForParams.top().idtype == IT::L) {
								*out.stream << "push OFFSET " << stackForParams.top().id << "\n";
							}
							else {
								*out.stream << "push " << stackForParams.top().id << "\n";
							}
							break;
						}

						case IT::INT: {
							*out.stream << "mov eax, " << stackForParams.top().id << "\n"; // Прямое перемещение INT
							*out.stream << "push eax\n"; // Поместить значение на стек
							break;
						}
						}
						stackForParams.pop();
					}
					*out.stream << "CALL F" << function->id << "\n";
				}
				break;
				}
			}
		}
	}

	void Functions(Out::OUT out, LA::LEX lex) {
		for (int i = 0; i < lex.idTable.size; i++) {
			if (lex.idTable.table[i].idtype == IT::F && lex.idTable.table[i].id != "main") {
				*out.stream << "\nF" << lex.idTable.table[i].id << " PROC uses ebx ecx edi esi";
				int cur = 1;
				while (lex.lexTable.table[lex.idTable.table[i].idxfirstLE + cur].lexema[0] != LEX_RIGHTTHESIS) {
					if (lex.lexTable.table[lex.idTable.table[i].idxfirstLE + cur].lexema[0] == LEX_ID
						&& lex.idTable.table[lex.lexTable.table[lex.idTable.table[i].idxfirstLE + cur].idxTI].idtype == IT::P) {
						*out.stream << ", " << lex.idTable.table[lex.lexTable.table[lex.idTable.table[i].idxfirstLE + cur].idxTI].id;
						switch (lex.idTable.table[lex.lexTable.table[lex.idTable.table[i].idxfirstLE + cur].idxTI].iddatatype)
						{
						case IT::BYTE: {
							*out.stream << " : SBYTE";
							break;
						}
						case IT::STR: {
							*out.stream << " : DWORD";
							break;
						}
						case IT::CH:
						case IT::BOOL: {
							*out.stream << " : BYTE";
							break;
						}
						}
					}
					cur++;
				}
				int startPos = lex.idTable.table[i].idxfirstLE + cur;
				while (lex.lexTable.table[lex.idTable.table[i].idxfirstLE + cur].lexema[0] != LEX_RETURN) {
					cur++;
				}
				cur += 4;

				int endPos = lex.idTable.table[i].idxfirstLE + cur;
				Expression(out, lex, startPos, endPos);
				*out.stream << "ret\n";
				*out.stream << "F" << lex.idTable.table[i].id << " ENDP\n\n";
			}
		}
	}

	void Code(Out::OUT out, LA::LEX lex) {
		*out.stream << ".code\n";
		Functions(out, lex);
		*out.stream << "main PROC\n";
		*out.stream << "Invoke SetConsoleCP, 1251\n";
		*out.stream << "Invoke SetConsoleOutputCP, 1251\n";
		int mainPos = 0;
		int endPos = 0;
		for (int i = 0; i < lex.lexTable.size; i++) {
			if (lex.lexTable.table[i].lexema[0] == LEX_MAIN) {
				mainPos = i;
				break;
			}
		}
		endPos = lex.lexTable.size;
		Expression(out, lex, mainPos, endPos);
		*out.stream << "push -1\n";
		*out.stream << "call ExitProcess\n";
		*out.stream << "main ENDP\n";
		*out.stream << "end main\n";

	}

	void GenerateCode(LA::LEX lex, Out::OUT out) {
		Head(out);
		Const(out, lex);
		Data(out, lex);
		Code(out, lex);
	}

}


