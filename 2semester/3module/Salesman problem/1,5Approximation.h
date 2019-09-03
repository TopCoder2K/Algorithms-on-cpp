#ifndef SALESMANPROBLEM_2_1_5APPROXIMATION_H
#define SALESMANPROBLEM_2_1_5APPROXIMATION_H

// Finds minimum spanning tree and then tries to find minimum matchings among vertices that have odd degree.
// Ideally, search of minimum matchings has to be realised by search best matching on a subgraph of odd vertices of a minimal spanning tree.
// But there it is realised by brute force.

template <typename T>
struct Comparator {
    bool operator()(const T& a, const T& b) {
        return a.first < b.first;
    }
};

double distance(const double x1, const double y1, const double x2, const double y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void BuildGraph(IGraph *graph, std::vector<std::pair<double, std::pair<uint32_t, uint32_t>>>& edges,
                std::vector<std::pair<double, double>>& distribution) {
    for (uint8_t i = 0; i < distribution.size(); ++i) {
        std::pair<double, double> cur_vertex(distribution[i].first, distribution[i].second);

        for (uint8_t j = 0; j < distribution.size(); ++j) {
            std::pair<double, double> next_vertex(distribution[j].first, distribution[j].second);
            double cost = distance(cur_vertex.first, cur_vertex.second, next_vertex.first, next_vertex.second);
            graph->AddEdge(i, j, cost);

            if (i != j) {
                std::pair<uint32_t, uint32_t> tmp_pair(i, j);
                std::pair<double, std::pair<uint32_t, uint32_t>> tmp_p(cost, tmp_pair);
                edges.push_back(tmp_p);
            }
        }
    }
}

double GetPathCost(const IGraph *graph, std::vector<uint32_t>& path) {
    double path_cost = 0.0;
    for (int i = 1; i < path.size(); ++i) {
        path_cost += graph->GetCost(path[i - 1], path[i]);
    }

    return path_cost;
}

// Kruskal's algorithm.
double FindMinSpanningTree(const IGraph *graph,
                           std::vector<std::pair<double, std::pair<uint32_t, uint32_t>>>& edges,
                           std::vector<std::pair<uint32_t , uint32_t>>& path) {
    DSU<uint32_t> dsu(graph->VerticesCount());
    for (uint32_t i = 0; i < graph->VerticesCount(); ++i) {
        dsu.MakeSet(i);
    }

    double MST_weight = 0;

    for (uint32_t i = 0; i < edges.size(); ++i) {
        uint32_t from = edges[i].second.first, to = edges[i].second.second;
        double cost = edges[i].first;
        uint32_t first_leader = dsu.FindSet(from), second_leader = dsu.FindSet(to);
        if (first_leader != second_leader) {
            MST_weight += cost;
            path.push_back(edges[i].second);
            dsu.UnionSets(first_leader, second_leader);
        }
    }

    return MST_weight;
}

void MakeMST(const IGraph *graph, IGraph *MST, std::vector<std::pair<uint32_t , uint32_t>>& path) {
    for (uint32_t i = 0; i < path.size(); ++i) {
        uint32_t from = path[i].first, to = path[i].second;
        MST->AddEdge(from, to, graph->GetCost(from, to));
    }
}

void DFS(const IGraph *MST, uint32_t vertex, std::vector<uint32_t>& the_way, std::vector<char>& used) {
    used[vertex] = 1;
    the_way.push_back(vertex);

    std::vector<uint32_t> next_vertices;
    MST->GetNextVertices(vertex, next_vertices);

    for (uint32_t i = 0; i < next_vertices.size(); ++i) {
        uint32_t to = next_vertices[i];
        if (!used[to])
            DFS(MST, to, the_way, used);
    }
}

double GetPath(const IGraph *graph, const IGraph *MST, std::vector<uint32_t>& permutation, std::vector<uint32_t>& odd_vertices) {
    double summary_cost = 0.0;

    for (uint32_t i = 0; i < permutation.size(); i += 2) {
        uint32_t ii = permutation[i], jj = permutation[i + 1];
        uint32_t from = odd_vertices[ii], to = odd_vertices[jj];

        double curCost = MST->GetCost(from, to);

        if (curCost == -1.0) {
            summary_cost += graph->GetCost(from, to);
        } else {
            summary_cost += curCost;
        }
    }

    return summary_cost;
}

void AddMinMatchings(const IGraph *graph, IGraph *MST, std::vector<uint32_t>& the_way) {
    // Find vertices whose degree is odd.
    std::vector<uint32_t> odd_vertices;
    for (uint32_t i = 0; i < MST->VerticesCount(); ++i) {
        std::vector<uint32_t> next_vertices;
        MST->GetNextVertices(i, next_vertices);

        if (next_vertices.size() % 2 == 1) {
            odd_vertices.push_back(i);
        }
    }

    std::cout << "\nThese vertices have odd degree: ";
    for (uint32_t i = 0; i < odd_vertices.size(); ++i) {
        std::cout << odd_vertices[i];
        if (i + 1 < odd_vertices.size()) {
            std::cout << ", ";
        }
    }
    std::cout << '\n';

    // Brute force.
    std::vector<uint32_t> permutations(odd_vertices.size());
    for (uint32_t i = 0; i < permutations.size(); ++i) {
        permutations[i] = i;
    }

    double min_path_cost = 1000000.0;
    std::vector<uint32_t> min_order(permutations.size());
    do {
        double cur_path_cost = GetPath(graph, MST, permutations, odd_vertices);

        if (cur_path_cost < min_path_cost) {
            min_path_cost = cur_path_cost;
            min_order = permutations;
        }
    } while (std::next_permutation(permutations.begin(), permutations.begin() + permutations.size()));

    // Add edge if not exists.
    std::cout << "\nThese edges were added:\n";
    for (uint32_t i = 0; i < min_order.size(); i += 2) {
        uint32_t ii = min_order[i], jj = min_order[i + 1];
        uint32_t from = odd_vertices[ii], to = odd_vertices[jj];

        double cur_cost = MST->GetCost(from, to);

        if (cur_cost == -1.0) {
            MST->AddEdge(from, to, graph->GetCost(from, to));
            std::cout << from << ' ' << to << '\n';
            std::cout << to << ' ' << from << '\n';
        }
    }

    // Get answer by launching DFS.
    std::vector<char> used(MST->VerticesCount(), 0);
    DFS(MST, 0, the_way, used);
}

double Approximation1_5(uint32_t n, std::vector<std::pair<double, double>>& distribution) {
    std::cout << "\n1,5-Approximation salesman is working...\n";

    // Graph building.
    MatrixGraph graph(n);
    std::vector<std::pair<double, std::pair<uint32_t, uint32_t>>> edges;
    BuildGraph(&graph, edges, distribution);

    Comparator<std::pair<double, std::pair<uint32_t, uint32_t>>> cmp;
    sort(edges.begin(), edges.end(), cmp);

    // 1.5-approximation.
    std::vector<std::pair<uint32_t , uint32_t>> path;
    std::cout << "\nLength of the MST:\n";
    std::cout << FindMinSpanningTree(&graph, edges, path) << '\n';

    std::cout << "There are these edges in the MST:\n";
    for (uint32_t i = 0; i < path.size(); ++i) {
        std::cout << path[i].first << ' ' << path[i].second << '\n';
    }

    // Make a MST from the path.
    ListGraph MST(n);
    MakeMST(&graph, &MST, path);

    // Add new edges that can decrease answer (search for minimum weight matchings).
    std::vector<uint32_t> the_way;
    AddMinMatchings(&graph, &MST, the_way);

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

    std::cout << "\n\n1,5-Approximation salesman stopped working.\n\n\n";
    return last_edge_cost + path_cost;
}

#endif //SALESMANPROBLEM_2_1_5APPROXIMATION_H
