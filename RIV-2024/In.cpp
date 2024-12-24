#include "stdafx.h"
#include "In.h"
#include "Error.h"

namespace In {     
    IN getin(wchar_t infile[]) {
        IN in; 
        int cols = 1, error_pos = 0;
        in.text = new unsigned char[IN_MAX_LEN_TEXT];
        in.forbiddenChar = new unsigned char[IN_MAX_LEN_TEXT];
        in.errorLine = new int[IN_MAX_LEN_TEXT];
        in.errorCol = new int[IN_MAX_LEN_TEXT];
        bool isSingleQuotesOpened = false;
        bool isDoubleQuotesOpened = false;

        ifstream fin(infile);

        if (!fin.is_open()) {   
            throw ERROR_THROW(110);
        }

        char* buff = new char[BUFF_SIZE];  

        while (fin.getline(buff, BUFF_SIZE)) {  

            in.lines++;
            cols = 1;

            for (int position = 0; position < strlen(buff); position++)  {
       
                switch (in.code[int((unsigned char)buff[position])])  {

                case IN::T: 
                    in.text[in.size++] = (unsigned)buff[position];  
                    cols++;
                    break;

                case IN::I: 
                    in.ignore++;
                    cols++;  

                    break;

                case IN::F:  
                    in.forbiddenChar[error_pos] = buff[position];
                    in.text[in.size++] = '^';   // Запрещенный символ помечается символом '^'
                    in.errorLine[error_pos] = in.lines;  
                    in.errorCol[error_pos++] = position + 1;  
                    in.error_size++;
                    throw ERROR_THROW_IN(121, in.lines, cols);
                    break;
                    
                case IN::P:

                        if ((buff[position - 1] == SPACE || position == 0 || position == strlen(buff) - 1) && (!isSingleQuotesOpened && !isDoubleQuotesOpened)) {
                            in.ignore++;
                        }
                        else {
                            in.text[in.size++] = (unsigned)SPACE;
                        }
                        break;

                case IN::S: 

                    if (buff[position - 1] == SPACE && in.code[in.text[in.size - 1]] != IN::S) {
                        in.text[in.size - 1] = (unsigned)buff[position];
                        in.ignore++;

                        if (buff[position + 1] == SPACE) {
                            position++;
                            in.ignore++;
                        }
                    }
                    else if (buff[position + 1] == SPACE) {
                        in.text[in.size++] = (unsigned)buff[position];
                        position++;
                        in.ignore++;
                    }
                    
                    else {
                        in.text[in.size++] = (unsigned)buff[position];
                    }

                    //кавычки
                    if (buff[position] == SINGLE_QUOTE) {
                        if (!isSingleQuotesOpened && !isDoubleQuotesOpened) {
                            isSingleQuotesOpened = true;
                        }

                        else {
                            isSingleQuotesOpened = false;
                        }
                    }

                    else if (buff[position] == DOUBLE_QUOTE) {
                        if (!isDoubleQuotesOpened && !isSingleQuotesOpened) {
                            isDoubleQuotesOpened = true;
                        }

                        else {
                            isDoubleQuotesOpened = false;
                        }
                    }

                    break;

                default:  
                    in.text[in.size++] = static_cast<unsigned char>(in.code[buff[position]]);
                    cols++;
                    break;
                }
            }
            if (!fin.eof())
            {
                in.text[in.size++] = '\n';
            }
        }
  
        in.text[in.size] = '\0';   
        fin.close();           
        delete[] buff;

        if (isSingleQuotesOpened || isDoubleQuotesOpened) {
            throw ERROR_THROW(122);
        }

        return in; 
    }
    void deleteIn(IN in)
    {
        delete[] in.text;
        delete[] in.forbiddenChar;
        delete[] in.errorLine;
        delete[] in.errorCol;
    }
}
