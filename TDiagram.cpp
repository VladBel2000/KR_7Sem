#include "TDiagram.h"

#define isLogicalConst(type) (type == key_false || type == key_true)
#define isConst(type) (isLogicalConst(type) || type == const_double || type == const_int)
#define isType(type) (type == type_ident || type == key_double || type == key_bool)
#define isOperationsL3(type) (type == type_mult || type == type_div || type == type_mod || type == type_and ||  type == type_iskl)
#define isOperationsL2(type) (type == type_plus || type == type_minus || type == type_or)
#define isOperationsL1(type) (type == type_lt || type == type_gt || type == type_le || type == type_ge)
#define isOperationsL0(type) (type == type_eq || type == type_ne)

int TDiagram::lookForward(int k) {
    LEX lex;
    int savedUk = _sc->getUk();
    int savedLine = _sc->getLine();
    int savedPos = _sc->getPosition();

    int nextType = 0;
    for (int i = 0; i < k; i++)
        nextType = _sc->Scaner(lex);
    _sc->setLine(savedLine);
    _sc->putUk(savedUk);
    _sc->setPosition(savedPos);
    return nextType;
}

int TDiagram::Error(string mess, int line, int pos)
{
    cout << "Sintax Error at " << line + 1 << ':' << pos + 1 << " -> '" << lex << "'  \t �����������: " << mess << endl;
    throw syntaxError;
    return 0;
}

void TDiagram::setSemanticTree(SemanticTree* st)
{
    this->st = st;
}

void TDiagram::Program()
{
    do
    {
        type = lookForward(1);
        if (type == type_class)
        {
            Class();
        }
        else if (type == key_void)
        {
            Function();
        }
        else if (isType(type))
        {
            Data();
        }
        else if (type == type_rbrcs)
        {
            return;
        }
        else if (type != type_end)
        {
            _sc->Scaner(lex);
            Error("��������� ���������� ����� ����", _sc->getLine(), _sc->getPosition());
        }
    } while (type != type_end);
}

int TDiagram::getType()
{
    return type;
}

void TDiagram::Data()
{
    int type = _sc->Scaner(lex);
    if (!isType(type))
    {
        Error("�� �������� �����", _sc->getLine(), _sc->getPosition());
    }

    /*!!!Semantica*/ // ��������� ���� �� ����� ��� ��� ��� ������
    int typeVar = st->convertLexicalTypeToVarType(type);
    SemanticTree* classType = nullptr;
    TypeObject typeObject = ObjVar;
    if (typeVar == TypeUndef)
    {
        // ��������� ��������� ������
        classType = st->SemGetClass(lex);
        typeObject = ObjClass;
        typeVar = classType->getVarType();
    }

    do {
        type = _sc->Scaner(lex);
        if (type != type_ident)
        {
            Error("�� �������� ���������������", _sc->getLine(), _sc->getPosition());
        }

        /*!!!Semantica*/ // ������� � ������ ����������
        SemanticTree* v = st->SemInclude(lex, typeObject);
        st->SemSetTypeVar(v, typeVar);
        if (typeObject == ObjClass)
        {
            v->getData()->dataIdent.typeClass = classType; // ������� ����� �����
        }

        if (lookForward(1) == type_assign)
        {
            _sc->Scaner(lex);
            /*!!!Semantica*/ // ��������� ��� �������������� ��������, ���������, ������� ��������� ��������.
            v->isAssignable(Expression(), v->getData()->id);
        }
    } while (lookForward(1) == type_comma && _sc->Scaner(lex) > 0);

    type = _sc->Scaner(lex);
    if (type != type_semicolon)
    {
        Error("��������� ';'", _sc->getLine(), _sc->getPosition());
    }
}

