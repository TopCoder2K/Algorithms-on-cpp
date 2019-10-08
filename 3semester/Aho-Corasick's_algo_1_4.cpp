/*
 * Шаблон поиска задан строкой длины m, в которой кроме обычных символов могут встречаться символы “?”.
 * Найти позиции всех вхождений шаблона в тексте длины n. Каждое вхождение шаблона предполагает,
 * что все обычные символы совпадают с соответствующими из текста,
 * а вместо символа “?” в тексте встречается произвольный символ.
 * Время работы - O(n + m + Z), где Z - общее -число вхождений подстрок шаблона “между вопросиками” в исходном тексте.
 * m ≤ 5000, n ≤ 2000000.*/

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <string_view>
//#pragma GCC optimize("Ofast,unroll-all-loops")
//#pragma GCC optimize("-O3")

constexpr int32_t alphabet_size = 26;

class Trie {
public:
    class Node {
    public:
        std::vector<std::unique_ptr<Node>> children;
        std::vector<Node *> transition;
        std::vector<int32_t> pattern_substr_numbers;
        Node * const parent;
        const char char_to_parent;
        Node *up;
        Node *suff_link;

        explicit Node(Node *parent, const char edge) :
                parent(parent),
                char_to_parent(edge),
                up(nullptr),
                suff_link(nullptr) {
            children.resize(alphabet_size);
            transition.resize(alphabet_size);
        }
        ~Node() = default;
    };

    explicit Trie(std::vector<std::basic_string_view<char>>& pattern_substrings) {
        root_ = std::make_unique<Node>(root_.get(), 'a');
        state_ = root_.get();

        for (int32_t i = 0; i < pattern_substrings.size(); ++i) {
            AddString(pattern_substrings[i], i);
        }
    }
    ~Trie() = default;

    Trie& operator=(const Trie& other) = delete;
    Trie& operator=(Trie&& other) = delete;
    Trie(const Trie& other) = delete;
    Trie(Trie&& other) = delete;

    Node* GetSuffLink(Node * const node);
    Node* GetLink(Node * const node, const char symbol_to_parent);
    Node* GetUp(Node * const node);
    void AddString(const std::basic_string_view<char>& str, const int32_t str_number);
    void NextState(const char sym, std::vector<int32_t>& pattern_positions, std::vector<int32_t>& substr_end_positions);

    Node* GetRoot() { return root_.get();}
private:
    std::unique_ptr<Node> root_;
    Node *state_;
};

Trie::Node* Trie::GetSuffLink(Trie::Node * const node) {
    if (!node->suff_link) {
        // If node is root child we also make suffix link is root, because empty suffixes are not considered.
        if (node == root_.get() || node->parent == root_.get()) {
            node->suff_link = root_.get();
        } else {
            node->suff_link = GetLink(GetSuffLink(node->parent), node->char_to_parent);
        }
    }

    return node->suff_link;
}

Trie::Node* Trie::GetLink(Trie::Node * const node, const char symbol_to_parent) {
    Node *transition_ptr = node->transition[symbol_to_parent - 'a'];

    if (!transition_ptr) {
        if (node->children[symbol_to_parent - 'a']) {
            transition_ptr = node->children[symbol_to_parent - 'a'].get();
        } else if (node == root_.get()) {
            transition_ptr = root_.get();
        } else {
            transition_ptr = GetLink(GetSuffLink(node), symbol_to_parent);
        }
    }

    return transition_ptr;
}

Trie::Node* Trie::GetUp(Trie::Node * const node) {
    Node *suff_link = GetSuffLink(node);

    if (!node->up) {
        if (!suff_link->pattern_substr_numbers.empty()) {
            node->up = suff_link;
        } else if (suff_link == root_.get()) {
            node->up = root_.get();
        } else {
            node->up = GetUp(suff_link);
        }
    }

    return node->up;
}

void Trie::AddString(const std::basic_string_view<char>& str, const int32_t str_number) {
    Node *cur_node = root_.get();

    for (int32_t i = 0; i < str.size(); ++i) {
        Node *child = nullptr;
        if (cur_node) child = cur_node->children[str[i] - 'a'].get();

        if (!child) {
            cur_node->children[str[i] - 'a'] = std::make_unique<Node>(cur_node, str[i]);
            child = cur_node->children[str[i] - 'a'].get();
        }

        cur_node = child;
    }

    cur_node->pattern_substr_numbers.push_back(str_number);
}

void Trie::NextState(
        const char sym,
        std::vector<int32_t>& pattern_positions,
        std::vector<int32_t>& substr_end_positions
        ) {
    state_ = GetLink(state_, sym);
    Trie::Node *up = state_;

    while (up != root_.get()) {
        for (int32_t i = 0; i < up->pattern_substr_numbers.size(); ++i) {
            // Plus 1 because pattern substrings end positions are one more then their position in the pattern.
            int32_t pattern_pos = pattern_positions.size() - 1 - substr_end_positions[up->pattern_substr_numbers[i]] + 1;
            if (pattern_pos >= 0)
                ++pattern_positions[pattern_pos];
        }

        up = GetUp(up);
    }
}

//Aho-Corasick's algorithm.
std::vector<int32_t> FindAllEntries (Trie& trie, std::vector<int32_t>& substr_end_positions) {
    std::vector<int32_t> pattern_positions;
    char sym = 0;

    while (std::cin >> sym) {
        if (sym == '_')
            break;

        pattern_positions.push_back(0);

        trie.NextState(sym, pattern_positions, substr_end_positions);
    }

    return pattern_positions;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::vector<std::basic_string_view<char>> pattern_substrings;
    std::vector<int32_t> substr_end_position;

    char sym = 0;
    int32_t counter = 0;
    std::string pattern, cur_str;

    std::cin >> pattern;

    while (counter <= pattern.size()) {
        cur_str.clear();
        sym = pattern[counter];
        ++counter;

        while (sym != '?' && counter <= pattern.size()) {
            cur_str += sym;
            sym = pattern[counter];
            ++counter;
        }

        if (!cur_str.empty()) {
            std::basic_string_view<char> tmp(pattern.c_str() + counter - 1 - cur_str.size(), cur_str.size());
            pattern_substrings.push_back(tmp);
            substr_end_position.push_back(counter - 1);
        }
    }

    Trie trie(pattern_substrings);
    // pattern_positions[i] — the number of encountered in text pattern substrings
    // that begins in the text at i position.
    std::vector<int32_t> pattern_positions = FindAllEntries(trie, substr_end_position);

    for (int i = 0; i < pattern_positions.size(); ++i) {
        if (pattern_positions[i] == pattern_substrings.size() &&
            pattern_positions.size() >= i + counter - 1)
            std::cout << i << ' ';
    }

    return 0;
}

/*
IN
ab??aba
ababacaba
OUT
2
----------------------
IN
aa??bab?cbaa?
aabbbabbcbaabaabbbabbcbaab
OUT
0 13
----------------------
IN
??abc??
abcabcabc
OUT
3
----------------------
IN
???
abcabc
OUT*/