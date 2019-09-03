/*5_1. MSD для строк.
Дан массив строк. Количество строк не больше 10^5.
Отсортировать массив методом поразрядной сортировки MSD по символам.
Размер алфавита - 256 символов, но строки состоят только из латинских букв (кроме последнего символа).
Последний символ строки = ‘\0’.*/

#include<iostream>
#include <cassert>

// Primitive dynamic array.
template <typename T>
class Array {
public:
	Array();
	~Array() { delete[] _buffer; }

	Array& operator=(const Array& other);
	Array& operator=(Array&& other) noexcept;
	Array(const Array& other) : Array() { *this = other; }
	Array(Array&& other) noexcept : Array() { *this = std::forward<Array>(other); }

	T& operator[](int index) { return _buffer[index]; }
	const T operator[](int index) const { return _buffer[index]; }

	void PushBack(const T& value);
	int Size() const noexcept { return _array_size; }
private:
	void _Reallocate();
	T* _buffer;
	int _array_size;
	int _buffer_size;
};

template <typename T>
Array<T>::Array() {
	_buffer_size = 1;
	_array_size = 0;
	_buffer = new T[_buffer_size];
}

template <typename T>
Array<T>& Array<T>::operator=(const Array& other) {
	if (this == &other) { return *this; }

	delete[] _buffer;
	_buffer = new T[other._buffer_size];
	for (int i = 0; i < other._array_size; ++i) {
		_buffer[i] = other._buffer[i];
	}

	_array_size = other._array_size;
	_buffer_size = other._buffer_size;

	return *this;
}

template <typename T>
Array<T>& Array<T>::operator=(Array&& other) noexcept {
	std::swap(_buffer, other._buffer);
	std::swap(_buffer_size, other._buffer_size);
	std::swap(_array_size, other._array_size);

	return *this;
}

template <typename T>
void Array<T>::PushBack(const T& value) {
	if (_array_size == _buffer_size) _Reallocate();

	_buffer[_array_size] = value;
	++_array_size;
}

template <typename T>
void Array<T>::_Reallocate() {
	int new_buffer_size = _buffer_size * 2;

	T* new_buffer = new T[new_buffer_size];
	for (int i = 0; i < _array_size; i++)
		new_buffer[i] = _buffer[i];

	delete[] _buffer;
	_buffer = new_buffer;

	_buffer_size = new_buffer_size;
}

template <typename T>
void CountingSort(Array<T>* arr, int left, int right, const int k_alphabet_power, int digit, int* baskets) {
	for (int i = 0; i < k_alphabet_power; ++i) {
		baskets[i] = 0;
	}

	// Counting.
	for (int i = left; i <= right; ++i) {
		if (static_cast<int>(arr[i][digit]) >= 97) {
			++baskets[static_cast<int>(arr[i][digit]) - 96];
		}
		else {
			++baskets[0];
		}
	}

	// Get positions of ends of group of identical elements.
	for (int i = 1; i < k_alphabet_power; ++i) {
		baskets[i] += baskets[i - 1];
	}

	// Copy positions because we can't change them, we will need it to divide elements in groups.
	int* tmp_baskets = new int[k_alphabet_power];
	for (int i = 0; i < k_alphabet_power; ++i) {
		tmp_baskets[i] = baskets[i];
	}

	auto sorted_arr = new Array<T>[right - left + 1];
	for (int i = right; i >= left; --i) {
		if (static_cast<int>(arr[i][digit]) >= 97) {
			sorted_arr[--tmp_baskets[static_cast<int>(arr[i][digit]) - 96]] = arr[i];
		}
		else {
			sorted_arr[--tmp_baskets[0]] = arr[i];
		}
	}

	for (int i = left; i <= right; ++i) {
		arr[i] = sorted_arr[i - left];
	}

	delete[] tmp_baskets;
	delete[] sorted_arr;
}

template<typename T>
void MSDSort(Array<T>* arr, int left, int right, const int k_alphabet_power, int digit) {
	if (right == left) {
		return;
	}

	int* baskets = new int[k_alphabet_power];
	CountingSort(arr, left, right, k_alphabet_power, digit, baskets);

	// Digitization.
	for (int i = 1; i < k_alphabet_power; ++i) {
		if (baskets[i] != baskets[i - 1]) {
			MSDSort(arr, left + baskets[i - 1], left + (baskets[i] - 1), k_alphabet_power, digit + 1);
		}
	}

	delete[] baskets;
}

int main() {
	int n = 0;
	std::cin >> n;

	auto arr = new Array<char>[n];
	char sym = getchar();
	for (int i = 0; i < n; ++i) {
		while ((sym = getchar()) != EOF) {
			if (sym == '\n') {
				break;
			}

			arr[i].PushBack(sym);
		}
	}

	const int k_alphabet_power = 27;
	MSDSort(arr, 0, n - 1, k_alphabet_power, 0);

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < arr[i].Size(); ++j) {
			std::cout << arr[i][j];
		}
		std::cout << std::endl;
	}

	delete[] arr;
	return 0;
}

/*
IN
4
ab
a
aaa
aa
OUT
a
aa
aaa
ab
*/
