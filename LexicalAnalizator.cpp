#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include "TScaner.h"
#include "TDiagram.h"
#include "SemanticTree.h"

using namespace std;


int main(void)
{
    setlocale(LC_ALL, "rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    FILE* fout = fopen("lexemes.dat", "w");
    TScaner* sc = new TScaner();
    TDiagram* td = new TDiagram(sc);
    SemanticTree* st = new SemanticTree();
    st->SetCur(st);
    td->setSemanticTree(st);

    printf("good_code.cpp \n");
    sc->GetData("good_code.txt");
    st->setTScaner(sc);
    try {
        do {
            td->Program();
        } while (td->getType() != type_end);
    }
    catch (int e)
    {
        switch (e) {
        case syntaxError:
            cout << " Синтаксический анализатор нашел ошибку и прекратил выполнение анализа" << endl;
            break;
        case semanticError:
            cout << " Семантический анализатор нашел ошибку и прекратил выполнение анализа" << endl;
            break;
        }
    }
    return 0;
}
