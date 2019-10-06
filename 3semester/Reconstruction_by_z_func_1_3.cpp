/*
Найти лексикографически-минимальную строку, построенную по z-функции, в алфавите a-z.*/

#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <stdexcept>
#include <iterator>
#define INT_MAX 2147483647

const uint32_t alphabet_size = 26;

char FindMinCharacter(const std::string& answer, std::vector<int32_t> forbidden_chars) {
    std::bitset<alphabet_size> alphabet(INT_MAX);

    for (int32_t i = 0; i < forbidden_chars.size(); ++i) {
        int32_t str_pos = forbidden_chars[i];
        alphabet[answer[str_pos] - 'a'] = false;
    }

    for (int32_t i = 0; i < alphabet_size; ++i) {
        if (alphabet[i])
            return static_cast<char>(i + 'a');
    }

    throw std::runtime_error("Not valid z-function.");
}

template <typename in_iter_t, typename out_iter_t>
void ReconstructionByZ(in_iter_t start_in, in_iter_t end_in, out_iter_t start_out) {
    // Initialization.
    std::string answer;
    std::vector<int32_t> forbidden_chars;

    int32_t cur_z_value = 0;
    int32_t prefix_length = 0;
    int32_t prefix_remaining_length = 0;
    int32_t cur_pos = 0;

    answer += "a";
    forbidden_chars.push_back(0);
    ++start_in; // Skip first value.

    while(start_in != end_in) {
        cur_z_value = *start_in;
        ++start_in;
        ++cur_pos;

        // Update coping if longer prefix is found.
        if (cur_z_value > prefix_remaining_length) {
            prefix_remaining_length = prefix_length = cur_z_value;
            forbidden_chars.clear();
            forbidden_chars.push_back(0);
        }

        // In case prefix_remaining_length > 0 we just copy, otherwise we should analyse what symbol will be put.
        if (prefix_remaining_length) {
            // Copy the symbol.
            answer += answer[prefix_length - prefix_remaining_length];
            --prefix_remaining_length;

            // If current z_value gives prefix that ends in cur_pos + prefix remaining length - 1,
            // we have to prohibit the appropriate character.
            if (cur_z_value == prefix_remaining_length + 1)
                forbidden_chars.push_back(cur_z_value);
        } else {
            answer += FindMinCharacter(answer, forbidden_chars);
            forbidden_chars.clear();
            forbidden_chars.push_back(0);
        }
    }

    for (int32_t i = 0; i < answer.size(); ++i) {
        *start_out = answer[i];
        ++start_out;
    }
}

int main() {
    std::istream_iterator<int32_t> start_in(std::cin);
    std::istream_iterator<int32_t> end_in;
    std::ostream_iterator<char> start_out(std::cout);

    ReconstructionByZ(start_in, end_in, start_out);

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