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
#include <stack>

const int32_t alphabet_size = 26;

class SymbolTree {
public:
    class Node {
    public:
        std::vector<Node *> children;
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
            children.resize(alphabet_size, nullptr);
            transition.resize(alphabet_size, nullptr);
        }
        ~Node() = default;
    };

    explicit SymbolTree(std::vector<std::string>& pattern_substrings) {
        root_ = new Node(root_, 'a');

        for (int32_t i = 0; i < pattern_substrings.size(); ++i) {
            AddString(pattern_substrings[i], i);
        }
    }
    ~SymbolTree();

    SymbolTree& operator=(const SymbolTree& other) = delete;
    SymbolTree& operator=(SymbolTree&& other) = delete;
    SymbolTree(const SymbolTree& other) = delete;
    SymbolTree(SymbolTree&& other) = delete;

    Node* GetSuffLink(Node * const node);
    Node* GetLink(Node * const node, const char symbol_to_parent);
    Node* GetUp(Node * const node);
    void AddString(std::string& str, const int32_t str_number);

    Node* GetRoot() { return root_;}
private:
    Node *root_;
};

SymbolTree::~SymbolTree() {
    std::stack<Node*> stack;
    stack.push(root_);

    while (!stack.empty()) {
        Node *cur_node = stack.top();
        stack.pop();

        for (int32_t i = 0; i < alphabet_size; ++i) {
            if (cur_node->children[i])
                stack.push(cur_node->children[i]);
        }

        delete cur_node;
    }
}

SymbolTree::Node* SymbolTree::GetSuffLink(SymbolTree::Node * const node) {
    if (!node->suff_link) {
        // If node is root child we also make suffix link is root, because empty suffixes are not considered.
        if (node == root_ || node->parent == root_) {
            node->suff_link = root_;
        } else {
            node->suff_link = GetLink(GetSuffLink(node->parent), node->char_to_parent);
        }
    }

    return node->suff_link;
}

SymbolTree::Node* SymbolTree::GetLink(SymbolTree::Node * const node, const char symbol_to_parent) {
    Node *transition_ptr = node->transition[symbol_to_parent - 'a'];

    if (!transition_ptr) {
        if (node->children[symbol_to_parent - 'a']) {
            transition_ptr = node->children[symbol_to_parent - 'a'];
        } else if (node == root_) {
            transition_ptr = root_;
        } else {
            transition_ptr = GetLink(GetSuffLink(node), symbol_to_parent);
        }
    }

    return transition_ptr;
}

SymbolTree::Node* SymbolTree::GetUp(SymbolTree::Node * const node) {
    Node *suff_link = GetSuffLink(node);

    if (!node->up) {
        if (!suff_link->pattern_substr_numbers.empty()) {
            node->up = suff_link;
        } else if (suff_link == root_) {
            node->up = root_;
        } else {
            node->up = GetUp(suff_link);
        }
    }

    return node->up;
}

void SymbolTree::AddString(std::string& str, const int32_t str_number) {
    Node *cur_node = root_;

    for (int32_t i = 0; i < str.size(); ++i) {
        Node *child = cur_node->children[str[i] - 'a'];

        if (!child) {
            child = new Node(cur_node, str[i]);
            cur_node->children[str[i] - 'a'] = child;
        }

        cur_node = child;
    }

    cur_node->pattern_substr_numbers.push_back(str_number);
}

//Aho-Corasick's algorithm.
void FindAllEntries (
        SymbolTree& bor,
        std::vector<int32_t>& pattern_positions,
        std::vector<int32_t>& substr_end_positions
) {
    char sym = 0;
    int32_t cur_pos_in_text = -1;
    SymbolTree::Node *root = bor.GetRoot();
    SymbolTree::Node *cur_node = root;

    /*std::istreambuf_iterator<char> start(std::cin);
    std::istreambuf_iterator<char> end;
    ++start; // Skip ' ' or '\n' after pattern.

    while (sym != '\n' && start != end) {
        sym = *start;
        ++start;

        if (sym < 'a') break; // New line.

        ++cur_pos_in_text;*/
    while (std::cin >> sym) {
        ++cur_pos_in_text;
        if (sym == '_')
            break;

        pattern_positions.push_back(0);

        cur_node = bor.GetLink(cur_node, sym);
        SymbolTree::Node *up = cur_node;

        while (up != root) {
            for (int32_t i = 0; i < up->pattern_substr_numbers.size(); ++i) {
                // Plus 1 because pattern substrings end positions are one more then their position in the pattern.
                int32_t pattern_pos = cur_pos_in_text - substr_end_positions.at(up->pattern_substr_numbers.at(i)) + 1;
                if (pattern_pos >= 0 && pattern_pos < pattern_positions.size())
                    ++pattern_positions.at(pattern_pos);
            }

            up = bor.GetUp(up);
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::vector<std::string> pattern_substrings;
    std::vector<int32_t> substr_end_position;

    char sym = 0;
    int32_t counter = 0;
    std::string pattern, cur_str;

    std::cin >> pattern;

    while (counter < pattern.size()) {
        cur_str.clear();
        sym = pattern[counter];
        ++counter;

        while (sym != '?' && counter < pattern.size()) {
            cur_str += sym;
            sym = pattern[counter];
            ++counter;
        }

        if (!cur_str.empty()) {
            pattern_substrings.push_back(cur_str);
            substr_end_position.push_back(counter - 1);
        }
    }

    SymbolTree bor(pattern_substrings);
    // pattern_positions[i] — the number of encountered in text pattern substrings
    // that begins in the text at i position.
    std::vector<int32_t> pattern_positions;
    FindAllEntries(bor, pattern_positions, substr_end_position);

    for (int i = 0; i < pattern_positions.size(); ++i) {
        if (pattern_positions.at(i) == pattern_substrings.size() &&
            pattern_positions.size() > i + counter - 1)
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