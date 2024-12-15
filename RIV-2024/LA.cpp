#include"LA.h"
#include"stdafx.h"
#include"In.h"
#include"Error.h"
#include <stdio.h>
#include <string.h>
#include <iomanip>

namespace LA
{
    LT::lexTable lexTable = LT::Create(LT_MAXSIZE - 1);  //Таблица для хранения лексем
    IT::idTable idTable = IT::Create(TI_MAXSIZE - 1);    //Таблица для хранения идентификаторов
    //различные состояния программы
    bool byteFlag = false;
    bool intFlag = false;
    bool boolFlag = false;
    bool falseFlag = false;
    bool trueFlag = false;
    bool chFlag = false;
    bool stringFlag = false;
    bool parmFlag = false;
    bool mainFlag = false;
    bool letFlag = false;
    bool keyWord = false;
    bool declareFunctionflag = false;
    bool addedToITFlag = false; // Флаг добавления в таблицу идентификаторов
    bool literalFlag = false;
    char* str = new char[MAX_LEX_SIZE];    //хранение текущей лексемы

    char FST()
    {
        FST_BYTE
        FST_INT
            FST_BOOL
            FST_CH
            FST_STR
            FST_FUNC
            FST_LET
            FST_RETURN
            FST_MAIN
            FST_PRINT
            FST_IF
            FST_ELSE
            FST_REST
            FST_MODULE
            FST_LITERAL
            FST_IDENF

            FstLexeme lexemes[] = {
            {_byte, LEX_BYTE, &byteFlag},
            {_int, LEX_INT, &intFlag},
            {_bool, LEX_BOOL, &boolFlag},
            {_ch, LEX_CH, &chFlag},
            {_string, LEX_STRING, &stringFlag},
            {_function, LEX_FUNCTION, nullptr},
            {_let, LEX_DECLARE, nullptr},
            {_return, LEX_RETURN, nullptr},
            {_main, LEX_MAIN, &mainFlag},
            {_print, LEX_PRINT, nullptr},
            {_if, LEX_IF, nullptr},
            {_else, LEX_ELSE, nullptr},
            {_rest, LEX_REST, nullptr},
            {_module, LEX_MODULE, nullptr},
            {literal_int, LEX_LITERAL, nullptr},
            {idenf, LEX_ID, nullptr}
            
        };

        for (int i = 0; i < FST_AMOUNT; i++) {
            if (Execute(lexemes[i].fst)) {
                if (lexemes[i].flag) {
                    *lexemes[i].flag = true;
                    keyWord = true;
                }
                return lexemes[i].lexeme;
            }
        }

        return NULL;
    }

