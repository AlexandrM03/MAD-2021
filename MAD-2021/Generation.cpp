#include "Generation.h"

using namespace std;
#define EXPR while (lexT.table[i].lexema != LEX_SEMICOLON) {																	 \
	switch (lexT.table[i].lexema) {																								 \
	case LEX_ID:																												 \
	case LEX_LITERAL:																											 \
		if (idT.table[lexT.table[i].idxTI].idtype == IT::F)																		 \
			flag_callfunc = true;																								 \
		if (idT.table[lexT.table[i].idxTI].iddatatype == IT::INT || idT.table[lexT.table[i].idxTI].iddatatype == IT::BOOL) {	 \
			out << "\tpush " << idT.table[lexT.table[i].idxTI].id << "\n";														 \
			stk.push(idT.table[lexT.table[i].idxTI].id);																		 \
			break;																												 \
		}																														 \
		if (idT.table[lexT.table[i].idxTI].iddatatype == IT::STR) {																 \
			char* s;																											 \
			if (idT.table[lexT.table[i].idxTI].idtype == IT::L) {																 \
				out << "\tpush offset " << idT.table[lexT.table[i].idxTI].id << "\n";											 \
				s = new char[8]{ "offset " };																					 \
			}																													 \
			else {																												 \
				out << "\tpush " << idT.table[lexT.table[i].idxTI].id << "\n";													 \
				s = new char[1]{ "" };																							 \
			}																													 \
																																 \
			size_t len1 = strlen((char*)s);																						 \
			size_t len2 = strlen((char*)idT.table[lexT.table[i].idxTI].id);														 \
			char* result = (char*)malloc(len1 + len2 + 1);																		 \
			memcpy(result, s, len1);																							 \
			memcpy(result + len1, (char*)idT.table[lexT.table[i].idxTI].id, len2 + 1);											 \
			stk.push(result);																									 \
			break;																												 \
		}																														 \
																																 \
	case LEX_OPERATOR:																											 \
		switch (lexT.table[i].op) {																								 \
		case LT::OMUL:																											 \
			out << "\tpop eax\n\tpop ebx\n";																					 \
			out << "\tmul ebx\n\tpush eax\n";																					 \
			break;																												 \
		case LT::OPLUS:																											 \
			out << "\tpop eax\n\tpop ebx\n";																					 \
			out << "\tadd eax, ebx\n\tpush eax\n";																				 \
			break;																												 \
		case LT::OMINUS:																										 \
			out << "\tpop ebx\n\tpop eax\n";																					 \
			out << "\tsub eax, ebx\n\tpush eax\n";																				 \
			break;																												 \
		case LT::ODIV:																											 \
			out << "\tpop ebx\n\tpop eax\n";																					 \
			out << "\tcdq\n\tidiv ebx\n\tpush eax\n";																			 \
			break;																												 \
		case LT::OMOD:																											 \
			out << "\tpop ebx\n\tpop eax\n";																					 \
			out << "\tcdq\n\tidiv ebx\n\tpush edx\n";																			 \
			break;																												 \
		}																														 \
		break;																													 \
																																 \
	case '@':																													 \
		countParm = (char)lexT.table[i + 1].lexema - '0';																		 \
																																 \
		for (int j = 1; j <= countParm; j++)																					 \
			out << "\tpop edx\n";																								 \
																																 \
		for (int j = 1; j <= countParm; j++) {																					 \
			out << "\tpush " << stk.top() << "\n";																				 \
			stk.pop();																											 \
		}																														 \
		out << "\tcall " << idT.table[lexT.table[i].idxTI].id << "\n\tpush eax\n";												 \
		flag_callfunc = false;																									 \
		break;																													 \
	}																															 \
																																 \
	i++;																														 \
	}																															 \

namespace Gen {
	Generator::Generator(LT::LexTable plexT, IT::IdTable pidT, wchar_t pout[], std::stack<std::string> libs)
	{
		lexT = plexT;
		idT = pidT;
		out = ofstream(pout, ios_base::out);

		Head(libs);
		Const();
		Data();
		Code();
		out.close();
	}

