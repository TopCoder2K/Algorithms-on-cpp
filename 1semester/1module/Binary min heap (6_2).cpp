/*Во всех задачах из следующего списка следует написать структуру данных, обрабатывающую команды push* и pop*.
Формат входных данных.
В первой строке количество команд n. n ≤ 1000000.
Каждая команда задаётся как 2 целых числа: a b.
a = 1 - push front
a = 2 - pop front
a = 3 - push back
a = 4 - pop back
Для очереди используются команды 2 и 3. Для дека используются все четыре команды.
Если дана команда pop*, то число b - ожидаемое значение. Если команда pop вызвана для пустой структуры данных, то ожидается “-1”.
Формат выходных данных.
Требуется напечатать YES - если все ожидаемые значения совпали. Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.

4_1. Реализовать очередь с динамическим зацикленным буфером.*/

#include <iostream>
#include <cassert>


class MinHeap {
public:
	MinHeap();
	~MinHeap() { delete[] _buffer; }

	MinHeap& operator=(const MinHeap& other);
	MinHeap& operator=(MinHeap&& other) noexcept;
	MinHeap(const MinHeap& other) : MinHeap() { *this = other; }
	MinHeap(MinHeap&& other) noexcept : MinHeap() { *this = std::forward<MinHeap>(other); }

	void Add(int value);
	int GetMin() const noexcept { return _buffer[0]; }
	int ExtractMin();
	void BuildHeap(int* arr, int size);
	int Size() const noexcept { return _real_size; }
private:
	void _SiftDown(int index);
	void _SiftUp(int index);
	int* _buffer;
	void _Reallocate();
	int _real_size, _buffer_size;
};

MinHeap::MinHeap() {
	_buffer_size = 1;
	_real_size = 0;
	_buffer = new int[_buffer_size];
}

MinHeap& MinHeap::operator=(const MinHeap& other) {
	if (this == &other) { return *this; }

	delete[] _buffer;
	_buffer = new int[other._buffer_size];
	for (int i = 0; i < other._real_size; ++i) {
		_buffer[i] = other._buffer[i];
	}

	_real_size = other._real_size;
	_buffer_size = other._buffer_size;

	return *this;
}

MinHeap& MinHeap::operator=(MinHeap&& other) noexcept {
	std::swap(other._real_size, _real_size);
	std::swap(other._buffer_size, _buffer_size);
	std::swap(other._buffer, _buffer);

	return *this;
}

void MinHeap::_SiftUp(int index) {
	while (index > 0) {
		int parent = (index - 1) / 2;

		if (_buffer[parent] > _buffer[index]) {
			std::swap(_buffer[parent], _buffer[index]);
			index = parent;
		}
		else {
			break;
		}
	}
}

void MinHeap::_SiftDown(int index) {
	while (true) {
		int left = index * 2 + 1, right = index * 2 + 2, smallest = index;

		if (left < _real_size && _buffer[left] < _buffer[smallest]) {
			smallest = left;
		}
		if (right < _real_size && _buffer[right] < _buffer[smallest]) {
			smallest = right;
		}

		if (smallest != index) {
			std::swap(_buffer[smallest], _buffer[index]);
			index = smallest;
		}
		else {
			break;
		}
	}
}

void MinHeap::BuildHeap(int* arr, int size) {
	_real_size = _buffer_size = size;

	int* new_buffer = new int[_buffer_size];
	for (int i = 0; i < _buffer_size; ++i) {
		new_buffer[i] = arr[i];
	}

	int* tmp = _buffer;
	_buffer = new_buffer;
	delete[] tmp;

	for (int i = (_real_size - 1) / 2; i >= 0; --i) {
		_SiftDown(i);
	}
}

void MinHeap::Add(int value) {
	if (_real_size == _buffer_size) {
		_Reallocate();
	}

	_buffer[_real_size] = value;
	_SiftUp(_real_size);
	++_real_size;
}

void MinHeap::_Reallocate() {
	int new_buffer_size = 2 * _buffer_size;

	int* new_buffer = new int[new_buffer_size];
	for (int i = 0; i < _buffer_size; ++i) {
		new_buffer[i] = _buffer[i];
	}

	int* tmp;
	tmp = _buffer;
	_buffer = new_buffer;

	delete[] tmp;
	_buffer_size = new_buffer_size;
}

int MinHeap::ExtractMin() {
	assert(_real_size > 0);

	int result = _buffer[0];
	_buffer[0] = _buffer[_real_size - 1];
	_SiftDown(0);
	--_real_size;

	return result;
}

int main() {
	int n = 0;
	std::cin >> n;

	int* arr = new int[n];
	for (int i = 0; i < n; i++) {
		std::cin >> arr[i];
	}

	MinHeap answer;
	answer.BuildHeap(arr, n);

	int sum = 0;
	while (answer.Size() > 1) {
		int cur_sum = 0;
		cur_sum += answer.ExtractMin();
		cur_sum += answer.ExtractMin();
		sum += cur_sum;
		answer.Add(cur_sum);
	}
	std::cout << sum;

	delete[] arr;
	return 0;
}

/*
in
5
5 2 3 4 6
out
45
------------
in
5
3 7 6 1 9
out
56
*/
