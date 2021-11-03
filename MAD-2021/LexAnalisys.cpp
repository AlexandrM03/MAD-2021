#include "stdafx.h"
#include "LexAnalisys.h"
#include "Div.h"
#include "FST.h"
#include "Graphs.h"
#define DIV			'|'
#define SPACE		' '
#define PLUS		'+'
#define MINUS		'-'
#define STAR		'*'
#define DIRSLASH	'/'
#define EQUAL		'='
using namespace std;

namespace Lex {
	LEX LexAnaliz(Log::LOG log, In::IN in) {
		LEX lex;
		LT::LexTable lextable = LT::Create(LT_MAXSIZE);
		IT::IdTable idtable = IT::Create(TI_MAXSIZE);

		cleanup(in.text, in.size, log);

		char** word = new char* [max_word];
		for (int i = 0; i < max_word; i++)
			word[i] = new char[size_word] {NULL};

		do {
			word = divideText(in.text, in.size);
		} while (word == NULL);

		for (int i = 0; word[i]; i++) {
			cout << word[i];
		}
		cout << endl;

		int indexLex = 0;
		int indexID = 0;
		int literalCounter = 1;
		int line = 1;
		int position = 0;
		bool findMain = false;
		int mainCounter = 0;
		bool findDeclaration = false;
		bool findType = false;
		int bufPos = 0;

		char* prefix = new char[10]{ "" };
		char* bufprefix = new char[10]{ "" };
		char* oldprefix = new char[10]{ "" };
		char* L = new char[2]{ "L" };
		char* bufL = new char[TI_STR_MAXSIZE];
		char* nameLiteral = new char[10]{ "" };
		char* charclit = new char[10]{ "" };

		bool findFunc = false;
		bool findParm = false;
		IT::Entry entryIT;

		for (int i = 0; word[i] != NULL; i++, indexLex++) {
			bool findSameID = false;

			FST::FST fstDeclare(word[i], FST_DECLARE);
			if (FST::execute(fstDeclare)) {
				LT::Entry entryLT = WriteEntry(entryLT, LEX_DECLARE, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				findDeclaration = true;
				continue;
			}

			FST::FST fstTypeInteger(word[i], FST_INTEGER);
			if (FST::execute(fstTypeInteger)) {
				LT::Entry entryLT = WriteEntry(entryLT, LEX_INTEGER, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				findType = true;
				entryIT.iddatatype = IT::INT;
				continue;
			}

			FST::FST fstTypeString(word[i], FST_STRING);
			if (FST::execute(fstTypeString)) {
				LT::Entry entryLT = WriteEntry(entryLT, LEX_STRING, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				findType = true;
				entryIT.iddatatype = IT::STR;
				strcpy(entryIT.value.vstr.str, "");
				continue;
			}

			FST::FST fstFunction(word[i], FST_FUNCTION);
			if (FST::execute(fstFunction)) {
				LT::Entry entryLT = WriteEntry(entryLT, LEX_FUNCTION, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				findFunc = true;
				entryIT.idtype = IT::F;
				continue;
			}

			FST::FST fstReturn(word[i], FST_RETURN);
			if (FST::execute(fstReturn)) {
				LT::Entry entryLT = WriteEntry(entryLT, LEX_RETURN, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}

			FST::FST fstPrint(word[i], FST_PRINT);
			if (FST::execute(fstPrint)) {
				LT::Entry entryLT = WriteEntry(entryLT, LEX_PRINT, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}

			FST::FST fstMain(word[i], FST_MAIN);
			if (FST::execute(fstMain)) {
				strcpy(entryIT.id, word[i]);
				entryIT.idxFirstLE = indexLex;
				entryIT.idtype = IT::F;
				entryIT.iddatatype = IT::INT;
				IT::Add(idtable, entryIT);
				entryIT = {};

				LT::Entry entryLT = WriteEntry(entryLT, LEX_MAIN, IT::IsId(idtable, word[i]), line);
				LT::Add(lextable, entryLT);
				strcpy(oldprefix, prefix);
				strcpy(prefix, word[i]);
				findMain = true;
				mainCounter++;
				continue;
			}

			FST::FST fstIdentif(word[i], FST_ID);
			if (FST::execute(fstIdentif)) {
				if (findFunc) {
					int idx = IT::IsId(idtable, word[i]);
					if (idx != TI_NULLIDX) {
						Log::WriteError(log, Error::geterrorin(308, line, -1));
					}
				}
				else {
					int idx = IT::IsId(idtable, word[i]);
					if (idx != TI_NULLIDX) {
						LT::Entry entryLT = WriteEntry(entryLT, LEX_ID, idx, line);
						LT::Add(lextable, entryLT);
						findFunc = false;
						continue;
					}
					strcpy(bufprefix, prefix);
					word[i] = strcat(bufprefix, word[i]);
					idx = IT::IsId(idtable, word[i]);
					if (idx != TI_NULLIDX) {
						if (findDeclaration == true)
							throw ERROR_THROW_IN(122, line, position);
						LT::Entry entryLT = WriteEntry(entryLT, LEX_ID, idx, line);
						LT::Add(lextable, entryLT);
						continue;
					}
				}

				LT::Entry entryLT = WriteEntry(entryLT, LEX_ID, indexID++, line);
				LT::Add(lextable, entryLT);
				findDeclaration = false;
				if (findParm) {
					entryIT.idtype = IT::P;
					idtable.table[bufPos].countOfPar++;
					strcpy(entryIT.visibility, prefix);
				}
				else if (!findFunc) {
					entryIT.idtype = IT::V;
					strcpy(entryIT.visibility, prefix);
					if (entryIT.iddatatype == IT::INT)
						entryIT.value.vint == TI_INT_DEFAULT;
					if (entryIT.iddatatype == IT::STR) {
						entryIT.value.vstr.len = 0;
						memset(entryIT.value.vstr.str, TI_STR_DEFAULT, sizeof(char));
					}
				}
				else {
					strcpy(oldprefix, prefix);
					strcpy(prefix, word[i]);
				}

				entryIT.idxFirstLE = indexLex;
				strcpy(entryIT.id, word[i]);
				IT::Add(idtable, entryIT);
				entryIT = {};
				findFunc = false;
				findType = false;
				continue;
			}

			FST::FST fstLiteralInt(word[i], FST_INTLIT);
			if (FST::execute(fstLiteralInt)) {
				int value = atoi((char*)word[i]);
				for (int k = 0; k < idtable.size; k++) {
					if (idtable.table[k].value.vint == value && idtable.table[k].idtype == IT::L && idtable.table[k].iddatatype == IT::INT) {
						LT::Entry entryLT = WriteEntry(entryLT, LEX_LITERAL, k, line);
						LT::Add(lextable, entryLT);
						findSameID = true;
						break;
					}
				}

				if (findSameID) continue;
				LT::Entry entryLT = WriteEntry(entryLT, LEX_LITERAL, indexID++, line);
				LT::Add(lextable, entryLT);
				entryIT.idtype = IT::L;
				entryIT.iddatatype = IT::INT;
				entryIT.value.vint = value;
				entryIT.idxFirstLE = indexLex;
				_itoa_s(literalCounter++, charclit, sizeof(char) * 10, 10);
				strcpy(bufL, L);
				word[i] = strcat(bufL, charclit);
				strcpy(entryIT.id, word[i]);
				IT::Add(idtable, entryIT);
				entryIT = {};
				continue;
			}

			FST::FST fstLiteralString(word[i], FST_STRLIT);
			if (FST::execute(fstLiteralString)) {
				int length = strlen(word[i]);
				for (int k = 0; k < length; k++)
					word[i][k] = word[i][k + 1];
				word[i][length - 2] = 0;

				for (int k = 0; k < idtable.size; k++) {
					if (!(strcmp(idtable.table[k].value.vstr.str, word[i]))) {
						findSameID = true;
						LT::Entry entryLT = WriteEntry(entryLT, LEX_LITERAL, k, line);
						LT::Add(lextable, entryLT);
						break;
					}
				}
				if (findSameID) continue;

				LT::Entry entryLT = WriteEntry(entryLT, LEX_LITERAL, indexID++, line);
				LT::Add(lextable, entryLT);
				strcpy(entryIT.value.vstr.str, word[i]);
				entryIT.value.vstr.len = length - 2;
				entryIT.idtype = IT::L;
				entryIT.iddatatype = IT::STR;
				entryIT.idxFirstLE = indexLex;

				_itoa_s(literalCounter++, charclit, sizeof(char) * 10, 10);
				strcpy(bufL, L);
				nameLiteral = strcat(bufL, (char*)charclit);
				strcpy(entryIT.id, nameLiteral);
				IT::Add(idtable, entryIT);
				entryIT = {};
				continue;
			}

			FST::FST fstOperator(word[i], FST_OPERATOR);
			if (FST::execute(fstOperator)) {
				strcpy(entryIT.id, word[i]);
				entryIT.idxFirstLE = indexLex;
				entryIT.idtype = IT::OP;
				if (IT::IsId(idtable, word[i]) == TI_NULLIDX)
					IT::Add(idtable, entryIT);
				entryIT = { };

				LT::Entry entryLT = WriteEntry(entryLT, LEX_OPERATOR, IT::IsId(idtable, word[i]), line);
				switch (word[i][0]) {
				case PLUS:
					entryLT.priority = 2;
					entryLT.op = LT::operations::OPLUS;
					break;
				case MINUS:
					entryLT.priority = 2;
					entryLT.op = LT::operations::OMINUS;
					break;
				case DIRSLASH: 
					entryLT.priority = 3;
					entryLT.op = LT::operations::ODIV;
					break;
				case STAR:
					entryLT.priority = 3;
					entryLT.op = LT::operations::OMUL;
					break;
				}
				LT::Add(lextable, entryLT);
				continue;
			}

			FST::FST fstSemicolon(word[i], FST_SEMICOLON);
			if (FST::execute(fstSemicolon)) {
				LT::Entry entryLT = WriteEntry(entryLT, LEX_SEMICOLON, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}

			FST::FST fstComma(word[i], FST_COMMA);
			if (FST::execute(fstComma)) {
				LT::Entry entryLT = WriteEntry(entryLT, LEX_COMMA, LT_TI_NULLIDX, line);
				entryLT.priority = 1;
				LT::Add(lextable, entryLT);
				continue;
			}

			FST::FST fstLeftBrace(word[i], FST_LEFTBRACE);
			if (FST::execute(fstLeftBrace)) {
				LT::Entry entryLT = WriteEntry(entryLT, LEX_LEFTBRACE, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}

			FST::FST fstRightBrace(word[i], FST_BRACELET);
			if (FST::execute(fstRightBrace)) {
				LT::Entry entryLT = WriteEntry(entryLT, LEX_BRACELET, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}

			FST::FST fstLeftThesis(word[i], FST_LEFTTHESIS);
			if (FST::execute(fstLeftThesis)) {
				LT::Entry entryLT = WriteEntry(entryLT, LEX_LEFTTHESIS, LT_TI_NULLIDX, line);
				entryLT.priority = 0;
				LT::Add(lextable, entryLT);
				if (idtable.table[indexID - 1].idtype == IT::F)
					findParm = true;
				bufPos = indexID - 1;
				continue;
			}

			FST::FST fstRightThesis(word[i], FST_RIGHTTHESIS);
			if (FST::execute(fstRightThesis)) {
				LT::Entry entryLT = WriteEntry(entryLT, LEX_RIGHTTHESIS, LT_TI_NULLIDX, line);
				entryLT.priority = 0;
				if (findParm && word[i + 1][0] != LEX_LEFTBRACE && word[i + 2][0] != LEX_LEFTBRACE)
					strcpy(prefix, oldprefix);
				findParm = false;
				LT::Add(lextable, entryLT);
				continue;
			}

			FST::FST fstEqual(word[i], FST_EQUAL);
			if (FST::execute(fstEqual)) {
				LT::Entry entryLT = WriteEntry(entryLT, LEX_EQUAL, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}

			position += strlen(word[i]);
			if (word[i][0] == DIV) {
				line++;
				position = 0;
				indexLex--;
				continue;
			}

			throw ERROR_THROW_IN(205, line, position);
		}

		if (!findMain) throw ERROR_THROW(302);
		if (mainCounter > 1) throw ERROR_THROW(301);

		lex.idtable = idtable;
		lex.lextable = lextable;
		return lex;
	}
}