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

void FindSubstr(const std::string& pattern) {
    // Calculate prefix function for the pattern.
    const std::vector<int32_t> pattern_pref_func = CalcPrefixFunc(pattern);
    char sym = 0;
    int32_t cur_pos = 0, previous_pref_func_val = pattern_pref_func[pattern_pref_func.size() - 1];

    // Read another element, calculate its prefix function.
    while(std::cin >> sym) {
        previous_pref_func_val = CalcPrefixFunc(pattern, pattern_pref_func, previous_pref_func_val, sym);

        if (previous_pref_func_val == pattern.size() - 1)
            std::cout << cur_pos + 2 - pattern.size() << ' ';

        ++cur_pos;
    }
}

int main() {
    std::cin.tie(nullptr);

    std::string pattern;
    std::cin >> pattern;
    pattern += "#";

    FindSubstr(pattern);

    return 0;
}

/*
IN
abc
abcababc
OUT
0 5
*/