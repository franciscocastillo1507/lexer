#include <stdio.h>
#include <string>
#include <fstream> // Función de biblioteca de flujo de archivos
#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <set>
#include <algorithm>

// Author: Francisco Javier Castillo Hernandez
// Date Release : 16/3/22
// Tecnologico de Monterrey Campus Qro
// Compiladores (Primera parte)

using namespace std;
string initial = "";
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
void print(std::vector<string> const &input)
{
    for (int i = 0; i < input.size(); i++)
    {
        std::cout << input.at(i) << ' ';
    }
}
void printSet(std::set<string> const &input)
{
    for (auto it = input.begin(); it != input.end(); it++)
        cout << *it << " ";
}
map<string, set<string> > foundFirsts(
    map<string, vector<string> > production, map<string, set<string> > firsts, map<string, Token> finals)
{
    map<string, vector<string> >::iterator itr;
    int i = 0;
    for (itr = production.begin(); itr != production.end(); itr++)
    {
        if (i == 0)
        {
            i++;
            continue;
        }
        map<string, vector<string> >::iterator itrFirstsNT;
        set<string> firstVector;
        vector<string> vectorFirst = itr->second;
        bool isArrowOn = false;
        string auxString = vectorFirst.front();
        string pendingNF = "";

        if (finals.find(auxString) != finals.end())
        {
            vector<string>::iterator itrAux;
            map<string, Token>::iterator aux;
            firstVector.insert(auxString);
            isArrowOn = true;
            for (itrAux = vectorFirst.begin(); itrAux != vectorFirst.end(); itrAux++)
            {
                auxString = *itrAux;
                if (isArrowOn == false)
                {
                    if (finals.find(auxString) != finals.end())
                    {
                        firstVector.insert(auxString);
                        isArrowOn = true;
                    }else{
                        if (auxString != "->")
                        {
                            pendingNF = auxString;
                        }
                    }
                }
                if (auxString == "->")
                {
                    auto nx = next(itrAux, 1);
                    if(*nx == "->"){
                        firstVector.insert("ϵ");
                    }
                    isArrowOn = false;
                }
            }
            if(pendingNF != "")
            {
                if(pendingNF == ""){
                    itrFirstsNT = production.find(auxString);
                }else{
                    auxString = pendingNF;
                    itrFirstsNT = production.find(auxString);
                }
                vectorFirst = itrFirstsNT->second;
                auxString = vectorFirst.front();
                while (finals.find(auxString) == finals.end())
                {
                    itrFirstsNT = production.find(auxString);
                    vectorFirst = itrFirstsNT->second;
                    auxString = vectorFirst.front();
                }
                vector<string>::iterator itrAux2;
                bool isSecArrowOn = false;
                for (itrAux2 = vectorFirst.begin(); itrAux2 != vectorFirst.end(); itrAux2++)
                {
                    auxString = *itrAux2;

                    if (isSecArrowOn == false)
                    {
                        if (finals.find(auxString) != finals.end())
                        {
                            firstVector.insert(auxString);
                            isSecArrowOn = true;
                        }
                    }
                    if (auxString == "->")
                    {
                        auto nx = next(itrAux, 1);
                        if(*nx == "->"){
                            firstVector.insert("ϵ");
                        }
                        isSecArrowOn = false;
                    }
                }
        }
            pair<string, set<string> > p1 = make_pair(itr->first, firstVector);
            firsts.insert(p1);
        }
        else if(auxString != "->" ) 
        {
            firstVector.clear();
            itrFirstsNT = production.find(auxString);
            vectorFirst = itrFirstsNT->second;
            auxString = vectorFirst.front();
            bool isSecArrowOn = false;
            if(auxString != itr->first){
                while (finals.find(auxString) == finals.end())
                {
                    itrFirstsNT = production.find(auxString);
                    vectorFirst = itrFirstsNT->second;
                    auxString = vectorFirst.front();
                }
            }else{
                isSecArrowOn = true;
            }
            
            vector<string>::iterator itrAux2;
            for (itrAux2 = vectorFirst.begin(); itrAux2 != vectorFirst.end(); itrAux2++)
            {
                auxString = *itrAux2;

                if (isSecArrowOn == false)
                {
                    if (finals.find(auxString) != finals.end())
                    {
                        firstVector.insert(auxString);
                        isSecArrowOn = true;
                    }else{
                        if (auxString != "->")
                        {
                            pendingNF = auxString;
                        }
                    }
                }
                if (auxString == "->")
                {
                    auto nx = next(itrAux2, 1);
                    if(*nx == "->"){
                        firstVector.insert("ϵ");
                    }
                    isSecArrowOn = false;
                }
            }
            if(pendingNF != "")
            {
                if(pendingNF == ""){
                    itrFirstsNT = production.find(auxString);
                }else{
                    auxString = pendingNF;
                    itrFirstsNT = production.find(auxString);
                }
                vectorFirst = itrFirstsNT->second;
                auxString = vectorFirst.front();
                bool isSecArrowOn = false;
                if(auxString != pendingNF){
                    while (finals.find(auxString) == finals.end())
                    {
                        itrFirstsNT = production.find(auxString);
                        vectorFirst = itrFirstsNT->second;
                        auxString = vectorFirst.front();
                    }
                }else{
                    auxString = vectorFirst.at(vectorFirst.size() - 2);
                    itrFirstsNT = production.find(auxString);
                    vectorFirst = itrFirstsNT->second;
                }
                vector<string>::iterator itrAux2;
                for (itrAux2 = vectorFirst.begin(); itrAux2 != vectorFirst.end(); itrAux2++)
                {
                    auxString = *itrAux2;

                    if (isSecArrowOn == false)
                    {
                        if (finals.find(auxString) != finals.end())
                        {
                            firstVector.insert(auxString);
                            isSecArrowOn = true;
                        }
                    }
                    if (auxString == "->")
                    {
                        auto nx = next(itrAux2, 1);
                        if(*nx == "->"){
                            firstVector.insert("ϵ");
                        }
                        isSecArrowOn = false;
                    }
                }
        }
            pair<string, set<string> > p1 = make_pair(itr->first, firstVector);
            firsts.insert(p1);
        }
    }
    return firsts;
}

