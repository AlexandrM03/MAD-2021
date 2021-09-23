#pragma once
#include "Error.h"
#include "Parm.h"
#define LEXEMA_FIXSIZE	1
#define LT_MAXSIZE		4096
#define LT_TI_NULLIDX	0xffffffff
#define LEX_INTEGER		't'
#define LEX_STRING		't'
#define LEX_ID			'i'
#define LEX_LITERAL		'l'
#define LEX_FUNCTION	'f'
#define LEX_DECLARE		'd'
#define LEX_RETURN		'r'
#define LEX_PRINT		'p'
#define LEX_MAIN		'm'
#define LEX_SEMICOLON	';'
#define LEX_COMMA		','
#define LEX_LEFTBRACE	'{'
#define LEX_BRACELET	'}'
#define LEX_LEFTTHESIS	'('
#define LEX_RIGHTTHESIS	')'
#define LEX_PLUS		'v'
#define LEX_MINUS		'v'
#define LEX_STAR		'v'
#define LEX_DIRSLASH	'v'
#define LEX_OPERATOR	'v'
#define LEX_EQUAL		'='

namespace LT {
	struct Entry {
		char lexema;
		int line;
		int idxTI;
		int priority;
	};

	struct LexTable {
		int maxsize;
		int size;
		Entry* table;
	};

	LexTable Create(int size);
	void Add(LexTable& lextable, Entry entry);
	Entry GetEntry(LexTable& lextable, int n);
	void Delete(LexTable& lextable);
	Entry WriteEntry(Entry& entry, char lexema, int indx, int line);
	void ShowTable(LexTable lextable, Parm::PARM parm);
}