	void Generator::Head(std::stack<std::string> libs) {
		out << ".586\n";
		out << ".model flat, stdcall\n";

		out << "includelib libucrt.lib\n";
		out << "includelib kernel32.lib\n";
		out << "includelib ../Debug/StaticLibrary.lib\n";
		out << "ExitProcess PROTO :DWORD\n\n";
		
		while (!libs.empty()) {
			if (libs.top() == "math") {
				out << "EXTRN mpow: proc\n";
				out << "EXTRN mrand: proc\n";
			}
			else if (libs.top() == "string") {
				out << "EXTRN slen: proc\n";
				out << "EXTRN scpy: proc\n";
			}
			libs.pop();
		}

		out << "EXTRN BREAKL: proc\n";
		out << "EXTRN OutputInt: proc\n";
		out << "EXTRN OutputStr: proc\n";
		out << "EXTRN OutputIntLn: proc\n";
		out << "EXTRN OutputStrLn: proc\n";

		out << "\n.stack 4096\n\n";
	}

	void Generator::Const() {
		out << ".const\n";
		for (int i = 0; i < idT.size; i++) {
			if (idT.table[i].idtype == IT::L) {
				out << "\t" << idT.table[i].id;
				if (idT.table[i].iddatatype == IT::STR)
					out << " BYTE \"" << idT.table[i].value.vstr.str << "\", 0";
				if (idT.table[i].iddatatype == IT::INT || idT.table[i].iddatatype == IT::BOOL)
					out << " SDWORD " << idT.table[i].value.vint;
				out << "\n";
			}
		}
	}

	void Generator::Data() {
		out << "\n.data\n";
		out << "\tbuffer BYTE 256 dup(0)\n";
		for (int i = 0; i < lexT.size; i++) {
			if (lexT.table[i].lexema == LEX_LET) {
				if (idT.table[lexT.table[i + 2].idxTI].idtype == IT::V) {
					out << "\t" << idT.table[lexT.table[i + 2].idxTI].id;
					if (idT.table[lexT.table[i + 2].idxTI].iddatatype == IT::STR)
						out << " DWORD ?\n";
					if (idT.table[lexT.table[i + 2].idxTI].iddatatype == IT::INT || idT.table[lexT.table[i + 2].idxTI].iddatatype == IT::BOOL)
						out << " SDWORD 0\n";
				}
			}
		}
	}

