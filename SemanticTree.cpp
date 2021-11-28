#include "SemanticTree.h"
#define max(a,b) a<b? b : a


SemanticTree::SemanticTree(SemanticTree* l, SemanticTree* r, SemanticTree* u, Node* d)
// ����������� ������� ���� � ��������� ������� � �������
{
	n = new Node();
	Up = u; Left = l; Right = r; // ���������� ������
	memcpy(n, d, sizeof(Node)); // ���������� ������
}

SemanticTree::SemanticTree(void)
// ����������� ������� ����� ���� � ������� ������� � �������
{
	n = new Node();
	Up = NULL; Left = NULL; Right = NULL;
	memcpy(n, &("------"), sizeof(Node));
}

void SemanticTree::SetLeft(Node* Data)
// ������� ������ ������� �� ������� ������� this
{
	SemanticTree* a = new SemanticTree(NULL, NULL, this, Data); // ����� �������
	Left = a; // ������� this � ����� ��������
}

void SemanticTree::SetRight(Node* Data)
// ������� ������� ������� �� ������� ������� this
{
	SemanticTree* a = new SemanticTree(NULL, NULL, this, Data); // ����� �������
	Right = a; // ������� this � ����� ��������
}

SemanticTree* SemanticTree::FindUp(LEX id)
// ����� ������ � ������, ������� �� ������� ������� this
// �� ��� ����� ����� �� ������
{
	return FindUp(this, id);
}

SemanticTree* SemanticTree::FindUp(SemanticTree* From, LEX id)
// ����� ������ � ������ �� �������� ������� From
// �� ��� ����� ����� �� ������
{
	SemanticTree* i = From; // ������� ������� ������
	while ((i != NULL) &&
		(memcmp(id, i->n->id, max(strlen(i->n->id), strlen(id))) != 0))
		i = i->Up; // ����������� ������ �� ������
	return i;
}

SemanticTree* SemanticTree::FindRightLeft(LEX id)
// ����� ������ �������� ������� ������� this
{
	return FindRightLeft(this, id);
}

SemanticTree* SemanticTree::FindRightLeft(SemanticTree* From, LEX id)
// ����� ������ �������� �������� ������� From
{
	SemanticTree* i = From->Right; // ������� ������� ������
	while ((i != NULL) && (memcmp(id, i->n->id, max(strlen(i->n->id), strlen(id))) != 0))
		i = i->Left;
	// ������� ������ ������� �� ����� ������
	return i;
}

void SemanticTree::Print(void)
// ���������� ��������� ������ ������
{
	printf("������� � ������� %s ----->", n->id);
	if (Left != NULL) printf(" ����� ������ %s", Left->n->id);
	if (Right != NULL) printf(" ������ ������ %s", Right->n->id);
	printf("\n");
	if (Left != NULL) Left->Print();
	if (Right != NULL) Right->Print();
}

Node* SemanticTree::getData()
{
	return this->n;
}

TypeVar SemanticTree::convertFromConstToVarType(int type)
{
	switch (type)
	{
	case key_false:
		return TypeBool;
	case key_true:
		return TypeBool;
	case const_int:
		return TypeDouble;
	default:
		break;
	}
}

SemanticTree* SemanticTree::getLchild()
{
	return this->Left;
}

SemanticTree* SemanticTree::getRchild()
{
	return this->Right;
}

SemanticTree* SemanticTree::getRLchild()
{
	if (this->Right)
		return this->Right->Left;
	return nullptr;
}

SemanticTree* SemanticTree::FindUpOneLevel(SemanticTree* From, LEX id)
// ����� �������� id ����� �� ������ �� ������� ������� From.
// ����� �������������� �� ����� ������ ����������� �� ����� ������
{
	SemanticTree* i = From; // ������� ������� ������
	while ((i != NULL)
		&& (i->Up)
		&& (i->Up->Right != i)
		)
	{
		if (memcmp(id, i->n->id, max(strlen(i->n->id), strlen(id))) == 0)
			return i; // �a��� ����������� �������������
		i = i->Up; // ����������� ������ �� ������
	}
	return NULL;
}

SemanticTree::~SemanticTree()
// ���������� 
{
	if (Left != nullptr)
		delete Left;
	if (Right != nullptr)
		delete Right;
	delete n;
}








// ���������� 9 ������

SemanticTree* SemanticTree::Cur = nullptr;
TScaner* SemanticTree::_sc = nullptr;
int SemanticTree::lastUsersType = FirstUserType;

void SemanticTree::setTScaner(TScaner*& ts)
{
	_sc = ts;
}

void SemanticTree::PrintError(const char* error, LEX lex, int except)
{
	printf("Semantic Error: ");
	printf("���:%d \t%s -> %s\n", _sc->getLine() + 1, error, lex);
	if (except)
		throw semanticError;
}

