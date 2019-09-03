/*
 * Дано число N и последовательность из N целых чисел.
 * Найти вторую порядковую статистику на заданных диапазонах.
 * Для решения задачи используйте структуру данных Sparse Table.
 * Требуемое время обработки каждого диапазона O(1). Время подготовки структуры данных O(n log n).
 *
 * Формат ввода
 * В первой строке заданы 2 числа: размер последовательности N и количество диапазонов M.
 * Следующие N целых чисел задают последовательность. Далее вводятся M пар чисел - границ диапазонов.
 * Гарантируется, что каждый диапазон содержит как минимум 2 элемента.
 * Формат выходных данных.
 * Для каждого из M диапазонов напечатать элемент последовательности - 2ю порядковую статистику. По одному числу в строке.
*/

#include <iostream>
#include <vector>
#include <cmath>

uint32_t logarithm_2(uint32_t length) {
    if (length == 1)
        return 0;

    return logarithm_2(length / 2) + 1;
}

void precalc_log(std::vector<uint32_t>& logarithms) {
    for (uint32_t i = 0; i < logarithms.size(); ++i) {
        logarithms[i] = logarithm_2(i + 1);
    }
}

class SparseTable {
    // Table will consist of pairs because we need 2nd statistics.
    // Pair will contain two indices: 1st and 2nd statistics positions.
    std::vector<std::vector<std::pair<uint32_t, uint32_t>>> _sparse_table;
    std::vector<uint32_t> _logarithms;
public:
    explicit SparseTable(uint32_t num_of_strings){
        // Precalculation of logarithms.
        _logarithms.resize(num_of_strings, 0);
        precalc_log(_logarithms);

        // + 2 because of two reasons: 1) number of columns should be log2(num_of_strings) + 1 because otherwise there will be 2 numbers on the rightmost column.
        // 2) In all cycles counters does not "reach" table borders (operator < will be used).
        uint32_t num_of_columns = _logarithms[num_of_strings - 1] + 2;

        _sparse_table.resize(num_of_strings, std::vector<std::pair<uint32_t, uint32_t>>(num_of_columns, std::make_pair(0, 0)));
    }

    void BuildTable(const std::vector<int32_t>& sequence);
    int32_t GetSecondMin(uint32_t l, uint32_t r, std::vector<int32_t>& sequence) const;
};

// Finds among four indices indices of elements that are first and second statistics.
std::pair<uint32_t, uint32_t> FindFirstAndSecondStatistics(uint32_t index1, uint32_t index2, uint32_t index3,
                                                           uint32_t index4, const std::vector<int32_t>& sequence) {
    std::vector<uint32_t> indices_of_candidates = {index1, index2, index3, index4};

    int32_t first_min_index = index1, second_min_index = -1;
    for(uint32_t c = 0; c < 4; ++c) {
        if (sequence[first_min_index] > sequence[indices_of_candidates[c]]) {
            second_min_index = first_min_index;
            first_min_index = indices_of_candidates[c];
        } else if ((second_min_index == -1 || sequence[second_min_index] > sequence[indices_of_candidates[c]])
                   && indices_of_candidates[c] != first_min_index) {
            second_min_index = indices_of_candidates[c];
        }
    }

    return std::make_pair(first_min_index, second_min_index);
}

void SparseTable::BuildTable(const std::vector<int32_t>& sequence) {
    // 2 ^ 0 = 1 => we just copy original array.
    for (uint32_t i = 0; i < sequence.size(); ++i) {
        _sparse_table[i][0].first = i;
        _sparse_table[i][0].second = i;
    }

    uint32_t num_of_columns = _logarithms[sequence.size() - 1] + 2;

    for (uint32_t j = 1; j < num_of_columns; ++j) {
        uint32_t segment_len = static_cast<int>(std::pow(2, j - 1));

        for (uint32_t i = 0; i + segment_len < sequence.size() - segment_len + 1; ++i) {
            _sparse_table[i][j] = FindFirstAndSecondStatistics(_sparse_table[i][j - 1].first, _sparse_table[i][j - 1].second,
                    _sparse_table[i + segment_len][j - 1].first, _sparse_table[i + segment_len][j - 1].second,
                    sequence);
        }
    }
}

int32_t SparseTable::GetSecondMin(uint32_t l, uint32_t r, std::vector<int32_t>& sequence) const {
    int32_t j = _logarithms[(r - l + 1) - 1];
    uint32_t segment_len = static_cast<int>(std::pow(2, j));

    return sequence[FindFirstAndSecondStatistics(_sparse_table[l][j].first, _sparse_table[l][j].second,
                                                 _sparse_table[r - segment_len + 1][j].first,
                                                 _sparse_table[r - segment_len + 1][j].second,
                                                 sequence).second];
}

int main() {
    uint32_t n = 0, m = 0;
    std::cin >> n >> m;

    std::vector<int32_t> sequence(n);
    for (uint32_t i = 0; i < n; ++i) {
        std::cin >> sequence[i];
    }

    // Create sparse_table.
    SparseTable sparse_table(n);
    sparse_table.BuildTable(sequence);

    // Queries.
    for (uint32_t j = 0; j < m; ++j) {
        uint32_t l = 0, r = 0;
        std::cin >> l >> r;
        --l, --r;
        std::cout << sparse_table.GetSecondMin(l, r, sequence) << std::endl;
    }

    return 0;
}


/*
IN
10 3
1 2 3 4 5 6 7 8 9 10
1 2
1 10
2 7
OUT
2
2
3
*/