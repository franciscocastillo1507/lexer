#include <stdio.h>
#include <string>
#include <fstream> // Función de biblioteca de flujo de archivos
#include <iostream>
#include <map>
#include <list>

// Author: Francisco Javier Castillo Hernandez
// Date Release : 16/3/22
// Tecnologico de Monterrey Campus Qro
// Compiladores (Primera parte)

using namespace std;
// Class Tag
class Tag
{
public:
    static const int ENOF = 65535,
                     NUMBER = 300, ID = 301,
                     ERROR = 302;
};
// Class Token
class Token
{
private:
    string tagToken;

public:
    Token(string tagG)
    {
        tagToken = tagG;
    }
    string toStringToken();
};

string Token::toStringToken()
{
    return tagToken;
}

// Class Word
class Word : public Token
{
private:
    string _lexeme;
    int _tag;

public:
    Word(string lexeme, int tag) : Token(lexeme)
    {
        _lexeme = lexeme;
        _tag = tag;
    }
    void toStringWord();
};

void Word::toStringWord()
{
    cout << "word, lexeme = " << _lexeme << endl;
}

// Class Number
class Number
{
private:
    int tag;
    int value;

public:
    Number(int valueL)
    {
        tag = Tag::NUMBER;
        value = valueL;
    }

    int getValue()
    {
        return value;
    }
    void toStringNumber();
};

void Number::toStringNumber()
{
    cout << "Number, value = " << value << endl;
}

char singleOperator[24][10] = { // Codificación de operador de un solo byte
    "+", "-", "*", "/", "<", ">",
    ";", "(", ")", "^", ",", "#",
    "|", "%", "~", "[", "]", "{",
    "}", ".", ":", "!", "=", "&"};

static char mutilOperator[14][10] = { // Codificación de operador de doble byte
    "++", "--", "==", "<=", ">=", "<<",
    ">>", "&&", "||", "!=", "\\", "\?",
    "\"", "\'"};

// Determinar si es una letra
int isLetter(char ch)
{
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
    {
        return 1;
    }
    return 0;
}

// Determina si es un número
int isDigit(char ch)
{
    if ((ch >= '0' && ch <= '9'))
    {
        return 1;
    }
    return 0;
}
// Determinar si es un operador
int isOperater(char ch)
{
    int i;
    for (i = 0; i < sizeof(singleOperator) / 10; i++)
    {
        if (ch == singleOperator[i][0])
        {
            return 1;
        }
    }
    return 0;
}

void clear(bool isReadingWord, bool isReadingNumber, string wordAux, int v, bool isReadingOperator, string opAux)
{
    isReadingWord = false;
    isReadingNumber = false;
    isReadingOperator = false;
    wordAux = "";
    opAux = "";
    v = 0;
}

