#include "SA.h"


namespace SA {

	void operands(LA::LEX lex) {
		for (int i = 0; i < lex.lexTable.size; i++) {

			if (lex.lexTable.table[i].lexema[0] == LEX_COMPARISON || lex.lexTable.table[i].lexema[0] == LEX_EQUAL)
			{
				if (lex.lexTable.table[i].lexema[0] == LEX_EQUAL) {
					int cur = -1;
					IT::IDDATATYPE datatype = (IT::IDDATATYPE)0;
					while (lex.lexTable.table[i + cur].lexema[0] != LEX_SEMICOLON) {
						if (lex.lexTable.table[i + cur].lexema[0] == LEX_ID || lex.lexTable.table[i + cur].lexema[0] == LEX_LITERAL)
						{
							if (datatype == (IT::IDDATATYPE)0) {
								datatype = lex.idTable.table[lex.lexTable.table[i + cur].idxTI].iddatatype;
							}
			
							if (lex.idTable.table[lex.lexTable.table[i + cur].idxTI].idtype == IT::F) {
								while (lex.lexTable.table[i + cur].lexema[0] != LEX_RIGHTTHESIS) {
									cur++;
								}
							}
						}
						if (datatype == IT::STR && lex.lexTable.table[i + cur].lexema[0] == LEX_COMPARISON && cur != 0) {
							throw ERROR_THROW_IN(704, lex.lexTable.table[i + cur].sn, lex.lexTable.table[i + cur].idxTI)
						}
						if (datatype == IT::CH && lex.lexTable.table[i + cur].lexema[0] == LEX_COMPARISON && cur != 0 && lex.lexTable.table[i + cur].lexema[1] != '+' && lex.lexTable.table[i + cur].lexema[1] != '-') {
							throw ERROR_THROW_IN(704, lex.lexTable.table[i + cur].sn, lex.lexTable.table[i + cur].idxTI)
						}
						if (datatype == IT::BOOL && lex.lexTable.table[i + cur].lexema[0] == LEX_COMPARISON && cur != 0) {
							throw ERROR_THROW_IN(704, lex.lexTable.table[i + cur].sn, lex.lexTable.table[i + cur].idxTI)
						}
						cur++;
					}
					i += cur - 1;
				}
			}
		}
	}

