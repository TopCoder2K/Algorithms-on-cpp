/*
 * Реализуйте структуру данных “массив строк” на основе декартового дерева по неявному ключу со следующими методами:
 * // Добавление строки в позицию position.
 * // Все последующие строки сдвигаются на одну позицию вперед.
 * void InsertAtAt( int position, const std::string& value );
 * // Удаление строки из позиции position.
 * // Все последующие строки сдвигаются на одну позицию назад.
 * void DeleteAt( int position );
 * // Получение строки из позиции position.
 * std::string GetAt( int position );
 * Все методы должны работать за O(log n) в среднем, где n – текущее количество строк в массиве.
 *
 * Формат ввода
 * Первая строка содержит количество команд k ≤ 10^6.
 * Последующие k строк содержат описания команд:
 * Команда “+ 10 hello” означает добавление строки hello в позицию 10.
 * Команда “- 14 16” означает удаление строки от позиции 14 до позиции 16.
 * Команда “? 33” означает запрос на вывод строки из массива в позиции 33.
 *
 * Формат выходных данных.
 * Выведите все строки, запрошенные командами “?”.
*/

#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>

template<class Type>
class ImplicitTreap {
    struct _TreapNode {
        Type _value; // Stores data.
        int _key; // An index (in "array") of current node.
        int _priority; // Generated randomly.
        int _successors; // The number of vertices in the subtree of our vertex (the vertex is included in the subtree).
        _TreapNode *_right_child;
        _TreapNode *_left_child;

        _TreapNode(int index, const Type& value) : _value(value), _key(index), _successors(1),
                                                   _right_child(nullptr), _left_child(nullptr), _priority(std::rand()) {}

        _TreapNode() : _value(0), _priority(0), _key(0), _successors(1), _right_child(nullptr), _left_child(nullptr) {}

        int UpdateKey();

        void UpdateNode();
    };
public:
    ImplicitTreap() : _root(nullptr) {}
    ~ImplicitTreap();

    ImplicitTreap& operator=(const ImplicitTreap& other);
    ImplicitTreap& operator=(ImplicitTreap&& other) noexcept;

    ImplicitTreap(const ImplicitTreap& other) : ImplicitTreap() { *this = other; }
    ImplicitTreap(ImplicitTreap&& other) noexcept : ImplicitTreap() { *this = std::forward<ImplicitTreap>(other); }


    void InsertAt(int key, const Type& value);
    void DeleteAt(int from, int to);
    Type& GetAt(int index);
private:
    _TreapNode *_root;

    void _delete_treap(_TreapNode *tree);

    _TreapNode* _merge(_TreapNode *left, _TreapNode *right);
    void _split(_TreapNode *current_node, int key, _TreapNode *&left, _TreapNode *&right);

    Type& _search(_TreapNode *node, int position);
};

// Update auxiliary value (number of successors).
template<typename T>
void ImplicitTreap<T>::_TreapNode::UpdateNode() {
    _successors = 1 + (_left_child != nullptr ? _left_child->_successors : 0)
                  + (_right_child != nullptr ? _right_child->_successors : 0);
}

// A function to delete treap.
template<class Type>
void ImplicitTreap<Type>::_delete_treap(_TreapNode *tree) {
    if (tree != nullptr) {
        std::queue<_TreapNode *> queue;

        queue.push(tree);

        while (!queue.empty()) {
            if (queue.front()->_left_child != nullptr)
                queue.push(queue.front()->_left_child);

            if (queue.front()->_right_child != nullptr)
                queue.push(queue.front()->_right_child);

            _TreapNode *node = queue.front();
            queue.pop();
            delete node;
        }
    }
}

template<class Type>
ImplicitTreap<Type>::~ImplicitTreap() {
    _delete_treap(_root);
}

template<class Type>
ImplicitTreap<Type>& ImplicitTreap<Type>::operator=(const ImplicitTreap& other) {
    if (this == &other) return *this;

    _delete_treap(_root);

    if (other._root != nullptr) {
        std::queue<_TreapNode *> queue;
        queue.push(other._root);

        while (!queue.empty()) {
            if (queue.front()->_left_child != nullptr)
                queue.push(queue.front()->_left_child);

            if (queue.front()->_right_child != nullptr)
                queue.push(queue.front()->_right_child);

            _TreapNode *node = queue.front();
            queue.pop();
            InsertAt(node->_value, node->_priority);
        }
    }

    return *this;
}

