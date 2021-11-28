#include "TScaner.h"

#define KEYWORD_MAX 12
// таблица ключевых слов
LEX KeyWords[KEYWORD_MAX] = {
	"bool", "double", "void", "false", "true",
	"class", "return", "break", "continue", "while", "int"
};
int KeyWordIndex[KEYWORD_MAX] = {
	key_bool, key_double, key_void, key_false, key_true,
	type_class, type_ret, type_brk, type_cnt, type_while, type_int
};

void TScaner::putUk(int i)
{
	uk = i;
}

int TScaner::getUk(void)
{
	return uk;
}

int TScaner::getPosition()
{
	return pos;
}

int TScaner::getLine()
{
	return line;
}

void TScaner::setLine(int line)
{
	this->line = line;
}

void TScaner::setPosition(int position)
{
	pos = position;
}

// выдача сообщения об ошибке
void TScaner::PrintError(int i, const char* error, bool critical)
{
	if (error == 0)
		error = new char[] { "" };
	printf("Lexical Error at");
	switch (i)
	{

	case err_symbol:
		// ошибочный символ
		printf(" Стр %d стлб %d \tОшибка  %d : Неверный символ %s\n", line + 1, pos + 1, i, error);
		break;
	case err_type:
		// ошибочный тип
		printf(" Стр %d стлб %d \tОшибка %d: Неверная лексема %s\n", line + 1, pos + 1, i, error);
		break;
	case err_const:
		// ошибочная константа
		printf(" Стр %d стлб %d \tОшибка %d: Неверная константа %s\n", line + 1, pos + 1, i, error);
		break;
	case err_ident:
		// ошибочный идентификатор
		printf(" Стр %d стлб %d \tОшибка %d: Неверный идентификатор %s\n", line + 1, pos + 1, i, error);
		break;

	default:
		printf(" Стр %d стлб %d \tНеизвестная ошибка %d!  %s\n", line + 1, pos + 1, i, error);
		break;
	}

	// завершаем сканирование
	if (stopAfterError && critical)
		exit(0);
}

