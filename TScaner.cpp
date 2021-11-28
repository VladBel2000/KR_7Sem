#include "TScaner.h"

#define KEYWORD_MAX 12
// ������� �������� ����
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

// ������ ��������� �� ������
void TScaner::PrintError(int i, const char* error, bool critical)
{
	if (error == 0)
		error = new char[] { "" };
	printf("Lexical Error at");
	switch (i)
	{

	case err_symbol:
		// ��������� ������
		printf(" ��� %d ���� %d \t������  %d : �������� ������ %s\n", line + 1, pos + 1, i, error);
		break;
	case err_type:
		// ��������� ���
		printf(" ��� %d ���� %d \t������ %d: �������� ������� %s\n", line + 1, pos + 1, i, error);
		break;
	case err_const:
		// ��������� ���������
		printf(" ��� %d ���� %d \t������ %d: �������� ��������� %s\n", line + 1, pos + 1, i, error);
		break;
	case err_ident:
		// ��������� �������������
		printf(" ��� %d ���� %d \t������ %d: �������� ������������� %s\n", line + 1, pos + 1, i, error);
		break;

	default:
		printf(" ��� %d ���� %d \t����������� ������ %d!  %s\n", line + 1, pos + 1, i, error);
		break;
	}

	// ��������� ������������
	if (stopAfterError && critical)
		exit(0);
}

int TScaner::Scaner(LEX l)
{
	int type = 0;
	int i; // ������� ����� �������
	for (i = 0; i < MAX_LEX; i++) // �������� �������
		l[i] = 0;
	i = 0;

start:

	// ������� �����������
	while (t[uk] == '/' && t[uk + 1] == '/')
	{
		uk += 2;
		while (t[uk++] != '\n' && t[uk] != '\0');
		line++;
		pos = 0;
		goto start;
	}

	// ������� ���������� ��������
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

	// ������ �������
		// ����� ��������� ������
	if (t[uk] == '\0')
	{
		l[0] = t[uk];
		pos++;
		return type_end;
	}
	else
		// ��������� ��������� c ��������� ������
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

			if (i < 3 || isBigLetter || isSmallLetter || t[uk] == '_') // �������� �� ������ ������������
			{
				l[i++] = t[uk];
				PrintError(err_type, l);
				return type_error;
			}
			if (i >= MAX_DOUBLE)
			{
				PrintError(err_const, " ������� ������� double ���������");
				return type_error;
			}
			return const_double;
		}
		else
			// ��������� ������������� ���������
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
				// �������� �� double
				if (t[uk] == '.')
				{
					l[i++] = t[uk++];
					pos++;
					goto double_go;
				}

				if (isBigLetter || isSmallLetter || t[uk] == '_')  // �������� �� ������ ������������
				{
					l[i++] = t[uk];
					PrintError(err_type, l);
					return type_error;
				}

				if (i >= MAX_INT)
				{
					PrintError(err_const, " C������ ������� ��������� c ��������� ������");
					return type_error;
				}

				return const_int;
			}
			else
				// ��������� ��������������
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
						PrintError(err_ident, " ������� ������� ������������� ");

					// ��������� �� ���������� � ��������� �������
					for (int j = 0; j < KEYWORD_MAX; j++)
						if (strcmp(KeyWords[j], l) == 0) return KeyWordIndex[j];

					return type_ident;
				}
				else
					// ��������� +
					if (t[uk] == '+')
					{
						l[i++] = t[uk++];
						pos++;
						return type_plus;
					}
					else
						// ��������� -
						if (t[uk] == '-')
						{
							l[i++] = t[uk++];
							pos++;
							return type_minus;
						}
						else
							// ��������� *
							if (t[uk] == '*')
							{
								l[i++] = t[uk++];
								pos++;
								return type_mult;
							}
							else
								// ��������� /
								if (t[uk] == '/')
								{
									l[i++] = t[uk++];
									pos++;
									return type_div;
								}
								else
									// ��������� %
									if (t[uk] == '%')
									{
										l[i++] = t[uk++];
										pos++;
										return type_mod;
									}
									else
										// ��������� .
										if (t[uk] == '.')
										{
											l[i++] = t[uk++];
											pos++;
											return type_dot;
										}
										else
											// ��������� ,
											if (t[uk] == ',')
											{
												l[i++] = t[uk++];
												pos++;
												return type_comma;
											}
											else
												// ��������� ;
												if (t[uk] == ';')
												{
													l[i++] = t[uk++];
													pos++;
													return type_semicolon;
												}
												else
													// ��������� (
													if (t[uk] == '(')
													{
														l[i++] = t[uk++];
														pos++;
														return type_lprths;
													}
													else
														// ��������� )
														if (t[uk] == ')')
														{
															l[i++] = t[uk++];
															pos++;
															return type_rprths;
														}
														else
															// ��������� {
															if (t[uk] == '{')
															{
																l[i++] = t[uk++];
																pos++;
																return type_lbrcs;
															}
															else
																// ��������� }
																if (t[uk] == '}')
																{
																	l[i++] = t[uk++];
																	pos++;
																	return type_rbrcs;
																}
																else
																	// ��������� &
																	if (t[uk] == '&')
																	{
																		l[i++] = t[uk++];
																		pos++;
																		return type_and;
																	}
																	else
																		// ��������� |
																		if (t[uk] == '|')
																		{
																			l[i++] = t[uk++];
																			pos++;
																			return type_or;
																		}
																		else
																			// ��������� ! � !=
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
																				// ��������� > � >=
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
																					// ��������� < � <=
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
																						// ��������� = � ==
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
																							// ��������� ^
																							if (t[uk] == '^')
																							{
																								l[i++] = t[uk++];
																								pos++;
																								return type_iskl;
																							}
																							else
																							{
																								// ������ �������
																								l[i++] = t[uk++];
																								pos++;
																								PrintError(err_symbol, l, true);
																								return type_error;
																							}
																						}

	return 0;
}

// ��������� �������� ������
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