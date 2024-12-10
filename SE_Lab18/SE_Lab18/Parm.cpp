#include "stdafx.h"
#include "Parm.h"
#include "Error.h"

namespace Parm {    //�������� ������������� ��������� in

	PARM getparm(int argc, _TCHAR* argv[]) {

		PARM parm{};
		bool in = false, out = false, log = false;
		for (int i = 1; i < argc; i++) {
			if (wcslen(argv[i]) > PARM_MAX_SIZE)  {
				throw ERROR_THROW(104);
			}

			if (!wcsncmp(argv[i], PARM_IN, wcslen(PARM_IN))) {  //��������� ����� ��������� ����������� wcslen(PARM_IN)
				wcscpy_s(parm.in, argv[i] + wcslen(PARM_IN));
				in = true;
			}
			else if (!wcsncmp(argv[i], PARM_OUT, wcslen(PARM_OUT))) {  //���� ������ ���������� � PARM_OUT
				wcscpy_s(parm.out, argv[i] + wcslen(PARM_OUT));
				wcscat_s(parm.out, PARM_OUT_DEFAULT_EXT);  // ��������� ���������� out
				out = true;
			}
			else if (!wcsncmp(argv[i], PARM_LOG, wcslen(PARM_LOG))) {  //���� ������ ���������� � PARM_LOG
				wcscpy_s(parm.log, argv[i] + wcslen(PARM_LOG));
				wcscat_s(parm.log, PARM_LOG_DEFAULT_EXT);  // ��������� ���������� log
				log = true;
			}
		}

		if (!in) {
			throw ERROR_THROW(100);
		}
		if (!out) {
			wcscpy_s(parm.out, parm.in);			   //���������� ��� �������� ����� � ��� out
			wcscat_s(parm.out, PARM_OUT_DEFAULT_EXT);  //��������� ���������� out
		}
		if (!log) {
			wcscpy_s(parm.log, parm.in);
			wcscat_s(parm.log, PARM_LOG_DEFAULT_EXT);
		}
		return parm;

	}
}