/*ÐÐ°Ð½Ð¾ ÑÐ¸ÑÐ»Ð¾ N < 10^6 Ð¸ Ð¿Ð¾ÑÐ»ÐµÐ´Ð¾Ð²Ð°ÑÐµÐ»ÑÐ½Ð¾ÑÑÑ Ð¿Ð°Ñ ÑÐµÐ»ÑÑ ÑÐ¸ÑÐµÐ» Ð¸Ð· [-2^31; 2^31] Ð´Ð»Ð¸Ð½Ð¾Ð¹ N.
ÐÐ¾ÑÑÑÐ¾Ð¸ÑÑ Ð´ÐµÐºÐ°ÑÑÐ¾Ð²Ð¾ Ð´ÐµÑÐµÐ²Ð¾ Ð¸Ð· N ÑÐ·Ð»Ð¾Ð², ÑÐ°ÑÐ°ÐºÑÐµÑÐ¸Ð·ÑÑÑÐ¸ÑÑÑ Ð¿Ð°ÑÐ°Ð¼Ð¸ ÑÐ¸ÑÐµÐ» {Xi, Yi}.
ÐÐ°Ð¶Ð´Ð°Ñ Ð¿Ð°ÑÐ° ÑÐ¸ÑÐµÐ» {Xi, Yi} Ð¾Ð¿ÑÐµÐ´ÐµÐ»ÑÐµÑ ÐºÐ»ÑÑ Xi Ð¸ Ð¿ÑÐ¸Ð¾ÑÐ¸ÑÐµÑ Yi Ð² Ð´ÐµÐºÐ°ÑÑÐ¾Ð²Ð¾Ð¼ Ð´ÐµÑÐµÐ²Ðµ.
ÐÐ¾Ð±Ð°Ð²Ð»ÐµÐ½Ð¸Ðµ ÑÐ·Ð»Ð° Ð² Ð´ÐµÐºÐ°ÑÑÐ¾Ð²Ð¾ Ð´ÐµÑÐµÐ²Ð¾ Ð²ÑÐ¿Ð¾Ð»Ð½ÑÐ¹ÑÐµ Ð»ÑÐ±Ð¾Ð¹ Ð²ÐµÑÑÐ¸ÐµÐ¹ Ð°Ð»Ð³Ð¾ÑÐ¸ÑÐ¼Ð°, ÑÐ°ÑÑÐºÐ°Ð·Ð°Ð½Ð½Ð¾Ð³Ð¾ Ð½Ð° Ð»ÐµÐºÑÐ¸Ð¸.
ÐÐ¾ÑÑÑÐ¾Ð¸ÑÑ ÑÐ°ÐºÐ¶Ðµ Ð½Ð°Ð¸Ð²Ð½Ð¾Ðµ Ð´ÐµÑÐµÐ²Ð¾ Ð¿Ð¾Ð¸ÑÐºÐ° Ð¿Ð¾ ÐºÐ»ÑÑÐ°Ð¼ Xi.
2_1. ÐÑÑÐ¸ÑÐ»Ð¸ÑÑ ÑÐ°Ð·Ð½Ð¸ÑÑ Ð³Ð»ÑÐ±Ð¸Ð½ Ð½Ð°Ð¸Ð²Ð½Ð¾Ð³Ð¾ Ð´ÐµÑÐµÐ²Ð° Ð¿Ð¾Ð¸ÑÐºÐ° Ð¸ Ð´ÐµÐºÐ°ÑÑÐ¾Ð²Ð¾Ð³Ð¾ Ð´ÐµÑÐµÐ²Ð°. Ð Ð°Ð·Ð½Ð¸ÑÐ° Ð¼Ð¾Ð¶ÐµÑ Ð±ÑÑÑ Ð¾ÑÑÐ¸ÑÐ°ÑÐµÐ»ÑÐ½Ð°.
*/

#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
#include <stack>
#include <cassert>

// Interface
template <class Type>
class Tree {
public:
	virtual void Insert(const Type& key) = 0;
	virtual void Remove(const Type& key) = 0;
	virtual int GetDepth() const = 0;
};

template <class Type>
class BST : public Tree<Type> {
public:
	BST() : _root(nullptr) {};
	virtual ~BST() { _DeleteTree(); }

	BST& operator=(const BST& other);
	BST& operator=(BST&& other) noexcept;
	BST(const BST& other) : BST() { *this = other; }
	BST(BST&& other) noexcept : BST() { *this = std::forward<BST>(other); }
public:
	void Insert(const Type& key) override;
	void Remove(const Type& key) override;
	void PrintBST() const;
	int GetDepth() const override;

private:
	struct _Node {
		explicit _Node(const Type& value) : _value(value), _right_child(nullptr), _left_child(nullptr) {}
		Type _value;
		_Node* _right_child;
		_Node* _left_child;
	};