int TScaner::Scaner(LEX l)
{
	int type = 0;
	int i; // текущая длина лексемы
	for (i = 0; i < MAX_LEX; i++) // отчищаем лексему
		l[i] = 0;
	i = 0;

start:

	// пропуск комментария
	while (t[uk] == '/' && t[uk + 1] == '/')
	{
		uk += 2;
		while (t[uk++] != '\n' && t[uk] != '\0');
		line++;
		pos = 0;
		goto start;
	}

	// пропуск незначащих символов
	while (t[uk] == ' ' || t[uk] == '\t' || t[uk] == '\n')
	{
		if (t[uk++] == '\n')
		{
			line++;
			pos = 0;
		}
		else
		{
			pos++;
		}
		goto start;
	}

	// Начало анализа
		// конец исходного модуля
	if (t[uk] == '\0')
	{
		l[0] = t[uk];
		pos++;
		return type_end;
	}
	else
		// обработка константы c плавающей точкой
		if (isPointDigit)
		{
			l[i++] = t[uk++];
			l[i++] = t[uk++];
			pos += 2;
		double_go:
			while (isNumber)
			{
				if (i < MAX_LEX)
					l[i++] = t[uk++];
				else
					uk++;
				pos++;
			};

			if (i < 3 || isBigLetter || isSmallLetter || t[uk] == '_') // проверка на символ ограничетель
			{
				l[i++] = t[uk];
				PrintError(err_type, l);
				return type_error;
			}
			if (i >= MAX_DOUBLE)
			{
				PrintError(err_const, " слишком длинная double константа");
				return type_error;
			}
			return const_double;
		}
		else
			// обработка целочисленной константы
			if (isNumber)
			{
				l[i++] = t[uk++];
				pos++;
				while (isNumber)
				{
					if (i < MAX_LEX)
						l[i++] = t[uk++];
					else
						uk++;
					pos++;
				};
				// Проверка на double
				if (t[uk] == '.')
				{
					l[i++] = t[uk++];
					pos++;
					goto double_go;
				}

				if (isBigLetter || isSmallLetter || t[uk] == '_')  // проверка на символ ограничетель
				{
					l[i++] = t[uk];
					PrintError(err_type, l);
					return type_error;
				}

				if (i >= MAX_INT)
				{
					PrintError(err_const, " Cлишком длинная константа c плавающей точкой");
					return type_error;
				}

				return const_int;
			}
			else
				// обработка идентификатора
				if (isBigLetter || isSmallLetter)
				{
					l[i++] = t[uk++];
					pos++;
					while (isBigLetter || isSmallLetter || isNumber || t[uk] == '_')
					{
						if (i < MAX_LEX)
							l[i++] = t[uk++];
						else
							uk++;
						pos++;
					}

					if (i == MAX_LEX)
						PrintError(err_ident, " Слишком длинный идентификатор ");

					// проверяем на совпадение с ключевыми словами
					for (int j = 0; j < KEYWORD_MAX; j++)
						if (strcmp(KeyWords[j], l) == 0) return KeyWordIndex[j];

					return type_ident;
				}
				else
					// обработка +
					if (t[uk] == '+')
					{
						l[i++] = t[uk++];
						pos++;
						return type_plus;
					}
					else
						// обработка -
						if (t[uk] == '-')
						{
							l[i++] = t[uk++];
							pos++;
							return type_minus;
						}
						else
							// обработка *
							if (t[uk] == '*')
							{
								l[i++] = t[uk++];
								pos++;
								return type_mult;
							}
							else
								// обработка /
								if (t[uk] == '/')
								{
									l[i++] = t[uk++];
									pos++;
									return type_div;
								}
								else
									// обработка %
									if (t[uk] == '%')
									{
										l[i++] = t[uk++];
										pos++;
										return type_mod;
									}
									else
										// обработка .
										if (t[uk] == '.')
										{
											l[i++] = t[uk++];
											pos++;
											return type_dot;
										}
										else
											// обработка ,
											if (t[uk] == ',')
											{
												l[i++] = t[uk++];
												pos++;
												return type_comma;
											}
											else
												// обработка ;
												if (t[uk] == ';')
												{
													l[i++] = t[uk++];
													pos++;
													return type_semicolon;
												}
												else
													// обработка (
													if (t[uk] == '(')
													{
														l[i++] = t[uk++];
														pos++;
														return type_lprths;
													}
													else
														// обработка )
														if (t[uk] == ')')
														{
															l[i++] = t[uk++];
															pos++;
															return type_rprths;
														}
														else
															// обработка {
															if (t[uk] == '{')
															{
																l[i++] = t[uk++];
																pos++;
																return type_lbrcs;
															}
															else
																// обработка }
																if (t[uk] == '}')
																{
																	l[i++] = t[uk++];
																	pos++;
																	return type_rbrcs;
																}
																else
																	// обработка &
																	if (t[uk] == '&')
																	{
																		l[i++] = t[uk++];
																		pos++;
																		return type_and;
																	}
																	else
																		// обработка |
																		if (t[uk] == '|')
																		{
																			l[i++] = t[uk++];
																			pos++;
																			return type_or;
																		}
																		else
																			// обработка ! и !=
																			if (t[uk] == '!')
																			{
																				l[i++] = t[uk++];
																				pos++;
																				if (t[uk] == '=')
																				{
																					l[i++] = t[uk++];
																					pos++;
																					return type_ne;
																				}
																				return type_not;
																			}
																			else
																				// обработка > и >=
																				if (t[uk] == '>')
																				{
																					l[i++] = t[uk++];
																					pos++;
																					if (t[uk] == '=')
																					{
																						l[i++] = t[uk++];
																						pos++;
																						return type_ge;
																					}
																					return type_gt;
																				}
																				else
																					// обработка < и <=
																					if (t[uk] == '<')
																					{
																						l[i++] = t[uk++];
																						pos++;
																						if (t[uk] == '=')
																						{
																							l[i++] = t[uk++];
																							pos++;
																							return type_le;
																						}
																						return type_lt;
																					}
																					else
																						// обработка = и ==
																						if (t[uk] == '=')
																						{
																							l[i++] = t[uk++];
																							pos++;
																							if (t[uk] == '=')
																							{
																								l[i++] = t[uk++];
																								pos++;
																								return type_eq;
																							}
																							return type_assign;
																						}
																						else
																						{
																							// обработка ^
																							if (t[uk] == '^')
																							{
																								l[i++] = t[uk++];
																								pos++;
																								return type_iskl;
																							}
																							else
																							{
																								// ошибка символа
																								l[i++] = t[uk++];
																								pos++;
																								PrintError(err_symbol, l, true);
																								return type_error;
																							}
																						}

	return 0;
}

// считываем исходный модуль
void TScaner::GetData(const char* fileName)
{
	if (fileName[0] == '\0')
		in = fopen("input.dat", "r");
	else
		in = fopen(fileName, "r");
	int i = 0;
	for (int j = 0; j < MAX_TEXT; j++)
	{
		t[i] = 0;
	}
	while (!feof(in))
	{
		fscanf(in, "%c", &t[i++]);
	}
	t[i] = '\0';
	uk = 0;
	line = 0; pos = 0;
	fclose(in);
}