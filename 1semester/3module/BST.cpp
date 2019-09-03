/*Дано число N < 10^6 и последовательность целых чисел из [-2^31..2^31] длиной N.
Требуется построить бинарное дерево поиска, заданное наивным порядком вставки.
Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
Выведите элементы в порядке level-order (по слоям, “в ширину”).*/

#include <iostream>
#include <cassert>
#include <queue>
#include <stack>

// Interface
template <class Type, class Comparator = std::less<Type>>
class Tree {
public:
	virtual void Insert(const Type& key) = 0;
	virtual void Remove(const Type& key) = 0;
	virtual int GetDepth() const = 0;
};

template <class Type, class Comparator = std::less<Type>>
class BST : public Tree<Type, Comparator> {
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

template<class Type, class Comparator>
void BST<Type, Comparator>::_DeleteTree() {
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

template<class Type, class Comparator>
BST<Type, Comparator>& BST<Type, Comparator>::operator=(const BST<Type, Comparator>& other) {
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

template<class Type, class Comparator>
BST<Type, Comparator>& BST<Type, Comparator>::operator=(BST<Type, Comparator>&& other) noexcept {
	std::swap(_root, other._root);
	return *this;
}

// Returns a parent node for a node with _value == key.
template<class Type, class Comparator>
typename BST<Type, Comparator>::_Node* BST<Type, Comparator>::_FindParentNode(const Type& key) const noexcept {
	assert(_root != nullptr);

	Comparator less;

	if (!less(_root->_value, key) && !less(key, _root->_value)) {
		return _root;
	}

	_Node* current_node = _root;
	while (current_node != nullptr) {
		if (less(key, current_node->_value)) {
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

template<class Type, class Comparator>
void BST<Type, Comparator>::Insert(const Type& key) {
	if (_root == nullptr) {
		_root = new _Node(key);
		return;
	}

	Comparator less;
	auto new_node = new _Node(key);

	_Node* current_node = _root;
	while (true) {
		if (less(key, current_node->_value)) {
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
template<class Type, class Comparator>
typename BST<Type, Comparator>::_Node* BST<Type, Comparator>::_GetChild(typename BST<Type, Comparator>::_Node* node) {
	if (node->_left_child != nullptr)
		return node->_left_child;

	return node->_right_child;
}

// This function is applied only to nodes that have two children.
template<class Type, class Comparator>
Type BST<Type, Comparator>::_GetNextAfterCurrent(_Node* node) {
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

	Type key = current_node->_value;

	delete current_node;
	return key;
}

template<class Type, class Comparator>
void BST<Type, Comparator>::Remove(const Type& key) {
	// Parent node is necessary when updating pointers during removing a node.
	_Node* parent = _FindParentNode(key);
	_Node* child = nullptr;
	bool left_child = false;
	Comparator less;

	// Getting the child.
	if (parent->_left_child != nullptr && (!less(key, parent->_left_child->_value) && !less(parent->_left_child->_value, key))) {
		child = parent->_left_child;
		left_child = true;
	}
	if (parent->_right_child != nullptr && (!less(key, parent->_right_child->_value) && !less(parent->_right_child->_value, key))) {
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
template<class Type, class Comparator>
void BST<Type, Comparator>::PrintBST() const {
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

template<class Type, class Comparator>
int BST<Type, Comparator>::GetDepth() const {
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

template<class Type, class Comparator = std::less<Type>>
BST<Type, Comparator> CheckBST(BST<Type, Comparator> a) {
	return a;
}

int main() {
	int n;
	std::cin >> n;

	BST<int> tree;
	for (int i = 0; i < n; ++i) {
		int key = 0;
		std::cin >> key;
		tree.Insert(key);
	}

	/*int remove_key = 0;
	std::cin >> remove_key;
	tree.Remove(remove_key);*/

	BST<int> tree2 = CheckBST(tree);

	// tree.PrintBST();
	tree2.PrintBST();
	// std::cout << std::endl << tree.GetDepth();

	return 0;
}

/*
IN
3
2 1 3
OUT
2 1 3
---------
IN
3
1 2 3
OUT
1 2 3
---------
IN
3
3 1 2
OUT
3 1 2
--------
IN
4
3 1 4 2
OUT
3 1 4 2*/