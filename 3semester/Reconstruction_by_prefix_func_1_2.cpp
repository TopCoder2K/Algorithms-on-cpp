/*
Найти лексикографически-минимальную строку, построенную по префикс-функции, в алфавите a-z.*/

#include <iostream>
#include <vector>
#include <cassert>
#include <bitset>
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

char FindMinCharacter(const std::string& answer, const std::vector<size_t>& prefix_func) {
    if (prefix_func.empty())
        throw std::runtime_error("Non valid prefix function!");

    if (prefix_func.size() == 1) return 'a';

    std::bitset<alphabet_size> alphabet(std::numeric_limits<size_t>::max());
    size_t position = prefix_func[prefix_func.size() - 2];

    while(position >= 0) {
        alphabet[static_cast<size_t>(answer[position]) - 'a'] = false;

        if (!position) break;

        position = prefix_func[position - 1];
    }

    size_t first_non_zero_bit = FirstNonZeroBit(alphabet);
    if (alphabet.size() == first_non_zero_bit)
        throw std::runtime_error("Non valid prefix function!");

    return static_cast<char>(first_non_zero_bit + 'a');
}

template <typename InIterT, typename OutIterT>
void ReconstructionByPrefix(InIterT begin_in, InIterT end_in, OutIterT begin_out) {
    std::vector<size_t> prefix_func;
    std::string answer;

    size_t cur_value = 0;
    while(begin_in != end_in) {
        cur_value = *begin_in;
        ++begin_in;

        prefix_func.push_back(cur_value);

        if (cur_value > 0) {
            answer.push_back(answer[cur_value - 1]);
        } else {
            answer.push_back(FindMinCharacter(answer, prefix_func));
        }

        *begin_out = answer[answer.size() - 1];
        ++begin_out;
    }
}

int main() {
    std::istream_iterator<size_t> begin_in(std::cin);
    std::istream_iterator<size_t> end_in;
    std::ostream_iterator<char> begin_out(std::cout);

    ReconstructionByPrefix(begin_in, end_in, begin_out);

    return 0;
}

/*
IN
0 1 2 3 0
OUT
aaaab
-------------
IN
0 0 1 2 0 1 2 3
ababbaba*/