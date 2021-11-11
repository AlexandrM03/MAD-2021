#include "SemAnalysis.h"

namespace Semantic {
	bool Analyze(Lex::LEX lex, Log::LOG log) {
		bool is_ok = true;
		int j = 0;
		for (int i = 0; i < lex.lextable.size; i++) {
			switch (lex.lextable.table[i].lexema) {
			/*case LEX_OPERATOR:
				if (lex.lextable.table[i].op == LT::ODIV) {
					int j = i;
					do {
						j++;
					} while (lex.lextable.table[j].lexema != LEX_LITERAL && lex.lextable.table[j].lexema != LEX_ID);

					if (lex.idtable.table[lex.lextable.table[j].idxTI].value.vint == 0)
						Log::WriteError(log, Error::geterrorin(311, lex.lextable.table[j].line, -1));
				}*/
				
			case LEX_EQUAL:
				j = i + 1;
				for (j; lex.lextable.table[j].lexema != LEX_SEMICOLON; j++) {
					switch (lex.lextable.table[j].lexema) {
					case LEX_ID:
					case LEX_LITERAL:
						if ((lex.idtable.table[lex.lextable.table[j].idxTI].iddatatype != lex.idtable.table[lex.lextable.table[i - 1].idxTI].iddatatype)) {
							Log::WriteError(log, Error::geterrorin(312, lex.lextable.table[j].line, -1));
							is_ok = false;
						}
						continue;

					case LEX_LEFTTHESIS:
						if (lex.idtable.table[lex.lextable.table[j - 1].idxTI].idtype == IT::F) {
							do {
								j++;
							} while (lex.lextable.table[j].lexema != LEX_RIGHTTHESIS);
						}
						continue;
					}
				}
				continue;
			case LEX_FUNCTION: // Возврат выражения?
				j = i + 1;
				int type = lex.idtable.table[lex.lextable.table[j].idxTI].iddatatype;
				int functions = -1;
				do {
					j++;
					if (lex.lextable.table[j].lexema == LEX_LEFTBRACE)
						functions++;
					else if (lex.lextable.table[j].lexema == LEX_BRACELET)
						functions--;
				} while ((lex.lextable.table[j].lexema != LEX_RET) || functions != 0);
				if (lex.idtable.table[lex.lextable.table[j + 1].idxTI].iddatatype != type) {
					Log::WriteError(log, Error::geterrorin(314, lex.lextable.table[j].line, -1));
					is_ok = false;
				}
			}
		}

		return is_ok;
	}
}