void SemanticTree::SetCur(SemanticTree* a)
// ���������� ������� ���� ������
{
	Cur = a;
}

SemanticTree* SemanticTree::GetCur(void)
// �������� �������� �������� ���� ������
{
	return Cur;
}

SemanticTree* SemanticTree::SetMakeNewArea()
{
	SemanticTree* semTreeVert;
	Node b;
	memcpy(&b.id, &"", 2); b.dataIdent.typeObject = EMPTY; // ������ �������
	Cur->SetLeft(&b); // ������� ������� - ������� ���������
	Cur = Cur->Left;
	semTreeVert = Cur; // ��� ����� �������� ����� ������ �� ��������
	memcpy(&b.id, &"", 2); b.dataIdent.typeObject = EMPTY; // ������ �������
	Cur->SetRight(&b); // ������� ������ �������
	Cur = Cur->Right;
	return semTreeVert;
}

SemanticTree* SemanticTree::SemInclude(LEX a, TypeObject t)
// ��������� �������������� a � ������� � ����� t
{
	if (DupControl(Cur, a))
		PrintError("��������� �������� �������������� ", a, 0); /////////////////////////////////////////////////////////////////////
		// ��������� ��������� � ������������ 
	SemanticTree* semTreeVert; Node b;
	if (t == ObjTypeClass)
	{
		memcpy(b.id, a, strlen(a) + 1); b.dataIdent.typeObject = t;
		Cur->SetLeft(&b); // ������� ������� - �������� ������
		Cur = Cur->Left;
		semTreeVert = Cur; // ��� ����� �������� ����� ������ �� ��������
		memcpy(&b.id, &"", 2); b.dataIdent.typeObject = EMPTY; // ������ �������
		Cur->SetRight(&b); // ������� ������ �������
		Cur = Cur->Right;
		return semTreeVert;
	}
	else if (t == ObjFunct)
	{
		memcpy(b.id, a, strlen(a) + 1); b.dataIdent.typeObject = t;
		b.dataIdent.Param = 0; // ���������� ���������� �������
		Cur->SetLeft(&b); // ������� ������� - �������
		Cur = Cur->Left;
		semTreeVert = Cur; // ��� ����� �������� ����� ������ �� �������
		memcpy(&b.id, &"", 2); b.dataIdent.typeObject = EMPTY; // ������ �������
		Cur->SetRight(&b); // ������� ������ �������
		Cur = Cur->Right;
		return semTreeVert;
	}
	else
	{
		memcpy(b.id, a, strlen(a) + 1); b.dataIdent.typeObject = t;
		Cur->SetLeft(&b); // ������� ������� - ����������
		Cur = Cur->Left;
		return Cur;
	}
}

void SemanticTree::SemSetTypeObject(SemanticTree* Addr, TypeObject t)
// ���������� ��� t ��� ���������� �� ������ Addr
{
	Addr->n->dataIdent.typeObject = t;
}

void SemanticTree::SemSetTypeVar(SemanticTree* Addr, int t)
// ���������� ��� t ��� ���������� �� ������ Addr
{
	Addr->n->dataIdent.typeVar = t;
}

void SemanticTree::SemSetParam(SemanticTree* Addr, int num)
// ���������� ����� ���������� ���������� n ��� ������� �� ������ Addr
{
	Addr->n->dataIdent.Param = num;
}
void SemanticTree::SemControlParam(SemanticTree* Addr, int num)
// ��������� ��������� ����� ���������� ���������� ��������
// n ��� ������� �� ������ Addr
{
	if (num != Addr->n->dataIdent.Param)
		PrintError("�������� ����� ���������� � ������� ", Addr->n->id, 0);  /////////////////////////
}

SemanticTree* SemanticTree::SemGetVar(LEX a)
// ����� � ������� ���������� � ������ a
// � ������� ������ �� ��������������� ������� ������
{
	SemanticTree* typeVar = FindUp(Cur, a);
	if (typeVar == NULL)
	{
		PrintError("����������� �������� �������������� ���������� ", a, 0); ////////////////////////////////////// 
		typeVar = Cur->SemInclude(a, ObjVar); // �������� �������� ��������������
	}
	else
		if (typeVar->n->dataIdent.typeObject == ObjFunct)
		{
			PrintError("�������� ������������� ������ ������� ", a, 0);

		}
		else
			if (typeVar->n->dataIdent.typeObject == ObjClass)
			{
				PrintError("�������� ������������� ������ ������� ������ ", a, 0);
			}
	return typeVar;
}

