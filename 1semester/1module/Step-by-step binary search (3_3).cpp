/*3_3. Даны два массива неповторяющихся целых чисел, упорядоченные по возрастанию.
A[0..n-1] и B[0..m-1]. n >> m. Найдите их пересечение.
Требуемое время работы: O(m * log k), где k - позиция элемента B[m-1] в массиве A.
В процессе поиска очередного элемента B[i] в массиве A пользуйтесь результатом поиска элемента B[i-1].
n, k ≤ 10000.
*/

#include <iostream>

void FindIntersection(uint32_t n, uint32_t m, uint32_t *A, uint32_t *B, bool *Intersection)
{
    uint32_t left = 0, right = 1;
    for (uint32_t i = 0; i < m; ++i) {
        // Using binary search in array A - log(k). And saving the interval.
        if (B[i] > A[right]) {
            left = right;

            if (right == n - 1) {
                ++i;
            }

            if (2 * right >= n) {
                right = n - 1;
            } else {
                right *= 2;
            }

            --i;
        } else {
            uint32_t middle = 0, cur_left = left, cur_right = right + 1;

            while (cur_right - cur_left > 0) {
                middle = cur_left + (cur_right - cur_left) / 2;

                if (A[middle] == B[i]) {
                    break;
                }

                if (A[middle] > B[i]) {
                    cur_right = middle;
                } else {
                    cur_left = middle + 1;
                }
            }

            if (B[i] == A[middle]) {
                Intersection[i] = true;
            }
        }
    }
}

int main()
{
    uint32_t n = 0, m = 0;
    std::cin >> n >> m;

    auto *A = new uint32_t[n];
    for (uint32_t i = 0; i < n; ++i) {
        std::cin >> A[i];
    }
    auto *B = new uint32_t[m];
    for (uint32_t i = 0; i < m; ++i) {
        std::cin >> B[i];
    }

    // Creating an array to check for intersection.
    auto *Intersection = new bool[m];
    for (uint32_t i = 0; i < m; ++i) {
        Intersection[i] = false;
    }

    FindIntersection(n, m, A, B, Intersection);

    for (uint32_t i = 0; i < m; ++i) {
        if (Intersection[i]) {
            std::cout << B[i] << ' ';
        }
    }

    delete[] A;
    delete[] B;
    delete[] Intersection;

    return 0;
}

/*
in
5
3
1 2 3 4 5
1 3 5

out
1 3 5
*/