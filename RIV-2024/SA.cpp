#include "SA.h"


namespace SA {

    bool SA(LA::LEX& lex, Log::LOG log) {
        LT::lexTable& lexTable = lex.lexTable;
        IT::idTable& idTable = lex.idTable;
        bool isSemCorrect = true;

        for (int i = 0; i < lexTable.size; i++) {
            switch (lexTable.table[i].lexema[0]) {
            case LEX_OPERATION:
                if (lexTable.table[i].lexema[1] == DIRSLASH) { 
                    if (!DivisionByZero(lexTable, idTable, i)) {
                        isSemCorrect = false;
                    }
                }
                break;
            }
        }
        return isSemCorrect;
    }

    bool DivisionByZero(LT::lexTable& lexTable,IT::idTable& idTable, int index) {

        if (idTable.table[lexTable.table[index + 1].idxTI].value.vint == 0) {
            throw ERROR_THROW(129);
            return false;
        }
        return true;
    }
}
