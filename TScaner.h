#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "defs.h"
#include <fstream>

class TScaner
{
protected:
	IM t; // исходный модуль - текст программы
	int uk;
	FILE* in;
	int line, pos; // строка, столбец
	int type = 0;
public:
	bool stopAfterError = false;
	void putUk(int i);
	int getUk(void);
	int getPosition();
	int getLine();
	void setLine(int line);
	void setPosition(int position);
	void PrintError(int i, const char* error = 0, bool critical = false);
	void GetData(const char* fileName = 0);
	int Scaner(LEX l);
};
#pragma once
