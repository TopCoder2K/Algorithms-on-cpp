/*
Найти лексикографически-минимальную строку, построенную по z-функции, в алфавите a-z.*/

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>

class Node {
public:
    std::unordered_set<char> forbidden_chars;
    std::vector<int32_t> move_to_prohibit;
    char character;

    Node() : character(0) {
        forbidden_chars.insert('a');
    }
    ~Node() = default;
};

char AddCharacter(const std::unordered_set<char>& forbidden_chars) {
    for (int32_t i = 0; i < 26; ++i) {
        if (!forbidden_chars.count(static_cast<char>(i + 97)))
            return static_cast<char>(i + 97);
    }

    return 'z';
}

void ReconstructionByZ(std::vector<Node>& answer) {
    // Initialization.
    int32_t cur_z_value = 0, prefix_length = 0, prefix_remaining_length = 0;
    answer[0].character = 'a';
    int32_t counter = 0;
    std::cin >> cur_z_value;

    while(std::cin >> cur_z_value && cur_z_value != -1) {
        ++counter;

        // Update coping if longer prefix is found.
        if (cur_z_value > prefix_remaining_length) {
            prefix_remaining_length = prefix_length = cur_z_value;
        }

        // In case prefix_remaining_length > 0 we just copy, otherwise we should analyse what symbol will be put.
        if (prefix_remaining_length) {
            // Copy the symbol.
            answer[counter] = answer[prefix_length - prefix_remaining_length];
            --prefix_remaining_length;

            // Check if there are some symbols to prohibit.
            for (int32_t j = 0; j < answer[counter].move_to_prohibit.size(); ++j) {
                int32_t to = answer[counter].move_to_prohibit[j];
                if (to < answer.size())
                    answer[to].forbidden_chars.insert(answer[counter].character);
            }

            // In case we meet cur_z_value that is greater than we do not know about answer[cur_z_value].character.
            // So, we have to indicate that later we will prohibit the character.
            if (counter < cur_z_value) {
                answer[cur_z_value].move_to_prohibit.push_back(counter + cur_z_value);
            } else if (counter + cur_z_value < answer.size()) {        // Otherwise, we just prohibit the character.
                answer[counter + cur_z_value].forbidden_chars.insert(answer[cur_z_value].character);
            }
        } else {
            answer[counter].character = AddCharacter(answer[counter].forbidden_chars);
        }
    }
}

int main() {
    // We have to take into account that zero element is string length.
    int32_t str_size = 1000;
    // std::cin >> str_size;

    std::vector<Node> answer(str_size);

    ReconstructionByZ(answer);

    for (int32_t i = 0; i < str_size && answer[i].character; ++i) {
        std::cout << answer[i].character;
    }

    return 0;
}

/*
IN
5 3 2 1 0
OUT
aaaab
----------------
IN
8 0 0 2 0 3 0 0
OUT
abbababb
*/