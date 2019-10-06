/*
Найти лексикографически-минимальную строку, построенную по z-функции, в алфавите a-z.*/

#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <stdexcept>
#include <iterator>

constexpr uint32_t alphabet_size = 26;

template <size_t n>
size_t FirstNonZeroBit(std::bitset<n>& alphabet) {
    for(size_t i = 0; i < alphabet.size(); ++i) {
        if (alphabet[i])
            return i;
    }

    return n;
}

char FindMinCharacter(const std::string& answer, const std::vector<size_t>& forbidden_chars) {
    std::bitset<alphabet_size> alphabet(std::numeric_limits<size_t>::max());

    for (size_t i = 0; i < forbidden_chars.size(); ++i) {
        size_t str_pos = forbidden_chars[i];
        alphabet[answer[str_pos] - 'a'] = false;
    }

    size_t first_non_zero_bit = FirstNonZeroBit(alphabet);

    if (first_non_zero_bit == alphabet.size())
        throw std::runtime_error("Not valid z-function.");

    return static_cast<char>(first_non_zero_bit + 'a');
}

template <typename InIterT, typename OutIterT>
void ReconstructionByZ(InIterT begin_in, InIterT end_in, OutIterT begin_out) {
    // Initialization.
    std::string answer;
    std::vector<size_t> forbidden_chars;

    size_t cur_z_value = 0;
    size_t prefix_length = 0;
    size_t prefix_remaining_length = 0;

    answer += "a";
    forbidden_chars.push_back(0);
    ++begin_in; // Skip first value.
    *begin_out = 'a';
    ++begin_out;

    while(begin_in != end_in) {
        cur_z_value = *begin_in;
        ++begin_in;

        // Update copying if longer prefix is found.
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

        // Print answer.
        *begin_out = answer[answer.size() - 1];
        ++begin_out;
    }
}

int main() {
    std::istream_iterator<size_t> begin_in(std::cin);
    std::istream_iterator<size_t> end_in;
    std::ostream_iterator<char> begin_out(std::cout);

    ReconstructionByZ(begin_in, end_in, begin_out);

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