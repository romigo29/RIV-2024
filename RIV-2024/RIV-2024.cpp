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
    

        LA::LEX lexTables = LA::LA(parm, in);

       MFST_TRACE_START
            MFST::Mfst mfst(lexTables.lexTable, GRB::getGreibach());

        mfst.start();
        mfst.savededucation();
        mfst.printrules();

        if (PN::startPolish(lexTables)) {
            cout << "\n�������� ������ ���������\n";
        }
        else {
            cout << "\n�������� ������ �� ���������\n";
        }

        LT::PrintLT(lexTables.lexTable);

      /* if (SA::SA(lexTables, log)) {
            cout << "\n������������� ���������� �������� ������ ��� ������\n";
        }

        else {
            cout << "\n� ������ �������������� ����������� ���� ������\n";
        }*/

       CodeGeneration::GenerateCode(lexTables, out);

       cout << endl << "��������� ��������� �������!" << endl;

       /*LT::Delete(lexTables.lexTable);
       IT::Delete(lexTables.idTable);*/

       Log::Close(log);
       Out::CloseOut(out);

    }
    catch (Error::ERROR e)
    {
        cout << "������" << e.id << ':' << e.message << endl << endl;
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
