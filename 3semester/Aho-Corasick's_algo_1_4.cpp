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

constexpr int32_t k_alphabet_size = 26;

class Trie {
    class Node_ {
    public:
        std::vector<std::unique_ptr<Node_>> children_;
        std::vector<Node_ *> transition_;
        std::vector<int32_t> pattern_substr_numbers_;
        Node_ * const parent_;
        const char char_to_parent_;
        Node_ *up_;
        Node_ *suff_link_;

        explicit Node_(Node_ *parent, const char edge) :
                parent_(parent),
                char_to_parent_(edge),
                up_(nullptr),
                suff_link_(nullptr) {
            children_.resize(k_alphabet_size);
            transition_.resize(k_alphabet_size);
        }
    };

    Node_* GetSuffLink_(Node_ * const node);
    Node_* GetLink_(Node_ * const node, const char symbol_to_parent);
    Node_* GetUp_(Node_ * const node);
    void AddString_(const std::basic_string_view<char>& str, const int32_t str_number);

public:
    explicit Trie(std::vector<std::basic_string_view<char>>& pattern_substrings) {
        root_ = std::make_unique<Node_>(root_.get(), 'a');
        state_ = root_.get();

        for (int32_t i = 0; i < pattern_substrings.size(); ++i) {
            AddString_(pattern_substrings[i], i);
        }
    }

    Trie& operator=(const Trie& other) = delete;
    Trie& operator=(Trie&& other) = delete;
    Trie(const Trie& other) = delete;
    Trie(Trie&& other) = delete;

    void NextState(
            const char sym,
            std::vector<int32_t>& pattern_positions,
            const std::vector<int32_t>& substr_end_positions
            );

private:
    std::unique_ptr<Node_> root_;
    Node_ *state_;
};

Trie::Node_* Trie::GetSuffLink_(Trie::Node_ * const node) {
    if (!node->suff_link_) {
        // If node is root child we also make suffix link is root, because empty suffixes are not considered.
        if (node == root_.get() || node->parent_ == root_.get()) {
            node->suff_link_ = root_.get();
        } else {
            node->suff_link_ = GetLink_(GetSuffLink_(node->parent_), node->char_to_parent_);
        }
    }

    return node->suff_link_;
}

Trie::Node_* Trie::GetLink_(Trie::Node_ * const node, const char symbol_to_parent) {
    Node_ *transition_ptr = node->transition_[symbol_to_parent - 'a'];

    if (!transition_ptr) {
        if (node->children_[symbol_to_parent - 'a']) {
            transition_ptr = node->children_[symbol_to_parent - 'a'].get();
        } else if (node == root_.get()) {
            transition_ptr = root_.get();
        } else {
            transition_ptr = GetLink_(GetSuffLink_(node), symbol_to_parent);
        }
    }

    return transition_ptr;
}

Trie::Node_* Trie::GetUp_(Trie::Node_ * const node) {
    Node_ *suff_link = GetSuffLink_(node);

    if (!node->up_) {
        if (!suff_link->pattern_substr_numbers_.empty()) {
            node->up_ = suff_link;
        } else if (suff_link == root_.get()) {
            node->up_ = root_.get();
        } else {
            node->up_ = GetUp_(suff_link);
        }
    }

    return node->up_;
}

void Trie::AddString_(const std::basic_string_view<char>& str, const int32_t str_number) {
    Node_ *cur_node = root_.get();

    for (int32_t i = 0; i < str.size(); ++i) {
        Node_ *child = nullptr;
        if (cur_node) child = cur_node->children_[str[i] - 'a'].get();

        if (!child) {
            cur_node->children_[str[i] - 'a'] = std::make_unique<Node_>(cur_node, str[i]);
            child = cur_node->children_[str[i] - 'a'].get();
        }

        cur_node = child;
    }

    cur_node->pattern_substr_numbers_.push_back(str_number);
}

void Trie::NextState(
        const char sym,
        std::vector<int32_t>& pattern_positions,
        const std::vector<int32_t>& substr_end_positions
        ) {
    state_ = GetLink_(state_, sym);
    Trie::Node_ *up = state_;

    while (up != root_.get()) {
        for (int32_t i = 0; i < up->pattern_substr_numbers_.size(); ++i) {
            // Plus 1 because pattern substrings end positions are one more then their position in the pattern.
            int32_t pattern_pos = pattern_positions.size() - 1 - substr_end_positions[up->pattern_substr_numbers_[i]] + 1;
            if (pattern_pos >= 0)
                ++pattern_positions[pattern_pos];
        }

        up = GetUp_(up);
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
OUT
0 1 2 3*/