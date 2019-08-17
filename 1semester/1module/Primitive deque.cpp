#include <iostream>
#include <string>
#include <cassert>

class Deque {
public:
	Deque() : _real_size(0), _head(nullptr), _tail(nullptr) {}
	~Deque();

	Deque& operator=(const Deque& other);
	Deque& operator=(Deque&& other) noexcept;
	Deque(const Deque& other) : Deque() { *this = other; }
	Deque(Deque&& other) noexcept : Deque() { *this = std::forward<Deque>(other); }

	void PushFront(char symbol);
	void PushBack(char symbol);
	char PopFront();
	char PopBack();
	int size() const noexcept { return _real_size; }
private:
	struct _Node {
		_Node(char symbol) : _symbol(symbol), _next(nullptr), _previous(nullptr) {}
		_Node* _previous;
		_Node* _next;
		char _symbol;
	};

	_Node* _head;
	_Node* _tail;
	int _real_size;
};

Deque::~Deque() {
	while (_real_size > 0)
		PopBack();
}

Deque& Deque::operator=(const Deque& other) {
	_Node* other_element = other._head;
	_Node* current_element = _head;

	// Copying while there are elements in both deques.
	while (current_element != _tail && other_element != other._tail) {
		current_element->_symbol = other_element->_symbol;
		current_element = current_element->_next;
		other_element = other_element->_next;
	}
	if (_real_size > 0)
		_tail->_symbol = other._tail->_symbol;

	// If other._real_size <= _real_size then we must delete excess nodes from this.
	if (other_element == _tail) {
		_Node* tmp = current_element;
		current_element = current_element->_next;

		while (current_element != nullptr) {
			_Node* next_current_element = current_element->_next;
			delete current_element;
			current_element = next_current_element;
		}

		_tail = tmp;
	}
	else { // Else we must create new nodes and add them to this.
		while (other_element != nullptr) {
			_Node* new_current_element = new _Node(other_element->_symbol);

			if (current_element == nullptr) {
				_head = current_element = new_current_element;
			}
			else {
				current_element->_next = new_current_element;
				new_current_element->_previous = current_element;
				current_element = new_current_element;
			}

			other_element = other_element->_next;
		}

		_tail = current_element;
	}

	_real_size = other._real_size;

	return *this;
}

Deque& Deque::operator=(Deque&& other) noexcept {
	std::swap(other._head, _head);
	std::swap(other._tail, _tail);
	std::swap(other._real_size, _real_size);

	return *this;
}

char Deque::PopBack() {
	assert(_real_size > 0);

	char result = _tail->_symbol;
	_Node* tmp = _tail;

	if (_real_size > 1)
		_tail->_previous->_next = nullptr;

	_tail = _tail->_previous;
	--_real_size;

	delete tmp;
	return result;
}

char Deque::PopFront() {
	assert(_real_size > 0);

	char result = _head->_symbol;
	_Node* tmp = _head;

	if (_real_size > 1)
		_head->_next->_previous = nullptr;

	_head = _head->_next;
	--_real_size;

	delete tmp;
	return result;
}

void Deque::PushFront(char symbol) {
	_Node* tmp = new _Node(symbol);

	if (_real_size > 0) {
		_head->_previous = tmp;
		tmp->_next = _head;
		_head = tmp;
	}
	else {
		_head = _tail = tmp;
	}

	++_real_size;
}

void Deque::PushBack(char symbol) {
	_Node* tmp = new _Node(symbol);

	if (_real_size > 0) {
		_tail->_next = tmp;
		tmp->_previous = _tail;
		_tail = tmp;
	}
	else {
		_head = _tail = tmp;
	}

	++_real_size;
}

Deque CheckDeque(Deque deque) {
	return deque;
}

int main() {
	Deque a;
	char k;
	int n;
	std::cin >> n;

	for (int i = 1; i <= n; ++i) {
		std::string str;
		std::cin >> str;

		if (str == "pushfront") {
			std::cin >> k;
			a.PushFront(k);
		}
		if (str == "pushback") {
			std::cin >> k;
			a.PushBack(k);
		}
		if (str == "popfront") {
			std::cout << a.PopFront() << std::endl;
		}
		if (str == "popback") {
			std::cout << a.PopBack() << std::endl;
		}
	}

	Deque b = a;
	b.PushBack(10);
	b = CheckDeque(a);
	while (b.size() > 0) {
		std::cout << b.PopBack() << ' ';
	}

	return 0;
}

/*
in
3
pushfront a
pushfront b
pushback c

out
b a c
 */