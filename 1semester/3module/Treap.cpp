#include <iostream>
#include <queue>
#include <cmath>
#include <algorithm>
#include <stack>
#include <vector>

template <typename Type, typename PriorityType, class KeyComparator = std::less<Type>, class PriorityComparator = std::less<PriorityType>>
class Treap {
	struct _Node {
		Type _value;
		PriorityType _priority;
		_Node* _right_child;
		_Node* _left_child;
		_Node(const Type& value, PriorityType priority) : _value(value), _priority(priority), _right_child(nullptr), _left_child(nullptr) {}
		_Node() : _value(0), _priority(0), _right_child(nullptr), _left_child(nullptr) {}
	};
public:
	Treap() : _root(nullptr) {}
	~Treap() { _DeleteTreap(_root); }

	Treap& operator=(const Treap& other);
	Treap& operator=(Treap&& other) noexcept;
	Treap(const Treap& other) : Treap() { *this = other; }
	Treap(Treap&& other) noexcept : Treap() { *this = std::forward<Treap>(other); }

	void Insert(const Type& key, const PriorityType& priority);
	void Remove(const Type& key);
	void PrintTreap() const;
	int GetDepth() const noexcept { return _GetDepth(_root); }
private:
	int _GetDepth(_Node* node) const noexcept;
	void _DeleteTreap(_Node* root);
	_Node* _root;
	_Node* _merge(_Node* left, _Node* right) noexcept;
	void _split(_Node* current_node, const Type& key, _Node*& left, _Node*& right) noexcept;
};

template <typename Type, typename PriorityType, class KeyComparator, class PriorityComparator>
void Treap<Type, PriorityType, KeyComparator, PriorityComparator>::_DeleteTreap(_Node* root) {
	if (root != nullptr) {
		std::stack<_Node*> stack;

		stack.push(root);
		while (!stack.empty()) {
			_Node* node = stack.top();
			stack.pop();

			if (node->_left_child != nullptr)
				stack.push(node->_left_child);

			if (node->_right_child != nullptr)
				stack.push(node->_right_child);

			delete node;
		}
	}
}

template <typename Type, typename PriorityType, class KeyComparator, class PriorityComparator>
Treap<Type, PriorityType, KeyComparator, PriorityComparator>& Treap<Type, PriorityType, KeyComparator, PriorityComparator>::operator=(const Treap& other) {
	if (this == &other) return *this;

	_DeleteTreap(_root);

	// Replication other.
	if (other._root != nullptr) {
		std::stack<_Node*> stack;

		stack.push(other._root);
		while (!stack.empty()) {
			_Node* node = stack.top();
			stack.pop();

			if (node->_left_child != nullptr)
				stack.push(node->_left_child);

			if (node->_right_child != nullptr)
				stack.push(node->_right_child);

			Insert(node->_value, node->_priority);
		}
	}
	else {
		_root = nullptr;
	}

	return *this;
}

template <typename Type, typename PriorityType, class KeyComparator, class PriorityComparator>
Treap<Type, PriorityType, KeyComparator, PriorityComparator>& Treap<Type, PriorityType, KeyComparator, PriorityComparator>::operator=(Treap&& other) noexcept {
	std::swap(_root, other._root);

	return *this;
}

// Merge operation (merge two treaps).
template <typename Type, typename PriorityType, class KeyComparator, class PriorityComparator>
typename Treap<Type, PriorityType, KeyComparator, PriorityComparator>::_Node* Treap<Type, PriorityType, KeyComparator, PriorityComparator>::_merge(_Node* left, _Node* right) noexcept {
	PriorityComparator priority_less;

	if (left == nullptr || right == nullptr) {
		return left == nullptr ? right : left;
	}

	if (priority_less(right->_priority, left->_priority)) {
		left->_right_child = _merge(left->_right_child, right);
		return left;
	}

	right->_left_child = _merge(left, right->_left_child);
	return right;
}