SemanticTree* SemanticTree::SemGetObject(LEX a)
// ����� � ������� ������ � ������ a
// � ������� ������ �� ��������������� ������� ������
{
	SemanticTree* typeVar = FindUp(Cur, a);
	if (typeVar == NULL) {
		PrintError("����������� �������� ������� ", a, 0); ////////////////////////////////////// 
		typeVar = Cur->SemInclude(a, ObjClass); // �������� �������� ��������������
	}
	else
		if (typeVar->n->dataIdent.typeObject == ObjVar)
		{
			PrintError("�������� ������������� ���������� ", a, 0);
			this->SemInclude(a, ObjClass);
		}
		else
			if (typeVar->n->dataIdent.typeObject == ObjFunct)
			{
				PrintError("�������� ������������� ������ ������� ", a, 0);
				this->SemInclude(a, ObjClass);
			}
			else
				if (typeVar->n->dataIdent.typeObject == ObjTypeClass)
				{
					PrintError("�������� ������������� ���� ������ ", a, 0);
					typeVar = this->SemInclude(a, ObjClass);
					typeVar->getData()->dataIdent.typeClass = typeVar;
				}


	return typeVar;
}

SemanticTree* SemanticTree::SemGetClass(LEX a)
// ����� � ������� ��� ������ � ������ a
// � ������� ������ �� ��������������� ������� ������
{
	SemanticTree* typeVar = FindUp(Cur, a);
	if (typeVar == NULL) {
		PrintError("����������� �������� ������ ", a, 0);
		////////////////////////////////////// 
		SemanticTree* v = Cur->SemInclude(a, ObjTypeClass); // �������� �������� ��������������
		this->SetCur(v);
		typeVar = v;
	}
	else
		if (typeVar->n->dataIdent.typeObject == ObjVar)
			PrintError("�������� ������������� ���������� ", a, 0);
		else
			if (typeVar->n->dataIdent.typeObject == ObjFunct)
				PrintError("�������� ������������� ������ ������� ", a, 0);

	return typeVar;
}


SemanticTree* SemanticTree::SemGetFunct(LEX a)
// ����� � ������� ������� � ������ a
// � ������� ������ �� ��������������� ������� ������.
{
	SemanticTree* typeVar = FindUp(Cur, a);
	if (typeVar == NULL)
	{
		PrintError("����������� �������� ������� ", a, 0);
		SemanticTree* v = Cur->SemInclude(a, ObjFunct); // �������� �������� ��������������
		this->SetCur(v);
		typeVar = v;
	}
	else
		if (typeVar->n->dataIdent.typeObject != ObjFunct)
		{
			PrintError("������������� �� �������� ��������  ", a, 0);
			// ������� ������������� ��������
			typeVar->getData()->dataIdent.typeObject = ObjFunct;
			Node b; memcpy(&b.id, &"", 2); b.dataIdent.typeObject = EMPTY; // ������ �������
			typeVar->SetRight(&b); // ������� ������ �������
		}
	return typeVar;
}

int SemanticTree::DupControl(SemanticTree* Addr, LEX a)
// �������� �������������� � �� ��������� �������� ������ �����.
// ����� �������������� ����� �� ������� Addr.
{
	if (FindUpOneLevel(Addr, a) == NULL) return 0;
	return 1;
}


TypeVar SemanticTree::convertLexicalTypeToVarType(int type)
{
	switch (type)
	{
	case key_bool:
		return TypeBool;
		break;
	case key_double:
		return TypeDouble;
		break;
	default:
		return TypeUndef;
		break;
	}
}

int SemanticTree::getVarType() {
	return n->dataIdent.typeVar;
}

int SemanticTree::combineVarType(int typeA, int typeB)
{
	if (typeA == typeB)
		return typeA;

	if (typeA < FirstUserType && typeB < FirstUserType)
	{
		return max(typeA, typeB);
	}
	else
	{
		return TypeUndef;
	}
}

void SemanticTree::isAssignable(int typeVar, LEX lex)
{
	SemanticTree* v = Cur->SemGetVar(lex);
	if (v->getVarType() < typeVar)
	{
		PrintError("���������� ��������� ��������� ��������� ����������\t ", lex, 0);
		// ���������� ������ � ���� ������
	}

	else
		this->getData()->dataIdent.FlagInit = 1;
}


///
///
/// ������������� ������������� ������
/// 
/// 

SemanticTree* SemanticTree::AddField(SemanticTree* object, LEX a) {
	Node n;
	SemanticTree* r = object->getRchild();
	if (r)
		while (r->Left)
			r = Left;
	else {
		strcpy(n.id, "");
		n.dataIdent.typeObject = EMPTY;
		object->SetRight(&n);
		r = object->getRchild();
	}
	strcpy(n.id, a);
	n.dataIdent.typeObject = ObjVar;
	n.dataIdent.typeVar = TypeUndef;
	r->SetLeft(&n);
	n.dataIdent.typeClass = r->Left;
	return r->Left;
};