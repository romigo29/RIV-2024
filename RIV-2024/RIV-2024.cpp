#include "stdafx.h"
#include "MFST.h"
#include "PolishNotation.h"
#include "SA.h"

int _tmain(int argc, _TCHAR* argv[]) {

    setlocale(LC_ALL, "rus");

    try {
        Parm::PARM parm = Parm::getparm(argc, argv);
        In::IN in = In::getin(parm.in);
        cout << in.text << endl;
        cout << "Всего символов: " << in.size << endl;
        cout << "Всего строк: " << in.lines << endl;
        cout << "Пропущено: " << in.ignore << endl;
    }
    catch (Error::ERROR e) {
        cout << "Ошибка " << e.id << ": " << e.message << endl << endl;
        cout << "строка " << e.inext.line << " позиция " << e.inext.col << endl << endl;
    };

    Log::LOG log= Log::INITLOG;
    Parm::PARM parm = Parm::getparm(argc, argv);
    Out::OUT out = Out::INITOUT;
    In::IN in = In::getin(parm.in);

    try {
        out = Out::getout(parm.out);
        log = Log::getlog(parm.log);

        Log::WriteLog(log);
        Log::WriteParm(log, parm);
        Out::WriteOut(in, parm.out);
        Log::WriteIn(log, in);
        Log::WriteForbidden(log, in);
        Log::Close(log);
        Out::CloseOut(out);

        LA::Lex LexTables = LA::LA(parm, in);

        MFST_TRACE_START
            MFST::Mfst mfst(LexTables.lexTable, GRB::getGreibach());

        mfst.start();
        mfst.savededucation();
        mfst.printrules();

        if (PN::startPolish(LexTables)) {
            cout << "\nПольская запись построена\n";
        }
        else {
            cout << "\nПольская запись не построена\n";
        }

        LT::PrintLT(LexTables.lexTable);

       if (SA::SA(LexTables, log)) {
            cout << "\nСемантический анализатор выполнил работу без ошибок\n";
        }

        else {
            cout << "\nВ работе семантического анализатора были ошибки\n";
        }
    }
    catch (Error::ERROR e)
    {
        cout << "Ошибка" << e.id << ':' << e.message << endl << endl;
        if (e.inext.line)
            Log::WriteError(log, e);
        Out::WriteError(out, e);
        Log::Close(log);
        Out::CloseOut(out);
    }

    In::deleteIn(in);
    system("pause");
    return 0;
}