// Escanea un caracter a la vez
void Scanner(string &filePath)
{

    map<int, Token> words = map<int, Token>();
    int line = 0;
    char ch;
    bool isReadingWord = false;
    bool isReadingNumber = false;
    bool isReadingOperator = false;
    string wordAux = "";
    string opAux = "";
    int v = 0;
    int it = 0;
    map<string, Token> nonFinals;
    map<string, Token> finals;
    fstream fin(filePath, fstream::in);

    while (fin >> noskipws >> ch)
    {
        if (line == 0)
        {
            line++;
            continue;
        }
        // first validation, skips tabs and count lines
        if (ch == '\t')
        {
            continue;
        }

        // Validate if it is a number
        if (isReadingNumber == true && isReadingWord == false)
        {
            if (isDigit(ch))
            {
                v = (10 * (v) + (int)ch - '0');

                // cout << "n: " << ch << endl;
            }
            else
            {
                Number n(v);
                // n.toStringNumber();
                isReadingNumber = false;
                v = 0;
            }
        }
        else if (isReadingNumber == false && isReadingWord == true)
        {
            // Validate if it is a word
            if (isLetter(ch))
            {
                wordAux.push_back(ch);
            }
            else
            {
                Word w = Word(wordAux, Tag::ID);
                // w.toStringWord();
                if (it == 0)
                {
                    // add non final
                    pair<string, Token> p1 = make_pair(wordAux, w);
                    nonFinals.insert(p1);
                    it = 1;
                }
                else
                {
                    pair<string, Token> p1 = make_pair(wordAux, w);
                    finals.insert(p1);
                }
                isReadingWord = false;
                wordAux = "";
            }
        }
        else if (isReadingOperator == true)
        {
            // Validate if it is a op
            if (isOperater(ch))
            {
                opAux.push_back(ch);
            }
            else
            {
                if (it == 1)
                {
                    isReadingOperator = false;
                    opAux = "";
                    it = 2;
                    continue;
                }
                Token t = Token(opAux);
                // t.toStringToken();
                pair<string, Token> p1 = make_pair(opAux, t);
                finals.insert(p1);
                isReadingOperator = false;
                opAux = "";
            }
        }
        if (ch == '(' || ch == ')' || ch == '/')
        {
            if (opAux != "")
            {
                opAux.pop_back();
                Token t = Token(opAux);
                // t.toStringToken();
                pair<string, Token> p1 = make_pair(opAux, t);
                finals.insert(p1);
                isReadingOperator = false;
                opAux = "";
            }

            string aux;
            aux.push_back(ch);
            Token t = Token(aux);
            // t.toStringToken();
            pair<string, Token> p1 = make_pair(aux, t);
            finals.insert(p1);
            isReadingOperator = false;
            opAux = "";
            continue;
        }
        else if (ch == '\n')
        {
            clear(isReadingWord, isReadingNumber, wordAux, v, isReadingOperator, opAux);
            line = line + 1;
            it = 0;
            // cout << "New Line" << endl;
        }

        // Validate if it is digit or not
        if (isDigit(ch) && isReadingNumber == false)
        {
            v = (int)ch - '0';
            isReadingNumber = true;
            isReadingWord = false;
            isReadingOperator = false;
        } // Validate if it is letter or not
        else if (isLetter(ch) && isReadingWord == false)
        {
            wordAux = ch;
            isReadingWord = true;
            isReadingNumber = false;
            isReadingOperator = false;
        } // Validate if it is a op
        else if (isOperater(ch) && isReadingOperator == false)
        {
            opAux = ch;
            isReadingOperator = true;
            isReadingWord = false;
            isReadingNumber = false;
        }
    }
    if (isReadingWord == true)
    {
        Word w = Word(wordAux, Tag::ID);
        // w.toStringWord();
        pair<string, Token> p1 = make_pair(wordAux, w);
        finals.insert(p1);
    }
    else if (isReadingNumber == true)
    {
        Number n(v);
        // n.toStringNumber();
    }
    else
    {
        Token t = Token(opAux);
        // t.toStringToken();
    }

    // PRINTING non terminals
    map<string, Token>::iterator itr;
    int i = 0;
    int total = 0;

    // PRINTING terminals
    cout << "Terminal: ";
    for (itr = finals.begin(); itr != finals.end(); itr++)
    {

        if (nonFinals.find(itr->first) != nonFinals.end())
        {
            total++;
            continue;
        }
        cout << itr->first;
        if (i < finals.size() - total - 1)
        {
            cout << ", ";
        }
        i++;
    }
    cout << endl;

    i = 0;
    map<string, Token>::iterator itr2;
    cout << "Non terminal: ";
    for (itr2 = nonFinals.begin(); itr2 != nonFinals.end(); itr2++)
    {
        cout << itr2->first;

        if (i < nonFinals.size() - 1)
        {
            cout << ", ";
        }
        i++;
    }
    cout << endl;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Uso: Ver Repositorio para uso de este programa, lexico.cpp archivo1.txt" << endl;
        return 0;
    }
    string arg = argv[1];
    string path = "./" + arg;
    fstream my_file;
    my_file.open(path, ios::in);
    Scanner(path);
    my_file.close();
    return 0;
}
