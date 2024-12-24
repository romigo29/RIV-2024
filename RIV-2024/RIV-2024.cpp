#include "stdafx.h"
#include "MFST.h"
#include "PolishNotation.h"
#include "SA.h"
#include "CodeGeneration.h"

int _tmain(int argc, _TCHAR* argv[]) {

    setlocale(LC_ALL, "rus");

    try {
        Parm::PARM parm = Parm::getparm(argc, argv);
        In::IN in = In::getin(parm.in);
        cout << in.text << endl;
        cout << "����� ��������: " << in.size << endl;
        cout << "����� �����: " << in.lines << endl;
        cout << "���������: " << in.ignore << endl;
    }
    catch (Error::ERROR e) {
        cout << "������ " << e.id << ": " << e.message << endl << endl;
        cout << "������ " << e.inext.line << " ������� " << e.inext.col << endl << endl;
    };

    Log::LOG log = Log::INITLOG;
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


        LA::LEX lexTables = LA::LA(parm, in);

        MFST_TRACE_START(*log.stream)
            MFST::Mfst mfst(lexTables.lexTable, GRB::getGreibach());

        mfst.start(log);
        mfst.savededucation();
        mfst.printrules(log);

        if (PN::startPolish(lexTables)) {
            cout << "\n�������� ������ ���������\n";
        }
        else {
            cout << "\n�������� ������ �� ���������\n";
        }

        LT::PrintLT(lexTables.lexTable);

        if (SA::startSA(lexTables)) {
            cout << "\n������������� ���������� �������� ������ ��� ������\n";
        }

        else {
            cout << "\n� ������ �������������� ����������� ���� ������\n";
        }

        CodeGeneration::GenerateCode(lexTables, out);

        cout << endl << "��������� ��������� �������!" << endl;

        Log::Close(log);
        Out::CloseOut(out);


        system(R"(D:\BSTU\course\RIV-2024\RIV-2024\CompilationOfGenCode.bat)");

    }
    catch (Error::ERROR e)
    {
        *log.stream << "������ " << e.id << ':' << e.message << endl;
        cout << "������ " << e.id << ':' << e.message << endl;
        if (e.inext.line) {
            *log.stream << "������: " << e.inext.line << ", �������: " << e.inext.col << endl;
            cout << "������: " << e.inext.line << ", �������: " << e.inext.col << endl;
            Log::WriteError(log, e);

        }

        Out::WriteError(out, e);
        Log::Close(log);
        Out::CloseOut(out);
    }

    In::deleteIn(in);

    system("pause");
    return 0;
}