map<string, Token> deleteDuplicates(map<string, Token> finals, map<string, Token> nonFinals)
{

    map<string, Token>::iterator itr;
    map<string, Token>::iterator aux;
    vector<string> duplicates;
    vector<string>::iterator itr2;
    for (itr = finals.begin(); itr != finals.end(); itr++)
    {

        if (nonFinals.find(itr->first) != nonFinals.end())
        {
            duplicates.push_back(itr->first);
        }
    }
    for (itr2 = duplicates.begin(); itr2 != duplicates.end(); itr2++)
    {
        finals.erase(*itr2);
    }
    map<string, Token> returnFinals = finals;
    return returnFinals;
}
map<string, set<string> > foundFollows(
    map<string, vector<string> > production, map<string, set<string> > follows, map<string, set<string> > firsts, map<string, Token > nonFinals, map<string, Token > finals)
{
    map<string, vector<string> >::iterator itr;
    map<string, string> auxFollows;
    set<string> followsSet;
    int i = 0;
    // cout << "found: "<< initial << endl;
    for (itr = production.begin(); itr != production.end(); itr++)
    {

        if (i == 0)
        {
            i++;
            continue;
        }
        if(itr->first == initial){
            followsSet.insert("$");
            pair<string, set<string> > p1 = make_pair(itr->first, followsSet);
            follows.insert(p1);
            followsSet.clear();
            i++;
        }
        vector<string> productionLine = itr->second;

        // FORM B -> a A b | Follow(A) = First(b)
        // Total number of productions and have the shape before mentioned, and index
        
        vector<string>::iterator itrVec;
        int counter = 0;
        int index = 0;
        bool hasFormaAb = false;

        for (itrVec = productionLine.begin(); itrVec != productionLine.end(); itrVec++)
            {
                if(*itrVec != "->"){
                    counter++;
                }else{
                    counter = 0;
                    hasFormaAb = false;
                }
                if(counter == 1){
                    auto nx = next(itrVec, 1);
                    if(*nx == "->" && nonFinals.find(*itrVec) != nonFinals.end()){
                        // include in the other
                        pair<string, string > p1 = make_pair(*itrVec,itr->first);
                        auxFollows.insert(p1);
                        
                    }else{
                        continue;
                    }
                }
                if(counter == 2){
                    if(nonFinals.find(*itrVec) != nonFinals.end()){
                        hasFormaAb = true;
                    }
                    auto nx = next(itrVec, 1);
                    if(*nx == "->"){
                        // FORM B -> a A [e] | Follow(A) = Follow(B)
                        // itr->first -> a *itrVec
                        // Follow(*itrVec) -> Fol (itr->first)
                        // hasEpsilon podemos ignorarlo
                        pair<string, string > p1 = make_pair(*itrVec,itr->first);
                        auxFollows.insert(p1);
                        
                    }else{
                        continue;
                    }
                }
                if(counter == 3 && hasFormaAb == true){
                    // FORM B -> a A b | Follow(A) = First(b)
                    if(itr->first == *itrVec){
                        auto before = next(itrVec, -1);
                        pair<string, string > p1 = make_pair(*before,itr->first);
                        auxFollows.insert(p1);
                        
                        if(follows.find(*before) != follows.end()){
                            set<string> foundFollowSet = firsts.find(*itrVec)->second;
                            foundFollowSet.erase("ϵ");
                            follows.find(*before)->second = foundFollowSet;
                        }else{
                            set<string> foundFollowSet = firsts.find(*itrVec)->second;
                            foundFollowSet.erase("ϵ");
                            pair<string, set<string> > p1 = make_pair(*before, foundFollowSet);
                            follows.insert(p1);
                            foundFollowSet.clear();
                        }

                    }
                    else if (finals.find(*itrVec) != finals.end())
                    {
                        auto before = next(itrVec, -1);
                        if(follows.find(*before) != follows.end()){
                            set<string> foundFollowSet = follows.find(*before)->second;
                            foundFollowSet.insert(*itrVec);
                            foundFollowSet.erase("ϵ");
                            follows.find(*before)->second = foundFollowSet;
                        }else{
                            followsSet.insert(*itrVec);
                            followsSet.erase("ϵ");
                            pair<string, set<string> > p1 = make_pair(itr->first, followsSet);
                            follows.insert(p1);
                            followsSet.clear();
                        }
                    }
                }
            }
    }

    //get first
    map<string, string>::iterator itr5 = auxFollows.begin();
    map<string, string>::iterator itr6;
    int auxFollowsSize = auxFollows.size();
    for (int i = 0; i < auxFollowsSize; i++){
        // cout << "Map Key: " << itr5->first << ", v: " << itr5->second << endl;
        auto it2 = follows.find(itr5->first);
        set<string> finalSet;
        if (it2 == follows.end()){
            // cout << "Key: " << itr5->first << ", v: " << itr5->second << endl;
            finalSet = follows.find(itr5->second)->second;
            // printSet(finalSet);
            // cout << endl;
            pair<string, set<string> > p1 = make_pair(itr5->first, finalSet);
            // cout << "Pair: " << itr5->first << endl;
            // printSet(finalSet);
            // cout << endl;
            follows.insert(p1);
        }else{
            // cout << "KeyF: " << itr5->first << ", v: " << itr5->second << endl;
            set<string> neutral = follows.find(itr5->first)->second;
            neutral.erase("ϵ");
            set<string> addFollow = follows.find(itr5->second)->second ;
            addFollow.erase("ϵ");
            set<string> finalSet;
            set_union(neutral.begin(), neutral.end(),
                    addFollow.begin(), addFollow.end(),
                    inserter(finalSet, finalSet.begin()));
            // printSet(finalSet);
            // cout << endl;
            follows.find(itr5->first)->second = finalSet;
        }
        for(itr6 = auxFollows.begin(); itr6 != auxFollows.end(); itr6++){
            if(itr6->second == itr5->first){
                itr5 = itr6;
                break;
            }
        }
    }
        
    
    return follows;
}

