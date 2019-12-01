/*
Найдите все вхождения шаблона в строку. Длина шаблона – p, длина строки – n. Время O(n + p), доп. память – O(p).
p <= 30000, n <= 300000.
Использовать один из методов:
- С помощью префикс-функции;
- С помощью z-функции.

Формат ввода
Шаблон, символ перевода строки, строка.

Формат вывода
Позиции вхождения шаблона в строке.*/

#include <iostream>
#include <string>
#include <vector>
#include <iterator>



size_t CalcPrefixFunc(const std::string& s, const std::vector<size_t>& prefix_func,
                       size_t previous_pref_func_val, char sym) {
    while (previous_pref_func_val > 0 && s[previous_pref_func_val] != sym) {
        previous_pref_func_val = prefix_func[previous_pref_func_val - 1];
    }

    if (s[previous_pref_func_val] == sym) ++previous_pref_func_val;

    return previous_pref_func_val;
}

template <typename InIterT, typename OutIterT>
void FindSubstr(InIterT begin_in, InIterT end_in, OutIterT begin_out, const std::string& pattern) {
    // Calculate prefix function for the pattern.
    std::vector<size_t> pattern_pref_func(pattern.size(), 0);

    for (size_t i = 1; i < pattern.size(); ++i) {
        pattern_pref_func[i] = CalcPrefixFunc(pattern, pattern_pref_func, pattern_pref_func[i - 1], pattern[i]);
    }

    ++begin_in; // Skip '\n'.

    size_t cur_pos = 0;
    char sym = *begin_in;
    size_t previous_pref_func_val = pattern_pref_func[pattern_pref_func.size() - 1];

    // Read another element, calculate its prefix function.
    while(begin_in != end_in) {
        ++begin_in;

        previous_pref_func_val = CalcPrefixFunc(pattern, pattern_pref_func, previous_pref_func_val, sym);

        if (previous_pref_func_val == pattern.size() - 1) {
            *begin_out = cur_pos + 2 - pattern.size();
            ++begin_out;
        }

        ++cur_pos;
        sym = *begin_in;
    }
}

int main() {
    std::cin.tie(nullptr);
    noskipws(std::cin);

    std::string pattern;
    std::cin >> pattern;
    pattern += '#';

    std::istream_iterator<char> begin_in(std::cin);
    std::istream_iterator<char> end_in;
    std::ostream_iterator<size_t> begin_out(std::cout, " ");

    FindSubstr(begin_in, end_in, begin_out, pattern);

    return 0;
}

/*
IN
abc
abcababc
OUT
0 5
*/