    LA::LEX LA(Parm::PARM parm, In::IN in)
    {
        LEX tables;
        int indexIT;
        LT::Entry current_entry_LT;
        int bufferIndex = 0;
        current_entry_LT.sn = 0;
        current_entry_LT.idxTI = 0;
        current_entry_LT.lexema[0] = NULL;
        stack<IT::Entry*> scope;
        scope.push(NULL);
        int number_literal = 0;
        IT::Entry current_entry_IT;
        lexTable.size = 0;
        int currentLine = 1;
        ofstream LT_file;
        ofstream IT_file;
        LT_file.open("LT.txt");
        IT_file.open("IT.txt");
        for (int i = 0; i < in.size; i++)
        {
            if (in.code[(int)in.text[i]] == In::IN::T || in.text[i] == SINGLE_QUOTE || in.text[i] == DOUBLE_QUOTE || literalFlag)
            {
                str[bufferIndex++] = in.text[i];
                if (bufferIndex >= MAX_LEX_SIZE)
                {
                    throw ERROR_THROW(119);
                }
            }
            else
            {
                str[bufferIndex] = '\0';
                current_entry_LT.lexema[0] = FST();
                switch (current_entry_LT.lexema[0])
                {
                case LEX_MAIN:
                    mainFlag = true;
                    current_entry_LT.idxTI = idTable.size;
                    memcpy(current_entry_IT.id, str, 5);
                    current_entry_IT.id[5] = '\0';
                    current_entry_IT.iddatatype = IT::INT;
                    current_entry_IT.idtype = IT::M;
                    current_entry_IT.value.vint = NULL;
                    current_entry_IT.line = currentLine;
                    current_entry_IT.idxfirstLE = lexTable.size;
                    current_entry_IT.scope = NULL;
                    indexIT = IT::search(idTable, current_entry_IT);
                    if (indexIT >= 0)
                    {
                        throw ERROR_THROW(120);
                    }
                    if (indexIT == -1)
                    {
                        current_entry_LT.idxTI = idTable.size;
                        IT::Add(idTable, current_entry_IT);
                    }
                    break;
                case LEX_LITERAL:
                    current_entry_IT.idtype = IT::L;
                    current_entry_IT.iddatatype = IT::INT;
                    sprintf_s(current_entry_IT.id, "L%d", number_literal);
                    if (in.text[i - strlen(str) - 1] == MINUS && in.text[i - strlen(str)] != SPACE && !isdigit(in.text[i - strlen(str) - 2]))
                    {
                        current_entry_IT.value.vint = -atoi(str);
                        if (str[0] == '0')
                            current_entry_IT.value.vint = -stoi(str, nullptr, 8);
                        lexTable.size--;
                    }
                    else
                    {
                        current_entry_IT.value.vint = atoi(str);
                        if (str[0] == '0')
                            current_entry_IT.value.vint = stoi(str, nullptr, 8);
                    }

                    if (trueFlag)
                    {
                        current_entry_IT.iddatatype = IT::BOOL;
                        current_entry_IT.value.vint = 1;
                    }
                    if (falseFlag)
                    {
                        current_entry_IT.iddatatype = IT::BOOL;
                        current_entry_IT.value.vint = 0;
                    }

                    indexIT = IT::search(idTable, current_entry_IT);
                    if (indexIT > 0)
                    {
                        current_entry_LT.idxTI = indexIT;
                    }
                    else {
                        /*sprintf_s(current_entry_IT.id, "L%d", number_literal);*/
                        if (current_entry_IT.value.vint<127 && current_entry_IT.value.vint>-128)
                            current_entry_IT.iddatatype = IT::BYTE;
                        else
                            current_entry_IT.iddatatype = IT::INT;

                        if (trueFlag)
                        {
                            strcpy_s(current_entry_IT.id, "true");
                            current_entry_IT.iddatatype = IT::BOOL;
                            current_entry_IT.value.vint = 1;
                            trueFlag = false;
                        }
                        if (falseFlag)
                        {
                            strcpy_s(current_entry_IT.id, "false");
                            current_entry_IT.iddatatype = IT::BOOL;
                            current_entry_IT.value.vint = 0;
                            falseFlag = false;
                        }

                        current_entry_IT.line = currentLine;
                        current_entry_IT.idxfirstLE = lexTable.size;
                        current_entry_IT.scope = NULL;
                        current_entry_LT.idxTI = idTable.size;
                        IT::Add(idTable, current_entry_IT);
                        number_literal++;
                    }
                    break;
                case LEX_ID:
                    // Проверка области видимости
                    if (scope.empty())
                        current_entry_IT.scope = NULL; // Если стек пуст, область видимости равна NULL
                    else
                        current_entry_IT.scope = scope.top(); // Установка области видимости как вершину стека

                    current_entry_LT.idxTI = idTable.size; // Установка индекса для лексемы
                    memcpy(current_entry_IT.id, str, ID_SIZE); // Копирование идентификатора в таблицу идентификаторов
                    current_entry_IT.id[ID_SIZE] = '\0'; // Завершение строки идентификатора
                    current_entry_IT.iddatatype = IT::INT; // Установка типа данных идентификатора
                    current_entry_IT.value.vint = 0; // Значение инициализировано как 0
                    current_entry_IT.line = currentLine;
                    current_entry_IT.idxfirstLE = lexTable.size;
                    current_entry_IT.idtype = IT::V; // Установка типа идентификатора как переменной

                    // Обработка случая, если предыдущая лексема - это объявление
                    if (lexTable.table[lexTable.size - 2].lexema[0] == LEX_DECLARE)
                    {

                        if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_BYTE && byteFlag)
                        {
                            current_entry_IT.iddatatype = IT::BYTE;
                            current_entry_IT.value.vbyte = 0;
                            byteFlag = false;
                        }

                        if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_CH && chFlag)
                        {
                            current_entry_IT.iddatatype = IT::CH;
                            current_entry_IT.value.vchar = '\0';
                            chFlag = false;
                        }

                        if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_INT && intFlag)
                        {
                            current_entry_IT.iddatatype = IT::INT;
                            current_entry_IT.value.vint = 0;
                            intFlag = false;
                        }


                        if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_STRING && stringFlag)
                        {
                            current_entry_IT.iddatatype = IT::STR;
                            strcpy_s(current_entry_IT.value.vstr->str, "");
                            stringFlag = false;
                        }

                        if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_BOOL && boolFlag)
                        {
                            current_entry_IT.iddatatype = IT::BOOL;
                            current_entry_IT.value.vbool = 0;
                            boolFlag = false;
                        }

