/*
Найти лексикографически-минимальную строку, построенную по префикс-функции, в алфавите a-z.*/

#include <iostream>
#include <vector>
#include <cassert>
#include <bitset>
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

        if (position) {
            position = prefix_func[position - 1];
        } else {
            break;
        }
    }

    return FirstNonZeroBit(alphabet);
}

template <typename T>
void ReconstructionByPrefix(std::istreambuf_iterator<T> start, std::istreambuf_iterator<T> end) {
    std::vector<int32_t> prefix_func;
    std::string answer;

    int32_t cur_value = 0;
    while(cur_value != '\n' && start != end) {
        cur_value = *start;
        ++start;

        if (cur_value < '0') continue; // Whitespaces.

        cur_value -= '0';

        prefix_func.push_back(cur_value);

        if (cur_value > 0) {
            answer.push_back(answer[cur_value - 1]);
        } else {
            answer.push_back(FindMinCharacter(answer, prefix_func));
        }
    }

    std::cout << answer;
}

int main() {
    std::istreambuf_iterator<char> start(std::cin);
    std::istreambuf_iterator<char> end;

    ReconstructionByPrefix(start, end);

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