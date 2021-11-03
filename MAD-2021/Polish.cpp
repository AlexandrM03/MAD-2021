#include "Polish.h"

namespace Polish {
	bool polishNotation(int i, Lex::LEX& lex) {
		std::stack<LT::Entry> stack;
		std::queue<LT::Entry> queue;

		LT::Entry temp;
		temp.idxTI = -1;
		temp.lexema = '#';
		temp.line = lex.lextable.table[i].line;

		LT::Entry func;
		func.idxTI = LT_TI_NULLIDX;
		func.lexema = '@';
		func.line = lex.lextable.table[i].line;

		int countLex = 0;
		int countParm = 1;
		int posLex = i;
		char* buf = new char[i];

		bool findFunc = false;

		for (i; lex.lextable.table[i].lexema != LEX_SEMICOLON; i++, countLex++) {
			switch (lex.lextable.table[i].lexema) {
			case LEX_ID:
			case LEX_LITERAL:
				if (lex.idtable.table[lex.lextable.table[i].idxTI].idtype == IT::F)
					findFunc = true;
				else
					queue.push(lex.lextable.table[i]);
				continue;

			case LEX_COMMA:
				countParm++;
				continue;

			case LEX_LEFTTHESIS:
				stack.push(lex.lextable.table[i]);
				continue;

			case LEX_RIGHTTHESIS:
				while (stack.top().lexema != LEX_LEFTTHESIS) {
					queue.push(stack.top());
					stack.pop();
					if (stack.empty())
						return false;
				}

				if (!findFunc)
					stack.pop();
				else {
					lex.lextable.table[i] = func;
					queue.push(lex.lextable.table[i]);
					_itoa_s(countParm, buf, 2, 10);
					stack.top().lexema = buf[0];
					stack.top().idxTI = func.idxTI;
					stack.top().line = func.line;
					queue.push(stack.top());
					stack.pop();
					findFunc = false;
				}
				continue;

			case LEX_OPERATOR:
				while (!stack.empty() && lex.lextable.table[i].priority <= stack.top().priority) {
					queue.push(stack.top());
					stack.pop();
				}
				stack.push(lex.lextable.table[i]);
				continue;
			}
		}

		while (!stack.empty()) {
			if (stack.top().lexema == LEX_LEFTTHESIS || stack.top().lexema == LEX_RIGHTTHESIS)
				return false;

			queue.push(stack.top());
			stack.pop();
		}

		while (countLex != 0) {
			if (!queue.empty()) {
				lex.lextable.table[posLex++] = queue.front();
				queue.pop();
			}
			else
				lex.lextable.table[posLex++] = temp;

			countLex--;
		}

		for (int i = 0; i < posLex; i++) {
			if (lex.lextable.table[i].lexema == LEX_OPERATOR || lex.lextable.table[i].lexema == LEX_LITERAL)
				lex.idtable.table[lex.lextable.table[i].idxTI].idxFirstLE = i;
		}

		return true;
	}

	void startPolish(Lex::LEX& lex) {
		for (int i = 0; i < lex.lextable.size; i++) {
			if (lex.lextable.table[i].lexema == '=') {
				polishNotation(i + 1, lex);
			}
		}
	}
}