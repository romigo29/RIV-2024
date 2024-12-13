#include "stdafx.h" 
#include "Error.h"
namespace Error
{
	// серии ошибок: 0 - 99 системные ошибки
	//				 100 - 199 - ошибки параметров
	//				 110 - 119 - ошибки открытия и чтения файлов 
	
	ERROR errors[ERROR_MAX_ENTRY] = {
		ERROR_ENTRY(0,"Недопустимый код ошибки"),
		ERROR_ENTRY(1,"Системный сбой"),
		ERROR_ENTRY(2,"Ошибочное выражние"),
		ERROR_ENTRY(3,"Неизвестная библиотека"),
		ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5),
		ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40),
		ERROR_ENTRY_NODEF10(50), ERROR_ENTRY_NODEF10(60), ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80),ERROR_ENTRY_NODEF10(90),
		ERROR_ENTRY(100,"Параметр -in должен быть задан"),
		ERROR_ENTRY_NODEF(101), ERROR_ENTRY_NODEF(102),ERROR_ENTRY_NODEF(103),
		ERROR_ENTRY(104,"Превышена длина входного параметра"),
		ERROR_ENTRY(105,"Ошибка при создании выходного файла с расширением .out"),//113
		ERROR_ENTRY(106,"Недопустимый размер таблицы лексем"),//114
		ERROR_ENTRY(107,"Превышен размер таблицы лексем"),//115
		ERROR_ENTRY_NODEF(108),ERROR_ENTRY_NODEF(109),
		ERROR_ENTRY(110,"Ошибка при открытии файла с исходным кодом (-in)"),
		ERROR_ENTRY(111,"Недопустимый символ в исходном файле (-in)"),
		ERROR_ENTRY(112,"Ошибка при создании файла с протоколом (-log)"),
		ERROR_ENTRY(113,"Недопустимый идентификатор"),
		ERROR_ENTRY(114,"Дублирование идентификатора"),//105
		ERROR_ENTRY(115,"Идентификатор не определён"),//106
		ERROR_ENTRY(116,"Некорректное использование индекса"),//107
		ERROR_ENTRY(117,"Недопустимый размер таблицы идентификаторов"),//нарушена нумерация ошибок
		ERROR_ENTRY(118,"Превышен размер таблицы идентификаторов"),
		ERROR_ENTRY(119,"Превышена длина лексемы"),
		ERROR_ENTRY(120,"Дублирование main"),//не используется
		ERROR_ENTRY_NODEF(121),
		ERROR_ENTRY_NODEF(122),
		ERROR_ENTRY(123, "Вышеперечисленные переменные не проициницилизированы"),
		ERROR_ENTRY(124,"Попытка деления на 0"),
		ERROR_ENTRY(125,"Несоответствие типа возвращаемого значения"),
		ERROR_ENTRY(126, "Некорректный блок кода"),
		ERROR_ENTRY(127,"Присвоить можно значение можно только переменной"),
		ERROR_ENTRY(128,"Чтение доступно только для переменной"),//переместить
		ERROR_ENTRY(129,"Попытка деления на 0"),
		ERROR_ENTRY_NODEF10(130),ERROR_ENTRY_NODEF10(140),ERROR_ENTRY_NODEF10(150),ERROR_ENTRY_NODEF10(160),
		ERROR_ENTRY_NODEF10(170),ERROR_ENTRY_NODEF10(180),ERROR_ENTRY_NODEF10(190),ERROR_ENTRY_NODEF100(200),ERROR_ENTRY_NODEF100(300),
		ERROR_ENTRY_NODEF100(400),ERROR_ENTRY_NODEF100(500),
		ERROR_ENTRY(600,"Неверная структура программы"),
		ERROR_ENTRY(601,"Ошибочный оператор"),
		ERROR_ENTRY(602,"Ошибка в выражении"),
		ERROR_ENTRY(603,"Ошибка в параметрах функции"),
		ERROR_ENTRY(604,"Ошибка в параметрах обьъявляемой функции"),
		ERROR_ENTRY(605,"Синтаксически анализ завершён досрочно"),
		ERROR_ENTRY_NODEF(606),ERROR_ENTRY_NODEF(607),ERROR_ENTRY_NODEF(608),ERROR_ENTRY_NODEF(609),ERROR_ENTRY_NODEF10(610),
		ERROR_ENTRY_NODEF10(620),ERROR_ENTRY_NODEF10(630),ERROR_ENTRY_NODEF10(640),ERROR_ENTRY_NODEF10(650),ERROR_ENTRY_NODEF10(660),ERROR_ENTRY_NODEF10(670),
		ERROR_ENTRY_NODEF10(680),ERROR_ENTRY_NODEF10(690),
		ERROR_ENTRY_NODEF100(700),ERROR_ENTRY_NODEF100(800),
		ERROR_ENTRY_NODEF100(900)
	};

	ERROR geterror(int id) {

		if (id < 0 || id > ERROR_MAX_ENTRY) {   //если код вне диапазона таблицы ошибок
			return errors[0];
		}

		else {
			return errors[id];
		}
	}
	ERROR geterrorin(int id, int line = -1, int col = -1) {

		if (id < 0 || id > ERROR_MAX_ENTRY) {
			return errors[0];
		}

		else {
			errors[id].inext.line = line;
			errors[id].inext.col = col;
			return errors[id];
		}
	}
};