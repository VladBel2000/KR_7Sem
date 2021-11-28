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
protected:    // ���, ����� ������� ��
    TScaner* _sc;
    SemanticTree* st;
    int type;
    LEX lex;
    void Data(); // ������
    void Function(); // �������
    void Class(); // �����
    SemanticTree* Field(); // ���� - ���������� ��� ����������
    void ListOfFormalParameters(SemanticTree* func); // ������ ���������� ����������
    void ListOfParameters(SemanticTree* func); // ������ ����������
    void FunctionCall(); // ����� �������
    void Operator(); // ��������
    void SimpleOperator(); // ������� ��������
    void ComplexOperator(); // ��������� ��������
    void CircleWhile(); //���� while(;;)
    void Assignment(); // ������������

    int Expression(); // ��������� 
    int Expression1(); // ��������� 1
    int Expression2(); // ��������� 2
    int Expression3(); // ��������� 3
    int Expression4(); // ��������� 4
    int ElementaryExpression(); // ������������ ��������� 

    int lookForward(int k);

    int Error(string mess, int line, int pos);
public:
    TDiagram(TScaner* sc) { _sc = sc; }
    ~TDiagram() {}
    void setSemanticTree(SemanticTree* st);
    void Program(); // �������
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

