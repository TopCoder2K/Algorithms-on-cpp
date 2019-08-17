#include <iostream>

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

	void push_back(const T& value);
	int size() const noexcept { return _array_size; }
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
void Array<T>::push_back(const T& value) {
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
Array<T> CheckArray(Array<T> a) {
	return a;
}

int main()
{
	int n = 0;
	std::cin >> n;

	Array<int> arr;
	for (int i = 0; i < n; ++i) {
		int tmp = 0;
		std::cin >> tmp;
		arr.push_back(tmp);
	}

	Array<int> arr2 = CheckArray(arr);
	for (int i = 0; i < n; i++) {
		std::cout << arr2[i] << ' ';
	}

	return 0;
}

/*
in
5
1 2 3 4 5
out
1 2 3 4 5*/