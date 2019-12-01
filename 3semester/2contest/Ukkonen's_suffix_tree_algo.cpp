/* Даны строки s и t. Постройте сжатое суффиксное дерево, которое содержит все суффиксы строки s и строки t.
 * Найдите такое дерево, которое содержит минимальное количество вершин.
 *
 * Формат ввода
 * В первой строке записана строка s (1 ≤ |s| ≤ 10^5), последний символ строки равен `$',
 * остальные символы строки — маленькие латинские буквы.
 * Во второй строке записана строка t (1 ≤ |t| ≤ 10^5), последний символ строки равен `#',
 * остальные символы строки — маленькие латинские буквы.
 *
 * Формат вывода
 * Пронумеруйте вершины дерева от 0 до n-1 в порядке обхода в глубину,
 * обходя поддеревья в порядке лексикографической сортировки исходящих из вершины ребер.
 * Используйте ASCII-коды символов для определения их порядка.
 * В первой строке выведите целое число n — количество вершин дерева.
 * В следующих n-1 строках выведите описание вершин дерева, кроме корня, в порядке увеличения их номеров.
 *
 * Описание вершины дерева v состоит из четырех целых чисел:
 * p, w, lf, rg, где p (0 ≤ p < n, p ≠ v) — номер родителя текущей вершины,
 * w (0 ≤ w ≤ 1) — номер строки для определения подстроки на ребре.
 * Если w = 0, то на ребре, ведущем из p в v, написана подстрока s[lf … rg - 1] (0 ≤ lf < rg ≤ |s|).
 * Если w = 1, то на ребре, ведущем из p в v, написана подстрока t[lf … rg -1] (0 ≤ lf < rg ≤ |t|).*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <stack>



constexpr int32_t alphabet_size = 28;
constexpr int32_t leaf_end = std::numeric_limits<int32_t>::max();

class SuffixTree {
public:
    explicit SuffixTree(const std::string& str);

    void PrintTreeByDFS(const int32_t first_str_len, const int32_t second_str_len);
private:
    struct Node_ {
        explicit Node_(const bool flag, const int32_t start, const int32_t end);
        explicit Node_() : Node_(false, -1, -1) {}
        int32_t edge_length_(const int32_t cur_text_pos) const { return std::min(cur_text_pos + 1, end_) - start_; }

        // We won't store all edges inside nodes.
        // As every node has only one parent, we will store the edge to his parent.
        // [start_; end_) interval specifies this edge.
        int32_t start_, end_, suf_link_;

        bool from_second_str_;
        int32_t parent_order_pos_, order_pos_;  // They are necessary for printing tree.

        std::vector<int32_t> children_;
    };

    std::vector<Node_> suffix_tree_;
    const int32_t root_ = 0;    // Because root node will be placed there.
    int32_t active_node_, active_edge_, active_len_;
    int32_t need_suf_link_, remainder_;
    int32_t last_added_node_pos_;

    // A wrapper on node addition.
    int32_t add_new_node_(const int32_t delimiter_pos, const int32_t start, const int32_t end);

    static int32_t get_symbol_order_pos_(char symbol);

    void add_suf_link_(const int32_t node_pos);
    bool walk_down_(const int32_t cur_text_pos, const int32_t node_pos);
    void build_tree_(const std::string& str);
};

SuffixTree::Node_::Node_(const bool flag, const int32_t start, const int32_t end = leaf_end) {
    start_ = start, end_ = end, suf_link_ = 0;
    from_second_str_ = flag;
    parent_order_pos_ = order_pos_ = 0;

    children_.resize(alphabet_size, 0);
}

int32_t SuffixTree::add_new_node_(const int32_t delimiter_pos, const int32_t start, const int32_t end = leaf_end) {
    bool from_second_str_ = true;
    if (start <= delimiter_pos) from_second_str_ = false;

    suffix_tree_[++last_added_node_pos_] = Node_(from_second_str_, start, end);

    return last_added_node_pos_;
}

int32_t SuffixTree::get_symbol_order_pos_(char symbol) {
    if (symbol >= 'a') return symbol - 'a' + 2;

    if (symbol == '#') return 0;

    return 1;
}

SuffixTree::SuffixTree(const std::string& str) {
    suffix_tree_.resize(2 * str.size());
    last_added_node_pos_ = -1;
    add_new_node_(false, -1, -1);

    active_node_ = root_, active_edge_ = 0, active_len_ = 0;
    need_suf_link_ = 0, remainder_ = 0;

    build_tree_(str);
}

void SuffixTree::add_suf_link_(const int32_t node_pos) {
    if (need_suf_link_)
        suffix_tree_[need_suf_link_].suf_link_ = node_pos;

    need_suf_link_ = node_pos;
}

bool SuffixTree::walk_down_(const int32_t cur_text_pos, const int32_t node_pos) {
    int32_t edge_length = suffix_tree_[node_pos].edge_length_(cur_text_pos);

    if (active_len_ >= edge_length) {
        active_len_ -= edge_length;
        active_edge_ += edge_length;
        active_node_ = node_pos;

        return true;
    }

    return false;
}