	void _DeleteTree();
	_Node* _root;
	_Node* _FindParentNode(const Type& key) const noexcept;
	_Node* _GetRoot() const noexcept { return _root; }
	_Node* _GetChild(_Node* node);
	Type _GetNextAfterCurrent(_Node* node);
};

template<class Type>
void BST<Type>::_DeleteTree() {
	if (_root != nullptr) {
		std::stack<_Node*> stack;

		stack.push(_root);
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

template<class Type>
BST<Type>& BST<Type>::operator=(const BST<Type>& other) {
	if (this == &other) {
		return *this;
	}

	_DeleteTree();

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

			Insert(node->_value);
		}

		return *this;
	}
}

template<class Type>
BST<Type>& BST<Type>::operator=(BST<Type>&& other) noexcept {
	std::swap(_root, other._root);
	return *this;
}

// Returns a parent node for a node with _value == key.
template<class Type>
typename BST<Type>::_Node* BST<Type>::_FindParentNode(const Type& key) const noexcept {
	assert(_root != nullptr);

	if (_root->_value == key) {
		return _root;
	}

	_Node* current_node = _root;
	while (current_node != nullptr) {
		if (key < current_node->_value) {
			if (key == current_node->_left_child->_value)
				return current_node;

			current_node = current_node->_left_child;
		}
		else {
			if (key == current_node->_right_child->_value)
				return current_node;

			current_node = current_node->_right_child;
		}
	}

	assert(false);
}

template<class Type>
void BST<Type>::Insert(const Type& key) {
	if (_root == nullptr) {
		_root = new _Node(key);
		return;
	}

	auto new_node = new _Node(key);

	_Node* current_node = _root;
	while (true) {
		if (key < current_node->_value) {
			if (current_node->_left_child != nullptr) {
				current_node = current_node->_left_child;
			}
			else {
				current_node->_left_child = new_node;
				break;
			}
		}
		else {
			if (current_node->_right_child != nullptr) {
				current_node = current_node->_right_child;
			}
			else {
				current_node->_right_child = new_node;
				break;
			}
		}
	}
}

// A special function which returns pointer to only child of the node.
// It is applied only to nodes that have one child.
template<class Type>
typename BST<Type>::_Node* BST<Type>::_GetChild(typename BST<Type>::_Node* node) {
	if (node->_left_child != nullptr)
		return node->_left_child;

	return node->_right_child;
}

// This function is applied only to nodes that have two children.
template<class Type>
Type BST<Type>::_GetNextAfterCurrent(_Node* node) {
	_Node* current_node = node->_right_child;
	_Node* parent = node;
	bool step = false;

	while (current_node->_left_child != nullptr) {
		parent = current_node;
		current_node = current_node->_left_child;
		step = true;
	}

	if (current_node->_right_child != nullptr) {
		if (step) {
			parent->_left_child = current_node->_right_child;
		}
		else {
			parent->_right_child = current_node->_right_child;
		}
	}
	else {
		if (step) {
			parent->_left_child = nullptr;
		}
		else {
			parent->_right_child = nullptr;
		}
	}

	int key = current_node->_value;

	delete current_node;
	return key;
}

template<class Type>
void BST<Type>::Remove(const Type& key) {
	// Parent node is necessary when updating pointers during removing a node.
	_Node* parent = _FindParentNode(key);
	_Node* child = nullptr;
	bool left_child = false;

	// Getting the child.
	if (parent->_left_child != nullptr && key == parent->_left_child->_value) {
		child = parent->_left_child;
		left_child = true;
	}
	if (parent->_right_child != nullptr && key == parent->_right_child->_value) {
		child = parent->_right_child;
	}
	// Root case.
	if (!child)
		child = parent;


	// 3 cases: 1) no children, 2) one child, 3) two children.
	// 1) no children.
	if (child->_left_child == nullptr && child->_right_child == nullptr) {
		delete child;
		if (left_child) {
			parent->_left_child = nullptr;
		}
		else {
			parent->_right_child = nullptr;
		}
	}
	// 2) one child.
	else if (child->_left_child == nullptr || child->_right_child == nullptr) {
		if (left_child) {
			parent->_left_child = _GetChild(child);
		}
		else {
			parent->_right_child = _GetChild(child);
		}

		delete child;
	}
	// 3) two children.
	else {
		child->_value = _GetNextAfterCurrent(child);
	}
}

