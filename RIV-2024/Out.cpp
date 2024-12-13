#include "stdafx.h"
#include "Out.h"
#include "In.h"
#include "Error.h"

namespace Out {

    OUT getout(wchar_t outfile[]) {

        OUT out;

        out.stream = new ofstream;
        out.stream->open(outfile);

        if (!out.stream->is_open()) {
            throw ERROR_THROW(112);
        }

        wcscpy_s(out.outfile, outfile);
        return out;
    }

    void WriteOut(In::IN in, wchar_t out[]) {

        ofstream fout(out);

        fout << in.text;
        
        if (in.error_size) {
            cout << "����������� ������� ������� �������� �������� '^'" << endl;
            for (int i = 0; i < in.error_size; i++) {
                cout << "����������� ������ '" << in.forbiddenChar[i] << "' �� ������ " << in.errorLine[i] << " �� ������� " << in.errorCol[i] << endl;
            }
        }

        fout.close();
    }

    void WriteError(OUT out, Error::ERROR error) {
        if (out.stream) {
            *out.stream << " ----  ������  ---- " << endl;
            *out.stream << "������ " << error.id << ": " << error.message << endl;
            if (error.inext.line != -1)
            {
                *out.stream << "������: " << error.inext.line << endl << "�������: " << error.inext.col << endl << endl;
            }
        }
        else {
            cout << "������ " << error.id << ": " << error.message << ", ������" << error.inext.line << ", �������" << error.inext.col << endl;
        }
    }

    void CloseOut(OUT out) {
        out.stream->close();
        delete out.stream;
    }
}
