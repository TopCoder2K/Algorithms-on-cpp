/*Дана строка длины n. Найти количество ее различных подстрок. Используйте суффиксный массив.
Построение суффиксного массива выполняйте за O(n log n). Вычисление количества различных подстрок выполняйте за O(n).

Формат ввода
Строка.

Формат вывода
Число - количество различных подстрок.*/

#include <iostream>
#include <vector>
#include <string>

constexpr int32_t k_alphabet_size = 256;

// Returns indices of sorted array.
std::vector<int32_t> CountSort(const std::string& string_to_sort) {
    std::vector<int32_t> sorted_array(string_to_sort.size());
    std::vector<int32_t> bucket(k_alphabet_size, 0);
    int32_t str_size = string_to_sort.size();

    for (int32_t i = 0; i < str_size; ++i) {
        ++bucket[string_to_sort[i]];
    }

    for (int32_t i = 1; i < k_alphabet_size; ++i) {
        bucket[i] += bucket[i - 1];
    }

    for (int32_t i = str_size - 1; i >= 0; --i) {
        char cur_sym = string_to_sort[i];

        sorted_array[--bucket[cur_sym]] = i;
    }

    return sorted_array;
}

int32_t max(int32_t x, int32_t y) {
    int32_t tmp = x;
    if (y > x) tmp = y;

    return tmp;
}

// -----------------------------------------------------------------------
class SuffixArray {
public:
    explicit SuffixArray(const std::string& str);

    std::vector<int32_t> CountSort(
            const std::vector<int32_t>& array_to_sort,
            const std::vector<int32_t>& alphabet_order,
            const int32_t alphabet_power);
    void MakeNewPermutations(uint32_t phase);
    void MakeNewClasses(uint32_t phase);
    int32_t GetSuffix(int32_t index) { return permutations_[index]; }
    // Returns longest common prefix for two adjacent suffixes in suffix array. lcp[i] = lcp(i, i - 1)??????????????????
    std::vector<int32_t> GetLCP(const std::string& str);

private:
    std::vector<int32_t> permutations_;
    std::vector<int32_t> classes_;
    int32_t number_of_classes_;
};

// Returns indices of sorted array.
std::vector<int32_t> SuffixArray::CountSort(
        const std::vector<int32_t>& array_to_sort,
        const std::vector<int32_t>& alphabet_order,
        const int32_t alphabet_power) {
    std::vector<int32_t> sorted_array(array_to_sort.size());
    std::vector<int32_t> bucket(alphabet_power, 0);

    for (int32_t i = 0; i < array_to_sort.size(); ++i) {
        int32_t cur_element = array_to_sort[i];

        ++bucket[alphabet_order[cur_element]];
    }

    for (int32_t i = 1; i < alphabet_power; ++i) {
        bucket[i] += bucket[i - 1];
    }

    for (int32_t i = array_to_sort.size() - 1; i >= 0; --i) {
        int32_t cur_element_order = alphabet_order[array_to_sort[i]];

        sorted_array[--bucket[cur_element_order]] = i;
    }

    return sorted_array;
}

void SuffixArray::MakeNewPermutations(uint32_t phase) {
    std::vector<int32_t> next_permutations(permutations_);
    int32_t n = permutations_.size();

    // "Sorting" the second component of pairs.
    for (int32_t i = 0; i < n; ++i) {
        next_permutations[i] = permutations_[i] - (1u << phase);

        if (next_permutations[i] < 0)  next_permutations[i] += n;
    }

    std::vector<int32_t> permutations_indices = CountSort(next_permutations, classes_, number_of_classes_);
    for (int32_t i = 0; i < n; ++i) {
        permutations_[i] = next_permutations[permutations_indices[i]];
    }
}

void SuffixArray::MakeNewClasses(uint32_t phase) {
    int32_t p_size = permutations_.size();
    std::vector<int32_t> new_classes(classes_.size());

    new_classes[permutations_[0]] = 0;
    number_of_classes_ = 1;
    for (int32_t i = 1; i < p_size; ++i) {
        int32_t middle_1 = (permutations_[i - 1] + (1u << phase)) % p_size;
        int32_t middle_2 = (permutations_[i] + (1u << phase)) % p_size;

        if (classes_[permutations_[i]] != classes_[permutations_[i - 1]] || classes_[middle_1] != classes_[middle_2])
            ++number_of_classes_;
        new_classes[permutations_[i]] = number_of_classes_ - 1;
    }

    classes_ = new_classes;
}

SuffixArray::SuffixArray(const std::string& str) {
    // Build first permutations and classes.
    // Zero element in permutations is always end symbol, which is smaller than all letters.
    std::vector<int32_t> tmp_vec = ::CountSort(str);

    permutations_.resize(tmp_vec.size() + 1);
    const int32_t size = permutations_.size();
    permutations_[0] = str.size();
    for (int32_t i = 0; i < tmp_vec.size(); ++i) {
        permutations_[i + 1] = tmp_vec[i];
    }

    classes_.resize(size);
    classes_[permutations_[0]] = 0;
    number_of_classes_ = 1;
    for (int32_t i = 1; i < size; ++i) {
        if (str[permutations_[i]] != str[permutations_[i - 1]])
            ++number_of_classes_;

        classes_[permutations_[i]] = number_of_classes_ - 1;
    }

    for (uint32_t phase = 0; (1u << phase) < size; ++phase) {
        MakeNewPermutations(phase);
        MakeNewClasses(phase);
    }
}

std::vector<int32_t> SuffixArray::GetLCP(const std::string& str) {
    int32_t size = permutations_.size();
    std::vector<int32_t> lcp(size, 0);
    std::vector<int32_t> inverse_suf_arr(size, 0);

    for (int32_t i = 0; i < size; ++i) {
        inverse_suf_arr[permutations_[i]] = i;
    }

    // There is always end symbol on the zero position in the permutations_, so let's skip it.
    int32_t common_suf_len = 0;
    for (int32_t i = 0; i < size - 1; ++i) {
        if (common_suf_len) --common_suf_len;

        // Case for the last suffix in lexicographical order.
        if (inverse_suf_arr[i] == size - 1) {
            common_suf_len = 0;
            continue;
        }

        int32_t next_in_suf_arr = permutations_[inverse_suf_arr[i] + 1];
        while (max(i + common_suf_len, next_in_suf_arr + common_suf_len) < size &&
                str[i + common_suf_len] == str[next_in_suf_arr + common_suf_len]) {
            ++common_suf_len;
        }
        lcp[inverse_suf_arr[i]] = common_suf_len;
    }

    return lcp;
}

// -----------------------------------------------------------------------

int32_t GetDifferentSubstrings(const std::string& string) {
    SuffixArray suf_arr(string);
    std::vector<int32_t> lcp = suf_arr.GetLCP(string);
    int32_t n = string.size();
    int32_t substrings = 0;

    // There is always end symbol on the zero position in the permutations_, so let's skip it.
    for (int32_t i = 1; i <= n; ++i) {
        // We don't add 1 to parenthesized expression, because last element is end element,
        // so (n + 1 - 1 - (suf_arr[i] - 1) + 1) = (n - suf_arr[i]).
        substrings += (n - suf_arr.GetSuffix(i)) - lcp[i];
    }

    return substrings;
}

int main() {
    std::string string;
    std::cin >> string;
    std::cout << GetDifferentSubstrings(string);

    return 0;
}

/*
IN
ababbab
OUT
19
---------
IN
abab
OUT
7
*/
