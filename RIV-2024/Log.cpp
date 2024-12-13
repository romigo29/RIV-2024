#include "stdafx.h"
#include "Log.h"
#include "Error.h"

namespace Log {

	LOG getlog(wchar_t logfile[]) {

		LOG log;

		log.stream = new ofstream;
		log.stream->open(logfile);

		if (!log.stream->is_open()) {
			throw ERROR_THROW(112);
		}

		wcscpy_s(log.logfile, logfile);
		return log;
	}

	void WriteLine(LOG log, char* c, ...) {

		string result;
		const char* str = c;
		// ���������� va_list ��� ��������� ����������� ����� ����������
		va_list args;
		va_start(args, c);

		while (str && str[0] != '\0') {
			result += str;
			str = va_arg(args, const char*);
		}

		va_end(args);

		// ������ � ���
		*(log.stream) << result << std::endl;
	}

	void WriteLine(LOG log, wchar_t* c, ...) {

		std::wstring result;
		const wchar_t* wstr = c;

		// ���������� va_list ��� ��������� ����������� ����� ����������
		va_list args;
		va_start(args, c);

		while (wstr && wstr[0] != L'\0') {
			result += wstr;
			wstr = va_arg(args, const wchar_t*);
		}

		va_end(args);

		// �������������� wchar_t* � char*
		size_t len = wcstombs(nullptr, result.c_str(), 0);
		if (len == (size_t)-1) return; // ��������� ������ ��������������
		char* cstr = new char[len + 1];
		wcstombs(cstr, result.c_str(), len + 1);

		// ������ � ���
		*(log.stream) << cstr << std::endl;
	}

	void WriteLog(LOG log) {

		time_t t = time(nullptr);
		tm localTime;
		localtime_s(&localTime, &t);
		// ����������� ����� � ������
		char timeStr[PARM_MAX_SIZE];
		strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &localTime);
		// ���������� ��������� � ��������
		*(log.stream) << "---- �������� ------ " << timeStr << "-------------" << endl;
	}

	void WriteParm(LOG log, Parm::PARM parm) {
		char buff[PARM_MAX_SIZE];
		size_t tsize = 0;
		//����� ���������� � ���������� � �����
		*log.stream << "----���������----" << endl;
		wcstombs_s(&tsize, buff, parm.log, PARM_MAX_SIZE);  //log
		*log.stream << "-log: " << buff << endl;
		wcstombs_s(&tsize, buff, parm.out, PARM_MAX_SIZE);  //out
		*log.stream << "-out: " << buff << endl;
		wcstombs_s(&tsize, buff, parm.in, PARM_MAX_SIZE);  //in
		*log.stream << "-in: " << buff << endl;
	}

	void WriteIn(LOG log, In::IN in) {
		*log.stream << "---- �������� ������ " << endl;
		*log.stream << "���������� ��������: " << in.size << endl;
		*log.stream << "���������������: " << in.ignore << endl;
		*log.stream << "���������� �����: " << in.lines << endl;
	}

	void WriteError(LOG log, Error::ERROR error) {
		if (log.stream) {
			*log.stream << " ----	������	---- " << endl;
			*log.stream << "������ " << error.id << ": " << error.message << endl;
			if (error.inext.line != -1)
			{
				*log.stream << "������: " << error.inext.line << endl << "�������: " << error.inext.col << endl << endl;
			}
		}
		else {
			cout << "������ " << error.id << ": " << error.message << ", ������" << error.inext.line << ", �������" << error.inext.col << endl;
		}
	}

	void WriteForbidden(LOG log, In::IN in) {
		if (in.error_size) {
			*log.stream << "����������� ������� ������� �������� �������� '^'" << endl;
			for (int i = 0; i < in.error_size; i++) {
				*log.stream << "����������� ������ '" << in.forbiddenChar[i] << "' �� ������ " << in.errorLine[i] << " �� ������� " << in.errorCol[i] << endl;
			}
		}
	}

	void Close(LOG log) {
		log.stream->close();
		delete log.stream;
	}
}