SemanticTree* TDiagram::Field() // ���������� ������ �� ���� ��� ����������
{
    int type = _sc->Scaner(lex);
    if (type != type_ident)
    {
        Error("�� �������� ���������������", _sc->getLine(), _sc->getPosition());
    }
    if (lookForward(1) != type_dot)
    {
        return st->SemGetVar(lex);
    }

    /*!!!Semantica*/ // ���������, ��� � ������ ���� ����
    //
    SemanticTree* object = st->SemGetObject(lex);
    SemanticTree* field = nullptr;
    while (lookForward(1) == type_dot)
    {
        _sc->Scaner(lex);
        type = _sc->Scaner(lex);
        if (type != type_ident)
        {
            Error("�� �������� �����", _sc->getLine(), _sc->getPosition());
        }

        field = object->getData()->dataIdent.typeClass->FindRightLeft(lex);
        if (field == NULL)
        {

            field->PrintError(" � ������ �� ������ ������� ", lex, 0);//////////////////////////////////
            field = st->AddField(object, lex); // ��������� ������ ���������� ����
        }
        object = field;
    }
    return field;
}


void TDiagram::ListOfFormalParameters(SemanticTree* func)
{
    int type;
    int paramsN = 0;
    if (lookForward(1) != type_rprths)
    {
        do {
            type = _sc->Scaner(lex);
            if (!isType(type))
            {
                Error("�� �������� ����� ������", _sc->getLine(), _sc->getPosition());
            }
            /*!!!Semantica*/ // ��������� ���������� ��������
            int typeVar = st->convertLexicalTypeToVarType(type);
            SemanticTree* classType = nullptr;
            TypeObject typeObject = ObjVar;
            if (typeVar == TypeUndef)
            {
                // ��������� ��������� ������
                classType = st->SemGetClass(lex);
                typeObject = ObjClass;
                typeVar = classType->getVarType();
            }

            type = _sc->Scaner(lex);
            if (type != type_ident)
            {
                Error("�� �������� ��������������� ", _sc->getLine(), _sc->getPosition());
            }

            SemanticTree* v = st->SemInclude(lex, typeObject);
            st->SemSetTypeVar(v, typeVar);
            paramsN++;
        } while (lookForward(1) == type_comma && _sc->Scaner(lex));
    }
    func->getData()->dataIdent.Param = paramsN;
}

void TDiagram::ListOfParameters(SemanticTree* func)
{
    int type;
    /*!!!Semantica*/ // ���������� �������� ������������ ����������
    //
    int paramsN = func->getData()->dataIdent.Param;
    SemanticTree* param = func->getRLchild();
    int countParam = 0;
    do {
        if (lookForward(1) != type_rprths)
        {
            if (param->getData()->dataIdent.typeVar != Field()->getData()->dataIdent.typeVar)
            {
                st->PrintError("������������ ���� ����������� ��������� ", lex, 0);
                // ������ ���������� ��� ������
            }
            countParam++;
        }
    } while (lookForward(1) == type_comma && (_sc->Scaner(lex) > 0));
    if (paramsN - countParam > 0)
    {
        st->PrintError("�� ���������� ���������� ��� ������ ������� ", lex, 0); ////////////////////////////////////////////////////////////////
        // �������� ��� ������, ���� �������� ������ ���������, ��� ����� ������� ��� ������ ������.
    }
}

void TDiagram::Function()
{
    int type;

    type = _sc->Scaner(lex);
    if (type != key_void)
    {
        Error(" ��������� 'void'", _sc->getLine(), _sc->getPosition());
    }

    type = _sc->Scaner(lex);
    if (type != type_ident)
    {
        Error("�� �������� ��������������� ", _sc->getLine(), _sc->getPosition());
    }
    /*!!!Semantica*/ // ������� ������� � ������
    SemanticTree* func = st->SemInclude(lex, ObjFunct);

    type = _sc->Scaner(lex);
    if (type != type_lprths)
    {
        Error("��������� '('", _sc->getLine(), _sc->getPosition());
    }

    ListOfFormalParameters(func);

    type = _sc->Scaner(lex);
    if (type != type_rprths)
    {
        Error("��������� ')'", _sc->getLine(), _sc->getPosition());
    }
    Operator();
    st->SetCur(func);
}

