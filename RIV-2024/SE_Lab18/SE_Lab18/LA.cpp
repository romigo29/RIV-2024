#include"LA.h"
#include"stdafx.h"
#include"In.h"
#include"Error.h"
#include <stdio.h>
#include <string.h>
#include <iomanip>

namespace LA
{
    LT::LexTable LexTable = LT::Create(LT_MAXSIZE - 1);  //Таблица для хранения лексем
    IT::IdTable IDTable = IT::Create(TI_MAXSIZE - 1);    //Таблица для хранения идентификаторов
    //различные состояния программы
    bool byteFlag = false;
    bool intFlag = false;
    bool chFlag = false;
    bool stringFlag = false;
    bool parmFlag = false;
    bool mainFlag = false;
    bool declareFunctionflag = false;
    bool addedToITFlag = false; // Флаг добавления в таблицу идентификаторов
    bool literalFlag = false;
    char* str = new char[MAX_LEX_SIZE];    //хранение текущей лексемы

    char FST()
    {
        FST_BYTE
        FST_INT
            FST_CH
            FST_STR
            FST_FUNC
            FST_LET
            FST_RETURN
            FST_MAIN
            FST_PRINT
            FST_LITERAL
            FST_IF
            FST_IDENF
  

            FstLexeme lexemes[] = {
            {_byte, LEX_BYTE, &byteFlag},
            {_int, LEX_INT, &intFlag},
            {_ch, LEX_CH, &chFlag},
            {_string, LEX_STRING, &stringFlag},
            {_function, LEX_FUNCTION, nullptr},
            {_let, LEX_DECLARE, nullptr},
            {_return, LEX_RETURN, nullptr},
            {_main, LEX_MAIN, &mainFlag},
            {_print, LEX_PRINT, nullptr},
            {literal_int, LEX_LITERAL, nullptr},
            {_if, LEX_IF, nullptr},
            {idenf, LEX_ID, nullptr}
            
        };

        for (int i = 0; i < FST_AMOUNT; i++) {
            if (Execute(lexemes[i].fst)) {
                if (lexemes[i].flag) {
                    *lexemes[i].flag = true;
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
        LexTable.size = 0;
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
                    current_entry_LT.idxTI = IDTable.size;
                    memcpy(current_entry_IT.id, str, 5);
                    current_entry_IT.id[5] = '\0';
                    current_entry_IT.iddatatype = IT::INT;
                    current_entry_IT.value.vint = NULL;
                    current_entry_IT.idxfirstLE = currentLine;
                    current_entry_IT.scope = NULL;
                    /*scope.pop();*/
                    indexIT = IT::search(IDTable, current_entry_IT);
                    if (indexIT > 0)
                    {
                        throw ERROR_THROW(105);
                    }
                    if (indexIT == -1)
                    {
                        current_entry_LT.idxTI = IDTable.size;
                        IT::Add(IDTable, current_entry_IT);
                    }
                    break;
                case LEX_LITERAL:
                    current_entry_IT.idtype = IT::L;
                    current_entry_IT.iddatatype = IT::INT;
                    sprintf_s(current_entry_IT.id, "L%d", number_literal);
                    if (in.text[i - strlen(str) - 1] == MINUS && in.text[i - strlen(str)] != SPACE)
                    {
                        current_entry_IT.value.vint = -atoi(str);
                        if (str[0] == '0')
                            current_entry_IT.value.vint = -stoi(str, nullptr, 8);
                        LexTable.size--;
                    }
                    else
                    {
                        current_entry_IT.value.vint = atoi(str);
                        if (str[0] == '0')
                            current_entry_IT.value.vint = stoi(str, nullptr, 8);
                    }
                   
                    indexIT = IT::search(IDTable, current_entry_IT);
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
                        
                        current_entry_IT.idxfirstLE = currentLine;
                        current_entry_IT.scope = NULL;
                        current_entry_LT.idxTI = IDTable.size;
                        IT::Add(IDTable, current_entry_IT);
                        number_literal++;
                    }
                    break;
                case LEX_ID:
                    // Проверка области видимости
                    if (scope.empty())
                        current_entry_IT.scope = NULL; // Если стек пуст, область видимости равна NULL
                    else
                        current_entry_IT.scope = scope.top(); // Установка области видимости как вершину стека


                    current_entry_LT.idxTI = IDTable.size; // Установка индекса для лексемы
                    memcpy(current_entry_IT.id, str, ID_SIZE); // Копирование идентификатора в таблицу идентификаторов
                    current_entry_IT.id[ID_SIZE] = '\0'; // Завершение строки идентификатора
                    current_entry_IT.iddatatype = IT::INT; // Установка типа данных идентификатора
                    current_entry_IT.value.vint = 0; // Значение инициализировано как 0
                    current_entry_IT.idxfirstLE = currentLine;
                    current_entry_IT.idtype = IT::V; // Установка типа идентификатора как переменной

                    // Обработка случая, если предыдущая лексема - это объявление
                    if (LexTable.table[LexTable.size - 2].lexema[0] == LEX_DECLARE)
                    {

                        if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_BYTE && byteFlag)
                        {
                            current_entry_IT.iddatatype = IT::BYTE; 
                            current_entry_IT.value.vbyte = 0;
                            byteFlag = false; 
                        }

                        if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_CH && chFlag)
                        {
                            current_entry_IT.iddatatype = IT::CH; 
                            current_entry_IT.value.vchar = '\0';
                            chFlag = false; 
                        }

                        if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_INT && intFlag)
                        {
                            current_entry_IT.iddatatype = IT::INT;
                            current_entry_IT.value.vint = 0;
                            intFlag = false; 
                        }

                    
                        if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_STRING && stringFlag)
                        {
                            current_entry_IT.iddatatype = IT::STR; 
                            strcpy_s(current_entry_IT.value.vstr->str, ""); 
                            stringFlag = false;
                        }
                        current_entry_LT.idxTI = IDTable.size; // Установка индекса для лексемы
                        IT::Add(IDTable, current_entry_IT); // Добавление идентификатора в таблицу
                        addedToITFlag = true; 
                    }

                    // Обработка случая, если предыдущая лексема - это функция
                    if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_FUNCTION)
                    {
                        current_entry_IT.idtype = IT::F; 
                        declareFunctionflag = true; 

                        if (LexTable.table[LexTable.size - 2].lexema[0] == LEX_BYTE && byteFlag)
                        {
                            current_entry_IT.iddatatype = IT::BYTE; 
                            current_entry_IT.value.vbyte = 0;
                            byteFlag = false;
                        }

                        if (LexTable.table[LexTable.size - 2].lexema[0] == LEX_CH && chFlag)
                        {
                            ERROR_THROW(105);  // ТИП ФУНКЦИИ НЕ МОЖЕТ БЫТЬ char
                        }

                        if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_INT && intFlag)
                        {
                            current_entry_IT.iddatatype = IT::INT;
                            current_entry_IT.value.vint = 0;
                            intFlag = false;
                        }

                        // Если предыдущая лексема - строка и установлен флаг
                        if (LexTable.table[LexTable.size - 2].lexema[0] == LEX_STRING && stringFlag)
                        {
                            current_entry_IT.iddatatype = IT::STR; // Установка типа данных идентификатора как строки
                            strcpy_s(current_entry_IT.value.vstr->str, ""); // Инициализация строки
                            stringFlag = false; // Сброс флага
                        }

                        indexIT = IT::search(IDTable, current_entry_IT); // Поиск идентификатора в таблице
                        if (indexIT != -1) // Если идентификатор уже существует
                        {
                            throw ERROR_THROW(105); // Ошибка: идентификатор уже существует
                        }
                        current_entry_LT.idxTI = IDTable.size; // Установка индекса для лексемы
                        IT::Add(IDTable, current_entry_IT); // Добавление идентификатора в таблицу
                        addedToITFlag = true; // Установка флага добавления
                    }

                    // Проверка, является ли текущий идентификатор параметром функции
                    if (LexTable.table[LexTable.size - 2].lexema[0] == LEX_LEFTTHESIS &&
                        LexTable.table[LexTable.size - 3].lexema[0] == LEX_ID &&
                        LexTable.table[LexTable.size - 3].idxTI == IDTable.size - 1 &&
                        IDTable.table[IDTable.size - 1].idtype == IT::F) 
                    {
                        current_entry_IT.idtype = IT::P; // Установка типа идентификатора как параметра
    
                        if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_BYTE && byteFlag)
                        {
                            current_entry_IT.iddatatype = IT::BYTE;
                            current_entry_IT.value.vbyte = 0;
                            byteFlag = false;
                        }

                        if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_CH && chFlag)
                        {
                            current_entry_IT.iddatatype = IT::CH;
                            current_entry_IT.value.vchar = '\0';
                            chFlag = false;
                        }

                        if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_INT && intFlag)
                        {
                            current_entry_IT.iddatatype = IT::INT;
                            current_entry_IT.value.vint = 0;
                            intFlag = false;
                        }

                        if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_STRING && stringFlag)
                        {
                            current_entry_IT.iddatatype = IT::STR; 
                            strcpy_s(current_entry_IT.value.vstr->str, "");
                            stringFlag = false; 
                        }
                        indexIT = IT::search(IDTable, current_entry_IT); // Поиск идентификатора в таблице
                        if (indexIT != -1) // Если идентификатор уже существует
                        {
                            throw ERROR_THROW(105); // Ошибка: идентификатор уже существует
                        }
                        current_entry_LT.idxTI = IDTable.size; // Установка индекса для лексемы
                        IT::Add(IDTable, current_entry_IT); // Добавление идентификатора в таблицу
                        addedToITFlag = true; // Установка флага добавления
                        intFlag = false;
                        byteFlag = false;
                        chFlag = false;
                        stringFlag = false;
                    }

                    //несколько параметров
                    if (LexTable.table[LexTable.size - 2].lexema[0] == LEX_COMMA && IDTable.table[LexTable.table[LexTable.size - 2].idxTI].idtype == IT::P)
                    {
                        current_entry_IT.idtype = IT::P; // Установка типа идентификатора как параметра

                        if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_BYTE && byteFlag)
                        {
                            current_entry_IT.iddatatype = IT::BYTE;
                            current_entry_IT.value.vbyte = 0;
                            byteFlag = false;
                        }

                        if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_CH && chFlag)
                        {
                            current_entry_IT.iddatatype = IT::CH;
                            current_entry_IT.value.vchar = '\0';
                            chFlag = false;
                        }

                        if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_INT && intFlag)
                        {
                            current_entry_IT.iddatatype = IT::INT;
                            current_entry_IT.value.vint = 0;
                            intFlag = false;
                        }

                        if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_STRING && stringFlag)
                        {
                            current_entry_IT.iddatatype = IT::STR; 
                            strcpy_s(current_entry_IT.value.vstr->str, "");
                            stringFlag = false; 
                        }

                        indexIT = IT::search(IDTable, current_entry_IT); // Поиск идентификатора в таблице

                        if (indexIT != -1) // Если идентификатор уже существует
                        {
                            throw ERROR_THROW(105); // Генерация ошибки: идентификатор уже существует
                        }

                        IT::Add(IDTable, current_entry_IT); // Добавление идентификатора в таблицу
                        addedToITFlag = true; 
                        intFlag = false;
                        byteFlag = false;
                        chFlag = false;
                        stringFlag = false;
                    }

                    if (!addedToITFlag) // Если идентификатор не был добавлен
                    {
                        indexIT = IT::search(IDTable, current_entry_IT); // Повторный поиск идентификатора в таблице

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
                }

                bufferIndex = 0;
                memset(str, 0, sizeof(str));;
            }
            if (current_entry_LT.lexema[0] != NULL)    //если лексема распозналась
            {
                current_entry_LT.sn = currentLine;    // добавляем ее в таблицу
                LT::Add(LexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;    // обнуляем лексему
            }

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

                    current_entry_LT.idxTI = IDTable.size;
                    str[bufferIndex] = '\0';
                    literalFlag = false;
                    current_entry_LT.lexema[0] = LEX_LITERAL;
                    sprintf_s(current_entry_IT.id, "C%d", number_literal); // ID литерала
                    current_entry_IT.idtype = IT::L;
                    current_entry_IT.idxfirstLE = currentLine;

                    current_entry_LT.sn = currentLine;
                    if (!scope.empty())
                        current_entry_IT.scope = scope.top();
                    else
                        current_entry_IT.scope = NULL;

                    LT::Add(LexTable, current_entry_LT); // Добавляем в таблицу лексем
                    IT::Add(IDTable, current_entry_IT); // Добавляем в таблицу идентификаторов
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


                    current_entry_LT.idxTI = IDTable.size;
                    str[bufferIndex] = '\0';
                    literalFlag = false;
                    current_entry_LT.lexema[0] = LEX_LITERAL;
                    sprintf_s(current_entry_IT.id, "L%d", number_literal);
                    current_entry_IT.idtype = IT::L;
                    current_entry_IT.idxfirstLE = currentLine;
                    
                    current_entry_LT.sn = currentLine;
                    if (!scope.empty())
                        current_entry_IT.scope = scope.top();
                    else
                        current_entry_IT.scope = NULL;
                    number_literal++;
                    LT::Add(LexTable, current_entry_LT);
                    IT::Add(IDTable, current_entry_IT);
                    memset(str, 0, strlen(str));
                    current_entry_LT.lexema[0] = NULL;
                    break;
                }
                str[bufferIndex] = DOUBLE_QUOTE;
                bufferIndex++;
                break;
            case NEW_LINE:
                current_entry_LT.sn = currentLine++;
                current_entry_LT.lexema[0] = NULL;
                break;
            case SEMICOLON:
                current_entry_LT.lexema[0] = LEX_SEMICOLON;
                current_entry_LT.sn = currentLine;
                LT::Add(LexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;
                /*if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_BRACELET)
                {
                    scope.pop();
                    declareFunctionflag = false;
                }*/
                break;
            case LEFT_BRACE:
                
                current_entry_LT.lexema[0] = LEX_LEFTBRACE;
                current_entry_LT.sn = currentLine;
                LT::Add(LexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;

                if (mainFlag)
                {
                    scope.push(&IDTable.table[IDTable.size - 1]);
                }
                else {
                    for (int j = IDTable.size - 1; j >= 0; j--) // Перебор таблицы идентификаторов с конца
                    {
                        if (IDTable.table[j].idtype == IT::F) // Если идентификатор - функция
                        {
                            scope.push(&IDTable.table[j]); // Добавление функции в область видимости
                            break; // Выход из цикла
                        }
                    }
                }
                break;
            case RIGHT_BRACE:
                current_entry_LT.lexema[0] = LEX_BRACELET;
                current_entry_LT.sn = currentLine;
                LT::Add(LexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;
                if (!scope.empty()) // Если стек области видимости не пуст
                    scope.pop(); // Удаление верхнего элемента из стека
                break;
            case LEFTTHESIS:
                current_entry_LT.lexema[0] = LEX_LEFTTHESIS;
                current_entry_LT.sn = currentLine;
                LT::Add(LexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;
                if (declareFunctionflag)
                {
                    for (int j = IDTable.size - 1; j >= 0; j--) // Перебор таблицы идентификаторов с конца
                    {
                        if (IDTable.table[j].idtype == IT::F) // Если идентификатор - функция
                        {
                            scope.push(&IDTable.table[j]); // Добавление функции в область видимости
                            break; // Выход из цикла
                        }
                    }
                }
                break;
            case RIGHTTHESIS:
                current_entry_LT.lexema[0] = LEX_RIGHTTHESIS;
                current_entry_LT.sn = currentLine;
                LT::Add(LexTable, current_entry_LT);
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
                LT::Add(LexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;
                break;

            case AMPERSAND:
            case PIPE:
            case PLUS:
            case MINUS:
            case STAR:
            case DIRSLASH:
                current_entry_LT.lexema[0] = LEX_OPERATION;
                current_entry_LT.sn = currentLine;
                LT::Add(LexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;
                break;

            case LEX_GREATER:
                current_entry_LT.lexema[0] = LEX_GREATER;
                if (in.text[i + 1] == LEX_EQUAL)
                {
                    current_entry_LT.lexema[0] = LEX_GE;
                    i++;
                    current_entry_LT.sn = currentLine;
                    LT::Add(LexTable, current_entry_LT);
                    current_entry_LT.lexema[0] = NULL;
                    break;
                }

            case LEX_LESS:
                current_entry_LT.lexema[0] = LEX_LESS;
                if (in.text[i + 1] == LEX_EQUAL)
                {
                    current_entry_LT.lexema[0] = LEX_LE;
                    i++;
                }
                current_entry_LT.sn = currentLine;
                LT::Add(LexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;
                break;
            }
        }

        currentLine = 1;
        LT_file << currentLine;
        LT_file << '\t';
        for (int i = 0; i < LexTable.size; i++)
        {
            current_entry_LT = LT::GetEntry(LexTable, i);
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

        for (int i = 0; i < IDTable.size; i++) {
            IT::Entry temp_entry = IT::GetEntry(IDTable, i);
            IT_file << std::setw(5) << temp_entry.id << "_" << temp_entry.idxfirstLE;
            if (temp_entry.iddatatype == 1)  IT_file << std::setw(10) << "INT";
            if (temp_entry.iddatatype == 2)  IT_file << std::setw(10) << "STR";
            if (temp_entry.iddatatype == 3)  IT_file << std::setw(10) << "BYTE";
            if (temp_entry.iddatatype == 4)  IT_file << std::setw(10) << "CH";

            if (temp_entry.idtype == IT::V)  IT_file << std::setw(10) << "V";
            if (temp_entry.idtype == IT::L)  IT_file << std::setw(10) << "L";
            if (temp_entry.idtype == IT::F)  IT_file << std::setw(10) << "F";
            if (temp_entry.idtype == IT::P)  IT_file << std::setw(10) << "P";

            IT_file << std::setw(10) << temp_entry.idxfirstLE;

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
            if (temp_entry.iddatatype == IT::CH) IT_file << std::setw(10) << SINGLE_QUOTE << temp_entry.value.vchar << SINGLE_QUOTE;
          
            if (temp_entry.iddatatype == IT::STR) {
                IT_file << std::setw(7);
                for (int j = 1; j < strlen(temp_entry.value.vstr->str); j++) {
                    IT_file << temp_entry.value.vstr->str[j];
                }
                IT_file << std::setw(10);
            }

            IT_file << std::endl;
        }
        IT_file.close();

        tables.idTable = IDTable;
        tables.lexTable = LexTable;
        return tables;
    }
}