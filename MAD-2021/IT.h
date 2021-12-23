#pragma once
#define ID_MAXSIZE 15
#define TI_MAXSIZE 4096
#define TI_INT_DEFAULT 0x00000000
#define TI_STR_DEFAULT 0x00
#define TI_NULLIDX 0xffffffff
#define TI_STR_MAXSIZE 255

namespace IT {
	enum IDDATATYPE {INT = 1, STR = 2, BOOL = 3, PROC = 4};
	enum IDTYPE {V = 1, F = 2, P = 3, L = 4, OP = 5};

	struct Entry {
		int idxFirstLE;					// индекс первой строки в ТЛ
		char visibility[ID_MAXSIZE];	// область видимости
		char id[ID_MAXSIZE];			// идентификатор

		IDDATATYPE iddatatype;			// тип данных
		IDTYPE idtype;					// тип идентификатора
		int countOfPar = 0;				// количество параметров
		union {
			int vint = 0;				// целочисленное значение
			struct {
				int len;				// длина строки
				char str[TI_STR_MAXSIZE - 1];	// символы строки
			} vstr;						// строковое значение
		} value;						// значение идентификатора
	};

	struct IdTable {
		int maxsize;					// ёмкость ТИ
		int size;						// текущий размер ТИ
		Entry* table;					// массив строк ТИ
	};

	IdTable Create(int size);
	void Add(IdTable& idtable, Entry entry);
	Entry GetEntry(IdTable& idtable, int n);
	int IsId(IdTable& idtable, char id[ID_MAXSIZE]);

	void Delete(IdTable& idtable);
	void ShowTable(IdTable& idtable, std::ostream& stream_out);
}