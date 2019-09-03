/*3_1. Солдаты. В одной военной части солдат решили построить в одну шеренгу по росту.
Т.к. часть была далеко не образцовая, то солдаты часто приходили не вовремя,
а то их и вовсе приходилось выгонять из шеренги за плохо начищенные сапоги.
Однако солдаты в процессе прихода и ухода должны были всегда быть выстроены по росту – сначала самые высокие,
а в конце – самые низкие. За расстановку солдат отвечал прапорщик,
который заметил интересную особенность – все солдаты в части разного роста.
Ваша задача состоит в том, чтобы помочь прапорщику правильно расставлять солдат,
а именно для каждого приходящего солдата указывать, перед каким солдатом в строе он должен становиться.
Требуемая скорость выполнения команды - O(log n).

Формат входных данных.
Первая строка содержит число N – количество команд (1 ≤ N ≤ 30 000).
В каждой следующей строке содержится описание команды: число 1 и X, если солдат приходит в строй
(X – рост солдата, натуральное число до 100 000 включительно),
и число 2 и Y, если солдата, стоящего в строе на месте Y, надо удалить из строя. Солдаты в строе нумеруются с нуля.
Формат выходных данных.
На каждую команду 1 (добавление в строй) Вы должны выводить число K – номер позиции, на которую должен встать этот солдат (все стоящие за ним двигаются назад).
*/

#include <iostream>
#include <algorithm>
#include <queue>
#include <stack>
#include <cassert>

template <class Type>
class AVLTree {
	struct _Node {
		Type key;
		unsigned char height;
		_Node* left;
		_Node* right;
		// lchild_num and rchild_num store a number of nodes in left and right subtree respectively.
		int lchild_num, rchild_num;
		explicit _Node(Type k) : key(k), height(1), left(nullptr), right(nullptr), lchild_num(0), rchild_num(0) {}
	};

public:
	AVLTree() = default;
	~AVLTree();
	AVLTree& operator=(const AVLTree& other);
	AVLTree& operator=(AVLTree&& other) noexcept;
	AVLTree(const AVLTree& other) { *this = other; }
	AVLTree(AVLTree&& other) noexcept { *this = std::forward<AVLTree>(other); }

	void Insert(const Type& k) { _root = _Insert(_root, k); }
	void Remove(const Type& k) { _root = _Remove(_root, k); }
	_Node* GetRoot() const noexcept { return _root; }

	void PrintAVLTree() const;

	int FindK(const Type& key) const noexcept { return _FindK(_root, key, 0); }
	void RemoveByK(int k) noexcept { _Node* p = _FindByK(_root, k); _root = _Remove(_root, p->key); }
private:
	_Node* _Insert(_Node* p, const Type& k);
	_Node* _Remove(_Node* p, const Type& k) noexcept;
	std::pair<_Node*, _Node*> _FindAndRemoveMaxInLeftSubtree(std::pair<_Node*, _Node*> pair) noexcept;

	unsigned char _GetHeight(_Node* p) const noexcept { return p ? p->height : 0; }
	// BalanceFactor is only applied to nodes which have 2 children.
	char _BalanceFactor(_Node* p) const noexcept { return _GetHeight(p->right) - _GetHeight(p->left); }
	void _FixHeightAndNumberOfChildren(_Node* p) noexcept;
	_Node* _RotateRight(_Node* p) noexcept;
	_Node* _RotateLeft(_Node* p) noexcept;
	_Node* _Balance(_Node* p) noexcept;

	int _FindK(_Node* p, const Type& key, int k) const noexcept;
	_Node* _FindByK(_Node* p, int k) const noexcept;

	_Node* _root = nullptr;
};

template <class Type>
AVLTree<Type>::~AVLTree() {
	while (_root != nullptr)
		_root = _Remove(_root, _root->key);
}

template <class Type>
AVLTree<Type>& AVLTree<Type>::operator=(const AVLTree<Type>& other) {
	if (this == &other) {
		return *this;
	}

	while (_root != nullptr) {
		_root = _Remove(_root, _root->key);
	}

	// Replication other.
	if (other._root != nullptr) {
		std::stack <_Node*> stack;
		stack.push(other._root);
		while (!stack.empty()) {
			_Node* node = stack.top();
			stack.pop();

			if (node->left != nullptr)
				stack.push(node->left);

			if (node->right != nullptr)
				stack.push(node->right);

			Insert(node->key);
		}
	}

	return *this;
}

template <class Type>
AVLTree<Type>& AVLTree<Type>::operator=(AVLTree<Type>&& other) noexcept {
	std::swap(_root, other._root);

	return *this;
}

// This function is applied after rotation and during balancing (because of insertion).
template <class Type>
void AVLTree<Type>::_FixHeightAndNumberOfChildren(_Node* p) noexcept {
	p->height = std::max(_GetHeight(p->left), _GetHeight(p->right)) + 1;

	if (p->left == nullptr) {
		p->lchild_num = 0;
	}
	else {
		p->lchild_num = p->left->lchild_num + p->left->rchild_num + 1;
	}

	if (p->right == nullptr) {
		p->rchild_num = 0;
	}
	else {
		p->rchild_num = p->right->lchild_num + p->right->rchild_num + 1;
	}
}

template <class Type>
typename AVLTree<Type>::_Node* AVLTree<Type>::_RotateRight(_Node* p) noexcept {
	_Node* left_child = p->left;

	p->left = left_child->right;
	left_child->right = p;

	_FixHeightAndNumberOfChildren(p);
	_FixHeightAndNumberOfChildren(left_child);

	return left_child;
}