// Prints BST in level-order.
template<class Type>
void BST<Type>::PrintBST() const {
	std::queue<_Node*> queue;

	queue.push(_GetRoot());
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

template<class Type>
int BST<Type>::GetDepth() const {
	int depth = 0;
	std::stack<std::pair<BST::_Node*, int>> stack;

	std::pair<BST::_Node*, int> tmp;
	tmp.first = _GetRoot();
	tmp.second = 1;

	stack.push(tmp);
	while (!stack.empty()) {
		std::pair<BST::_Node*, int> current_top = stack.top();
		stack.pop();

		if (current_top.first->_left_child != nullptr) {
			tmp.first = current_top.first->_left_child;
			tmp.second = current_top.second + 1;
			stack.push(tmp);
		}
		else {
			if (current_top.second > depth)
				depth = current_top.second;
		}

		if (current_top.first->_right_child != nullptr) {
			tmp.first = current_top.first->_right_child;
			tmp.second = current_top.second + 1;
			stack.push(tmp);
		}
		else {
			if (current_top.second > depth)
				depth = current_top.second;
		}
	}

	return depth;
}




template <typename Type, typename PriorityType>
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

template <typename Type, typename PriorityType>
void Treap<Type, PriorityType>::_DeleteTreap(_Node* root) {
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

template <typename Type, typename PriorityType>
Treap<Type, PriorityType>& Treap<Type, PriorityType>::operator=(const Treap& other) {
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

template <typename Type, typename PriorityType>
Treap<Type, PriorityType>& Treap<Type, PriorityType>::operator=(Treap&& other) noexcept {
	std::swap(_root, other._root);

	return *this;
}

// Merge operation (merge two treaps).
template <typename Type, typename PriorityType>
typename Treap<Type, PriorityType>::_Node* Treap<Type, PriorityType>::_merge(_Node* left, _Node* right) noexcept {
	if (left == nullptr || right == nullptr) {
		return left == nullptr ? right : left;
	}

	if (left->_priority > right->_priority) {
		left->_right_child = _merge(left->_right_child, right);
		return left;
	}

	right->_left_child = _merge(left, right->_left_child);
	return right;
}

// Split operation (split a treap pointed by current_node by key into two treaps pointed by left and right).
template <typename Type, typename PriorityType>
void Treap<Type, PriorityType>::_split(_Node* current_node, const Type& key, _Node*& left, _Node*& right) noexcept {
	if (current_node == nullptr) {
		left = nullptr;
		right = nullptr;
	}
	else if (key > current_node->_value) {
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

template <typename Type, typename PriorityType>
void Treap<Type, PriorityType>::Insert(const Type& key, const PriorityType& priority) {
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

template <typename Type, typename PriorityType>
void Treap<Type, PriorityType>::Remove(const Type& key) {
	_Node* splitted_left = nullptr;
	_Node* splitted_right = nullptr;

	_split(_root, key, splitted_left, splitted_right);

	_Node* splitted_right_left = nullptr;
	_Node* splitted_right_right = nullptr;

	_split(splitted_right, key + 1, splitted_right_left, splitted_right_right);

	_DeleteTreap(splitted_right_left);

	_root = _merge(splitted_left, splitted_right_right);
}

template <typename Type, typename PriorityType>
void Treap<Type, PriorityType>::PrintTreap() const {
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
template <typename Type, typename PriorityType>
int Treap<Type, PriorityType>::_GetDepth(_Node* node) const noexcept {
	if (node == nullptr) return 0;

	return 1 + std::max(_GetDepth(node->_left_child), _GetDepth(node->_right_child));
}

template <typename Type, typename PriorityType>
Treap<Type, PriorityType> CheckTreap(Treap<Type, PriorityType> treap) {
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

	/*Treap<int> treap2 = CheckTreap(treap);
	std::cout << treap2.GetDepth() << std::endl;
	for (int i = 0; i < n - 3; ++i) {
		treap2.Remove(data[i].first);
	}
	treap2.PrintTreap();
	std::cout << std::endl << treap2.GetDepth() << std::endl;*/

	BST<int> tree = BST<int>();
	for (int i = 0; i < n; ++i)
		tree.Insert(data[i].first);

	std::cout << tree.GetDepth() - treap.GetDepth();
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
0
----------------
IN
10
5 11
18 8
25 7
50 12
30 30
15 15
20 10
22 5
40 20
45 9
OUT
2
----------------
IN
10
38 19
37 5
47 15
35 0
12 3
0 42
31 37
21 45
30 26
41 6
OUT
2*/