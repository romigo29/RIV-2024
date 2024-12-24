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
		*out.stream << "module PROTO : DWORD \n\n";
		*out.stream << "rest PROTO : DWORD, : BYTE \n\n";
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
				case IT::CH: {
					*out.stream << " BYTE " << (int)lex.idTable.table[i].value.vchar << " ; ch";
					break;
				}
				case IT::STR: {
					*out.stream << " DB " << lex.idTable.table[i].value.vstr->str << ", 0 ; str";
					break;
				}
				case IT::BOOL: {
					*out.stream << " BYTE " << lex.idTable.table[i].value.vbool << " ; bool";
				}
				case IT::BYTE:
				case IT::INT: {
					*out.stream << " DWORD " << lex.idTable.table[i].value.vint << " ; int (4 bytes)";
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
				case IT::BYTE:
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

				char operation = lex.lexTable.table[i].lexema[1];

				IT::Entry* leftOperand = nullptr;
				IT::Entry* rightOperand = nullptr;
				IT::Entry* result = nullptr;
				int iterator = 1;
				while (!leftOperand || !rightOperand || !result) {
					if (i - iterator < 0) {
						break;
					}

					// Получение текущей лексемы
					const auto& currentLex = lex.lexTable.table[i - iterator];
					if (!leftOperand && currentLex.lexema[0] != ';') {
						leftOperand = &lex.idTable.table[currentLex.idxTI];
					}
					else if (!rightOperand && (i - iterator - 1) >= 0) {
						const auto& nextLex = lex.lexTable.table[i - iterator - 1];
						if (nextLex.lexema[0] != ';') {
							rightOperand = &lex.idTable.table[nextLex.idxTI];
						}
					}
					else if (!result && (i - iterator - 2) >= 0) {
						const auto& resultLex = lex.lexTable.table[i - iterator - 2];
						if (resultLex.lexema[0] != ';') {
							result = &lex.idTable.table[resultLex.idxTI];
						}
					}
					iterator++;
				}

				switch (operation) {
				case PLUS: { 
					*out.stream << "; Addition\n";

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

				int statrt = i + 1;

				while (i < lex.lexTable.size && lex.lexTable.table[i].lexema[0] != LEX_SEMICOLON) i++;
				*out.stream << ";aboba\n";

				if (statrt == i - 1)
				{

					if (lex.idTable.table[lex.lexTable.table[statrt].idxTI].iddatatype == IT::INT ||
						lex.idTable.table[lex.lexTable.table[statrt].idxTI].iddatatype ==IT::BYTE)
					{
						*out.stream << "mov eax, " << lex.idTable.table[lex.lexTable.table[statrt].idxTI].id << "\n";
		
					}
					else if (lex.idTable.table[lex.lexTable.table[statrt].idxTI].iddatatype == IT::STR)
					{
						*out.stream << "mov eax, OFFSET " << lex.idTable.table[lex.lexTable.table[statrt].idxTI].id << "\n";
					}
					else {
						*out.stream << "movsx eax, " << lex.idTable.table[lex.lexTable.table[statrt].idxTI].id << "\n";
					}

					if (lex.idTable.table[lex.lexTable.table[statrt - 1].idxTI].iddatatype == IT::INT ||
						lex.idTable.table[lex.lexTable.table[statrt - 1].idxTI].iddatatype == IT::BYTE)
					{
						*out.stream << "mov " << lex.idTable.table[lex.lexTable.table[statrt - 1].idxTI].id << ", eax\n";

					}

					else if (lex.idTable.table[lex.lexTable.table[statrt - 1].idxTI].iddatatype == IT::STR) {
						*out.stream << "mov [" << lex.idTable.table[lex.lexTable.table[statrt - 1].idxTI].id << "], eax\n";
					}

					else {
						*out.stream << "mov " << lex.idTable.table[lex.lexTable.table[statrt - 1].idxTI].id << ", al\n";
					}
					break;
				}

				ExpressionHandler(out.stream, lex, statrt, i - 1);
				*out.stream << "mov " << lex.idTable.table[lex.lexTable.table[statrt - 2].idxTI].id << ", eax\n";

				
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
							if (first->iddatatype == IT::CH || first->iddatatype == IT::BOOL) {
								*out.stream << "movzx eax, " << first->id << "\n";
							}
							else if (first->iddatatype == IT::BYTE || first->iddatatype == IT::INT) {
								*out.stream << "mov eax, " << first->id << "\n";
							}
							*out.stream << "mov ebx, 1\n";
							*out.stream << "cmp eax, ebx\n";
							*out.stream << "je If_End" << currentIf << "\n";
							*out.stream << "jmp If_End" << ifs << "\n";
							break;
						}

						if (first->iddatatype == IT::CH || first->iddatatype == IT::BOOL ) {
							*out.stream << "movzx eax, " << first->id << "\n";
						}
						else if (first->iddatatype == IT::INT || first->iddatatype == IT::BYTE) {
							*out.stream << "mov eax, " << first->id << "\n";
						}
						if (second->iddatatype == IT::CH || second->iddatatype == IT::BOOL) {
							*out.stream << "movzx ebx, " << second->id << "\n";
						}
						else if (second->iddatatype == IT::INT || second->iddatatype == IT::BYTE) {
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
			
					if (returnValue->idtype == IT::L) {
						*out.stream << "movsx eax, " << returnValue->id << "\n"; 
					}
					else {
						*out.stream << "movsx eax, " << returnValue->id << "\n";
						*out.stream << "mov " << returnValue->id << ", al\n"; 
					}
				}
				if (returnValue->iddatatype == IT::INT ) {
	
					*out.stream << "mov eax, " << returnValue->id << "\n";
				}
				else if (returnValue->iddatatype == IT::STR) {
					
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
				case IT::BYTE:
				case IT::INT: {
					*out.stream << "\nmov eax, " << lex.idTable.table[lex.lexTable.table[i + 2].idxTI].id << "\n"; 
					*out.stream << "push eax\n";
					*out.stream << "CALL writeint" << '\n';
					*out.stream << "pop eax\n";
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
						case IT::BYTE:
						case IT::INT: {
							*out.stream << "mov eax, " << stackForParams.top().id << "\n"; 
							*out.stream << "push eax\n"; 
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

	bool IsIndentifier(const LA::LEX& lex, int i) {
		return lex.lexTable.table[i].lexema[0] == LEX_ID;
	}

	bool IsLexemaFunction(const LA::LEX& lex, int i) {
		return lex.lexTable.table[i].lexema[0] == '@';
	}

	void Operations(std::ofstream* stream, LT::Entry lexTable) {
		switch (lexTable.lexema[1]) {
		case PLUS:
		{
			*stream << "pop ebx\n";
			*stream << "pop eax\n";
			*stream << "add eax, ebx\n";
			break;
		}
		case MINUS:
		{
			*stream << "pop ebx\n";
			*stream << "pop eax\n";
			*stream << "sub eax, ebx\n";
			break;
		}
		case STAR:
		{
			*stream << "pop ebx\n";
			*stream << "pop eax\n";
			*stream << "mul ebx\n";
			break;
		}
		case AMPERSAND:
		{
			*stream << "pop ebx\n";
			*stream << "pop eax\n";
			*stream << "and eax, ebx\n";
			break;
		}
		case PIPE:
		{
			*stream << "pop ebx\n";
			*stream << "pop eax\n";
			*stream << "or eax, ebx\n";
			break;
		}

		}

		if (lexTable.lexema[0] == TILDE) {
			*stream << "pop eax\n";
			*stream << "not eax\n";
		}
	}

	void ExpressionHandler(std::ofstream* stream, LA::LEX lex, int startpos, int endpos) {
		for (int i = startpos; i <= endpos; i++) {
			if (lex.lexTable.table[i].lexema[0] == '#')
			{
				continue;
			}
			if (lex.lexTable.table[i].lexema[0] == LEX_ID && lex.lexTable.table[i].lexema[0] != '@' || lex.lexTable.table[i].lexema[0] == LEX_LITERAL) {
				*stream << "push " << lex.idTable.table[lex.lexTable.table[i].idxTI].id << endl;
			}
			else if (lex.lexTable.table[i].lexema[0] == '@') {
				if (lex.idTable.table[lex.lexTable.table[i].idxTI].idtype == IT::F) {
					*stream << "call F" << lex.idTable.table[lex.lexTable.table[i].idxTI].id << endl;
					*stream << "push eax " << endl;
					i++;
				}
				else if (lex.idTable.table[lex.lexTable.table[i].idxTI].idtype == IT::SF){
					if (!strcmp(lex.idTable.table[lex.lexTable.table[i].idxTI].id, "rest")) {
						*stream << "CALL rest\n";
						*stream << "push eax " << endl;
					}
					else if (!strcmp(lex.idTable.table[lex.lexTable.table[i].idxTI].id, "module")) {
						*stream << "CALL module\n";
						*stream << "push eax " << endl;
					}
				}

			}

			else {
				Operations(stream, lex.lexTable.table[i]);
				*stream << "push eax" << endl;
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


