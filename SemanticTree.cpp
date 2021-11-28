#include "SemanticTree.h"
#define max(a,b) a<b? b : a


SemanticTree::SemanticTree(SemanticTree* l, SemanticTree* r, SemanticTree* u, Node* d)
// конструктор создает узел с заданными связями и данными
{
	n = new Node();
	Up = u; Left = l; Right = r; // установили ссылки
	memcpy(n, d, sizeof(Node)); // установили данные
}

SemanticTree::SemanticTree(void)
// конструктор создает новый узел с пустыми связями и данными
{
	n = new Node();
	Up = NULL; Left = NULL; Right = NULL;
	memcpy(n, &("------"), sizeof(Node));
}

void SemanticTree::SetLeft(Node* Data)
// создать левого потомка от текущей вершины this
{
	SemanticTree* a = new SemanticTree(NULL, NULL, this, Data); // новая вершина
	Left = a; // связали this с новой вершиной
}

void SemanticTree::SetRight(Node* Data)
// создать правого потомка от текущей вершины this
{
	SemanticTree* a = new SemanticTree(NULL, NULL, this, Data); // новая вершина
	Right = a; // связали this с новой вершиной
}

SemanticTree* SemanticTree::FindUp(LEX id)
// поиск данных в дереве, начиная от текущей вершины this
// до его корня вверх по связям
{
	return FindUp(this, id);
}

SemanticTree* SemanticTree::FindUp(SemanticTree* From, LEX id)
// поиск данных в дереве от заданной вершины From
// до его корня вверх по связям
{
	SemanticTree* i = From; // текущая вершина поиска
	while ((i != NULL) &&
		(memcmp(id, i->n->id, max(strlen(i->n->id), strlen(id))) != 0))
		i = i->Up; // поднимаемся наверх по связям
	return i;
}

SemanticTree* SemanticTree::FindRightLeft(LEX id)
// поиск прямых потомков текущей вершины this
{
	return FindRightLeft(this, id);
}

SemanticTree* SemanticTree::FindRightLeft(SemanticTree* From, LEX id)
// поиск прямых потомков заданной вершины From
{
	SemanticTree* i = From->Right; // текущая вершина поиска
	while ((i != NULL) && (memcmp(id, i->n->id, max(strlen(i->n->id), strlen(id))) != 0))
		i = i->Left;
	// обходим только соседей по левым связям
	return i;
}

void SemanticTree::Print(void)
// отладочная программа печати дерева
{
	printf("Вершина с данными %s ----->", n->id);
	if (Left != NULL) printf(" слева данные %s", Left->n->id);
	if (Right != NULL) printf(" справа данные %s", Right->n->id);
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
// Поиск элемента id вверх по дереву от текущей вершины From.
// Поиск осуществляется на одном уровне вложенности по левым связям
{
	SemanticTree* i = From; // текущая вершина поиска
	while ((i != NULL)
		&& (i->Up)
		&& (i->Up->Right != i)
		)
	{
		if (memcmp(id, i->n->id, max(strlen(i->n->id), strlen(id))) == 0)
			return i; // нaшли совпадающий идентификатор
		i = i->Up; // поднимаемся наверх по связям
	}
	return NULL;
}

SemanticTree::~SemanticTree()
// деструктор 
{
	if (Left != nullptr)
		delete Left;
	if (Right != nullptr)
		delete Right;
	delete n;
}








// выполнение 9 работы

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
	printf("стр:%d \t%s -> %s\n", _sc->getLine() + 1, error, lex);
	if (except)
		throw semanticError;
}

void SemanticTree::SetCur(SemanticTree* a)
// установить текущий узел дерева
{
	Cur = a;
}

SemanticTree* SemanticTree::GetCur(void)
// получить значение текущего узла дерева
{
	return Cur;
}

SemanticTree* SemanticTree::SetMakeNewArea()
{
	SemanticTree* semTreeVert;
	Node b;
	memcpy(&b.id, &"", 2); b.dataIdent.typeObject = EMPTY; // пустая вершина
	Cur->SetLeft(&b); // сделали вершину - область видимости
	Cur = Cur->Left;
	semTreeVert = Cur; // это точка возврата после выхода из описания
	memcpy(&b.id, &"", 2); b.dataIdent.typeObject = EMPTY; // пустая вершина
	Cur->SetRight(&b); // сделали пустую вершину
	Cur = Cur->Right;
	return semTreeVert;
}

SemanticTree* SemanticTree::SemInclude(LEX a, TypeObject t)
// занесение идентификатора a в таблицу с типом t
{
	if (DupControl(Cur, a))
		PrintError("Повторное описание идентификатора ", a, 0); /////////////////////////////////////////////////////////////////////
		// пропустим сообщение о дублировании 
	SemanticTree* semTreeVert; Node b;
	if (t == ObjTypeClass)
	{
		memcpy(b.id, a, strlen(a) + 1); b.dataIdent.typeObject = t;
		Cur->SetLeft(&b); // сделали вершину - описание класса
		Cur = Cur->Left;
		semTreeVert = Cur; // это точка возврата после выхода из описания
		memcpy(&b.id, &"", 2); b.dataIdent.typeObject = EMPTY; // пустая вершина
		Cur->SetRight(&b); // сделали пустую вершину
		Cur = Cur->Right;
		return semTreeVert;
	}
	else if (t == ObjFunct)
	{
		memcpy(b.id, a, strlen(a) + 1); b.dataIdent.typeObject = t;
		b.dataIdent.Param = 0; // количество параметров функции
		Cur->SetLeft(&b); // сделали вершину - функцию
		Cur = Cur->Left;
		semTreeVert = Cur; // это точка возврата после выхода из функции
		memcpy(&b.id, &"", 2); b.dataIdent.typeObject = EMPTY; // пустая вершина
		Cur->SetRight(&b); // сделали пустую вершину
		Cur = Cur->Right;
		return semTreeVert;
	}
	else
	{
		memcpy(b.id, a, strlen(a) + 1); b.dataIdent.typeObject = t;
		Cur->SetLeft(&b); // сделали вершину - переменную
		Cur = Cur->Left;
		return Cur;
	}
}

