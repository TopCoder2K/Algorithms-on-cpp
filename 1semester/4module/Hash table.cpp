/*Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
Хранимые строки непустые и состоят из строчных латинских букв.
Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
Начальный размер таблицы должен быть равным 8-ми.
Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4.
Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества и проверки принадлежности данной строки множеству.
1_2. Для разрешения коллизий используйте двойное хеширование.

Формат входных данных
Каждая строка входных данных задает одну операцию над множеством. Запись операции состоит из типа операции и следующей за ним через пробел строки, над которой проводится операция.
Тип операции  – один из трех символов:
	+  означает добавление данной строки в множество;
	-  означает удаление  строки из множества;
	?  означает проверку принадлежности данной строки множеству.
При добавлении элемента в множество НЕ ГАРАНТИРУЕТСЯ, что он отсутствует в этом множестве. При удалении элемента из множества НЕ ГАРАНТИРУЕТСЯ, что он присутствует в этом множестве.
Формат выходных данных
Программа должна вывести для каждой операции одну из двух строк OK или FAIL, в зависимости от того, встречается ли данное слово в нашем множестве.
*/

#include <iostream>
#include <string>
#include <vector>

template <class Type>
class HashTable {
public:
	HashTable(const Type& empty, const Type& deleted) : _empty(empty), _deleted(deleted), _table_size(8), _number_of_keys(0) { _hashes.resize(8, empty); }
	~HashTable() = default;

	HashTable& operator=(const HashTable& other) = delete;
	HashTable& operator=(HashTable&& other) = delete;
	HashTable(const HashTable& other) = delete;
	HashTable(HashTable&& other) = delete;

	int Has(const Type& key) const;
	bool Remove(const Type& key);
	bool Add(const Type& key);
private:
	// NOEXCEPTTTTTTTTTTTTTTTTTT
	int _AuxiliaryH1(const Type& key) const noexcept;
	int _AuxiliaryH2(const Type& key) const noexcept;
	void _Rehash();

	int _table_size;    // _table_size is always 2^n.
	int _number_of_keys;
	std::vector<Type> _hashes;
	const Type _empty;
	const Type _deleted;
};

// Calculate the Horner polynomial (auxiliary hash function 1).
template <class Type>
int HashTable<Type>::_AuxiliaryH1(const Type& key) const noexcept {
	int index = 0;
	const int a = 137;  // is a primal number.

	for (int i = 0; i < key.size(); ++i) {
		index = (index * a + key[i]) % _table_size;
	}

	return index;
}

// Calculate the Horner polynomial (auxiliary hash function 2, returns odd numbers only).
template <class Type>
int HashTable<Type>::_AuxiliaryH2(const Type& key) const noexcept {
	int index = 0;
	const int a = 257;  // is a primal number.

	for (int i = 0; i < key.size(); ++i) {
		index = (index * a + key[i]) % _table_size;
	}

	if (index % 2 == 0)
		index = (index * 2 + 1) % _table_size;

	return index;
}

template <class Type>
int HashTable<Type>::Has(const Type& key) const {
	int h1 = _AuxiliaryH1(key);

	if (_hashes[h1] == key)
		return h1;

	if (_hashes[h1] == _empty)
		return -1;

	// Otherwise we have to to go further...
	int h2 = _AuxiliaryH2(key);

	for (int i = 1; i < _table_size; ++i) {
		if (_hashes[(h1 + h2 * i) % _table_size] == _empty)
			return -1;

		if (_hashes[(h1 + h2 * i) % _table_size] == key)
			return (h1 + h2 * i) % _table_size;
	}

	return -1;
}

template <class Type>
bool HashTable<Type>::Remove(const Type& key) {
	int index = Has(key);

	if (index >= 0) {
		_hashes[index] = _deleted;
		--_number_of_keys;

		return true;
	}

	return false;
}

template <class Type>
bool HashTable<Type>::Add(const Type& key) {
	int h1 = _AuxiliaryH1(key);

	if (_hashes[h1] == _empty) {
		_hashes[h1] = key;
		++_number_of_keys;

		if (static_cast<double>(_number_of_keys) / _table_size > 2.0 / 3) {
			_Rehash();
		}

		return true;
	}

	if (_hashes[h1] == key) return false;

	// Otherwise we have to go further...
	int first_del_pos = -1;   // first_del_pos is needed to avoid key repetition (we cannot put key in first deleted position once we found it.)
	if (_hashes[h1] == _deleted)
		first_del_pos = h1;

	int h2 = _AuxiliaryH2(key);
	for (int i = 1; i < _table_size; ++i) {
		if (first_del_pos == -1 && _hashes[(h1 + h2 * i) % _table_size] == _deleted) {
			first_del_pos = (h1 + h2 * i) % _table_size;
		}

		if (_hashes[(h1 + h2 * i) % _table_size] == _empty) {
			if (first_del_pos >= 0) {
				_hashes[first_del_pos] = key;
			}
			else {
				_hashes[(h1 + h2 * i) % _table_size] = key;
			}
			++_number_of_keys;

			if (static_cast<double>(_number_of_keys) / _table_size > 2.0 / 3) {
				_Rehash();
			}

			return true;
		}

		if (_hashes[(h1 + h2 * i) % _table_size] == key) {
			return false;
		}
	}

	if (first_del_pos >= 0) {
		_hashes[first_del_pos] = key;
		++_number_of_keys;

		if (static_cast<double>(_number_of_keys) / _table_size > 2.0 / 3) {
			_Rehash();
		}

		return true;
	}

	return false;
}

template <class Type>
void HashTable<Type>::_Rehash() {
	std::vector<Type> old_table = _hashes;
	int old_size = _table_size;

	_table_size *= 2;
	_hashes.resize(_table_size);

	for (int i = 0; i < _table_size; ++i) {
		_hashes[i] = _empty;
	}
	for (int i = 0; i < old_size; ++i) {
		if (old_table[i] != _empty && old_table[i] != _deleted) {
			Add(old_table[i]);
		}
	}
}

int main() {
	HashTable<std::string> table("EMPTY", "DELETED");

	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);

	char command = 0;
	while (std::cin >> command) {
		std::string str;
		std::cin >> str;
		if (command == '+') {
			if (table.Add(str)) {
				std::cout << "OK" << std::endl;
			}
			else {
				std::cout << "FAIL" << std::endl;
			}
		}
		if (command == '?') {
			if (table.Has(str) >= 0) {
				std::cout << "OK" << std::endl;
			}
			else {
				std::cout << "FAIL" << std::endl;
			}
		}
		if (command == '-') {
			if (table.Remove(str)) {
				std::cout << "OK" << std::endl;
			}
			else {
				std::cout << "FAIL" << std::endl;
			}
		}
	}

	return 0;
}

/*
IN
+ hello
+ bye
? bye
+ bye
- bye
? bye
? hello

OUT
OK
OK
OK
FAIL
OK
FAIL
OK
*/