string isLLOne (map<string, set<string> > firsts){
    bool isLLOneResult = true;
    map<string, set<string> >::iterator itr;
    for (itr = firsts.begin(); itr != firsts.end(); itr++)
    {
        set<string>::iterator itr2;
        set<string> set = itr->second;
        for(itr2 = set.begin(); itr2 != set.end(); itr2++){
            if(*itr2 == "ϵ"){
                isLLOneResult = false;
                break;
            }
        }
    }
    return isLLOneResult ? "Yes" : "No";

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
    string isLLOneResult;
    string wordAux = "";
    string opAux = "";
    int v = 0;
    int it = 0;
    map<string, Token> nonFinals;
    map<string, Token> finals;
    map<string, vector<string> > productions;
    map<string, set<string> > firsts;
    map<string, set<string> > follows;
    fstream fin(filePath, fstream::in);

    // Auxiliar variables for productions
    string production = "";
    vector<string> productionVector;

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
                    if(line == 2){
                        initial = wordAux;
                    }
                    // add non final
                    pair<string, Token> p1 = make_pair(wordAux, w);
                    nonFinals.insert(p1);
                    it = 1;
                    production = wordAux;
                    auto it = productions.find(production);
                    if (production == it->first)
                    {
                        productionVector = it->second;
                    }
                }
                else
                {
                    pair<string, Token> p1 = make_pair(wordAux, w);
                    finals.insert(p1);
                    productionVector.push_back(wordAux);
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
                productionVector.push_back(opAux);
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
                pair<string, Token> p1 = make_pair(opAux, t);
                finals.insert(p1);
                productionVector.push_back(opAux);
                isReadingOperator = false;
                opAux = "";
            }

            string aux;
            aux.push_back(ch);
            Token t = Token(aux);
            pair<string, Token> p1 = make_pair(aux, t);
            productionVector.push_back(aux);
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
            productionVector.push_back("->");
            // pair
            if (productions.count(production) > 0)
            {
                auto it = productions.find(production);
                it->second = productionVector;
            }
            else
            {
                pair<string, vector<string> > p1 = make_pair(production, productionVector);
                productions.insert(p1);
            }
            // cout << "New production: ." << production <<"."<< endl;
            // print(productionVector);
            // cout << endl;
            // clean aux helpers
            production = "";
            productionVector.clear();
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
    // End while
    if (isReadingWord == true)
    {
        Word w = Word(wordAux, Tag::ID);
        pair<string, Token> p1 = make_pair(wordAux, w);
        finals.insert(p1);
        auto it2 = productions.find(production);
        if (it2 != productions.end()){
            productionVector = it2->second;
            productionVector.push_back(wordAux);
            it2->second = productionVector;
        }
        // pair<string, vector<string> > p1 = make_pair(wordAux, productionVector);
        

    }
    else if (isReadingNumber == true)
    {
        Number n(v);
    }
    else
    {
        Token t = Token(opAux);
        auto it2 = productions.find(production);
        if (it2 != productions.end()){
            it2->second = productionVector;
        }
    }

    // PRINTING non terminals
    map<string, vector<string> >::iterator itr;
    int i = 0;
    int total = 0;
    finals = deleteDuplicates(finals, nonFinals);
    cout << "Productions: ";
    for (itr = productions.begin(); itr != productions.end(); itr++)
    {
        cout << itr->first << ": " << endl;
        print(itr->second);
        cout << endl;
    }
    cout << endl;
    map<string, Token >::iterator itr2;
    cout << "Terminal"<<endl;
    for (itr2 = finals.begin(); itr2 != finals.end(); itr2++)
    {
        cout << itr2->first << " " ;
    }
    cout << endl;
    firsts = foundFirsts(productions, firsts, finals);
    map<string, set<string> >::iterator itr3;
    cout << "Firsts: "<< firsts.size() << endl;
    for (itr3 = firsts.begin(); itr3 != firsts.end(); itr3++)
    {
        cout << itr3->first << ": " << endl;
        printSet(itr3->second);
        cout << endl;
    }
    cout << endl;
    follows = foundFollows(productions,follows,firsts,nonFinals,finals);
    // Agregar que es follow de algo tipo E -> EPRIME
    // asi al final de todo agregamos los follows despues de los iteradores
    // primero se llena con la forma de abajo y la inicial
    // y con lo que queda vemos cual no tiene un auxfollow y asi vanos llenando
    map<string, set<string> >::iterator itr4;
    cout << "Follows: "<< follows.size() << endl;
    for (itr4 = follows.begin(); itr4 != follows.end(); itr4++)
    {
        cout << itr4->first << ": " << endl;
        printSet(itr4->second);
        cout << endl;
    }
    cout << endl;

    isLLOneResult = isLLOne(firsts);
    cout << "LL(1): "<< isLLOneResult <<endl;


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
