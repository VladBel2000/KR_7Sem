#pragma once
#include "TScaner.h"
#include "defs.h"
#include <string.h>
#include <iostream>

#define EMPTY -1 // ������� ������ ������

enum TypeObject {
	ObjVar = 1, // ������� ����������
	ObjClass, // �����
	ObjTypeClass, // ��� ������
	ObjFunct, // �������
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
	// ��� �������� � ������� ���������������
{
	//LEX id; // ������������� ���������� 
	int typeObject; // ��� ���������� t=ObjVar, ��� ������� t=ObjFunct
	int typeVar; // ��� ��� ���������� �����
	SemanticTree* typeClass = nullptr;

	// ��� �������
	int Param = 0; // ���������� ����������

	// ������ �� ��������
	void* data = nullptr;

	// �������������� ������:
	int FlagInit; // ���� ��������� �������������
	char* Addr; // ����� � ����������������� ���������
};

struct Node // ���������� �� ����� ����������
{
	LEX id; // ������������� ����������
	DataIdent dataIdent; // ���������� � �������� ���� �������
};

class SemanticTree
{
protected:
	Node* n; // ������ �������
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
	static int lastUsersType; // ��� ������������� ���������������� �����
	static TypeVar convertLexicalTypeToVarType(int type); // �������� ��� ����������
	static TypeVar convertFromConstToVarType(int type); // �������� ��� ������������� ���������
	static int combineVarType(int typeA, int typeB); // �������� ��� ����������� ����������
	void isAssignable(int typeVar, LEX lex); // ���������, ����� �� ��������� typeVar ���� typeA
	Node* getData();
	SemanticTree* getRLchild();
	SemanticTree* getLchild();
	SemanticTree* getRchild();

	// ������������� ������������
	void SetCur(SemanticTree* a);
	// ���������� ������� ���� ������

	SemanticTree* GetCur(void);
	// �������� �������� �������� ���� ������

	SemanticTree* SemInclude(LEX a, TypeObject t);
	// ��������� �������������� a � ������� � ����� t

	void SemSetTypeObject(SemanticTree* Addr, TypeObject t);
	// ���������� ��� t ��� ���������� �� ������ Addr

	void SemSetTypeVar(SemanticTree* Addr, int t);
	// ���������� ��� ������� t ��� ���������� �� ������ Addr

	void SemSetParam(SemanticTree* Addr, int n);
	// ���������� ����� ���������� ���������� n ��� �������
	// �� ������ Addr

	void SemControlParam(SemanticTree* Addr, int n);
	// ��������� ��������� ����� ���������� ���������� n
	// ��� ������� �� ������ Addr;

	SemanticTree* SemGetVar(LEX a);
	// ����� � ������� ���������� � ������ a
	// � ������� ������ �� ��������������� ������� ������

	SemanticTree* SemGetObject(LEX a);
	// ����� � ������� ������ � ������ a
	// � ������� ������ �� ��������������� ������� ������

	SemanticTree* SemGetClass(LEX a);
	// ����� � ������� ������� � ������ a
	// � ������� ������ �� ��������������� ������� ������

	SemanticTree* SemGetFunct(LEX a);
	// ����� � ������� ������� � ������ a
	// � ������� ������ �� ��������������� ������� ������

	int DupControl(SemanticTree* Addr, LEX a);

	SemanticTree* SetMakeNewArea();

	int getVarType();
	// �������� ��� ������� ����������

	// ���� �������������
	SemanticTree* AddField(SemanticTree* obj, LEX a);

};