                        letFlag = false;
                        current_entry_LT.idxTI = idTable.size; // Установка индекса для лексемы
                        IT::Add(idTable, current_entry_IT); // Добавление идентификатора в таблицу
                        addedToITFlag = true;
                    }

                    // Обработка случая, если предыдущая лексема - это функция
                    if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_FUNCTION)
                    {
                        current_entry_IT.idtype = IT::F;
                        declareFunctionflag = true;

                        if (lexTable.table[lexTable.size - 2].lexema[0] == LEX_BYTE && byteFlag)
                        {
                            current_entry_IT.iddatatype = IT::BYTE;
                            current_entry_IT.value.vbyte = 0;
                            byteFlag = false;
                        }

                        if (lexTable.table[lexTable.size - 2].lexema[0] == LEX_CH && chFlag)
                        {
                            throw ERROR_THROW(105);  // ТИП ФУНКЦИИ НЕ МОЖЕТ БЫТЬ char
                        }

                        if (lexTable.table[lexTable.size - 2].lexema[0] == LEX_INT && intFlag)
                        {
                            current_entry_IT.iddatatype = IT::INT;
                            current_entry_IT.value.vint = 0;
                            intFlag = false;
                        }

                        if (lexTable.table[lexTable.size - 2].lexema[0] == LEX_BOOL && boolFlag)
                        {
                            current_entry_IT.iddatatype = IT::BOOL;
                            current_entry_IT.value.vbool = 0;
                            boolFlag = false;
                        }

                        // Если предыдущая лексема - строка и установлен флаг
                        if (lexTable.table[lexTable.size - 2].lexema[0] == LEX_STRING && stringFlag)
                        {
                            current_entry_IT.iddatatype = IT::STR; // Установка типа данных идентификатора как строки
                            strcpy_s(current_entry_IT.value.vstr->str, ""); // Инициализация строки
                            stringFlag = false; // Сброс флага
                        }

                        indexIT = IT::search(idTable, current_entry_IT); // Поиск идентификатора в таблице
                        if (indexIT != -1) // Если идентификатор уже существует
                        {
                            throw ERROR_THROW(105); // Ошибка: идентификатор уже существует
                        }
                        current_entry_LT.idxTI = idTable.size; // Установка индекса для лексемы
                        IT::Add(idTable, current_entry_IT); // Добавление идентификатора в таблицу
                        addedToITFlag = true; // Установка флага добавления
                    }

                    // Проверка, является ли текущий идентификатор параметром функции
                    if (lexTable.table[lexTable.size - 2].lexema[0] == LEX_LEFTTHESIS &&
                        lexTable.table[lexTable.size - 3].lexema[0] == LEX_ID &&
                        lexTable.table[lexTable.size - 3].idxTI == idTable.size - 1 &&
                        idTable.table[idTable.size - 1].idtype == IT::F)
                    {
                        current_entry_IT.idtype = IT::P; // Установка типа идентификатора как параметра

                        if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_BYTE && byteFlag)
                        {
                            current_entry_IT.iddatatype = IT::BYTE;
                            current_entry_IT.value.vbyte = 0;
                            byteFlag = false;
                        }

                        if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_CH && chFlag)
                        {
                            current_entry_IT.iddatatype = IT::CH;
                            current_entry_IT.value.vchar = '\0';
                            chFlag = false;
                        }

                        if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_INT && intFlag)
                        {
                            current_entry_IT.iddatatype = IT::INT;
                            current_entry_IT.value.vint = 0;
                            intFlag = false;
                        }

                        if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_STRING && stringFlag)
                        {
                            current_entry_IT.iddatatype = IT::STR;
                            strcpy_s(current_entry_IT.value.vstr->str, "");
                            stringFlag = false;
                        }
                        indexIT = IT::search(idTable, current_entry_IT); // Поиск идентификатора в таблице
                        if (indexIT != -1) // Если идентификатор уже существует
                        {
                            throw ERROR_THROW(105); // Ошибка: идентификатор уже существует
                        }
                        current_entry_LT.idxTI = idTable.size; // Установка индекса для лексемы
                        IT::Add(idTable, current_entry_IT); // Добавление идентификатора в таблицу
                        addedToITFlag = true; // Установка флага добавления
                        intFlag = false;
                        byteFlag = false;
                        boolFlag = false;
                        chFlag = false;
                        stringFlag = false;
                    }

                    //несколько параметров
                    if (lexTable.table[lexTable.size - 2].lexema[0] == LEX_COMMA && idTable.table[lexTable.table[lexTable.size - 2].idxTI].idtype == IT::P)
                    {
                        current_entry_IT.idtype = IT::P; // Установка типа идентификатора как параметра

                        if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_BYTE && byteFlag)
                        {
                            current_entry_IT.iddatatype = IT::BYTE;
                            current_entry_IT.value.vbyte = 0;
                            byteFlag = false;
                        }

                        if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_CH && chFlag)
                        {
                            current_entry_IT.iddatatype = IT::CH;
                            current_entry_IT.value.vchar = '\0';
                            chFlag = false;
                        }

                        if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_INT && intFlag)
                        {
                            current_entry_IT.iddatatype = IT::INT;
                            current_entry_IT.value.vint = 0;
                            intFlag = false;
                        }

                        if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_BOOL && boolFlag)
                        {
                            current_entry_IT.iddatatype = IT::BOOL;
                            current_entry_IT.value.vbool = 0;
                            boolFlag = false;
                        }

                        if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_STRING && stringFlag)
                        {
                            current_entry_IT.iddatatype = IT::STR;
                            strcpy_s(current_entry_IT.value.vstr->str, "");
                            stringFlag = false;
                        }

                        indexIT = IT::search(idTable, current_entry_IT); // Поиск идентификатора в таблице

                        if (indexIT != -1) // Если идентификатор уже существует
                        {
                            throw ERROR_THROW(105); // Генерация ошибки: идентификатор уже существует
                        }

                        IT::Add(idTable, current_entry_IT); // Добавление идентификатора в таблицу
                        addedToITFlag = true;
                        intFlag = false;
                        byteFlag = false;
                        boolFlag = false;
                        chFlag = false;
                        stringFlag = false;
                    }

                    if (!addedToITFlag) // Если идентификатор не был добавлен
                    {
                        indexIT = IT::search(idTable, current_entry_IT); // Повторный поиск идентификатора в таблице

                        if (indexIT >= 0) // Если идентификатор найден
                        {

                            current_entry_LT.idxTI = indexIT; // Установка индекса идентификатора для лексемы
                        }
                    }

                    memset(current_entry_IT.id, NULL, ID_SIZE); // Обнуление идентификатора
                    current_entry_IT.iddatatype = IT::INT; // Установка типа данных идентификатора как байт числа
                    current_entry_IT.value.vint = NULL; // Инициализация значения идентификатора
                    addedToITFlag = false;
                    break;

                case LEX_DECLARE:
                    letFlag = true;
                    break;

                case LEX_REST:
                case LEX_MODULE:
                    current_entry_LT.idxTI = idTable.size;
                    memcpy(current_entry_IT.id, str, 5);
                    current_entry_IT.id[5] = '\0';
                    current_entry_IT.iddatatype = IT::BYTE;
                    current_entry_IT.idtype = IT::SF;
                    current_entry_IT.value.vint = NULL;
                    current_entry_IT.line = currentLine;
                    current_entry_IT.idxfirstLE = lexTable.size;
                    if (!scope.empty())
                        current_entry_IT.scope = scope.top();
                    else
                        current_entry_IT.scope = NULL;

                    current_entry_LT.idxTI = idTable.size;
                    IT::Add(idTable, current_entry_IT);
  
                    break;
                }

                bufferIndex = 0;
                memset(str, 0, sizeof(str));;
            }
            if (current_entry_LT.lexema[0] != NULL)    //если лексема распозналась
            {
                current_entry_LT.sn = currentLine;    // добавляем ее в таблицу
                LT::Add(lexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;    // обнуляем лексему
            }

            if (lexTable.table[lexTable.size - 3].lexema[0] == LEX_DECLARE && keyWord) {
                throw ERROR_THROW(113);
            }

            keyWord = false;

            if ((literalFlag && in.text[i] != '\'') && (literalFlag && in.text[i] != '"'))
                continue;

            /*-----------------------------------------------------------------------------------------------------------------*/
            switch (in.text[i])
            {

            case SINGLE_QUOTE:
                literalFlag = true;
                if (str[1] == SINGLE_QUOTE && bufferIndex != 1 && str[2] != SINGLE_QUOTE) // Учитываем длину 1
                {
                    current_entry_IT.iddatatype = IT::CH; // Тип данных — символ
                    current_entry_IT.value.vchar = str[2]; // Сохраняем символ из строки

                    number_literal++;

                    current_entry_LT.idxTI = idTable.size;
                    str[bufferIndex] = '\0';
                    literalFlag = false;
                    current_entry_LT.lexema[0] = LEX_LITERAL;
                    sprintf_s(current_entry_IT.id, "C%d", number_literal); // ID литерала
                    current_entry_IT.idtype = IT::L;
                    current_entry_IT.line = currentLine;
                    current_entry_IT.idxfirstLE = lexTable.size;

                    current_entry_LT.sn = currentLine;
                    if (!scope.empty())
                        current_entry_IT.scope = scope.top();
                    else
                        current_entry_IT.scope = NULL;

                    LT::Add(lexTable, current_entry_LT); // Добавляем в таблицу лексем
                    IT::Add(idTable, current_entry_IT); // Добавляем в таблицу идентификаторов
                    memset(str, 0, strlen(str));
                    current_entry_LT.lexema[0] = NULL;
                    break;
                }
                str[bufferIndex] = SINGLE_QUOTE;
                bufferIndex++;
                break;
            case DOUBLE_QUOTE:

                literalFlag = true;
                if (str[0] == DOUBLE_QUOTE && bufferIndex != 1)
                {
                    current_entry_IT.iddatatype = IT::STR;
                    for (int i = 0; i < bufferIndex; i++)
                    {
                        current_entry_IT.value.vstr->str[i] = str[i];
                    }
                    current_entry_IT.value.vstr->str[bufferIndex] = '\0';
                    current_entry_IT.value.vstr->len = bufferIndex;
                    number_literal++;


                    current_entry_LT.idxTI = idTable.size;
                    str[bufferIndex] = '\0';
                    literalFlag = false;
                    current_entry_LT.lexema[0] = LEX_LITERAL;
                    sprintf_s(current_entry_IT.id, "L%d", number_literal);
                    current_entry_IT.idtype = IT::L;
                    current_entry_IT.line = currentLine;
                    current_entry_IT.idxfirstLE = lexTable.size;
                    
                    current_entry_LT.sn = currentLine;
                    if (!scope.empty())
                        current_entry_IT.scope = scope.top();
                    else
                        current_entry_IT.scope = NULL;
                    number_literal++;
                    LT::Add(lexTable, current_entry_LT);
                    IT::Add(idTable, current_entry_IT);
                    memset(str, 0, strlen(str));
                    current_entry_LT.lexema[0] = NULL;
                    break;
                }
                /*bufferIndex++;*/
                break;
            case NEW_LINE:
                current_entry_LT.sn = currentLine++;
                current_entry_LT.lexema[0] = NULL;
                break;
            case SEMICOLON:
                current_entry_LT.lexema[0] = LEX_SEMICOLON;
                current_entry_LT.sn = currentLine;
                LT::Add(lexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;
                break;
            case LEFT_BRACE:
                
                current_entry_LT.lexema[0] = LEX_LEFTBRACE;
                current_entry_LT.sn = currentLine;
                LT::Add(lexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;

                if (mainFlag)
                {
                    scope.push(&idTable.table[idTable.size - 1]);
                }
                else {
                    for (int j = idTable.size - 1; j >= 0; j--) // Перебор таблицы идентификаторов с конца
                    {
                        if (idTable.table[j].idtype == IT::F) // Если идентификатор - функция
                        {
                            scope.push(&idTable.table[j]); // Добавление функции в область видимости
                            break; // Выход из цикла
                        }
                    }
                }
                break;
            case RIGHT_BRACE:
                current_entry_LT.lexema[0] = LEX_RIGHTBRACE;
                current_entry_LT.sn = currentLine;
                LT::Add(lexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;
                if (!scope.empty()) // Если стек области видимости не пуст
                    scope.pop(); // Удаление верхнего элемента из стека
                break;
            case LEFTTHESIS:
                current_entry_LT.lexema[0] = LEX_LEFTTHESIS;
                current_entry_LT.sn = currentLine;
                LT::Add(lexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;
                if (declareFunctionflag)
                {
                    for (int j = idTable.size - 1; j >= 0; j--) // Перебор таблицы идентификаторов с конца
                    {
                        if (idTable.table[j].idtype == IT::F) // Если идентификатор - функция
                        {
                            scope.push(&idTable.table[j]); // Добавление функции в область видимости
                            break; // Выход из цикла
                        }
                    }
                }
                break;
            case RIGHTTHESIS:
                current_entry_LT.lexema[0] = LEX_RIGHTTHESIS;
                current_entry_LT.sn = currentLine;
                LT::Add(lexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;
                parmFlag = false;
                if (!scope.empty() && declareFunctionflag)
                {
                    scope.pop(); // Удаление верхнего элемента из стека области видимости
                    declareFunctionflag = false;
                }
                break;
 
            case COMMA:
            case EQUAL:
            case TILDE:
                current_entry_LT.lexema[0] = in.text[i];
                current_entry_LT.sn = currentLine;
                LT::Add(lexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;
                break;

            case AMPERSAND:
                current_entry_LT.lexema[0] = LEX_OPERATION;
                current_entry_LT.lexema[1] = AMPERSAND;
                current_entry_LT.sn = currentLine;
                LT::Add(lexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;
                current_entry_LT.lexema[1] = NULL;
                break;
            case PIPE:
                current_entry_LT.lexema[0] = LEX_OPERATION;
                current_entry_LT.lexema[1] = PIPE;
                current_entry_LT.sn = currentLine;
                LT::Add(lexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;
                current_entry_LT.lexema[1] = NULL;
                break;
            case PLUS:
                current_entry_LT.lexema[0] = LEX_OPERATION;
                current_entry_LT.lexema[1] = PLUS;
                current_entry_LT.sn = currentLine;
                LT::Add(lexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;
                current_entry_LT.lexema[1] = NULL;
                break;
            case MINUS:
                current_entry_LT.lexema[0] = LEX_OPERATION;
                current_entry_LT.lexema[1] = MINUS;
                current_entry_LT.sn = currentLine;
                LT::Add(lexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;
                current_entry_LT.lexema[1] = NULL;
                break;
            case STAR:
                current_entry_LT.lexema[0] = LEX_OPERATION;
                current_entry_LT.lexema[1] = STAR;
                current_entry_LT.sn = currentLine;
                LT::Add(lexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;
                current_entry_LT.lexema[1] = NULL;
                break;
            case DIRSLASH:
                current_entry_LT.lexema[0] = LEX_OPERATION;
                current_entry_LT.lexema[1] = DIRSLASH;
                current_entry_LT.sn = currentLine;
                LT::Add(lexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;
                current_entry_LT.lexema[1] = NULL;
                break;

            case LEX_GREATER:
                current_entry_LT.lexema[0] = LEX_GREATER;
                current_entry_LT.sn = currentLine;
                LT::Add(lexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;
                break;

            case LEX_LESS:
                current_entry_LT.lexema[0] = LEX_LESS;
                current_entry_LT.sn = currentLine;
                LT::Add(lexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;
                break;
            }
        }

        currentLine = 1;
        LT_file << currentLine;
        LT_file << '\t';
        for (int i = 0; i < lexTable.size; i++)
        {
            current_entry_LT = LT::GetEntry(lexTable, i);
            if (currentLine != current_entry_LT.sn)
            {
                currentLine = current_entry_LT.sn;
                LT_file << '\n';
                LT_file << currentLine;
                LT_file << '\t';
            }

            LT_file << current_entry_LT.lexema[0];
        }
        LT_file.close();
        IT_file << std::setw(5) << "id"
            << std::setw(15) << "datatype"
            << std::setw(10) << "idtype"
            << std::setw(10) << "Line"
            << std::setw(10) << "Scope"
            << std::setw(10) << "value" << std::endl;

        for (int i = 0; i < idTable.size; i++) {
            IT::Entry temp_entry = IT::GetEntry(idTable, i);
            IT_file << std::setw(5) << temp_entry.id << "_" << temp_entry.line;
            if (temp_entry.iddatatype == 1)  IT_file << std::setw(10) << "INT";
            if (temp_entry.iddatatype == 2)  IT_file << std::setw(10) << "STR";
            if (temp_entry.iddatatype == 3)  IT_file << std::setw(10) << "BYTE";
            if (temp_entry.iddatatype == 4)  IT_file << std::setw(10) << "CH";
            if (temp_entry.iddatatype == 5)  IT_file << std::setw(10) << "BOOl";

            if (temp_entry.idtype == IT::V)  IT_file << std::setw(10) << "V";
            if (temp_entry.idtype == IT::L)  IT_file << std::setw(10) << "L";
            if (temp_entry.idtype == IT::F)  IT_file << std::setw(10) << "F";
            if (temp_entry.idtype == IT::P)  IT_file << std::setw(10) << "P";
            if (temp_entry.idtype == IT::SF)  IT_file << std::setw(10) << "SF";
            if (temp_entry.idtype == IT::M)  IT_file << std::setw(10) << "M";

            IT_file << std::setw(10) << temp_entry.line;

            IT_file << std::setw(10);

            if (temp_entry.scope != NULL) {
                for (int j = 0; j < strlen(temp_entry.scope->id); j++)
                {
                    IT_file << temp_entry.scope->id[j];
                }
            }

            if (temp_entry.scope == NULL) {
                IT_file << "-" << std::setw(20);
            }

            if (temp_entry.iddatatype == IT::BYTE) IT_file << std::setw(10) << int(temp_entry.value.vbyte);
            if (temp_entry.iddatatype == IT::INT) IT_file << std::setw(10) << temp_entry.value.vint;
            if (temp_entry.iddatatype == IT::BOOL) IT_file << std::setw(10) << temp_entry.value.vbool;
            if (temp_entry.iddatatype == IT::CH) IT_file << std::setw(10) << SINGLE_QUOTE << temp_entry.value.vchar << SINGLE_QUOTE;
          
            if (temp_entry.iddatatype == IT::STR) {
                IT_file << std::setw(7);
                for (int j = 0; j < strlen(temp_entry.value.vstr->str); j++) {
                    IT_file << temp_entry.value.vstr->str[j];
                }
                IT_file << std::setw(10);
            }

            IT_file << std::endl;
        }
        IT_file.close();

        tables.idTable = idTable;
        tables.lexTable = lexTable;
        return tables;
    }
}