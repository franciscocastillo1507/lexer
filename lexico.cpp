#include <stdio.h>
#include <string>
#include <fstream> // Función de biblioteca de flujo de archivos
#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <set>
#include <algorithm>
#include <stack> 


// Author: Francisco Javier Castillo Hernandez
// Date Release : 29/5/22
// Tecnologico de Monterrey Campus Qro

using namespace std;
string initial = "";
bool isLLOneResultRepeat = false;
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
    int i = 0;
    for (auto it = input.begin(); it != input.end(); it++)
    {
        cout << *it;
        if (i < input.size() - 1)
        {
            cout << ", ";
        }
        i++;
    }
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
            if ((firstVector.insert(auxString)).second == false)
            {
                isLLOneResultRepeat = true;
            }
            else
            {

                firstVector.insert(auxString);
            }
            isArrowOn = true;
            for (itrAux = vectorFirst.begin(); itrAux != vectorFirst.end(); itrAux++)
            {
                auxString = *itrAux;

                if (isArrowOn == false)
                {
                    if (finals.find(auxString) != finals.end())
                    {
                        if ((firstVector.insert(auxString)).second == false)
                        {
                            isLLOneResultRepeat = true;
                        }
                        else
                        {

                            firstVector.insert(auxString);
                        }
                        isArrowOn = true;
                    }
                    else
                    {
                        if (auxString != "->")
                        {
                            pendingNF = auxString;
                        }
                    }
                }
                if (auxString == "->")
                {
                    auto nx = next(itrAux, 1);
                    if (*nx == "->")
                    {
                        firstVector.insert("ϵ");
                    }
                    isArrowOn = false;
                }
            }
            if (pendingNF != "")
            {
                if (pendingNF == "")
                {
                    itrFirstsNT = production.find(auxString);
                }
                else
                {
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
                            if ((firstVector.insert(auxString)).second == false)
                            {
                                isLLOneResultRepeat = true;
                            }
                            else
                            {

                                firstVector.insert(auxString);
                            }
                            isSecArrowOn = true;
                        }
                    }
                    if (auxString == "->")
                    {
                        auto nx = next(itrAux, 1);
                        if (*nx == "->")
                        {
                            firstVector.insert("ϵ");
                        }
                        isSecArrowOn = false;
                    }
                }
            }
            pair<string, set<string> > p1 = make_pair(itr->first, firstVector);
            firsts.insert(p1);
        }
        else if (auxString != "->")
        {
            firstVector.clear();
            itrFirstsNT = production.find(auxString);
            vectorFirst = itrFirstsNT->second;
            auxString = vectorFirst.front();
            bool isSecArrowOn = false;
            if (auxString != itr->first)
            {
                while (finals.find(auxString) == finals.end())
                {
                    itrFirstsNT = production.find(auxString);
                    vectorFirst = itrFirstsNT->second;
                    auxString = vectorFirst.front();
                }
            }
            else
            {
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
                        if ((firstVector.insert(auxString)).second == false)
                        {
                            isLLOneResultRepeat = true;
                        }
                        else
                        {

                            firstVector.insert(auxString);
                        }
                        isSecArrowOn = true;
                    }
                    else
                    {
                        if (auxString != "->")
                        {
                            pendingNF = auxString;
                        }
                    }
                }
                if (auxString == "->")
                {
                    auto nx = next(itrAux2, 1);
                    if (*nx == "->")
                    {
                        firstVector.insert("ϵ");
                    }
                    isSecArrowOn = false;
                }
            }
            if (pendingNF != "")
            {
                if (pendingNF == "")
                {
                    itrFirstsNT = production.find(auxString);
                }
                else
                {
                    auxString = pendingNF;
                    itrFirstsNT = production.find(auxString);
                }
                vectorFirst = itrFirstsNT->second;
                auxString = vectorFirst.front();
                bool isSecArrowOn = false;
                if (auxString != pendingNF)
                {
                    while (finals.find(auxString) == finals.end())
                    {
                        itrFirstsNT = production.find(auxString);
                        vectorFirst = itrFirstsNT->second;
                        auxString = vectorFirst.front();
                    }
                }
                else
                {
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
                            if ((firstVector.insert(auxString)).second == false)
                            {
                                isLLOneResultRepeat = true;
                            }
                            else
                            {

                                firstVector.insert(auxString);
                            }
                            isSecArrowOn = true;
                        }
                    }
                    if (auxString == "->")
                    {
                        auto nx = next(itrAux2, 1);
                        if (*nx == "->")
                        {
                            firstVector.insert("ϵ");
                        }
                        isSecArrowOn = false;
                    }
                }
            }
            pair<string, set<string> > p1 = make_pair(itr->first, firstVector);
            firsts.insert(p1);
        }
        else
        {
            if (auxString == "->")
            {
                firstVector.insert("ϵ");
                pair<string, set<string> > p1 = make_pair(itr->first, firstVector);
                firsts.insert(p1);
                firstVector.clear();
                continue;
            }
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
    map<string, vector<string> > production, map<string, set<string> > follows, map<string, set<string> > firsts, map<string, Token> nonFinals, map<string, Token> finals)
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
        if (itr->first == initial)
        {
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
            if (*itrVec != "->")
            {
                counter++;
            }
            else
            {
                counter = 0;
                hasFormaAb = false;
            }
            if (counter == 1)
            {
                auto nx = next(itrVec, 1);
                if (*nx == "->" && nonFinals.find(*itrVec) != nonFinals.end())
                {
                    // cout << "aqui "<< *itrVec << " "<< itr->first << endl;
                    // include in the other
                    pair<string, string> p1 = make_pair(*itrVec, itr->first);
                    auxFollows.insert(p1);
                }
                else if (itr->first == *itrVec && finals.find(*nx) != finals.end())
                {
                    if (follows.find(*itrVec) != follows.end())
                    {
                        set<string> foundFollowSet = follows.find(*itrVec)->second;
                        foundFollowSet.erase("ϵ");
                        foundFollowSet.insert(*nx);
                        follows.find(*itrVec)->second = foundFollowSet;
                    }
                    else
                    {
                        followsSet.insert(*nx);
                        followsSet.erase("ϵ");
                        pair<string, set<string> > p1 = make_pair(itr->first, followsSet);
                        follows.insert(p1);
                        followsSet.clear();
                    }
                }
            }
            if (counter == 2)
            {
                if (nonFinals.find(*itrVec) != nonFinals.end())
                {
                    hasFormaAb = true;
                }
                auto nx = next(itrVec, 1);
                if (*nx == "->" && *itrVec != itr->first)
                {
                    // FORM B -> a A [e] | Follow(A) = Follow(B)
                    // itr->first -> a *itrVec
                    // Follow(*itrVec) -> Fol (itr->first)
                    // hasEpsilon podemos ignorarlo
                    pair<string, string> p1 = make_pair(*itrVec, itr->first);
                    auxFollows.insert(p1);
                }
                else
                {
                    continue;
                }
            }
            if (counter == 3 && hasFormaAb == true)
            {
                // FORM B -> a A b | Follow(A) = First(b)
                if (itr->first == *itrVec)
                {
                    auto before = next(itrVec, -1);
                    pair<string, string> p1 = make_pair(*before, itr->first);
                    auxFollows.insert(p1);

                    if (follows.find(*before) != follows.end())
                    {
                        set<string> foundFollowSet = firsts.find(*itrVec)->second;
                        foundFollowSet.erase("ϵ");
                        follows.find(*before)->second = foundFollowSet;
                    }
                    else
                    {
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
                    if (follows.find(*before) != follows.end())
                    {
                        set<string> foundFollowSet = follows.find(*before)->second;
                        foundFollowSet.insert(*itrVec);
                        foundFollowSet.erase("ϵ");
                        follows.find(*before)->second = foundFollowSet;
                    }
                    else
                    {
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

    map<string, string>::iterator itr2;
    map<string, string>::iterator itr5;
    map<string, string>::iterator itr6;
    for (itr2 = auxFollows.begin(); itr2 != auxFollows.end(); itr2++)
    {
        // cout << "Map Key: " << itr2->first << ", v: " << itr2->second << endl;
        if (itr2->second == initial)
        {
            itr5 = itr2;
        }
        else
        {
            itr5 = auxFollows.begin();
        }
    }
    // get first
    int auxFollowsSize = auxFollows.size();
    for (int i = 0; i < auxFollowsSize; i++)
    {
        auto it2 = follows.find(itr5->first);
        set<string> finalSet;
        if (it2 == follows.end())
        {
            // cout << "Key: " << itr5->first << ", v: " << itr5->second << endl;
            finalSet = follows.find(itr5->second)->second;
            // printSet(finalSet);
            // cout << endl;
            pair<string, set<string> > p1 = make_pair(itr5->first, finalSet);
            // cout << "Pair: " << itr5->first << endl;
            // printSet(finalSet);
            // cout << endl;
            follows.insert(p1);
        }
        else
        {
            // cout << "KeyF: " << itr5->first << ", v: " << itr5->second << endl;
            set<string> neutral = follows.find(itr5->first)->second;
            neutral.erase("ϵ");
            set<string> addFollow = follows.find(itr5->second)->second;
            addFollow.erase("ϵ");
            set<string> finalSet;
            set_union(neutral.begin(), neutral.end(),
                      addFollow.begin(), addFollow.end(),
                      inserter(finalSet, finalSet.begin()));
            // printSet(finalSet);
            // cout << endl;
            follows.find(itr5->first)->second = finalSet;
        }
        for (itr6 = auxFollows.begin(); itr6 != auxFollows.end(); itr6++)
        {
            if (itr6->second == itr5->first)
            {
                itr5 = itr6;
                break;
            }
        }
    }

    return follows;
}

string isLLOne(map<string, set<string> > firsts, map<string, set<string> > follows)
{
    bool isLLOneResult = true;
    map<string, set<string> >::iterator itr;
    map<string, set<string> >::iterator itr2;
    for (itr = firsts.begin(), itr2 = follows.begin(); itr != firsts.end(), itr2 != follows.end(); itr++, itr2++)
    {
        set<string>::iterator itr3;
        set<string>::iterator itr4;
        set<string> setFollows = itr2->second;
        set<string> set = itr->second;
        if (set.find("ϵ") != set.end())
        {
            for (itr3 = set.begin(); itr3 != set.end(); itr3++)
            {
                for (itr4 = setFollows.begin(); itr4 != setFollows.end(); itr4++)
                {
                    if (*itr3 == *itr4)
                    {
                        isLLOneResult = false;
                    }
                }
            }
        }
    }

    map<string, set<string> >::iterator itr5;
    map<string, set<string> >::iterator itr6 = firsts.begin();
    bool firstRule = true;
    for (itr5 = firsts.begin(); itr5 != firsts.end(); itr5++)
    {
        if (itr5->second != itr6->second)
        {
            firstRule = false;
        }
    }
    if (firstRule == true)
        return "No";
    if (isLLOneResultRepeat == true)
        return "No";
    return isLLOneResult ? "Yes" : "No";
}
string printProductionTable (string final, string nonFinal,map<string, vector<string> > productions){
    // map<string, vector<string> >::iterator itr;

    // for (itr = productions.begin(); itr != productions.end(); itr++)
    // {
    //     cout << itr->first << ": " << endl;
    //     print(itr->second);
    //     cout << endl;
    // }
    // cout << endl;
    // vector<string> partOfProduction; 
    string partOfProduction = "";
    // if(nonFinal == "two" && final == "b"){
    //     cout << "printProductionTable: "<< final << " : " << nonFinal << endl;
    // }
    // cout << "printProductionTable: "<< final << " : " << nonFinal << endl;
    map<string, vector<string> >::iterator production = productions.find(nonFinal);
    vector<string> vec = production->second;
    vector<string>::iterator itrAux;
    bool findArrow = false;
    bool found = false;
    int count = 0;
    if(final == "$" || final.empty() || nonFinal.empty() ){
        // partOfProduction.push_back("/0");
        partOfProduction = "/0";
        return partOfProduction;
    }
    for (itrAux = vec.begin(); itrAux != vec.end(); itrAux++)
    {
        
        if(*itrAux == final && count == 0){
            findArrow = true;
        }
        if(findArrow == true && *itrAux == "->"){
            found = true;
            if(findArrow == true && *itrAux == "->" && partOfProduction == "->"){
                found = false;
            }
            break;
        }
        if(findArrow == true){
            // partOfProduction.push_back(*itrAux);
                partOfProduction = partOfProduction + *itrAux+" ";

            auto nx = next(itrAux, 1);
            if(nx == vec.end() && *nx != "->"){
                found = true;
            }
            
        }
        if(*itrAux == "->"){
            findArrow = true;
            count = 0;
        }
        count++;
        
    }
   
    if(found == true){
        return partOfProduction;
    }
    else{
        findArrow = true;
        for (itrAux = vec.begin(); itrAux != vec.end(); itrAux++)
        {
            
            if(*itrAux == final && count == 0){
                findArrow = true;
            }
            if(findArrow == true && *itrAux == "->"){
                found = true;
                if(findArrow == true && *itrAux == "->" && partOfProduction == "->"){
                    found = false;
                }
                break;
            }
            if(findArrow == true){
                // partOfProduction.push_back(*itrAux);
                partOfProduction = partOfProduction + *itrAux+" ";
                
            }
            if(*itrAux == "->"){
                findArrow = true;
                count = 0;
            }
            count++;
            
        }
        // partOfProduction = "/0";
        return partOfProduction;
    }
    // for (int a = 0; a < count; a++)
    // {
    //     cout << partOfProduction[a] << endl;
    // }
    return partOfProduction;
 
}
void generateTableLLOne(map<string, vector<string> > productions, map<string, set<string> > firsts, map<string, set<string> > follows, map<string, Token> nonFinals, map<string, Token> finals,vector<string> productionsTests)
{
    string tableLLOne[nonFinals.size() + 1][finals.size() + 2];

    for (int j = 0; j < nonFinals.size()+1; j++)
    {
        for (int i = 0; i < finals.size() + 2; i++)
        {
            map<string, Token>::iterator itrFinals = finals.begin();
            map<string, Token>::iterator itrNonFinals = nonFinals.begin();
            if (i == 0 && j == 0)
            {
                tableLLOne[i][j] = "NF";
                cout << tableLLOne[i][j] << " \t\t\t" ;
            }
            else if (j == 0)
            {
                // Add all of the finals
                if (i == finals.size() + 1)
                {
                    tableLLOne[i][j] = "$";
                }
                else
                {
                    auto nx = next(itrFinals, i-1);
                    tableLLOne[i][j] = nx->first;
                }

                cout << tableLLOne[i][j] << " \t\t\t" ;
            }
            else if (i == 0)
            {
                // Add all of the non finals
                // for (itrNonFinals = finals.begin(); itrNonFinals != finals.end(); itrNonFinals++)
                // {
                //     cout << "P: " << itrNonFinals->first<< endl;;
                // }
                auto nx = next(itrNonFinals, j-1);
                tableLLOne[i][j] = nx->first;

                cout << tableLLOne[i][j] << " \t\t\t" ;
            }else{
                string nonFinal = tableLLOne[0][j];
                string final = tableLLOne[i][0];
                if(nonFinal == "NF" || final == "NF" || final.empty() || nonFinal.empty()){
                    cout << tableLLOne[i][j] << " \t\t\t" ;
                    continue;
                }
                // cout << "L: " << final << "" << nonFinal << endl;
                bool epsilon = false;
                auto itFind = firsts.find(nonFinal);
                if (itFind != firsts.end())
                {
                    set<string> auxTableSet = itFind->second;
                    std::set<string>::iterator it;
                    for (it = auxTableSet.begin(); it != auxTableSet.end(); ++it) {
                        if(*it == "ϵ"){
                            epsilon = true;
                        }
                        if(*it == final){
                            tableLLOne[i][j]  =  printProductionTable(final,nonFinal,productions);
                            cout << nonFinal << " -> " << tableLLOne[i][j] << "\t\t" ;
                            // cout << tableLLOne[i][j] << " " << final << nonFinal ;
                        }
                    }
                    if(epsilon == true){
                        auto itFindFollows = follows.find(nonFinal);
                        set<string> auxTableSetFollows = itFindFollows->second;
                        std::set<string>::iterator it2;
                        for (it2 = auxTableSetFollows.begin(); it2 != auxTableSetFollows.end(); ++it2) {
                            // cout << "aqui "<< *it2 << " " << final <<endl;
                            if(*it2 == final){
                                tableLLOne[i][j]  = "ϵ";
                                cout << nonFinal << " -> " << tableLLOne[i][j] << "\t\t" ;
                            }
                        }
                        epsilon = false;
                    }
                if(tableLLOne[i][j].empty()){
                    cout << "\t\t\t" ;
                }
                 
                }else{
                    // vector <string> printProductionTableRes = printProductionTable(final,nonFinal,productions);
                    tableLLOne[i][j] = "/0";
                    cout << tableLLOne[i][j] << " \t\t" ;
                }
            }
           
        }
        cout<<endl;
    }

    // Evaluation of productions
    for(int i = 0; i < productionsTests.size();i++){
        // Get each production test
        string productionsTest = productionsTests[i]; 
        cout << endl;
        cout << "Cadena: " << productionsTest << endl;
        stack<string> stackInitial;
        stack<string> stackInput;
        bool isWorking = true;
        bool isAccepted = false;

        // Primero debe de tener dos stacks en el cual se agrega en el inicial $ y el initial y en el otro la cadena normal y el $
        stackInitial.push("$");
        stackInput.push("$");
        stackInitial.push(initial);
        
        string aux;
        for (int i = productionsTest.size()-1; i >= 0 ; i--){
            aux.push_back(productionsTest[i]);
            reverse(aux.begin(), aux.end());
            if(finals.find(aux) != finals.end()){
                stackInput.push(aux);
                aux = "";
            }
        }

        // acaba con una bandera en un while hasta que truene o termine temrina bien si ambas pilas tienen $ y $
        while(isWorking){
            if(stackInput.top() == "$" && stackInitial.top() == "$"){
                isAccepted == true;
                isWorking = false;
                cout << "Input #"<< i+1 << ": " << "Yes" << endl;
                break;
            }
            // Evaluar si tienen lo mismo y quitar en ambos si no
            if(stackInitial.top() == stackInput.top()){
            // cout << "Cadena top igual"  << endl;
                stackInput.pop();
                stackInitial.pop();

            }else {
            // cout << "Cadena top stackInitial "<< stackInitial.top() << endl;
            // cout << "Cadena top stackInput "<< stackInput.top() << endl;
                //Find el non final sino truena y adios ej: $A a  | b a$
                auto itFind = nonFinals.find(stackInitial.top());
                if (itFind == nonFinals.end())
                {
                    // cout << "lol3 "<< endl;
                    isWorking = false;
                    cout << "Input #"<< i+1 << ": " << "No" << endl;
                    break;
                }
                // Ver en la tabla que produce y obtener el valor  
                string nonFinal = stackInitial.top();
                string final = stackInput.top();

            // cout << "Cadena NF: "<< nonFinal << " " << final << endl;
                int posCol = 0;
                int posRow = 0;

                //get nonfinal col
                for(int i = 0; i < nonFinals.size() + 1; i++){
                    if(tableLLOne[0][i] == nonFinal){
                        posCol = i;
                    }
                }
                //get final row
                for(int i = 0; i < finals.size() + 2; i++){
                    if(tableLLOne[i][0] == final){
                        posRow = i;
                    }
                }
                string index = tableLLOne[posRow][posCol];
                if(index == "ϵ"){
                    // cout << "salio " << stackInitial.top() << endl;
                    stackInitial.pop();
                    // cout << "top " << stackInitial.top() << endl;
                    continue;
                }

            // cout << "Cadena index ."<< index << "."<< endl;
                if(index.empty() || index.size() == 0){
                    isWorking = false;
                    cout << "Input #"<< i+1 << ": " << "No" << endl;
                    break;
                }
                vector<string> stackAux;
                int total = 0;
                string auxStack = "";

                // agregarlo a la pila inicial al reves o agregar el elemento
                for(int i = 0; i < index.size(); i++){
                    if(index[i] == ' ' || index[i] == '\n'){
                        stackAux.push_back(auxStack);
                        total++;
                        auxStack = "";
                    }else{
                        auxStack += index[i];
                    }
                }
                stackInitial.pop();
                for(int j = stackAux.size() - 1; j >= 0; j--){

            // cout << "Cadena agregada "<< stackAux[j] << endl;
                    stackInitial.push(stackAux[j]);
                }


            }

            // Validar que sino esta en la tabal truena o si llega una pila al final y la otro no truena
            if(stackInitial.size() == 1 && stackInput.size() > 1){
                    // cout << " lol " << endl;
                    isWorking = false;
                    cout << "Input #"<< i+1 << ": " << "No" << endl;
                    break;
                }

            if(stackInput.size() == 1 && stackInitial.size() > 1){
                    for(int j = stackInitial.size() - 1; j >= 0; j--){
                        string nonFinal = stackInitial.top();
                        string final = stackInput.top();
                        if(stackInput.top() == "$" && stackInitial.top() == "$"){
                            isAccepted == true;
                            isWorking = false;
                            cout << "Input #"<< i+1 << ": " << "Yes" << endl;
                            break;
                        }

                        // cout << "Cadena NF: "<< nonFinal << " " << final << endl;
                            int posCol = 0;
                            int posRow = 0;

                        //get nonfinal col
                        for(int i = 0; i < nonFinals.size() + 1; i++){
                            if(tableLLOne[0][i] == nonFinal){
                                posCol = i;
                            }
                        }
                        //get final row
                        for(int i = 0; i < finals.size() + 2; i++){
                            if(tableLLOne[i][0] == final){
                                posRow = i;
                            }
                        }
                        string index = tableLLOne[posRow][posCol];
                        if(index == "ϵ"){
                            // cout << "salio " << stackInitial.top() << endl;
                            stackInitial.pop();
                            // cout << "top " << stackInitial.top() << endl;
                            continue;
                        }
                    }
                    if(stackInput.top() != "$" && stackInitial.top() != "$"){
                        isWorking = false;
                        cout << "Input #"<< i+1 << ": " << "No" << endl;
                        break;
                    }
            }
        }
        cout << endl;
    }
    
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
    bool productionsCapture = false;
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
    vector<string> productionsTests;
    fstream fin(filePath, fstream::in);

    // Auxiliar variables for productions
    string production = "";
    vector<string> productionVector;
    int lineNumber = 0;
    int testProductions = 0;
    string auxProd = "";

    while (fin >> noskipws >> ch)
    {
        if(lineNumber < line-1 && lineNumber > 0 && line > 0){
            //Obtener los strings
            if (ch == '\n'){
                productionsTests.push_back(auxProd);
                line++;
                auxProd = "";
            }
            if(ch != ' ' && ch != '\n'){
                auxProd += ch;
            }
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
                if(line == 0){
                    if(lineNumber != 0){
                        testProductions = v;

                        // cout << "xd tp: "<< testProductions << endl;
                        line++;
                    }else{
                        lineNumber = v;
                        // cout << "xd ln: "<< lineNumber << endl;
                    }
                }
                
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
                    if (line == 2)
                    {
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
            if (line == 0)
            {
                continue;
            }
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
            //  cout << "New production: ." << production <<"."<< endl;
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
        if (it2 != productions.end())
        {
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
        if (it2 != productions.end())
        {
            it2->second = productionVector;
        }
    }

    // PRINTING non terminals
    int i = 0;
    int total = 0;
    finals = deleteDuplicates(finals, nonFinals);

 
    // map<string, Token >::iterator itr2;
    // cout << "Terminal"<<endl;
    // for (itr2 = finals.begin(); itr2 != finals.end(); itr2++)
    // {
    //     cout << itr2->first << " " ;
    // }

    firsts = foundFirsts(productions, firsts, finals);
    follows = foundFollows(productions, follows, firsts, nonFinals, finals);
    map<string, set<string> >::iterator itr3;
    map<string, set<string> >::iterator itr4;
    // cout << "Size: " << firsts.size() << " " << follows.size() <<endl;
    for (itr3 = firsts.begin(), itr4 = follows.begin(); itr3 != firsts.end(), itr4 != follows.end(); itr3++, itr4++)
    // for (itr3  = firsts.begin(); itr3 != firsts.end(); itr3++)

    {
        cout << itr3->first << " =>  FIRST = {";
        printSet(itr3->second);
        cout << "}, FOLLOW = {";
        printSet(itr4->second);
        cout << "}" << endl;
    }
    isLLOneResult = isLLOne(firsts, follows);
    cout << "LL(1)? " << isLLOneResult << endl;
    if(isLLOneResult == "No"){
        cout << "Grammar is not LL(1)!" << endl;
    }
    generateTableLLOne(productions,firsts,follows,nonFinals,finals, productionsTests);
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