void SuffixTree::build_tree_(const std::string& str) {
    int32_t str_len = str.size();
    int32_t delimiter_pos = leaf_end;

    for (int32_t i = 0; i < str_len; ++i) {
        need_suf_link_ = 0;
        ++remainder_;

        while (remainder_) {
            // Update current symbol (at the beginning we always start from str[i]).
            if (active_len_ == 0) active_edge_ = i;
            int32_t symbol_order_pos = get_symbol_order_pos_(str[active_edge_]);

            // Here we have two cases: 1) add leaf to the active node, 2) add new node (split the edge).
            if (!suffix_tree_[active_node_].children_[symbol_order_pos]) {
                int32_t new_leaf = add_new_node_(delimiter_pos, i);
                suffix_tree_[active_node_].children_[symbol_order_pos] = new_leaf;

                add_suf_link_(active_node_);
            } else {
                int32_t active_child = suffix_tree_[active_node_].children_[symbol_order_pos];
                // If active edge is too short, we have to go further.
                if (walk_down_(i, active_child)) continue;

                // If the symbol has already been here, we add it implicitly.
                if (str[suffix_tree_[active_child].start_ + active_len_] == str[i]) {
                    ++active_len_;
                    add_suf_link_(active_node_);

                    break;
                }

                // Otherwise we split the edge.
                int32_t node_by_splitting = add_new_node_(
                        delimiter_pos,
                        suffix_tree_[active_child].start_,
                        suffix_tree_[active_child].start_ + active_len_);
                suffix_tree_[active_node_].children_[symbol_order_pos] = node_by_splitting;

                int32_t new_leaf = add_new_node_(delimiter_pos, i);
                char continuing_symbol = str[i];
                suffix_tree_[node_by_splitting].children_[get_symbol_order_pos_(continuing_symbol)] = new_leaf;


                suffix_tree_[active_child].start_ += active_len_;
                continuing_symbol = str[suffix_tree_[active_child].start_];
                suffix_tree_[node_by_splitting].children_[get_symbol_order_pos_(continuing_symbol)] = active_child;

                add_suf_link_(node_by_splitting);
            }

            --remainder_;
            // If active node == root, we shouldn't go further by suffix link. Otherwise, we should.
            if (active_node_ == root_ && active_len_) {
                active_edge_ = i - remainder_ + 1;
                --active_len_;
            } else {
                active_node_ = suffix_tree_[active_node_].suf_link_ ? suffix_tree_[active_node_].suf_link_ : root_;
            }
        }

        if (str[i] == '$') delimiter_pos = i;   // Update membership to the second sting.
    }
}

void SuffixTree::PrintTreeByDFS(const int32_t first_str_len, const int32_t second_str_len) {
    std::cout << last_added_node_pos_ + 1 << std::endl;

    std::stack<int32_t> stack;

    stack.push(root_);
    int32_t order_number = 0;
    while (!stack.empty()) {
        int32_t cur_node_pos = stack.top();
        stack.pop();

        if (cur_node_pos) {
            ++order_number;
            suffix_tree_[cur_node_pos].order_pos_ = order_number;
            // Print the current node.
            printf("%d %d ", suffix_tree_[cur_node_pos].parent_order_pos_, suffix_tree_[cur_node_pos].from_second_str_);

            // Fix borders: if current suffix starts at a position, which is less then delimiter_pos,
            // we crop the right border, otherwise we cut the left border.
            if (!suffix_tree_[cur_node_pos].from_second_str_) {
                if (suffix_tree_[cur_node_pos].end_ >= first_str_len)
                    suffix_tree_[cur_node_pos].end_ = first_str_len;
            } else {
                if (suffix_tree_[cur_node_pos].end_ == leaf_end)
                    suffix_tree_[cur_node_pos].end_ = first_str_len + second_str_len;

                suffix_tree_[cur_node_pos].end_ -= first_str_len;
                suffix_tree_[cur_node_pos].start_ -= first_str_len;
            }
            printf("%d %d\n", suffix_tree_[cur_node_pos].start_, suffix_tree_[cur_node_pos].end_);
        }

        for (int32_t i = alphabet_size - 1; i >= 0; --i) {
            int32_t child = suffix_tree_[cur_node_pos].children_[i];

            if (child) {
                suffix_tree_[child].parent_order_pos_ = suffix_tree_[cur_node_pos].order_pos_;

                stack.push(child);
            }
        }
    }
}

int main() {
    std::cin.tie(nullptr);

    std::string string1, string2;
    std::cin >> string1 >> string2;
    std::string string = string1 + string2;

    SuffixTree suf_tree(string);
    suf_tree.PrintTreeByDFS(string1.size(), string2.size());

    return 0;
}

/*
IN
ab$
ac#
OUT
8
0 1 2 3
0 0 2 3
0 0 0 1
3 0 1 3
3 1 1 3
0 0 1 3
0 1 1 3
--------
IN
aba$
baab#
OUT
14
0 1 4 5
0 0 3 4
0 0 0 1
3 0 3 4
3 1 2 5
3 0 1 2
6 1 4 5
6 0 2 4
0 0 1 2
9 1 4 5
9 0 2 3
11 0 3 4
11 1 2 5*/