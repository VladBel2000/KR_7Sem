#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "defs.h"
#include "TScaner.h"
#include "SemanticTree.h"
#include <string>
#include <iostream>
using namespace std;


class TDiagram
{
protected:    // все, кроме аксиомы КА
    TScaner* _sc;
    SemanticTree* st;
    int type;
    LEX lex;
    void Data(); // данные
    void Function(); // функция
    void Class(); // класс
    SemanticTree* Field(); // поле - возвращает тип переменной
    void ListOfFormalParameters(SemanticTree* func); // список формальных параметров
    void ListOfParameters(SemanticTree* func); // список параметров
    void FunctionCall(); // вызов функции
    void Operator(); // оператор
    void SimpleOperator(); // простой оператор
    void ComplexOperator(); // составной оператор
    void CircleWhile(); //цикл while(;;)
    void Assignment(); // присваивание

    int Expression(); // выражение 
    int Expression1(); // выражение 1
    int Expression2(); // выражение 2
    int Expression3(); // выражение 3
    int Expression4(); // выражение 4
    int ElementaryExpression(); // элементарное выражение 

    int lookForward(int k);

    int Error(string mess, int line, int pos);
public:
    TDiagram(TScaner* sc) { _sc = sc; }
    ~TDiagram() {}
    void setSemanticTree(SemanticTree* st);
    void Program(); // аксиома
    int getType();
};


/*
uk1=sc->getUK();     t=sc->scaner(l);
while ((t  ==TPlus) |   |  (t  == TMinus))     {
    A();
    uk1=sc->getUK();    t=sc->scaner(l);
}
sc->setUK(uk1);
*/

