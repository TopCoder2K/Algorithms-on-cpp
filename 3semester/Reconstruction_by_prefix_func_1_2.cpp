/*
Найти лексикографически-минимальную строку, построенную по префикс-функции, в алфавите a-z.*/

#include <iostream>
#include <vector>
#include <cassert>
#include <bitset>
#include <iterator>
#define INT_MAX 2147483647

const uint32_t alphabet_size = 26;

char FirstNonZeroBit(std::bitset<alphabet_size>& alphabet) {
    for(int32_t i = 0; i < alphabet.size(); ++i) {
        if (alphabet[i])
            return static_cast<char>('a' + i);
    }

    throw std::runtime_error("Not valid prefix function.");
}

char FindMinCharacter(const std::string& answer, const std::vector<int32_t>& prefix_func) {
    assert(!prefix_func.empty());

    if (prefix_func.size() == 1) return 'a';

    std::bitset<alphabet_size> alphabet(INT_MAX);
    int32_t position = prefix_func[prefix_func.size() - 2];

    while(position >= 0) {
        alphabet[static_cast<int32_t>(answer[position]) - 'a'] = false;

        if (!position) break;

        position = prefix_func[position - 1];
    }

    return FirstNonZeroBit(alphabet);
}

template <typename in_iter_t, typename out_iter_t>
void ReconstructionByPrefix(in_iter_t start_in, in_iter_t end_in, out_iter_t start_out) {
    std::vector<int32_t> prefix_func;
    std::string answer;

    int32_t cur_value = 0;
    while(start_in != end_in) {
        cur_value = *start_in;
        ++start_in;

        cur_value -= '0';

        prefix_func.push_back(cur_value);

        if (cur_value > 0) {
            answer.push_back(answer[cur_value - 1]);
        } else {
            answer.push_back(FindMinCharacter(answer, prefix_func));
        }
    }

    for (int32_t i = 0; i < answer.size(); ++i) {
        *start_out = answer[i];
        ++start_out;
    }
}

int main() {
    std::istream_iterator<char> start_in(std::cin);
    std::istream_iterator<char> end_in;
    std::ostream_iterator<char> start_out(std::cout);

    ReconstructionByPrefix(start_in, end_in, start_out);

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