/*Во всех вариантах данной задачи необходимо реализовать и использовать сортировку вставками.
1_4. Строки.
Напишите программу, печатающую набор строк в лексикографическом порядке.
Строки разделяются символом перевода строки '\n'.
Если последний символ в потоке ввода '\n', считать, что после него нет пустой строки.
Максимальная длина строки 255 символов. Написать свою функцию сравнения строк.*/

#include <iostream>
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

// Insertion sort.
template <typename T>
void InsertionSort(Array<T>* arr, int size, bool (*comp)(const Array<T>& a, const Array<T>& b)) {
	for (int i = 1; i < size; ++i) {
		Array<T> tmp = arr[i];
		int j = i - 1;
		for (; j >= 0 && comp(arr[j], tmp); --j) {
			arr[j + 1] = arr[j];
		}
		arr[j + 1] = tmp;
	}
}

template<typename T>
bool Greater(const Array<T>& a, const Array<T>& b) {
	int counter = 0;

	for (int i = 0; i < a.Size() && i < b.Size(); ++i) {
		if (static_cast<int>(a[i]) > static_cast<int>(b[i])) {
			return true;
		}
		else if (static_cast<int>(a[i]) < static_cast<int>(b[i])) {
			return false;
		}
		else {
			++counter;
		}
	}

	return counter == b.Size() && counter != a.Size();
}

int main() {
	int n = 0;
	std::cin >> n;

	auto* arr = new Array<char>[n];
	char sym = getchar();
	for (int i = 0; i < n; ++i) {
		while ((sym = getchar()) != EOF) {
			if (sym == '\n') {
				break;
			}

			arr[i].PushBack(sym);
		}
	}

	InsertionSort(arr, n, Greater);

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
caba
abba
ab
aba

OUT
ab
aba
abba
caba
*/
