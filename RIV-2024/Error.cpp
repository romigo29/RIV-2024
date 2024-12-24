#include "stdafx.h" 
namespace Error
{	
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
		ERROR_ENTRY(105,"Ошибка при создании выходного файла с расширением .out"),
		ERROR_ENTRY(106,"Недопустимый размер таблицы лексем"),
		ERROR_ENTRY(107,"Превышен размер таблицы лексем"),
		ERROR_ENTRY_NODEF(108),ERROR_ENTRY_NODEF(109),
		ERROR_ENTRY(110,"Ошибка при открытии файла с исходным кодом (-in)"),
		ERROR_ENTRY(111,"Недопустимый символ в исходном файле (-in)"),
		ERROR_ENTRY(112,"Ошибка при создании файла с протоколом (-log)"),
		ERROR_ENTRY(113,"Недопустимый идентификатор"),
		ERROR_ENTRY(114,"Дублирование идентификатора"),
		ERROR_ENTRY(115,"Идентификатор не определён"),
		ERROR_ENTRY(116,"Дублирование main"),
		ERROR_ENTRY(117,"Недопустимый размер таблицы идентификаторов"),
		ERROR_ENTRY(118,"Превышен размер таблицы идентификаторов"),
		ERROR_ENTRY(119,"Превышена длина лексемы"),
		ERROR_ENTRY(120,"Функция не может быть типа ch"),
		ERROR_ENTRY(121, "Использование запрещенного символа"),
		ERROR_ENTRY(122, "Кавычки не были закрыты"),
		ERROR_ENTRY(123, "Литералы типа ch должен содержать только 1 символ"),
		ERROR_ENTRY(124, "Превышен максильманый размер для литерала типа str (255)"),
		ERROR_ENTRY_NODEF(125),
		ERROR_ENTRY_NODEF(126),
		ERROR_ENTRY_NODEF(127),
		ERROR_ENTRY_NODEF(128),
		ERROR_ENTRY_NODEF(129),
		ERROR_ENTRY_NODEF10(130),ERROR_ENTRY_NODEF10(140),ERROR_ENTRY_NODEF10(150),ERROR_ENTRY_NODEF10(160),
		ERROR_ENTRY_NODEF10(170),ERROR_ENTRY_NODEF10(180),ERROR_ENTRY_NODEF10(190),ERROR_ENTRY_NODEF100(200),ERROR_ENTRY_NODEF100(300),
		ERROR_ENTRY_NODEF100(400),ERROR_ENTRY_NODEF100(500),
		ERROR_ENTRY(600,"Неверная структура программы"),
		ERROR_ENTRY(601,"Ошибочный оператор"),
		ERROR_ENTRY(602,"Ошибка в выражении"),
		ERROR_ENTRY(603,"Ошибка в параметрах функции"),
		ERROR_ENTRY(604,"Ошибка в параметрах обьъявляемой функции"),
		ERROR_ENTRY(605,"Ошибка в арифметических или логическмх операторах"),
		ERROR_ENTRY(606,"Ошибка в операторах сравнения"),
		ERROR_ENTRY(607,"Синтаксически анализ завершён досрочно"),
		ERROR_ENTRY_NODEF(608),ERROR_ENTRY_NODEF(609),ERROR_ENTRY_NODEF10(610),
		ERROR_ENTRY_NODEF10(620),ERROR_ENTRY_NODEF10(630),ERROR_ENTRY_NODEF10(640),ERROR_ENTRY_NODEF10(650),ERROR_ENTRY_NODEF10(660),ERROR_ENTRY_NODEF10(670),
		ERROR_ENTRY_NODEF10(680),ERROR_ENTRY_NODEF10(690),
		ERROR_ENTRY(700, "Тип возвращаемого значения функции не соответствует возвращаемому значению"),
		ERROR_ENTRY(701, "Не совпадает количество параметров функции при вызове и объявлении"),
		ERROR_ENTRY(702, "Не совпадают типы параметров функции при вызове и объявлении"),
		ERROR_ENTRY(703, "Типы данных, используемые в выражении, не совпадают"),
		ERROR_ENTRY(704, "Попытка вызвать операцию, неподдерживаемую типом данных"),
		ERROR_ENTRY(705, "Превышение максимального количества параметров функции (4)"),
		ERROR_ENTRY(706, "Невозможно использовать идентификатор функции кроме как для её вызова"),
		ERROR_ENTRY(707, "Попытка присвоить идентификатору значение функции, которое отлично от типа данных идентификатора"),
		ERROR_ENTRY(708, "параметры функции стандартной библиотеки ошибочны"),
		ERROR_ENTRY(709, "print обязательно должно принимать какое-либо значение"),
		ERROR_ENTRY_NODEF100(800),
		ERROR_ENTRY_NODEF100(900)
	};

	ERROR geterror(int id) {

		if (id < 0 || id > ERROR_MAX_ENTRY) {  
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