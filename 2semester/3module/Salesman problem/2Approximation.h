#ifndef SALESMANPROBLEM_2_2APPROXIMATION_H
#define SALESMANPROBLEM_2_2APPROXIMATION_H

// Finds minimum spanning tree (Kruskal's algorithm) and .
double Approximation2(uint32_t n, std::vector<std::pair<double, double>>& distribution) {
    std::cout << "\n2-Approximation salesman is working...\n";

    // Graph building.
    MatrixGraph graph(n);
    std::vector<std::pair<double, std::pair<uint32_t, uint32_t>>> edges;
    BuildGraph(&graph, edges, distribution);

    Comparator<std::pair<double, std::pair<uint32_t, uint32_t>>> cmp;
    sort(edges.begin(), edges.end(), cmp);

    // 2-approximation.
    std::vector<std::pair<uint32_t , uint32_t>> path;
    std::cout << "\nLength of the MST:\n";
    std::cout << FindMinSpanningTree(&graph, edges, path) << '\n';

    std::cout << "There are these edges in the MST:\n";
    for (uint32_t i = 0; i < path.size(); ++i) {
        std::cout << path[i].first << ' ' << path[i].second << '\n';
    }

    // Make an MST.
    ListGraph MST(n);
    MakeMST(&graph, &MST, path);

    // Get and shorten the way.
    std::vector<uint32_t> the_way;
    std::vector<char> used(n, 0);
    DFS(&MST, 0, the_way, used);

    std::cout << "\nPath is: ";
    for (uint32_t i = 0; i < the_way.size(); ++i) {
        std::cout << the_way[i];
        if (i + 1 < the_way.size()) {
            std::cout << "->";
        } else {
            std::cout << "->0";
        }
    }

    // Get answer.
    // Add edge from the last vertex to the first (we have to return).
    double last_edge_cost = graph.GetCost(the_way[0], the_way[the_way.size() - 1]);

    std::cout << "\nFinally, the answer is ";
    double path_cost = GetPathCost(&graph, the_way);
    std::cout << last_edge_cost + path_cost;

    std::cout << "\n\n2-Approximation salesman stopped working.\n\n\n";
    return last_edge_cost + path_cost;
}

#endif //SALESMANPROBLEM_2_2APPROXIMATION_H