void TDiagram::FunctionCall()
{
    int type;

    if (lookForward(1) != type_ident)
    {
        Error("�� �������� ��������������� ", _sc->getLine(), _sc->getPosition());
    }

    SemanticTree* func = 0;
    if (lookForward(2) == type_dot)
    {
        func = Field();
    }
    else
    {
        _sc->Scaner(lex);
        func = st->Cur->FindUp(lex);
    }

    type = _sc->Scaner(lex);
    if (type != type_lprths)
    {
        Error("��������� '('", _sc->getLine(), _sc->getPosition());
    }

    ListOfParameters(func);

    type = _sc->Scaner(lex);
    if (type != type_rprths)
    {
        Error("��������� ')'", _sc->getLine(), _sc->getPosition());
    }
}

void TDiagram::Operator()
{
    LEX lex;
    if (lookForward(1) == type_semicolon)
    {
        _sc->Scaner(lex);
    }
    else if (lookForward(1) == type_lbrcs)
    {
        ComplexOperator();
    }
    else
    {
        SimpleOperator();
    }
}

void TDiagram::SimpleOperator()
{
    int type = lookForward(1);

    if (type == type_brk)
    {
        _sc->Scaner(lex);
        type = _sc->Scaner(lex);
        if (type != type_semicolon)
        {
            Error("��������� ';'", _sc->getLine(), _sc->getPosition());
        }
    }
    else if (type == type_cnt)
    {
        _sc->Scaner(lex);
        type = _sc->Scaner(lex);
        if (type != type_semicolon)
        {
            Error("��������� ';'", _sc->getLine(), _sc->getPosition());
        }
    }
    else if (type == type_ret)
    {
        _sc->Scaner(lex);
        type = _sc->Scaner(lex);
        if (type != type_semicolon)
        {
            Error("��������� ';'", _sc->getLine(), _sc->getPosition());
        }
    }
    else if (type == type_while)
    {
        CircleWhile();
    }
    else if (type == type_ident)
    {
        int k = 2;
        while (lookForward(k) == type_dot && lookForward(k + 1) == type_ident)
            k += 2;
        type = lookForward(k);
        if (type == type_assign)
        {
            Assignment();
        }
        else
            if (type == type_lprths)
            {
                FunctionCall();
            }
            else {
                type = _sc->Scaner(lex);
                Error(" �������� ������� �������� ", _sc->getLine(), _sc->getPosition());
            }

        type = _sc->Scaner(lex);
        if (type != type_semicolon)
        {
            Error("��������� ';'", _sc->getLine(), _sc->getPosition());
        }
    }
    else
    {
        type = _sc->Scaner(lex);
        Error(" �������� ������� �������� ", _sc->getLine(), _sc->getPosition());
    }
}

void TDiagram::ComplexOperator()
{
    int type;

    type = _sc->Scaner(lex);
    if (type != type_lbrcs)
    {
        Error("��������� '{'", _sc->getLine(), _sc->getPosition());
    }

    do {
        type = lookForward(1);
        if (type == type_semicolon && (_sc->Scaner(lex) > 0));
        else if (type == type_lbrcs)
        {
            ComplexOperator();
        }
        else if (isType(type) && lookForward(2) == type_ident)
        {
            Data();
        }
        else if (type != type_rbrcs)
        {
            Operator();
        }
    } while (lookForward(1) != type_rbrcs);

    type = _sc->Scaner(lex);
    if (type != type_rbrcs)
    {
        Error("��������� '}'", _sc->getLine(), _sc->getPosition());
    }
}

