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
		*out.stream << "module PROTO : BYTE \n\n";
		*out.stream << "rest PROTO : BYTE, : BYTE \n\n";
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
				case IT::INT: {
					*out.stream << " DWORD " << lex.idTable.table[i].value.vint << " ; integer (4 bytes)";
					break;
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

	void Expression(Out::OUT out, LA::LEX lex, int startPos, int endPos) {
		for (int i = startPos; i < endPos; i++) {
			switch (lex.lexTable.table[i].lexema[0]) {
			case LEX_OPERATION: {

				int operandsAmount = 1;
				int j = 0;
				while (lex.lexTable.table[i - operandsAmount - 1].lexema[0] == LEX_ID || lex.lexTable.table[i - operandsAmount - 1].lexema[0] == LEX_LITERAL) {
					operandsAmount++;
				}

				char operation = lex.lexTable.table[i].lexema[1];

				IT::Entry* leftOperand = nullptr;
				IT::Entry* rightOperand = nullptr;
				IT::Entry* result = nullptr;

				if (operandsAmount == 1) {
					leftOperand = &lex.idTable.table[lex.lexTable.table[i - operandsAmount].idxTI];
					rightOperand = &lex.idTable.table[lex.lexTable.table[i - operandsAmount + 1].idxTI];
					result = &lex.idTable.table[lex.lexTable.table[i - operandsAmount - 2].idxTI];
				}
				else if (operandsAmount > 1) {
					leftOperand = &lex.idTable.table[lex.lexTable.table[i - operandsAmount].idxTI];
					rightOperand = &lex.idTable.table[lex.lexTable.table[i - operandsAmount - 2].idxTI];
					result = &lex.idTable.table[lex.lexTable.table[i - operandsAmount - 4].idxTI];
				}

				switch (operation) {
				case PLUS: { 
					*out.stream << "; Addition\n";

					if (operandsAmount > 1) {
						if (leftOperand->iddatatype == IT::BYTE && rightOperand->iddatatype == IT::BYTE) {
							*out.stream << "mov al, " << leftOperand->id << "\n";
							*out.stream << "add al, " << rightOperand->id << "\n";
							*out.stream << "mov " << result->id << ", al\n";
						}
						else if (leftOperand->iddatatype == IT::INT && rightOperand->iddatatype == IT::INT) {
							*out.stream << "mov eax, " << leftOperand->id << "\n";
							*out.stream << "add eax, " << rightOperand->id << "\n";
							*out.stream << "mov " << result->id << ", eax\n";
						}
					}

					if (operandsAmount == 1) {

						j = operandsAmount;
						while (lex.lexTable.table[i - j - 1].lexema[0] != LEX_EQUAL) {
							j++;
						}

						if (leftOperand->iddatatype == IT::BYTE) {
							*out.stream << "add al, " << leftOperand->id << "\n";

							result = &lex.idTable.table[lex.lexTable.table[i - j - 2].idxTI];
							*out.stream << "mov " << result->id << ", al\n";
						}
						else if (leftOperand->iddatatype == IT::INT) {
							*out.stream << "add eax, " << leftOperand->id << "\n";
							*out.stream << "mov " << result->id << ", eax\n";
						}
					}
					break;
				}
				case MINUS: { 
					*out.stream << "; Subtraction\n";

					if (leftOperand->iddatatype == IT::BYTE && rightOperand->iddatatype == IT::BYTE) {
						*out.stream << "mov al, " << leftOperand->id << "\n";  
						*out.stream << "sub al, " << rightOperand->id << "\n"; 
						*out.stream << "mov " << result->id << ", al\n";       
					}
					else if (leftOperand->iddatatype == IT::INT && rightOperand->iddatatype == IT::INT) {
						*out.stream << "mov eax, " << leftOperand->id << "\n"; 
						*out.stream << "sub eax, " << rightOperand->id << "\n"; 
						*out.stream << "mov " << result->id << ", eax\n";   
					}

					break;
				}
				case STAR: { 
					*out.stream << "; Multiplication\n";

					if (leftOperand->iddatatype == IT::BYTE && rightOperand->iddatatype == IT::BYTE) {
						*out.stream << "movsx ax, " << leftOperand->id << "\n"; 
						*out.stream << "movsx bx, " << rightOperand->id << "\n"; 
						*out.stream << "imul eax, ebx\n";                         

						if (result->iddatatype == IT::BYTE) {
							*out.stream << "mov " << result->id << ", al\n";
						}
						else if (result->iddatatype == IT::INT) {
							*out.stream << "mov " << result->id << ", eax\n";
						}
					}
					else if (leftOperand->iddatatype == IT::INT && rightOperand->iddatatype == IT::INT) {
						*out.stream << "mov eax, " << leftOperand->id << "\n"; 
						*out.stream << "imul eax, " << rightOperand->id << "\n"; 
						*out.stream << "mov " << result->id << ", eax\n";
					}
					break;
				}
				
				case AMPERSAND: {
					*out.stream << "; Logical AND\n";

					if (leftOperand->iddatatype == IT::BYTE && rightOperand->iddatatype == IT::BYTE) {
						*out.stream << "mov al, " << leftOperand->id << "\n";  
						*out.stream << "and al, " << rightOperand->id << "\n"; 
						*out.stream << "mov " << result->id << ", al\n";      
					}
					else if (leftOperand->iddatatype == IT::INT && rightOperand->iddatatype == IT::INT) {
						*out.stream << "mov eax, " << leftOperand->id << "\n"; 
						*out.stream << "and eax, " << rightOperand->id << "\n"; 
						*out.stream << "mov " << result->id << ", eax\n";    
					}
					break;
				}
				case PIPE: { 
					*out.stream << "; Logical OR\n";

					if (leftOperand->iddatatype == IT::BYTE && rightOperand->iddatatype == IT::BYTE) {
						*out.stream << "mov al, " << leftOperand->id << "\n";  
						*out.stream << "or al, " << rightOperand->id << "\n";  
						*out.stream << "mov " << result->id << ", al\n";      
					}
					else if (leftOperand->iddatatype == IT::INT && rightOperand->iddatatype == IT::INT) {
						*out.stream << "mov eax, " << leftOperand->id << "\n";
						*out.stream << "or eax, " << rightOperand->id << "\n";  
						*out.stream << "mov " << result->id << ", eax\n";     
					}
					break;
				}
				default:
					*out.stream << "; Unsupported operation\n";
					break;
				}
				break;
			}
			case TILDE: { 
				*out.stream << "; Logical NOT\n";
				IT::Entry* operand = &lex.idTable.table[lex.lexTable.table[i - 1].idxTI];
				IT::Entry* result = &lex.idTable.table[lex.lexTable.table[i - 3].idxTI];

				if (operand->iddatatype == IT::BYTE) {
					*out.stream << "mov al, " << operand->id << "\n";    
					*out.stream << "not al\n";                       
					*out.stream << "mov " << result->id << ", al\n";  
				}
				else if (operand->iddatatype == IT::INT) {
					*out.stream << "mov eax, " << operand->id << "\n";   
					*out.stream << "not eax\n";                         
					*out.stream << "mov " << result->id << ", eax\n";   
				}
				break;
			}

			case LEX_EQUAL: {
				*out.stream << "\n" << "; string #" << lex.lexTable.table[i].sn << " : ";
				int parmsAmount = 0;
				for (int j = -1; lex.lexTable.table[i + j].lexema[0] != LEX_SEMICOLON; j++) {
					*out.stream << lex.lexTable.table[i + j].lexema[0];
					if (lex.lexTable.table[i + j].lexema[0] == '@') {
						parmsAmount = lex.lexTable.table[i + j + 1].lexema[0] - '0';
					}
				}

				*out.stream << "\n";
				bool isArgs = false;
				IT::Entry* func, * save = nullptr;
				IT::Entry* recipent = &lex.idTable.table[lex.lexTable.table[i - 1].idxTI];
				IT::Entry* sender = &lex.idTable.table[lex.lexTable.table[i + parmsAmount + 1].idxTI];

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
							*out.stream << "movzx eax, " << sender->id << "\n"; 
						}
						else {
							*out.stream << "mov eax, " << sender->id << "\n"; 
						}
						*out.stream << "mov " << recipent->id << ", eax\n";
						break;
					}

					case IT::STR: { 
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
						if (strcmp(sender->id, "rest") == 0) {
							*out.stream << "CALL rest" << "\n";
						}
						if (strcmp(sender->id, "module") == 0) {
							*out.stream << "CALL module" << "\n";
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
				bool hasElse = false;
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
					case LEX_COMPARISON:  {
						char data = lex.lexTable.table[i + cur].lexema[1]; 

						if (data == '<') {
							op = "jl"; 
						}
						else if (data == '>') {
							op = "jg"; 
						}

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
						break;
					}
					case LEX_RIGHTBRACE: {
						isEnd = true;

						if (lex.lexTable.table[i + cur + 1].lexema[0] == LEX_ELSE) {
							hasElse = true;
						}

						break;
					}
					}
					if (isEnd) {

						if (hasElse) {
							*out.stream << "jmp If_Else" << endl;
						}
						else {
							*out.stream << "jmp If_End" << currentIf + 1 << endl;
						}

						*out.stream << "If_End" << currentIf << ":\n";

						

						ifs += 2;
						Expression(out, lex, startPos, i + cur);
						i = i + cur;

						if (hasElse) {
							*out.stream << "jmp If_End" << currentIf + 1 << endl;
						}

						break;
					}
					cur++;
				}
				break;
			}

			case LEX_ELSE: { 
				int cur = 1;
				if (lex.lexTable.table[i + cur].lexema[0] == LEX_LEFTBRACE) {
					*out.stream << "If_Else:\n"; 
					cur++; 
					while (lex.lexTable.table[i + cur].lexema[0] != LEX_RIGHTBRACE) { 
						Expression(out, lex, i + cur, i + cur + 1); 
						cur++;
					}

				}
				i += cur;
				*out.stream << "If_End2" << ":\n";

				break;
			}

			case LEX_RETURN: {
				*out.stream << "\n; return\n";
				IT::Entry* returnValue = &lex.idTable.table[lex.lexTable.table[i + 1].idxTI];

				if (returnValue->iddatatype == IT::BYTE) {
					// Если тип BYTE
					if (returnValue->idtype == IT::L) {
						*out.stream << "movsx eax, " << returnValue->id << "\n"; 
					}
					else {
						*out.stream << "movsx eax, " << returnValue->id << "\n";
						*out.stream << "mov " << returnValue->id << ", al\n"; 
					}
				}
				else if (returnValue->iddatatype == IT::INT) {
					// Если тип INT
					*out.stream << "mov eax, " << returnValue->id << "\n";
				}
				else if (returnValue->iddatatype == IT::STR) {
					// Если тип STR
					if (returnValue->idtype == IT::L) {
						*out.stream << "mov eax, OFFSET " << returnValue->id << "\n"; 
					}
					else {
						*out.stream << "mov eax, " << returnValue->id << "\n"; 
					}
				}
				else {
				
					*out.stream << "movzx eax, " << returnValue->id << "\n";
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
					*out.stream << "\nmov eax, " << lex.idTable.table[lex.lexTable.table[i + 2].idxTI].id << "\n"; 
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
			if (lex.idTable.table[i].idtype != IT::F) continue;

			*out.stream << "\nF" << lex.idTable.table[i].id << " PROC uses ebx ecx edi esi";

			int cur = 1;
			int firstLexIdx = lex.idTable.table[i].idxfirstLE;

			while (lex.lexTable.table[firstLexIdx + cur].lexema[0] != LEX_RIGHTTHESIS) {
				auto& curLex = lex.lexTable.table[firstLexIdx + cur];

				if (curLex.lexema[0] == LEX_ID) {
					int idxTI = curLex.idxTI;
					auto& curId = lex.idTable.table[idxTI];

					if (curId.idtype == IT::P) {
						*out.stream << ", " << curId.id;
						switch (curId.iddatatype) {
						case IT::BYTE:
							*out.stream << " : SBYTE";
							break;
						case IT::STR:
							*out.stream << " : DWORD";
							break;
						case IT::CH:
						case IT::BOOL:
							*out.stream << " : BYTE";
							break;
						case IT::INT:
							*out.stream << " : DWORD";
							break;
						}
					}
				}
				cur++;
			}

			int startPos = firstLexIdx + cur;
			while (lex.lexTable.table[firstLexIdx + cur].lexema[0] != LEX_RETURN) {
				cur++;
			}
			int endPos = firstLexIdx + cur + 4; 

			Expression(out, lex, startPos, endPos);

			*out.stream << "ret\n";
			*out.stream << "F" << lex.idTable.table[i].id << " ENDP\n\n";
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


