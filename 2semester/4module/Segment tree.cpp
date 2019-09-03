/*2_2. Последовательность единиц.
 * Дан массив из нулей и единиц a0, a1, ..., an-1.
 * Для каждого запроса [left, right] найдите такой подотрезок al, al+1, ..., ar  этого массива (0 <= left <= l <= r <= right < n),
 * что числа al, al+1, …,  ar являются максимально возможной последовательностью единиц.
 * Требуемое время ответа на запрос - O(log n).
 * Формат входных данных.
 * Описание каждого теста начинается с двух чисел n и m - длины массива и числа интересующих подотрезков.
 * В следующей строке содержится n нулей и единиц.
 * Далее следуют описания подотрезков, каждое описание состоит из двух чисел left и right, обозначающих левый и правый конец подотрезка (0 <= left <= right < n).
 * Формат выходных данных.
 * Для каждого примера выведите m чисел: искомую максимальную длину последовательности единиц для каждого из подотрезков.
*/

#include <iostream>
#include <vector>

class SegmentTree {
    struct _Node {
        uint32_t segment_len, prefix, suffix, max_seq;
        _Node* left;
        _Node* right;

        _Node() : segment_len(1), prefix(0), suffix(0), max_seq(0), left(nullptr), right(nullptr) {}
    };

    _Node* _BuildTree(const std::vector<uint16_t>& seq, uint32_t l, uint32_t r);
    _Node* _DivideSegment(SegmentTree::_Node *node, uint32_t l, uint32_t r, uint32_t l_border, uint32_t r_border) const;
    _Node* _root;
    void _DeleteTree(_Node *root);
public:
    explicit SegmentTree(const std::vector<uint16_t>& seq) { _root = _BuildTree(seq, 0, seq.size() - 1); }
    ~SegmentTree() { _DeleteTree(_root); }

    uint32_t GetMaxSeq(uint32_t l, uint32_t r, uint32_t size) const;
};

// A recursive function to delete tree.
void SegmentTree::_DeleteTree(_Node *root) {
    if (root != nullptr) {
        _DeleteTree(root->left);
        _DeleteTree(root->right);

        delete root;
    }
}

SegmentTree::_Node* SegmentTree::_BuildTree(const std::vector<uint16_t>& seq, uint32_t l, uint32_t r) {
    if (l != r) {
        uint32_t m = (l + r) / 2;
        _Node *left_child;
        _Node *right_child;

        left_child = _BuildTree(seq, l, m);
        right_child = _BuildTree(seq, m + 1, r);

        _Node *new_node = new _Node();

        new_node->left = left_child, new_node->right = right_child;

        new_node->segment_len = left_child->segment_len + right_child->segment_len;
        new_node->prefix = (left_child->prefix == left_child->segment_len ? left_child->segment_len + right_child->prefix : left_child->prefix);
        new_node->suffix = (right_child->suffix == right_child->segment_len ? right_child->segment_len + left_child->suffix : right_child->suffix);

        new_node->max_seq = std::max(std::max(left_child->max_seq, right_child->max_seq), left_child->suffix + right_child->prefix);

        return new_node;
    } else {
        auto new_node = new _Node();

        if (seq[l]) {
            new_node->max_seq = 1, new_node->prefix = 1, new_node->suffix = 1;
        } else {
            new_node->max_seq = 0, new_node->prefix = 0, new_node->suffix = 0;
        }

        return new_node;
    }
}

// The function divides the origin segment on other segments while they will not give the origin segment in union
// and get answer.
SegmentTree::_Node* SegmentTree::_DivideSegment(SegmentTree::_Node *node, uint32_t l, uint32_t r, uint32_t l_border, uint32_t r_border) const {
    if (l_border == l && r_border == r)
        return node;

    uint32_t m = (l_border + r_border) / 2;

    if (l_border <= l && r <= m) {
        return _DivideSegment(node->left, l, r, l_border, m);
    } else if (m + 1 <= l && r <= r_border) {
        return _DivideSegment(node->right, l, r, m + 1, r_border);
    } else {
        _Node *left_child = _DivideSegment(node->left, l, m, l_border, m);;
        _Node *right_child = _DivideSegment(node->right, m + 1, r, m + 1, r_border);

        _Node *new_node = new _Node();

        new_node->segment_len = left_child->segment_len + right_child->segment_len;

        new_node->prefix = (left_child->prefix == left_child->segment_len ? left_child->segment_len + right_child->prefix : left_child->prefix);
        new_node->suffix = (right_child->suffix == right_child->segment_len ? right_child->segment_len + left_child->suffix : right_child->suffix);

        new_node->max_seq = std::max(std::max(left_child->max_seq, right_child->max_seq), left_child->suffix + right_child->prefix);

        return new_node;
    }
}

uint32_t SegmentTree::GetMaxSeq(uint32_t l, uint32_t r, uint32_t size) const {
    return _DivideSegment(_root, l, r, 0, size - 1)->max_seq;
}


int main() {
    uint32_t n = 0, m = 0;
    std::cin >> n >> m;

    std::vector<uint16_t> sequence(n, 0);
    for (uint32_t i = 0; i < n; ++i) {
        std::cin >> sequence[i];
    }

    SegmentTree segment_tree(sequence);

    for (uint32_t i = 0; i < m; ++i) {
        uint32_t l = 0, r = 0;
        std::cin >> l >> r;
        std::cout << segment_tree.GetMaxSeq(l, r, n) << std::endl;
    }

    return 0;
}

/*
IN
10 4
0 1 0 1 1 1 1 0 1 1
2 2
1 5
0 9
9 9
OUT
0
3
4
1
*/