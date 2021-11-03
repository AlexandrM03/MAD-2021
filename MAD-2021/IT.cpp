#include "stdafx.h"
#include "IT.h"
#include "Error.h"
using namespace std;

namespace IT {
	IdTable Create(int size) {
		IdTable* table = new IdTable;
		if (size < TI_MAXSIZE) throw ERROR_THROW(203);
		table->maxsize = size;
		table->size = 0;
		table->table = new Entry[size];
		return *table;
	}

	void Add(IdTable& idtable, Entry entry) {
		if (idtable.size + 1 > idtable.maxsize) throw ERROR_THROW(204);

		idtable.table[idtable.size] = entry;
		idtable.size++;
	}

	Entry GetEntry(IdTable& idtable, int n) {
		return idtable.table[n];
	}

	int IsId(IdTable& idtable, char id[ID_MAXSIZE]) {
		for (int i = 0; i < idtable.size; i++) {
			if (strcmp(idtable.table[i].id, id) == 0) return i;
		}

		return TI_NULLIDX;
	}

	void Delete(IdTable& idtable) {
		delete[] idtable.table;
	}

	void ShowTable(IdTable& idtable) {
		int i, j, numberOP = 0;
		cout << setfill('-') << setw(87) << '-' << endl;
		cout << "   #" << " | " << "Идентификатор" << " | " << "Тип данных" << " | " << "Тип идентификатора" 
			<< " | " << "Индекс в ТЛ" << " | " << "Значение" << endl;
		cout << setw(87) << '-' << endl;

		for (i = 0, j = 0; i < idtable.size; i++, j++) {
			cout << setfill('0') << setw(4) << right << j << " | ";
			cout << setfill(' ') << setw(13) << left << idtable.table[i].id << " | ";

			switch (idtable.table[i].iddatatype) {
			case INT:
				cout << setw(10) << left;
				if (idtable.table[i].iddatatype == OP) cout << "-" << " | ";
				else cout << "integer" << " | ";
				break;
			case STR:
				cout << setw(10) << left;
				if (idtable.table[i].idtype == OP) cout << "-" << " | ";
				cout << "string" << " | ";
				break;
			default:
				cout << setw(10) << left << "unknown" << " | ";
				break;
			}

			switch (idtable.table[i].idtype) {
			case V: cout << setw(18) << left << "переменная" << " | "; break;
			case F: cout << setw(18) << left << "функция" << " | "; break;
			case P: cout << setw(18) << left << "параметр" << " | "; break;
			case L: cout << setw(18) << left << "литерал" << " | "; break;
			case OP:
				cout << setw(18) << left << "оператор" << " | ";
				numberOP++;
				break;
			default: cout << setw(18) << left << "unknown" << " | "; break;
			}
			cout << setw(11) << left << idtable.table[i].idxFirstLE << " | ";

			if (idtable.table[i].iddatatype == INT && (idtable.table[i].idtype == V || idtable.table[i].idtype == L))
				cout << setw(18) << left << idtable.table[i].value.vint;
			else if (idtable.table[i].iddatatype == STR && (idtable.table[i].idtype == V || idtable.table[i].idtype == L))
				cout << "[" << idtable.table[i].value.vstr.len << "]\"" << idtable.table[i].value.vstr.str << "\"";
			else cout << "-";
			cout << endl;
		}

		cout << setfill('-') << setw(87) << "-" << endl;
		cout << "Количество идентификаторов: " << i << endl;
		cout << setw(87) << "-" << endl;
	}
}