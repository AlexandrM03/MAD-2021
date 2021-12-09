#include "SemAnalysis.h"
#include <map>

namespace Semantic {
	bool Analyze(Lex::LEX lex, Log::LOG log) {
		bool is_ok = true;
		int j = 0;
		std::map<std::string, std::vector<IT::IDDATATYPE>> functions_parm;
		std::string function_id;
		std::vector<IT::IDDATATYPE> parms;
		for (int i = 0; i < lex.lextable.size; i++) {
			switch (lex.lextable.table[i].lexema) {
			case LEX_ID:
				j = i + 1;
				if (lex.idtable.table[lex.lextable.table[i].idxTI].idtype == IT::F && lex.lextable.table[i - 1].lexema != LEX_FUNCTION && lex.lextable.table[i - 1].lexema != LEX_PROCEDURE) {
					for (const auto& items : functions_parm) {
						if (items.first == lex.idtable.table[lex.lextable.table[i].idxTI].id) {
							std::vector<IT::IDDATATYPE> buffer = items.second;
							while (lex.lextable.table[j].lexema != LEX_RIGHTTHESIS) {
								if (lex.lextable.table[j].lexema == LEX_ID || lex.lextable.table[j].lexema == LEX_LITERAL) {
									if (!buffer.empty()) {
										if (lex.idtable.table[lex.lextable.table[j].idxTI].iddatatype != buffer[0]) {
											is_ok = false;
											Log::WriteError(log, Error::geterrorin(309, lex.lextable.table[j].line, -1));
										}
										buffer.erase(buffer.begin());
									}
									else {
										is_ok = false;
										Log::WriteError(log, Error::geterrorin(315, lex.lextable.table[j].line, -1));
									}
								}
								j++;
							}
							if (!buffer.empty()) {
								is_ok = false;
								Log::WriteError(log, Error::geterrorin(315, lex.lextable.table[j].line, -1));
							}
						}
					}
				}
				continue;
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
						if (lex.lextable.table[j - 1].lexema != LEX_OPERATOR && lex.lextable.table[j - 1].lexema != LEX_LEFTTHESIS) {
							do {
								j++;
							} while (lex.lextable.table[j].lexema != LEX_RIGHTTHESIS);
						}
						continue;
					}
				}
				continue;
			case LEX_FUNCTION:
			case LEX_PROCEDURE:
				for (const auto &items : functions_parm) {
					if (items.first == lex.idtable.table[lex.lextable.table[i + 1].idxTI].id) {
						Log::WriteError(log, Error::geterrorin(307, lex.lextable.table[i + 1].line, -1));
						is_ok = false;
					}
				}

				function_id = lex.idtable.table[lex.lextable.table[i + 1].idxTI].id;
				j = i + 2;
				do {
					if (lex.lextable.table[j].lexema == LEX_ID || lex.lextable.table[j].lexema == LEX_LITERAL) {
						parms.push_back(lex.idtable.table[lex.lextable.table[j].idxTI].iddatatype);
					}
					j++;
				} while (lex.lextable.table[j].lexema != LEX_RIGHTTHESIS);
				functions_parm.insert(std::make_pair(function_id, parms));
				parms.clear();

				if (lex.lextable.table[i].lexema == LEX_FUNCTION) {
					j = i + 1;
					int type = lex.idtable.table[lex.lextable.table[j].idxTI].iddatatype;
					do {
						j++;
					} while ((lex.lextable.table[j].lexema != LEX_RET));
					if (lex.idtable.table[lex.lextable.table[j + 1].idxTI].iddatatype != type) {
						Log::WriteError(log, Error::geterrorin(314, lex.lextable.table[j].line, -1));
						is_ok = false;
					}
				}
			}
		}

		return is_ok;
	}
}