void SemanticTree::SemSetTypeObject(SemanticTree* Addr, TypeObject t)
// установить тип t для переменной по адресу Addr
{
	Addr->n->dataIdent.typeObject = t;
}

void SemanticTree::SemSetTypeVar(SemanticTree* Addr, int t)
// установить тип t для переменной по адресу Addr
{
	Addr->n->dataIdent.typeVar = t;
}

void SemanticTree::SemSetParam(SemanticTree* Addr, int num)
// установить число формальных параметров n для функции по адресу Addr
{
	Addr->n->dataIdent.Param = num;
}
void SemanticTree::SemControlParam(SemanticTree* Addr, int num)
// проверить равенство числа формальных параметров значению
// n для функции по адресу Addr
{
	if (num != Addr->n->dataIdent.Param)
		PrintError("Неверное число параметров у функции ", Addr->n->id, 0);  /////////////////////////
}

SemanticTree* SemanticTree::SemGetVar(LEX a)
// найти в таблице переменную с именем a
// и вернуть ссылку на соответствующий элемент дерева
{
	SemanticTree* typeVar = FindUp(Cur, a);
	if (typeVar == NULL)
	{
		PrintError("Отсутствует описание идентификатора переменной ", a, 0); ////////////////////////////////////// 
		typeVar = Cur->SemInclude(a, ObjVar); // включаем описание идентификатора
	}
	else
		if (typeVar->n->dataIdent.typeObject == ObjFunct)
		{
			PrintError("Неверное использование вызова функции ", a, 0);

		}
		else
			if (typeVar->n->dataIdent.typeObject == ObjClass)
			{
				PrintError("Неверное использование вызова объекта класса ", a, 0);
			}
	return typeVar;
}

SemanticTree* SemanticTree::SemGetObject(LEX a)
// найти в таблице объект с именем a
// и вернуть ссылку на соответствующий элемент дерева
{
	SemanticTree* typeVar = FindUp(Cur, a);
	if (typeVar == NULL) {
		PrintError("Отсутствует описание объекта ", a, 0); ////////////////////////////////////// 
		typeVar = Cur->SemInclude(a, ObjClass); // включаем описание идентификатора
	}
	else
		if (typeVar->n->dataIdent.typeObject == ObjVar)
		{
			PrintError("Неверное использование переменной ", a, 0);
			this->SemInclude(a, ObjClass);
		}
		else
			if (typeVar->n->dataIdent.typeObject == ObjFunct)
			{
				PrintError("Неверное использование вызова функции ", a, 0);
				this->SemInclude(a, ObjClass);
			}
			else
				if (typeVar->n->dataIdent.typeObject == ObjTypeClass)
				{
					PrintError("Неверное использование типа класса ", a, 0);
					typeVar = this->SemInclude(a, ObjClass);
					typeVar->getData()->dataIdent.typeClass = typeVar;
				}


	return typeVar;
}

SemanticTree* SemanticTree::SemGetClass(LEX a)
// найти в таблице тип класса с именем a
// и вернуть ссылку на соответствующий элемент дерева
{
	SemanticTree* typeVar = FindUp(Cur, a);
	if (typeVar == NULL) {
		PrintError("Отсутствует описание класса ", a, 0);
		////////////////////////////////////// 
		SemanticTree* v = Cur->SemInclude(a, ObjTypeClass); // включаем описание идентификатора
		this->SetCur(v);
		typeVar = v;
	}
	else
		if (typeVar->n->dataIdent.typeObject == ObjVar)
			PrintError("Неверное использование переменной ", a, 0);
		else
			if (typeVar->n->dataIdent.typeObject == ObjFunct)
				PrintError("Неверное использование вызова функции ", a, 0);

	return typeVar;
}


SemanticTree* SemanticTree::SemGetFunct(LEX a)
// найти в таблице функцию с именем a
// и вернуть ссылку на соответствующий элемент дерева.
{
	SemanticTree* typeVar = FindUp(Cur, a);
	if (typeVar == NULL)
	{
		PrintError("Отсутствует описание функции ", a, 0);
		SemanticTree* v = Cur->SemInclude(a, ObjFunct); // включаем описание идентификатора
		this->SetCur(v);
		typeVar = v;
	}
	else
		if (typeVar->n->dataIdent.typeObject != ObjFunct)
		{
			PrintError("Идентификатор не является функцией  ", a, 0);
			// Сделаем идентификатор функцией
			typeVar->getData()->dataIdent.typeObject = ObjFunct;
			Node b; memcpy(&b.id, &"", 2); b.dataIdent.typeObject = EMPTY; // пустая вершина
			typeVar->SetRight(&b); // сделали пустую вершину
		}
	return typeVar;
}

int SemanticTree::DupControl(SemanticTree* Addr, LEX a)
// Проверка идентификатора а на повторное описание внутри блока.
// Поиск осуществляется вверх от вершины Addr.
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
		PrintError("Невозможно присвоить результат выражения переменной\t ", lex, 0);
		// пропускаем ошибку и идем дальше
	}

	else
		this->getData()->dataIdent.FlagInit = 1;
}


///
///
/// НЕЙТРАЛИЗАЦИЯ СЕМАНТИЧЕСКИХ ОШИБОК
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