	void Generator::Code() {
		stack<char*> stk;
		int num_of_points = 0,
			num_of_ret = 0,
			num_of_ends = 0,
			num_of_cycles = 0,
			countParm = 0;
		string strret = string(),
			cycle_code = string(),
			func_name = string();
		bool flag_func = false,
			flag_ret = false,
			flag_body = false,
			flag_if = false,
			flag_then = false,
			flag_else = false,
			flag_proc = false,
			flag_callfunc = false,
			flag_condition = false,
			flag_cycle = false;

		int result_position;
		int j;

		out << "\n.code\n\n";
		for (int i = 0; i < lexT.size; i++) {
			switch (lexT.table[i].lexema) {
			case LEX_PROCEDURE:
			case LEX_FUNCTION:
				while (lexT.table[i].lexema != LEX_RIGHTTHESIS) {
					if (lexT.table[i].lexema == LEX_ID && idT.table[lexT.table[i].idxTI].idtype == IT::F) {
						if (idT.table[lexT.table[i].idxTI].iddatatype == IT::PROC)
							flag_proc = true;
						out << (func_name = string((char*)idT.table[lexT.table[i].idxTI].id)) << " PROC ";
					}

					if (lexT.table[i].lexema == LEX_ID && idT.table[lexT.table[i].idxTI].idtype == IT::P) {
						out << idT.table[lexT.table[i].idxTI].id << " : ";
						if (idT.table[lexT.table[i].idxTI].iddatatype == IT::INT || idT.table[lexT.table[i].idxTI].iddatatype == IT::BOOL)
							out << "SDWORD";
						else
							out << "DWORD";
					}

					if (lexT.table[i].lexema == LEX_COMMA)
						out << ", ";

					i++;
				}
				flag_func = true;
				out << "\n";
				break;

			case LEX_MAIN:
				flag_body = true;
				out << "main PROC\n";
				break;

			case LEX_EQUAL:
				result_position = i - 1;
				EXPR
				out << "\tpop " << idT.table[lexT.table[result_position].idxTI].id << "\n";
				break;

			case '@':
				countParm = (char)lexT.table[i + 1].lexema - '0';
				for (int j = 1; j <= countParm; j++) {
					if (idT.table[lexT.table[i - j].idxTI].iddatatype == IT::INT || idT.table[lexT.table[i - j].idxTI].iddatatype == IT::BOOL)
						out << "\tpush " << idT.table[lexT.table[i - j].idxTI].id << "\n";
					else {
						if (idT.table[lexT.table[i - j].idxTI].idtype == IT::L)
							out << "\tpush offset " << idT.table[lexT.table[i - j].idxTI].id << "\n";
						else
							out << "\tpush " << idT.table[lexT.table[i - j].idxTI].id << "\n";
					}
				}

				out << "\tcall " << idT.table[lexT.table[i].idxTI].id << "\n";
				break;

			case LEX_RET:
				EXPR
				if (flag_func) {
					out << "\tjmp local" << num_of_ret << "\n";
					flag_ret = true;
				}
				if (flag_body) {
					out << "\t\tjmp theend\n";
					flag_ret = true;
				}
				break;

			case LEX_BRACELET:
				if (flag_body && !flag_then && !flag_else && !flag_func && !flag_cycle) {
					if (flag_ret) {
						out << "theend:\n";
						flag_ret = false;
					}
					out << "\tpush 0\n\tcall ExitProcess\nmain ENDP\nend main";
				}
				if (flag_func && !flag_then && !flag_else && !flag_cycle) {
					if (flag_ret) {
						out << "local" << num_of_ret++ << ":\n";
						out << "\tpop eax\n\tret\n";
						flag_ret = false;
					}
					if (flag_proc) {
						out << "\tret\n";
						flag_proc = false;
					}
					out << func_name << " ENDP\n\n";
					flag_func = false;
				}
				if (flag_then) {
					flag_then = false;
					if (flag_else) {
						out << "\tjmp e" << num_of_ends << "\n";
						flag_else = false;
					}
					out << "m" << num_of_points++ << ":\n";
				}
				if (flag_else) {
					flag_else = false;
					out << "e" << num_of_ends++ << ":\n";
				}
				if (flag_cycle) {
					out << cycle_code << "cyclenext" << num_of_cycles << ":\n";
					cycle_code.clear();
					num_of_cycles++;
					flag_cycle = false;
				}
				break;

			case LEX_WHERE:
				flag_if = true;
				break;

			case LEX_REPEAT:
				flag_cycle = true;
				flag_condition = true;
				break;

			case LEX_ELSE:
				flag_else = true;
				break;

			case LEX_LEFTTHESIS:
				if (flag_if) {
					if (idT.table[lexT.table[i + 1].idxTI].iddatatype == IT::BOOL && lexT.table[i + 2].lexema == LEX_RIGHTTHESIS) {
						out << "\tmov eax, " << idT.table[lexT.table[i + 1].idxTI].id << "\n";
						out << "\tcmp eax, 1\n";
						out << "\tjz m" << num_of_points << "\n";
						out << "\tjnz m" << num_of_points + 1 << "\n";
					}
					else {
						out << "\tmov eax, " << idT.table[lexT.table[i + 1].idxTI].id << "\n";
						out << "\tcmp eax, " << idT.table[lexT.table[i + 3].idxTI].id << "\n";

						if (lexT.table[i + 2].op == LT::OMORE) {
							out << "\tjg m" << num_of_points << "\n";
							out << "\tjl m" << num_of_points + 1 << "\n";
						}
						else if (lexT.table[i + 2].op == LT::OLESS) {
							out << "\tjl m" << num_of_points << "\n";
							out << "\tjg m" << num_of_points + 1 << "\n";
						}
						else if (lexT.table[i + 2].op == LT::OEQU) {
							out << "\tjz m" << num_of_points << "\n";
							out << "\tjnz m" << num_of_points + 1 << "\n";
						}
						else if (lexT.table[i + 2].op == LT::ONEQU) {
							out << "\tjnz m" << num_of_points << "\n";
							out << "\tjz m" << num_of_points + 1 << "\n";
						}
					}
					out << "\tje m" << num_of_points + 1 << "\n";
					int j = i;
					while (lexT.table[j++].lexema != LEX_BRACELET) {
						if (lexT.table[j + 1].lexema == LEX_ELSE) {
							flag_else = true;
							break;
						}
					}
				}

				if (flag_condition) {
					if (idT.table[lexT.table[i + 1].idxTI].iddatatype == IT::BOOL && lexT.table[i + 2].lexema == LEX_RIGHTTHESIS) {
						out << "\tmov eax, " << idT.table[lexT.table[i + 1].idxTI].id << "\n";
						out << "\tcmp eax, 1\n";
						cycle_code = "\tmov eax, " + string((char*)idT.table[lexT.table[i + 1].idxTI].id) + "\n" +
							"\tcmp eax, " + string((char*)idT.table[lexT.table[i + 1].idxTI].value.vint) + "\n";
						cycle_code += "\tjz cycle" + to_string(num_of_cycles) + "\n";
						out << "\tjz cycle" << num_of_cycles << "\n";
					}
					else {
						cycle_code = "\tmov eax, " + string((char*)idT.table[lexT.table[i + 1].idxTI].id) + "\n" +
							"\tcmp eax, " + string((char*)idT.table[lexT.table[i + 3].idxTI].id) + "\n";
						out << "\tmov eax, " << idT.table[lexT.table[i + 1].idxTI].id << "\n";
						out << "\tcmp eax, " << idT.table[lexT.table[i + 3].idxTI].id << "\n";

						if (lexT.table[i + 2].op == LT::OMORE) {
							cycle_code += "\tjg cycle" + to_string(num_of_cycles) + "\n";
							out << "\tjg cycle" << num_of_cycles << "\n";
						}
						else if (lexT.table[i + 2].op == LT::OLESS) {
							cycle_code += "\tjl cycle" + to_string(num_of_cycles) + "\n";
							out << "\tjl cycle" << num_of_cycles << "\n";
						}
						else if (lexT.table[i + 2].op == LT::OEQU) {
							cycle_code += "\tjz cycle" + to_string(num_of_cycles) + "\n";
							out << "\tjz cycle" << num_of_cycles << "\n";
						}
						else if (lexT.table[i + 2].op == LT::ONEQU) {
							cycle_code += "\tjnz cycle" + to_string(num_of_cycles) + "\n";
							out << "\tjnz cycle" << num_of_cycles << "\n";
						}
					}
					out << "\tjmp cyclenext" << num_of_cycles << "\n";
				}
				break;

			case LEX_RIGHTTHESIS:
				if (lexT.table[i + 1].lexema == LEX_LEFTBRACE && flag_if) {
					flag_then = true;
					out << "m" << num_of_points++ << ":\n";
					flag_if = false;
				}
				if (lexT.table[i + 1].lexema == LEX_LEFTBRACE && flag_condition) {
					out << "cycle" << num_of_cycles << ":\n";
					flag_condition = false;
				}
				break;

			case LEX_WRITE:
				j = i + 1;
				EXPR
				while (lexT.table[j].idxTI == LT_TI_NULLIDX)
					j++;
				if (idT.table[lexT.table[j].idxTI].iddatatype == IT::INT || idT.table[lexT.table[j].idxTI].iddatatype == IT::BOOL)
					out << "\n\tcall OutputInt\n";
				else 
					out << "\n\tcall OutputStr\n";
				break;

			case LEX_WRITELN:
				j = i + 1;
				EXPR
					while (lexT.table[j].idxTI == LT_TI_NULLIDX)
						j++;
				if (idT.table[lexT.table[j].idxTI].iddatatype == IT::INT || idT.table[lexT.table[j].idxTI].iddatatype == IT::BOOL)
					out << "\n\tcall OutputIntLn\n";
				else
					out << "\n\tcall OutputStrLn\n";
				break;
			}
		}
	}
}