template<class Type>
ImplicitTreap<Type>& ImplicitTreap<Type>::operator=(ImplicitTreap&& other) noexcept {
    std::swap(_root, other._root);

    return *this;
}

// Merge treaps.
template<class Type>
typename ImplicitTreap<Type>::_TreapNode *ImplicitTreap<Type>::_merge(_TreapNode *left, _TreapNode *right) {
    if (left == nullptr || right == nullptr)
        return left == nullptr ? right : left;

    if (left->_priority > right->_priority) {
        left->_right_child = _merge(left->_right_child, right);
        left->UpdateNode();

        return left;
    }

    right->_left_child = _merge(left, right->_left_child);
    right->UpdateNode();

    return right;
}

// Calculate key.
template<typename T>
int ImplicitTreap<T>::_TreapNode::UpdateKey() {
    _key += _left_child ? _left_child->_successors : 0;

    if (_right_child != nullptr)
        _right_child->_key = _key + 1;

    if (_left_child != nullptr)
        _left_child->_key = _key - _left_child->_successors;

    return _key;
};

// Split the treap by a key.
template<class Type>
void ImplicitTreap<Type>::_split(_TreapNode *current_node, int key, _TreapNode *&left, _TreapNode *&right) {
    if (current_node == nullptr) {
        left = nullptr;
        right = nullptr;
        return;
    }

    if (key > current_node->UpdateKey()) {
        _TreapNode *tmp_left;
        _TreapNode *tmp_right;

        _split(current_node->_right_child, key, tmp_left, tmp_right);

        right = tmp_right;
        left = current_node;
        left->_right_child = tmp_left;
    } else {
        _TreapNode *temp_left;
        _TreapNode *temp_right;

        _split(current_node->_left_child, key, temp_left, temp_right);

        left = temp_left;
        right = current_node;
        right->_left_child = temp_right;
    }

    current_node->UpdateNode();
}

// InsertAt a value.
template<class Type>
void ImplicitTreap<Type>::InsertAt(int key, const Type &value) {
    auto new_node = new _TreapNode(key, value);

    if (!_root) {
        _root = new_node;
    } else {
        _TreapNode *splitted_left = nullptr;
        _TreapNode *splitted_right = nullptr;
        _root->_key = 0;

        _split(_root, key, splitted_left, splitted_right);

        _root = _merge(_merge(splitted_left, new_node), splitted_right);
    }
}

// Delete a segment [from; to] of nodes.
template<class Type>
void ImplicitTreap<Type>::DeleteAt(int from, int to) {
    _TreapNode *split_left = nullptr;
    _TreapNode *split_right = nullptr;
    _root->_key = 0;

    _split(_root, from, split_left, split_right);

    _TreapNode *split_right_left = nullptr;
    _TreapNode *split_right_right = nullptr;
    split_right->_key = 0;

    _split(split_right, to - from + 1, split_right_left, split_right_right);

    _root = _merge(split_left, split_right_right);
    _delete_treap(split_right_left);
}

template<class Type>
Type& ImplicitTreap<Type>::_search(_TreapNode *node, int position) {
    node->UpdateKey();

    if (node->_key < position)
        return _search(node->_right_child, position);

    if (node->_key > position)
        return _search(node->_left_child, position);

    return node->_value;
}

// Get at the position.
template<typename T>
T& ImplicitTreap<T>::GetAt(int index) {
    _root->_key = 0;

    return _search(_root, index);
}

int main() {
    int k;
    std::cin >> k;

    ImplicitTreap<std::string> treap;
    for (int i = 0; i < k; ++i) {
        char command = 0;
        std::cin >> command;

        if (command == '+') {
            int pos = 0;
            std::string str;
            std::cin >> pos >> str;
            treap.InsertAt(pos, str);
        } else if (command == '?') {
            int pos = 0;
            std::cin >> pos;
            std::cout << treap.GetAt(pos) << std::endl;
        } else {
            int from = 0, to = 0;
            std::cin >> from >> to;
            treap.DeleteAt(from, to);
        }
    }
    return 0;
}

/*
IN
6
+ 0 myau
+ 0 krya
? 0
+ 2 gav
- 1 1
? 1
OUT
krya
gav
*/