template <class Type>
typename AVLTree<Type>::_Node* AVLTree<Type>::_RotateLeft(_Node* p) noexcept {
	_Node* right_child = p->right;

	p->right = right_child->left;
	right_child->left = p;

	_FixHeightAndNumberOfChildren(p);
	_FixHeightAndNumberOfChildren(right_child);

	return right_child;
}

template <class Type>
typename AVLTree<Type>::_Node* AVLTree<Type>::_Balance(_Node* p) noexcept {
	if (!p) return nullptr;

	_FixHeightAndNumberOfChildren(p);

	if (_BalanceFactor(p) == 2) {
		if (_BalanceFactor(p->right) < 0)
			p->right = _RotateRight(p->right);

		return _RotateLeft(p);
	}

	if (_BalanceFactor(p) == -2) {
		if (_BalanceFactor(p->left) > 0)
			p->left = _RotateLeft(p->left);

		return _RotateRight(p);
	}

	return p;
}

template <class Type>
typename AVLTree<Type>::_Node* AVLTree<Type>::_Insert(_Node* p, const Type& key) {
	if (!p) return new _Node(key);

	if (key < p->key) {
		p->left = _Insert(p->left, key);
	}
	else {
		p->right = _Insert(p->right, key);
	}

	return _Balance(p);
}

// This function helps in removing nodes.
// First component of the pair saves node with max key in left subtree, and the second one is "current node".
template <class Type>
std::pair<typename AVLTree<Type>::_Node*, typename AVLTree<Type>::_Node*> AVLTree<Type>::_FindAndRemoveMaxInLeftSubtree(std::pair<_Node*, _Node*> pair) noexcept {
	if (pair.second->right == nullptr) { // Here we save the necessary node and make a step on the left to change the right child of the previous node.
		pair.first = pair.second, pair.second = pair.second->left;
		return pair;
	}
	else { // Here we only make a step further on the right.
		std::pair<_Node*, _Node*> new_pair;
		new_pair.first = pair.first, new_pair.second = pair.second->right;
		new_pair = _FindAndRemoveMaxInLeftSubtree(new_pair);
		pair.first = new_pair.first, pair.second->right = new_pair.second;
	}

	pair.second = _Balance(pair.second);

	return pair;
}

template <class Type>
typename AVLTree<Type>::_Node* AVLTree<Type>::_Remove(_Node* p, const Type& key) noexcept {
	assert(p);

	if (key < p->key) {
		p->left = _Remove(p->left, key);
	}
	else if (key > p->key) {
		p->right = _Remove(p->right, key);
	}
	else {
		_Node* r = p->right;
		_Node* l = p->left;

		delete p;

		if (!l) return r;

		std::pair<_Node*, _Node*> pair;
		pair.first = nullptr, pair.second = l;
		pair = _FindAndRemoveMaxInLeftSubtree(pair);
		pair.first->right = r, pair.first->left = pair.second;
		return _Balance(pair.first);
	}

	return _Balance(p);
}

// k - is a position where a come soldier (node) has to stand.
template <class Type>
int AVLTree<Type>::_FindK(_Node* p, const Type& key, int k) const noexcept {
	if (key < p->key)
		k = _FindK(p->left, key, k);

	if (key > p->key) {
		k = _FindK(p->right, key, k);
		k += p->lchild_num + 1;
	}

	if (key == p->key)
		k = p->lchild_num;

	return k;
}

// This function finds a soldier (node) by a given position.
template <class Type>
typename AVLTree<Type>::_Node* AVLTree<Type>::_FindByK(_Node* p, int k) const noexcept {
	if (k < p->lchild_num) {
		p = _FindByK(p->left, k);
	}
	else if (k > p->lchild_num) {
		p = _FindByK(p->right, k - 1 - p->lchild_num);
	}
	else {
		return p;
	}
}

template<class Type>
void AVLTree<Type>::PrintAVLTree() const {
	std::queue<_Node*> queue;

	queue.push(_root);
	while (!queue.empty()) {
		if (queue.front()->left != nullptr)
			queue.push(queue.front()->left);

		if (queue.front()->right != nullptr)
			queue.push(queue.front()->right);

		std::cout << "Key: " << queue.front()->key << ", left child: " << queue.front()->lchild_num << ", right child: " << queue.front()->rchild_num << std::endl;
		queue.pop();
	}
}

template <class Type>
AVLTree<Type> CheckAVLTree(AVLTree<Type> avl_tree = AVLTree<Type>()) {
	return avl_tree;
}

int main() {
	int n = 0;
	std::cin >> n;

	int cur_number_of_nodes = 0;
	auto avl_tree = AVLTree<int>();
	for (int i = 0; i < n; ++i) {
		int command = 0;
		std::cin >> command;

		if (command == 1) {
			int key = 0;
			std::cin >> key;
			avl_tree.Insert(key);
			++cur_number_of_nodes;
			std::cout << cur_number_of_nodes - 1 - avl_tree.FindK(key) << std::endl;
		}
		else {
			int k = 0;
			std::cin >> k;
			avl_tree.RemoveByK(cur_number_of_nodes - 1 - k);
			--cur_number_of_nodes;
		}
	}

	/*auto avl_tree2 = CheckAVLTree(avl_tree);

	avl_tree2.PrintAVLTree();*/

	return 0;
}

/*
IN
5
1 100
1 200
1 50
2 1
1 150
OUT
0
0
2
1*/