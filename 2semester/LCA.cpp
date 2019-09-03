/*
 * Задано дерево с корнем, содержащее 𝑛 (1 ≤ 𝑛 ≤ 100 000) вершин, пронумерованных от 0 до 𝑛−1.
 * Требуется ответить на 𝑚 (1 ≤ 𝑚 ≤ 10 000 000) запросов о наименьшем общем предке для пары вершин.
 * Запросы генерируются следующим образом. Заданы числа 𝑎1, 𝑎2 и числа 𝑥, 𝑦 и 𝑧.
 * Числа 𝑎3, . . . , 𝑎2𝑚 генерируются следующим образом: 𝑎𝑖 = (𝑥·𝑎𝑖−2+𝑦·𝑎𝑖−1+𝑧) mod 𝑛.
 * Первый запрос имеет вид ⟨𝑎1, 𝑎2⟩. Если ответ на 𝑖−1-й запрос равен 𝑣, то 𝑖-й запрос имеет вид ⟨(𝑎2𝑖−1 + 𝑣) mod 𝑛, 𝑎2𝑖⟩.
 * Для решения задачи можно использовать метод двоичного подъёма.
 *
 * Формат входных данных.
 * Первая строка содержит два числа: 𝑛 и 𝑚. Корень дерева имеет номер 0.
 * Вторая строка содержит 𝑛 − 1 целых чисел, 𝑖-е из этих чисел равно номеру родителя вершины 𝑖.
 * Третья строка содержит два целых числа в диапазоне от 0 до 𝑛−1: 𝑎1 и 𝑎2.
 * Четвертая строка содержит три целых числа: 𝑥, 𝑦 и 𝑧, эти числа неотрицательны и не превосходят 10^9.
 *
 * Формат выходных данных.
 * Выведите в выходной файл сумму номеров вершин — ответов на все запросы.
*/

#include <iostream>
#include <vector>
#include <stack>
#include <cmath>

std::vector<uint32_t> CountHeights(std::vector<uint32_t>& parents) {
    std::vector<uint32_t> heights(parents.size(), INT32_MAX);

    heights[0] = 0;

    for (uint32_t i = 1; i < heights.size(); ++i) {
        if (heights[i] == INT32_MAX) {
            std::stack<uint32_t> cur_way;
            cur_way.push(i);

            // Get all vertices that have not been visited.
            while (heights[cur_way.top()] == INT32_MAX) {
                cur_way.push(parents[cur_way.top()]);
            }

            // Calculate heights.
            cur_way.pop();
            while (!cur_way.empty()) {
                uint32_t cur_vertex = cur_way.top();
                cur_way.pop();

                heights[cur_vertex] = heights[parents[cur_vertex]] + 1;
            }
        }
    }

    return heights;
}

void CountBinaryLifts(std::vector<std::vector<uint32_t>>& dp, std::vector<uint32_t>& parents) {
    for (uint32_t i = 0; i < parents.size(); ++i) {
        dp[i][0] = parents[i];
    }

    for (uint32_t j = 1; j < dp[0].size(); ++j) {
        for (uint32_t i = 0; i < parents.size(); ++i) {
            dp[i][j] = dp[dp[i][j - 1]][j - 1];
        }
    }
}

// Leveling heights (lower the higher vertex).
uint32_t SameLevel(uint32_t lower, uint32_t higher, std::vector<uint32_t>& heights, std::vector<std::vector<uint32_t>>& dp) {
    uint32_t num_of_columns = dp[0].size();

    for (int32_t i = num_of_columns - 1; i >= 0; --i) {
        if (heights[dp[higher][i]] >= heights[lower])
            higher = dp[higher][i];
    }

    return higher;
}


// Least common ancestor.
uint32_t LCA(uint32_t v, uint32_t u, std::vector<uint32_t>& heights, std::vector<std::vector<uint32_t>>& dp) {
    if (heights[v] != heights[u]) {
        if (heights[v] > heights[u])
            std::swap(v, u);

        u = SameLevel(v, u, heights, dp);
    }

    if (v == u) return v;

    uint32_t num_of_columns = dp[0].size();
    for (int32_t i = num_of_columns - 1; i >= 0; --i) {
        if (dp[v][i] != dp[u][i]) {
            v = dp[v][i];
            u = dp[u][i];
        }
    }

    return dp[v][0];
}

int main() {
    uint64_t n = 0, m = 0;
    std::cin >> n >> m;

    std::vector<uint32_t> parents(n, 0);
    for (uint32_t i = 1; i < n; ++i) {
        std::cin >> parents[i];
    }

    uint64_t a1 = 0, a2 = 0;
    std::cin >> a1 >> a2;

    uint64_t x = 0, y = 0, z = 0;
    std::cin >> x >> y >> z;

    std::vector<uint32_t> heights = CountHeights(parents);

    // Preprocessing.
    std::vector<std::vector<uint32_t>> dp(n, std::vector<uint32_t>(static_cast<uint32_t>(log2(n)) + 2)); // + 2 because of logarithm and cast to int.
    CountBinaryLifts(dp, parents);


    uint64_t sum_ans = 0;
    uint64_t ans = 0;
    for (uint32_t i = 0; i < m; ++i) {
        ans = LCA((a1 + ans) % n, a2, heights, dp);
        sum_ans += ans;

        a1 = (x * a1 + y * a2 + z) % n;
        a2 = (x * a2 + y * a1 + z) % n;
    }

    std::cout << sum_ans;

    return 0;
}

/*
IN
3 2
0 1
2 1
1 1 0
OUT
2*/