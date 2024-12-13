#include "stdafx.h"
#include "Parm.h"
#include "Error.h"

namespace Parm {    //проверка об€зательного параметра in

	PARM getparm(int argc, _TCHAR* argv[]) {

		PARM parm{};
		bool in = false, out = false, log = false;
		for (int i = 1; i < argc; i++) {
			if (wcslen(argv[i]) > PARM_MAX_SIZE)  {
				throw ERROR_THROW(104);
			}

			if (!wcsncmp(argv[i], PARM_IN, wcslen(PARM_IN))) {  //сравнение строк символами количеством wcslen(PARM_IN)
				wcscpy_s(parm.in, argv[i] + wcslen(PARM_IN));
				in = true;
			}
			else if (!wcsncmp(argv[i], PARM_OUT, wcslen(PARM_OUT))) {  //если строка начинаетс€ с PARM_OUT
				wcscpy_s(parm.out, argv[i] + wcslen(PARM_OUT));
				wcscat_s(parm.out, PARM_OUT_DEFAULT_EXT);  // добавл€ем расширение out
				out = true;
			}
			else if (!wcsncmp(argv[i], PARM_LOG, wcslen(PARM_LOG))) {  //если строка начинаетс€ с PARM_LOG
				wcscpy_s(parm.log, argv[i] + wcslen(PARM_LOG));
				wcscat_s(parm.log, PARM_LOG_DEFAULT_EXT);  // добавл€ем расширение log
				log = true;
			}
		}

		if (!in) {
			throw ERROR_THROW(100);
		}
		if (!out) {
			wcscpy_s(parm.out, parm.in);			   //используем им€ исходное файла и дл€ out
			wcscat_s(parm.out, PARM_OUT_DEFAULT_EXT);  //добавл€ем расширение out
		}
		if (!log) {
			wcscpy_s(parm.log, parm.in);
			wcscat_s(parm.log, PARM_LOG_DEFAULT_EXT);
		}
		return parm;

	}
}