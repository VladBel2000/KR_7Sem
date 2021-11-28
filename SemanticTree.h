#pragma once
#include "TScaner.h"
#include "defs.h"
#include <string.h>
#include <iostream>

#define EMPTY -1 // признак пустой ссылки

enum TypeObject {
	ObjVar = 1, // простая переменная
	ObjClass, // класс
	ObjTypeClass, // тип класса
	ObjFunct, // функция
	ObjFor
};

enum TypeVar {
	TypeBool = 1,
	TypeDouble,
	TypeInt,
	FirstUserType,
	TypeUndef = 10000
};

class SemanticTree;

struct DataIdent
	// для хранения в таблице идентификаторов
{
	//LEX id; // идентификатор переменной 
	int typeObject; // для переменной t=ObjVar, для функции t=ObjFunct
	int typeVar; // тип для приведения типов
	SemanticTree* typeClass = nullptr;

	// для функций
	int Param = 0; // количество параметров

	// ссылка на значение
	void* data = nullptr;

	// необязательные данные:
	int FlagInit; // Флаг начальной инициализации
	char* Addr; // адрес в оттранслированной программе
};

struct Node // информация об одной переменной
{
	LEX id; // идентификатор переменной
	DataIdent dataIdent; // информация о значении узла таблицы
};

class SemanticTree
{
protected:
	Node* n; // данные таблицы
	SemanticTree* Up, * Left, * Right;
	TScaner* ts;
public:
	SemanticTree(SemanticTree* l, SemanticTree* r, SemanticTree* u, Node* Data);
	SemanticTree();
	~SemanticTree();
	void SetLeft(Node* Data);
	void SetRight(Node* Data);
	SemanticTree* FindUp(SemanticTree* From, LEX id);
	SemanticTree* FindUp(LEX id);
	SemanticTree* FindRightLeft(SemanticTree* From, LEX id);
	SemanticTree* FindRightLeft(LEX id);
	SemanticTree* FindUpOneLevel(SemanticTree* From, LEX id);
	void Print(void);

	void PrintError(const char* error, LEX lex, int except = 1);
	static SemanticTree* Cur;
	static TScaner* _sc;
	static void setTScaner(TScaner*& ts);
	static int lastUsersType; // для идентификации пользовательских типов
	static TypeVar convertLexicalTypeToVarType(int type); // получить тип переменной
	static TypeVar convertFromConstToVarType(int type); // получить тип неименованной константы
	static int combineVarType(int typeA, int typeB); // получить тип приведенной переменной
	void isAssignable(int typeVar, LEX lex); // проверить, можно ли присвоить typeVar типу typeA
	Node* getData();
	SemanticTree* getRLchild();
	SemanticTree* getLchild();
	SemanticTree* getRchild();

	// СЕМАНТИЧЕСКИЕ ПОДПРОГРАММЫ
	void SetCur(SemanticTree* a);
	// установить текущий узел дерева

	SemanticTree* GetCur(void);
	// получить значение текущего узла дерева

	SemanticTree* SemInclude(LEX a, TypeObject t);
	// занесение идентификатора a в таблицу с типом t

	void SemSetTypeObject(SemanticTree* Addr, TypeObject t);
	// установить тип t для переменной по адресу Addr

	void SemSetTypeVar(SemanticTree* Addr, int t);
	// установить тип простой t для переменной по адресу Addr

	void SemSetParam(SemanticTree* Addr, int n);
	// установить число формальных параметров n для функции
	// по адресу Addr

	void SemControlParam(SemanticTree* Addr, int n);
	// проверить равенство числа формальных параметров n
	// для функции по адресу Addr;

	SemanticTree* SemGetVar(LEX a);
	// найти в таблице переменную с именем a
	// и вернуть ссылку на соответствующий элемент дерева

	SemanticTree* SemGetObject(LEX a);
	// найти в таблице объект с именем a
	// и вернуть ссылку на соответствующий элемент дерева

	SemanticTree* SemGetClass(LEX a);
	// найти в таблице функцию с именем a
	// и вернуть ссылку на соответствующий элемент дерева

	SemanticTree* SemGetFunct(LEX a);
	// найти в таблице функцию с именем a
	// и вернуть ссылку на соответствующий элемент дерева

	int DupControl(SemanticTree* Addr, LEX a);

	SemanticTree* SetMakeNewArea();

	int getVarType();
	// получить тип объекта переменной

	// БЛОК НЕЙТРАЛИЗАЦИИ
	SemanticTree* AddField(SemanticTree* obj, LEX a);

};