// Split operation (split a treap pointed by current_node by key into two treaps pointed by left and right).
template <typename Type, typename PriorityType, class KeyComparator, class PriorityComparator>
void Treap<Type, PriorityType, KeyComparator, PriorityComparator>::_split(_Node* current_node, const Type& key, _Node*& left, _Node*& right) noexcept {
	KeyComparator key_less;

	if (current_node == nullptr) {
		left = nullptr;
		right = nullptr;
	}
	else if (key_less(current_node->_value, key)) {
		_Node* tmp_left = nullptr;
		_Node* tmp_right = nullptr;

		_split(current_node->_right_child, key, tmp_left, tmp_right);

		right = tmp_right;
		left = current_node;
		left->_right_child = tmp_left;
	}
	else {
		_Node* tmp_left = nullptr;
		_Node* tmp_right = nullptr;

		_split(current_node->_left_child, key, tmp_left, tmp_right);

		left = tmp_left;
		right = current_node;
		right->_left_child = tmp_right;
	}
}

template <typename Type, typename PriorityType, class KeyComparator, class PriorityComparator>
void Treap<Type, PriorityType, KeyComparator, PriorityComparator>::Insert(const Type& key, const PriorityType& priority) {
	auto new_node = new _Node(key, priority);

	if (!_root) {
		_root = new_node;
	}
	else {
		_Node* splitted_left = nullptr;
		_Node* splitted_right = nullptr;

		_split(_root, key, splitted_left, splitted_right);
		_root = _merge(_merge(splitted_left, new_node), splitted_right);
	}
}

template <typename Type, typename PriorityType, class KeyComparator, class PriorityComparator>
void Treap<Type, PriorityType, KeyComparator, PriorityComparator>::Remove(const Type& key) {
	_Node* splitted_left = nullptr;
	_Node* splitted_right = nullptr;

	_split(_root, key, splitted_left, splitted_right);

	_Node* splitted_right_left = nullptr;
	_Node* splitted_right_right = nullptr;

	_split(splitted_right, key + 1, splitted_right_left, splitted_right_right);

	_DeleteTreap(splitted_right_left);

	_root = _merge(splitted_left, splitted_right_right);
}

template <typename Type, typename PriorityType, class KeyComparator, class PriorityComparator>
void Treap<Type, PriorityType, KeyComparator, PriorityComparator>::PrintTreap() const {
	std::queue<_Node*> queue;

	queue.push(_root);
	while (!queue.empty()) {
		if (queue.front()->_left_child != nullptr)
			queue.push(queue.front()->_left_child);

		if (queue.front()->_right_child != nullptr)
			queue.push(queue.front()->_right_child);

		_Node* node = queue.front();
		queue.pop();
		std::cout << node->_value << ' ';
	}
}

// Recursive finding of the depth of the treap.
template <typename Type, typename PriorityType, class KeyComparator, class PriorityComparator>
int Treap<Type, PriorityType, KeyComparator, PriorityComparator>::_GetDepth(_Node* node) const noexcept {
	if (node == nullptr) return 0;

	return 1 + std::max(_GetDepth(node->_left_child), _GetDepth(node->_right_child));
}

template <typename Type, typename PriorityType, class KeyComparator = std::less<Type>, class PriorityComparator = std::less<PriorityType>>
Treap<Type, PriorityType, KeyComparator, PriorityComparator> CheckTreap(Treap<Type, PriorityType> treap) {
	return treap;
}

int main() {
	int n;
	std::cin >> n;

	std::vector<std::pair<int, int>> data(n);
	for (int i = 0; i < n; ++i) {
		std::cin >> data[i].first >> data[i].second;
	}

	Treap<int, int> treap;
	for (int i = 0; i < n; ++i) {
		treap.Insert(data[i].first, data[i].second);
	}

	Treap<int, int> treap2 = CheckTreap<int, int>(treap);
	std::cout << treap2.GetDepth() << std::endl;
	for (int i = 0; i < n - 3; ++i) {
		treap2.Remove(data[i].first);
	}
	treap2.PrintTreap();
	std::cout << std::endl << treap2.GetDepth();

	return 0;
}

/*
IN
4
2 10
2 5
3 6
1 7
OUT
3
1 3
2
*/