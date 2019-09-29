/*
Найти лексикографически-минимальную строку, построенную по префикс-функции, в алфавите a-z.*/

#include <iostream>
#include <vector>

char AddCharacter(std::string& answer, const std::vector<int32_t>& prefix_fuc) {
    std::vector<bool> alphabet(26,true);
    // alphabet[0] = false;

    int32_t position = prefix_fuc.size() > 1 ? prefix_fuc[prefix_fuc.size() - 2] : -1;
    while(position >= 0) {
        alphabet[static_cast<int32_t>(answer[position]) - 97] = false;

        if (position) {
            position = prefix_fuc[position - 1];
        } else {
            break;
        }
    }

    for(int32_t i = 0; i < alphabet.size(); ++i) {
        if (alphabet[i])
            return static_cast<char>(97 + i);
    }
}

void ReconstructionByPrefix(std::string& answer) {
    std::vector<int32_t> prefix_fuc;

    int32_t cur_value;
    while(std::cin >> cur_value && cur_value != -1) {
        prefix_fuc.push_back(cur_value);

        if (cur_value > 0) {
            answer.push_back(answer[cur_value - 1]);
        } else {
            answer.push_back(AddCharacter(answer, prefix_fuc));
        }
    }

}

int main() {
    std::string answer;

    ReconstructionByPrefix(answer);

    std::cout << answer;

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