void TDiagram::CircleWhile()
{
    int type = _sc->Scaner(lex);
    if (type != type_while)
    {
        Error("��������� 'while'", _sc->getLine(), _sc->getPosition());
    }

    type = _sc->Scaner(lex);
    if (type != type_lprths)
    {
        Error("��������� '('", _sc->getLine(), _sc->getPosition());
    }

    SemanticTree* ret = st->SetMakeNewArea(); // ���������� ���������

    if (lookForward(1) != type_semicolon)
        Expression();

    type = _sc->Scaner(lex);
    if (type != type_rprths)
    {
        Error("��������� ')'", _sc->getLine(), _sc->getPosition());
    }

    Operator();
    st->SetCur(ret);
}

void TDiagram::Assignment()
{
    SemanticTree* f = Field();
    int type = _sc->Scaner(lex);
    if (type != type_assign)
    {
        Error("��������� '='", _sc->getLine(), _sc->getPosition());
    }
    // �������� ������������ �����
    f->isAssignable(Expression(), f->getData()->id);
}


// ������-�� const == undef
int TDiagram::Expression()
{
    LEX lex;
    int varType = Expression1();
    while (isOperationsL0(lookForward(1)))
    {
        _sc->Scaner(lex);
        varType = st->combineVarType(varType, Expression1());
    }
    return varType;
}

int TDiagram::Expression1()
{
    LEX lex;
    int varType = Expression2();
    while (isOperationsL1(lookForward(1)))
    {
        _sc->Scaner(lex);
        varType = st->combineVarType(varType, Expression2());
    }
    return varType;
}

int TDiagram::Expression2()
{
    LEX lex;
    int varType = Expression3();
    while (isOperationsL2(lookForward(1)))
    {
        _sc->Scaner(lex);
        varType = st->combineVarType(varType, Expression3());
    }
    return varType;
}

int TDiagram::Expression3()
{
    LEX lex;
    int varType = Expression4();
    while (isOperationsL3(lookForward(1)))
    {
        _sc->Scaner(lex);
        varType = st->combineVarType(varType, Expression4());
    }
    return varType;
}

int TDiagram::Expression4()
{
    LEX lex;
    int varType = ElementaryExpression();
    while (lookForward(1) == type_not)
    {
        _sc->Scaner(lex);
        varType = st->combineVarType(ElementaryExpression(), varType);
    }
    return varType;
}

int TDiagram::ElementaryExpression()
{
    int varType = TypeUndef;
    if (lookForward(1) == type_lprths && _sc->Scaner(lex))
    {
        varType = Expression();
        if (_sc->Scaner(lex) != type_rprths)
        {
            Error("��������� ')' ", _sc->getLine(), _sc->getPosition());
        }
    }
    else
        if (isConst(lookForward(1)))
        {
            type = _sc->Scaner(lex);
            varType = st->convertFromConstToVarType(type); // ������� ��� ���������
        }
        else
        {
            varType = Field()->getVarType();
        }
    return varType;
}


void TDiagram::Class()
{
    int type = _sc->Scaner(lex);
    if (type != type_class)
    {
        Error("�� �������� �������� ����� 'class'", _sc->getLine(), _sc->getPosition());
    }


    type = _sc->Scaner(lex);
    if (type != type_ident)
    {
        Error("�� �������� ���������������", _sc->getLine(), _sc->getPosition());
    }

    /*!!!Semantica*/ // ������� ����������� ������ � ������
    SemanticTree* v = st->SemInclude(lex, ObjTypeClass);
    v->getData()->dataIdent.typeVar = st->lastUsersType++; // ����������� ���������������� �����

    type = _sc->Scaner(lex);
    if (type != type_lbrcs)
    {
        Error("��������� '{' ", _sc->getLine(), _sc->getPosition());
    }

    Program();

    type = _sc->Scaner(lex);
    if (type != type_rbrcs)
    {
        Error("��������� '}' ", _sc->getLine(), _sc->getPosition());
    }
    st->SetCur(v);
}
