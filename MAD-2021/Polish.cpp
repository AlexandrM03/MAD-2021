#include "Polish.h"

namespace Polish {
	bool polishNotation(int i, Lex::LEX& lex) {
		std::stack<LT::Entry> stack;
		std::queue<LT::Entry> queue;

		LT::Entry temp;
		temp.idxTI = -1;
		temp.lexema = '#';
		temp.line = lex.lextable.table[i].line;

		int countLex = 0;
		int posLex = i;

		bool findFunc = false;

		for (i; lex.lextable.table[i].lexema != LEX_SEMICOLON; i++, countLex++) {
			switch (lex.lextable.table[i].lexema) {
			case LEX_ID:
			case LEX_LITERAL:
				queue.push(lex.lextable.table[i]);
				continue;

			case LEX_COMMA:
				if (findFunc)
					stack.push(lex.lextable.table[i]);
				continue;

			case LEX_LEFTTHESIS:
				if (lex.idtable.table[lex.lextable.table[i - 1].idxTI].idtype == IT::F) {
					findFunc = true;
				}

				findFunc ? queue.push(lex.lextable.table[i]) : stack.push(lex.lextable.table[i]);
				continue;

			case LEX_RIGHTTHESIS:
				if (findFunc)
					queue.push(lex.lextable.table[i]);
				else {
					while (stack.top().lexema != LEX_LEFTTHESIS) {
						queue.push(stack.top());
						stack.pop();
						if (stack.empty())
							return false;
					}
					stack.pop();
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