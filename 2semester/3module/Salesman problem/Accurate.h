#ifndef SALESMANPROBLEM_2_ACCURATE_H
#define SALESMANPROBLEM_2_ACCURATE_H

void BuildAccurateGraph(IGraph *graph, std::vector<std::pair<double, double>>& distribution) {
    for (uint8_t i = 0; i < distribution.size(); ++i) {
        std::pair<double, double> cur_vertex(distribution[i].first, distribution[i].second);
        for (uint8_t j = 0; j < distribution.size(); ++j) {
            std::pair<double, double> next_vertex(distribution[j].first, distribution[j].second);
            graph->AddEdge(i, j, distance(cur_vertex.first, cur_vertex.second, next_vertex.first, next_vertex.second));
        }
    }
}

double AccurateSalesman(uint32_t n, std::vector<std::pair<double, double>>& distribution) {
    std::cout << "\nAccurate salesman is working...\n";

    // Graph building.
    MatrixGraph graph(n);
    BuildAccurateGraph(&graph, distribution);

    // Brute force.
    std::vector<uint32_t> permutations(n - 1);
    for (uint8_t i = 0; i < n - 1; ++i) {
        permutations[i] = i + 1;
    }

    double min_path_cost = 1000000.0;
    std::vector<uint32_t> min_order(n - 1);
    do {
        double cur_path_cost = GetPathCost(&graph, permutations) + graph.GetCost(0, permutations[0]) + graph.GetCost(permutations[permutations.size() - 1], 0);

        if (cur_path_cost < min_path_cost) {
            min_path_cost = cur_path_cost;
            min_order = permutations;
        }
    } while (std::next_permutation(permutations.begin(), permutations.begin() + n - 1));

    std::cout << "\nPath is: 0->";
    for (uint32_t i = 0; i < min_order.size(); ++i) {
        std::cout << min_order[i];
        if (i + 1 < min_order.size()) {
            std::cout << "->";
        } else {
            std::cout << "->0";
        }
    }

    std::cout << "\nFinally, the answer is ";
    std::cout << min_path_cost;

    std::cout << "\n\nAccurate salesman stopped working.\n\n\n";
    return min_path_cost;
}

#endif //SALESMANPROBLEM_2_ACCURATE_H
