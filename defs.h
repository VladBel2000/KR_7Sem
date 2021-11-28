#pragma once

#define isSmallLetter ( (t[uk] >= 'a') && (t[uk] <= 'z') )
#define isBigLetter ( (t[uk] >= 'A') && (t[uk] <= 'Z') )
#define isNumber ( (t[uk] >= '0') && (t[uk] <= '9') )
#define isPointDigit ( (t[uk] >= '0') && (t[uk] <= '9') && (t[uk+1] == '.') )

#define MAX_LEX 30 // максимальная длина лексемы	
#define MAX_TEXT 10000 // максимальная длина текста исходного модуля
#define MAX_INT 20 // максимальная длина int числа
#define MAX_DOUBLE 20 // максимальная длина double числа 

typedef char IM[MAX_TEXT]; //  текст лексемы
typedef char LEX[MAX_LEX]; // лексема


// список ключевых слов
#define key_bool		1
#define	key_double	    2
#define	key_brk			3
#define	key_void		4
#define	key_false		5
#define	key_true		6
#define	type_class		7
#define	type_ret		8
#define	type_brk		9
#define	type_cnt		10
#define	type_while		11
#define	type_int		12


// список констант
#define	const_int		15
#define	const_double	16
// идентификатор
#define type_ident		18

// список знаков алгебраических операций
#define	type_plus		20
#define	type_minus		21
#define	type_mult       22
#define type_div		23
#define	type_mod		24

// список знаков разделителей
#define	type_dot		25
#define	type_comma		26
#define	type_semicolon	27

// список знаков операций сравнения
#define	type_lprths		28
#define	type_rprths		29
#define	type_lbrcs		30
#define type_rbrcs		31

// список знаков логических операций
#define	type_and	33
#define type_or		34
#define	type_not	35
#define	type_lt		36
#define	type_gt		37
#define	type_le		38
#define	type_ge		39
#define type_eq		40
#define	type_ne		41
#define type_iskl	42
// знак присваивания
#define type_assign	43

#define	type_end 100 // конец текста
#define	type_error 200 // ошибка

// список ошибок
#define err_symbol 1
#define err_type 2
#define err_const 3
#define err_ident 4


#define syntaxError 1
#define semanticError 2
