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

std::vector<int32_t> CalcPrefixFunc(const std::string& s) {
    std::size_t n = s.size();

    std::vector<int32_t> prefix_func(n, 0);

    for (int32_t i = 1; i < n; ++i) {
        int32_t j = prefix_func[i - 1];

        while (j > 0 && s[j] != s[i]) {
            j = prefix_func[j - 1];
        }

        if (s[j] == s[i]) ++j;

        prefix_func[i] = j;
    }

    return prefix_func;
}

int32_t CalcPrefixFunc(const std::string& s, const std::vector<int32_t>& prefix_func,
                       int32_t previous_pref_func_val, char sym) {
    while (previous_pref_func_val > 0 && s[previous_pref_func_val] != sym) {
        previous_pref_func_val = prefix_func[previous_pref_func_val - 1];
    }

    if (s[previous_pref_func_val] == sym) ++previous_pref_func_val;

    return previous_pref_func_val;
}

template <typename in_iter_t, typename out_iter_t>
void FindSubstr(in_iter_t start_in, in_iter_t end_in, out_iter_t start_out) {
    std::string pattern;
    char sym = 0;

    // Read pattern.
    while(start_in != end_in) {
        sym = *start_in;
        ++start_in;

        if (sym == '\n') break;

        pattern += sym;
    }
    pattern += '#';

    // Calculate prefix function for the pattern.
    const std::vector<int32_t> pattern_pref_func = CalcPrefixFunc(pattern);
    int32_t cur_pos = 0;
    int32_t previous_pref_func_val = pattern_pref_func[pattern_pref_func.size() - 1];

    // Read another element, calculate its prefix function.
    while(start_in != end_in) {
        sym = *start_in;
        ++start_in;

        if (sym == '\n') break;

        previous_pref_func_val = CalcPrefixFunc(pattern, pattern_pref_func, previous_pref_func_val, sym);

        if (previous_pref_func_val == pattern.size() - 1) {
            *start_out = cur_pos + 2 - pattern.size();
            ++start_out;
        }

        ++cur_pos;
    }
}

int main() {
    std::cin.tie(nullptr);
    noskipws(std::cin);

    std::istream_iterator<char> start_in(std::cin);
    std::istream_iterator<char> end_in;
    std::ostream_iterator<int32_t> start_out(std::cout, " ");

    FindSubstr(start_in, end_in, start_out);

    return 0;
}

/*
IN
abc
abcababc
OUT
0 5
*/