	void functions(LA::LEX lex) {
		for (int i = 0; i < lex.lexTable.size; i++) {
			if (lex.lexTable.table[i].lexema[0] == LEX_PRINT) {
				if (lex.lexTable.table[i + 2].lexema[0] == ')') {
					throw ERROR_THROW_IN(709, lex.lexTable.table[i + 2].sn, lex.lexTable.table[i + 2].idxTI);
				}
				IT::IDTYPE re = lex.idTable.table[lex.lexTable.table[i + 2].idxTI].idtype;
				IT::IDDATATYPE ree = lex.idTable.table[lex.lexTable.table[i + 2].idxTI].iddatatype;
			}

			if (lex.lexTable.table[i].lexema[0] == LEX_ID && lex.lexTable.table[i - 1].lexema[0] == LEX_FUNCTION && lex.idTable.table[lex.lexTable.table[i].idxTI].idtype == IT::F)
			{
				int cur = 1;
				IT::IDDATATYPE returnType = lex.idTable.table[lex.lexTable.table[i].idxTI].iddatatype;
				while (i + cur < lex.lexTable.size && lex.lexTable.table[i + cur].lexema[0] != LEX_RETURN) {
					cur++;
				}
				if (i + cur == lex.lexTable.size) {
					throw ERROR_THROW_IN(700, lex.lexTable.table[i].sn, lex.lexTable.table[i].idxTI);
				}
				if (i + cur < lex.lexTable.size && (lex.lexTable.table[i + cur + 1].lexema[0] == LEX_ID || lex.lexTable.table[i + cur + 1].lexema[0] == LEX_LITERAL)
					&& lex.idTable.table[lex.lexTable.table[i + cur + 1].idxTI].idtype != IT::F
					&& lex.idTable.table[lex.lexTable.table[i + cur + 1].idxTI].iddatatype != returnType) {
					throw ERROR_THROW_IN(700, lex.lexTable.table[i + cur].sn, lex.lexTable.table[i + cur].idxTI);
				}
			}
		}
		for (int i = 0; i < lex.lexTable.size; i++) {
			if (lex.lexTable.table[i].lexema[0] == LEX_ID && lex.idTable.table[lex.lexTable.table[i].idxTI].idtype == IT::F && lex.lexTable.table[i - 1].lexema[0] == LEX_FUNCTION) {
				IT::IDDATATYPE* ids = new IT::IDDATATYPE[16];
				int idsSize = 0;
				int funcPos = lex.idTable.table[lex.lexTable.table[i].idxTI].idxfirstLE;
				while (lex.lexTable.table[funcPos + 1].lexema[0] != LEX_RIGHTTHESIS)
				{
					if (lex.lexTable.table[funcPos + 1].lexema[0] == LEX_ID || lex.lexTable.table[funcPos + 1].lexema[0] == LEX_LITERAL) {
						ids[idsSize] = lex.idTable.table[lex.lexTable.table[funcPos + 1].idxTI].iddatatype;
						idsSize++;
					}
					funcPos++;
					if (idsSize == MAX_PARM_COUNT) { 
						throw ERROR_THROW_IN(705, lex.lexTable.table[i].sn, lex.lexTable.table[i].idxTI);
					}
				}
			}
			if (lex.lexTable.table[i].lexema[0] == '@'
				&& lex.idTable.table[lex.lexTable.table[i].idxTI].idtype == IT::SF) {

				int cur = 1;
				int numberOfParams = 0;
				while (lex.lexTable.table[i - cur].lexema[0] != LEX_EQUAL) {
					if (lex.lexTable.table[i - cur].lexema[0] == LEX_ID || lex.lexTable.table[i - cur].lexema[0] == LEX_LITERAL) {
						if (lex.idTable.table[lex.lexTable.table[i - cur].idxTI].iddatatype == IT::BYTE || lex.idTable.table[lex.lexTable.table[i - cur].idxTI].iddatatype == IT::INT) {
							numberOfParams++;
						}
						else {
							if (lex.idTable.table[lex.lexTable.table[i - cur].idxTI].idtype == IT::SF) {
							throw ERROR_THROW_IN(701, lex.lexTable.table[i].sn, lex.lexTable.table[i].idxTI);
							}
						}

					}
					cur++;
				}
				if (numberOfParams != 2 && !strcmp(lex.idTable.table[lex.lexTable.table[i].idxTI].id,"rest")) {
					throw ERROR_THROW_IN(708, lex.lexTable.table[i].sn, lex.lexTable.table[i].idxTI);
				}
				if (numberOfParams != 1 && !strcmp(lex.idTable.table[lex.lexTable.table[i].idxTI].id, "module")) {
					throw ERROR_THROW_IN(708, lex.lexTable.table[i].sn, lex.lexTable.table[i].idxTI);
				}
			}

			if (lex.lexTable.table[i].lexema[0] == '@' && lex.idTable.table[lex.lexTable.table[i].idxTI].idtype == IT::F && lex.lexTable.table[i - 1].lexema[0] != LEX_FUNCTION)
			{
				IT::IDDATATYPE* ids = new IT::IDDATATYPE[16];
				int idsSize = 0;
				int funcPos = lex.idTable.table[lex.lexTable.table[i].idxTI].idxfirstLE;

				while (lex.lexTable.table[funcPos + 1].lexema[0] != LEX_RIGHTTHESIS)
				{
					if (lex.lexTable.table[funcPos + 1].lexema[0] == LEX_ID || lex.lexTable.table[funcPos + 1].lexema[0] == LEX_LITERAL) {
						ids[idsSize] = lex.idTable.table[lex.lexTable.table[funcPos + 1].idxTI].iddatatype;
						idsSize++;
					}
					funcPos++;
					if (idsSize > MAX_PARM_COUNT) {
						throw ERROR_THROW_IN(705, lex.lexTable.table[i].sn, lex.lexTable.table[i].idxTI);
					}
				}
				int cur = 1;
				int paramCount = 0;
				while (lex.lexTable.table[i - cur].lexema[0] != LEX_EQUAL) {
					if (lex.lexTable.table[i - cur].lexema[0] == LEX_ID || lex.lexTable.table[i - cur].lexema[0] == LEX_LITERAL) {
						if (lex.idTable.table[lex.lexTable.table[i - cur].idxTI].iddatatype != ids[paramCount]) {
							throw ERROR_THROW_IN(702, lex.lexTable.table[i - cur].sn, lex.lexTable.table[i - cur].idxTI)
						}
						paramCount++;
					}
					cur++;
				}
				if (paramCount != idsSize) {
					throw ERROR_THROW_IN(701, lex.lexTable.table[i + cur].sn, lex.lexTable.table[i - cur].idxTI)
				}
				i += cur;
				delete[] ids;
			}
		}
	}
	void literals(LA::LEX lex) {
		for (int i = 0; i < lex.idTable.size; i++) {
			
			if (lex.idTable.table[i].idtype == IT::L && lex.idTable.table[i - 1].idtype == IT::V) {

				IT::IDDATATYPE currentDataType = lex.idTable.table[i].iddatatype;
				IT::IDDATATYPE prevDataType = lex.idTable.table[i - 1].iddatatype;

				if (prevDataType == IT::BOOL && currentDataType != IT::INT && currentDataType != IT::BYTE) {
					throw ERROR_THROW_IN(703, lex.idTable.table[i].line, lex.idTable.table[i].idxfirstLE);
				}

				if (prevDataType != IT::BOOL && currentDataType != prevDataType) {
					throw ERROR_THROW_IN(703, lex.idTable.table[i].line, lex.idTable.table[i].idxfirstLE);
				}
			}
		}
	}

	bool startSA(LA::LEX lex) {
		functions(lex);
		literals(lex);
		operands(lex);
		return true;
	};
}
