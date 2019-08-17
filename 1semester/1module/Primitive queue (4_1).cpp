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

class Queue {
public:
	Queue();
	~Queue();

	Queue& operator=(const Queue& other);
	Queue& operator=(Queue&& other) noexcept;
	Queue(const Queue& other) : Queue() { *this = other; }
	Queue(Queue&& other) noexcept : Queue() { *this = std::forward<Queue>(other); }

	void Enqueue(int a);
	int Dequeue() noexcept;
	int size() const noexcept;

private:
	int _head;
	int _tail;
	int* _buffer;
	int _buffer_size;
	int _real_size;
	void _Realloc();
};

Queue::Queue() {
	_buffer_size = 1;
	_buffer = new int[_buffer_size];
	_tail = 0;
	_head = 0;
	_real_size = 0;
}

Queue::~Queue() {
	delete[] _buffer;
}

Queue& Queue::operator=(const Queue& other) {
	_head = other._head, _tail = other._tail, _buffer_size = other._buffer_size, _real_size = other._real_size;

	int* new_buffer = new int[_buffer_size];
	for (int i = 0; i < _buffer_size; ++i) {
		new_buffer[i] = other._buffer[i];
	}

	delete[] _buffer;
	_buffer = new_buffer;

	return *this;
}
Queue& Queue::operator=(Queue&& other) noexcept {
	_head = other._head, _tail = other._tail, _buffer_size = other._buffer_size, _real_size = other._real_size;

	int* p_buffer = other._buffer;
	other._buffer = _buffer;
	_buffer = p_buffer;

	return *this;
}

int Queue::Dequeue() noexcept {
	if (_real_size == 0) return -1;

	int result = _buffer[_head];
	_head = (_head + 1) % _buffer_size;
	--_real_size;

	return result;
}

void Queue::Enqueue(int value) {
	if (_real_size == _buffer_size) _Realloc();

	_buffer[_tail] = value;
	_tail = (_tail + 1) % _buffer_size;
	++_real_size;
}

void Queue::_Realloc() {
	int new_buffer_size = 2 * _buffer_size;
	int* new_buffer = new int[new_buffer_size];

	int i = 0;
	for (; i < _buffer_size; ++i) {
		new_buffer[i] = _buffer[(i + _head) % _buffer_size];
	}

	++i;
	for (; i < new_buffer_size; ++i) {
		new_buffer[i] = 0;
	}

	int* p_buffer = _buffer;
	_buffer = new_buffer;
	delete[] p_buffer;

	_head = 0;
	_tail = _buffer_size;
	_buffer_size = new_buffer_size;
}

int Queue::size() const noexcept {
	return _real_size;
}

int main() {
	int n = 0;
	std::cin >> n;

	Queue example_queue;
	bool check = true;
	for (int i = 1; i <= n; ++i) {
		int a = 0, b = 0;
		std::cin >> a >> b;

		if (a == 3) {
			example_queue.Enqueue(b);
		}

		if (a == 2) {
			if (b != example_queue.Dequeue()) {
				check = false;
				break;
			}
		}
	}

	if (check) {
		std::cout << "YES";
	}
	else {
		std::cout << "NO";
	}

	return 0;
}

/*
------------
in
3
3 44
3 50
2 44

out
YES
------------
in
2
2 -1
3 10

out
YES
------------
in
2
3 44
2 66

out
NO
 */