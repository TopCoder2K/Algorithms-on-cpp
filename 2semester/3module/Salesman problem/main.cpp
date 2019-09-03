/*
Найдите приближенное решение метрической неориентированной задачи коммивояжера в полном графе (на плоскости)
с помощью минимального остовного дерева.
Оцените качество приближения на случайном наборе точек, нормально распределенном на плоскости с дисперсией 1.
Нормально распределенный набор точек получайте с помощью std::normal_distribution.
При фиксированном N, количестве вершин графа, несколько раз запустите оценку качества приближения.
Вычислите среднее значение и среднеквадратичное отклонение качества приближения для данного N.
Запустите данный эксперимент для всех N в некотором диапазоне, например, [2, 10].

Автоматизируйте запуск экспериментов.
В решении требуется разумно разделить код на файлы. Каждому классу - свой заголовочный файл и файл с реализацией.
*/

#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>
#include <ctime>
#include "IGraph.h"
#include "MatrixGraph.h"
#include "ListGraph.h"
#include "DSU.h"
#include "1,5Approximation.h"
#include "Accurate.h"
#include "2Approximation.h"

void GetDistribution(std::vector<std::pair<double, double>>& distribution) {
    // Random device class instance, source of 'true' randomness for initializing random seed.
    //std::random_device rd;
    std::random_device rd{};
    // Mersenne twister PRNG, initialized with seed from previous random device instance.
    //std::mt19937 gen(rd());
    std::mt19937 gen{rd()};

    // X-axis.
    std::cout << "X-axis:\n";
    for(uint8_t i = 0; i < distribution.size(); ++i) {
        // Instance of class std::normal_distribution with specific mean and stddev
        std::normal_distribution<double> d(0, 1);

        // Get random number with normal distribution using gen as random source
        distribution[i].first = d(gen);

        std::cout << 'x' << i + 1 << ": ";
        std::cout << distribution[i].first << '\n';
    }
    std::cout << std::endl;

    // Y-axis.
    std::cout << "Y-axis:\n";
    for(uint8_t i = 0; i < distribution.size(); ++i) {
        std::normal_distribution<double> d(0, 1);

        distribution[i].second = d(gen);

        std::cout << 'y' << i + 1 << ": ";
        std::cout << distribution[i].second << '\n';
    }
}

int main() {
    // n - number of vertices.
    for (uint32_t n = 2; n <= 10; ++n) {
        std::cout << "\n---------------------------------------------------------------\n" << "TEST " << n - 1 << "\nn = " << n;
        std::cout << "\n---------------------------------------------------------------\n\n";

        // At each n we do 7 measurements.
        std::vector<double> accurate_result(7, 0.0);
        std::vector<double> approx2_res(7, 0.0);
        std::vector<double> approx1_5_res(7, 0.0);
        for (uint32_t i = 0; i < 7; ++i) {
            std::vector<std::pair<double, double>> distribution(n);
            GetDistribution(distribution);

            accurate_result[i] = AccurateSalesman(n, distribution);
            approx2_res[i] = Approximation2(n, distribution);
            approx1_5_res[i] = Approximation1_5(n, distribution);
        }

        for (uint32_t i = 0; i < 7; ++i) {
            std::cout << accurate_result[i] << "    ";
        }
        std::cout << std::endl;
        for (uint32_t i = 0; i < 7; ++i) {
            std::cout << approx2_res[i] << "    ";
        }
        std::cout << std::endl;
        for (uint32_t i = 0; i < 7; ++i) {
            std::cout << approx1_5_res[i] << "    ";
        }
        std::cout << std::endl;

        // Get average values.
        double avg_accurate = 0.0, avg2 = 0.0, avg1_5 = 0.0;
        for (uint32_t i = 0; i < 7; ++i) {
            avg_accurate += accurate_result[i];
            avg2 += approx2_res[i];
            avg1_5 += approx1_5_res[i];
        }
        avg_accurate /= 7.0, avg2 /= 7.0, avg1_5 /= 7.0;
        std::cout << "Average values:\n";
        std::cout << avg_accurate << " - accurate answer,\n";
        std::cout << avg2 << " - 2-approximation answer,\n";
        std::cout << avg1_5 << " - 1.5-approximation answer.\n";

        // Get standard deviation.
        double sigma_accurate = 0.0, sigma2 = 0.0, sigma1_5 = 0.0;
        for (uint32_t i = 0; i < 7; ++i) {
            sigma_accurate += (avg_accurate - accurate_result[i]) * (avg_accurate - accurate_result[i]);
            sigma2 += (avg2 - approx2_res[i]) * (avg2 - approx2_res[i]);
            sigma1_5 += (avg1_5 - approx1_5_res[i]) * (avg1_5 - approx1_5_res[i]);
        }
        sigma_accurate /= 7.0, sigma2 /= 7.0, sigma1_5 /= 1.5;
        sigma_accurate = sqrt(sigma_accurate), sigma2 = sqrt(sigma2), sigma1_5 = sqrt(sigma1_5);
        std::cout << "\nStandard deviations:\n";
        std::cout << sigma_accurate << " - accurate answer,\n";
        std::cout << sigma2 << " - 2-approximation answer,\n";
        std::cout << sigma1_5 << " - 1.5-approximation answer.\n";

        std::cout << "\nPercentage deviations:\n";
        std::cout << (avg2 - avg_accurate) / avg_accurate * 100 << "% - 2-approximation answer,\n";
        std::cout << (avg1_5 - avg_accurate) / avg_accurate * 100 << "% - 1.5-approximation answer.\n";

        if (n != 10) {
            std::cout << "Should I continue? (yes/no)\n";
            std::string answer;
            std::cin >> answer;

            if (answer != "no" && answer != "yes") {
                std::cout << "I do not play like that.\n";
                break;
            }

            if (answer == "no") {
                std::cout << "So, bye.\n";
                break;
            }
        }
    }
    return 0;
}