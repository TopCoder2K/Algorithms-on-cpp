/* Дан неориентированный связный граф. Требуется найти вес минимального остовного дерева в этом графе.
 * Вариант 1. С помощью алгоритма Прима.
 * Вариант 2. С помощью алгоритма Крускала.
 * Вариант 3. С помощью алгоритма Борувки.
 * (Вариант 2)
 * Формат ввода
 * Первая строка содержит два натуральных числа n и m — количество вершин и ребер графа соответственно
 * (1 ≤ n ≤ 20000, 0 ≤ m ≤ 100000).
 * Следующие m строк содержат описание ребер по одному на строке.
 * Ребро номер i описывается тремя натуральными числами bi, ei и wi — номера концов ребра и его вес соответственно
 * (1 ≤ bi, ei ≤ n, 0 ≤ wi ≤ 100000).*/

#include <iostream>
#include <vector>
#include <algorithm>

struct IGraph {
    virtual ~IGraph() = default;

    virtual void AddEdge(const std::size_t from, const std::size_t to, const std::size_t cost) = 0;

    virtual std::size_t VerticesCount() const = 0;

    virtual void GetNextVertices(const std::size_t vertex, std::vector<std::pair<std::size_t, std::size_t>>& vertices) const = 0;
    virtual void GetPrevVertices(const std::size_t vertex, std::vector<std::pair<std::size_t, std::size_t>>& vertices) const = 0;
};

// Not oriented graph.
class ListGraph : public IGraph {
public:
    explicit ListGraph(const std::size_t VerticesCount) {
        _adjacent_vertices.resize(VerticesCount);
    }
    explicit ListGraph(const IGraph* graph) {
        _adjacent_vertices.resize(graph->VerticesCount());

        for (std::size_t i = 0; i < _adjacent_vertices.size(); ++i) {
            graph->GetNextVertices(i, _adjacent_vertices[i]);
        }
    }

    void AddEdge(const std::size_t from, const std::size_t to, const std::size_t cost) override {
        std::pair<std::size_t, std::size_t> tmp(to, cost);
        _adjacent_vertices[from].push_back(tmp);

        tmp.first = from;
        _adjacent_vertices[to].push_back(tmp);
    }

    std::size_t VerticesCount() const override {
        return _adjacent_vertices.size();
    }

    void GetNextVertices(const std::size_t vertex,
                         std::vector<std::pair<std::size_t, std::size_t>>& vertices) const override {
        vertices.clear();

        vertices.resize(_adjacent_vertices[vertex].size());
        for (std::size_t i = 0; i < vertices.size(); ++i) {
            vertices[i] = _adjacent_vertices[vertex][i];
        }
    }

    void GetPrevVertices(const std::size_t vertex,
                         std::vector<std::pair<std::size_t, std::size_t>>& vertices) const override {
        vertices.clear();

        for (int i = 0; i < _adjacent_vertices.size(); ++i) {
            for (int j = 0; j < _adjacent_vertices[i].size(); ++j) {
                if (_adjacent_vertices[i][j].first == vertex)
                    vertices.push_back(std::make_pair(i, _adjacent_vertices[i][j].second));
            }
        }
    }

private:
    std::vector<std::vector<std::pair<std::size_t, std::size_t>>> _adjacent_vertices;
};

template <typename T>
struct Comparator {
    bool operator()(const T& a, const T& b) {
        return a.first < b.first;
    }
};

template <typename T>
class DSU {
public:
    explicit DSU(const uint32_t size) { _parent.resize(size); _rank.resize(size); }

    void MakeSet(const T& new_set) { _parent[new_set] = new_set; _rank[new_set] = 0; }

    void UnionSets(const T& first_element, const T& second_element) {
        T first_leader = FindSet(first_element);
        T second_leader = FindSet(second_element);

        if (first_leader != second_leader) {
            if (_rank[first_leader] < _rank[second_leader])
                std::swap(first_leader, second_leader);

            _parent[second_leader] = first_leader;
            if (_rank[first_leader] == _rank[second_leader]) {
                ++_rank[first_leader];
            }
        }
    }

    T FindSet(const T& element) {
        if (element == _parent[element])
            return element;

        _parent[element] = FindSet(_parent[element]);
        return _parent[element];
    }
private:
    std::vector<T> _parent;
    std::vector<uint32_t> _rank; // For rank union heuristics.
};

std::size_t FindMinSpanningTree(const IGraph *graph,
        std::vector<std::pair<uint32_t, std::pair<uint32_t, uint32_t>>>& edges,
        std::vector<std::pair<uint32_t , uint32_t>>& path) {
    DSU<uint32_t> dsu(graph->VerticesCount());

    for (uint32_t i = 0; i < graph->VerticesCount(); ++i) {
        dsu.MakeSet(i);
    }

    uint64_t MST_weight = 0;

    for (uint32_t i = 0; i < edges.size(); ++i) {
        uint32_t from = edges[i].second.first, to = edges[i].second.second, cost = edges[i].first;
        uint32_t first_leader = dsu.FindSet(from), second_leader = dsu.FindSet(to);

        if (first_leader != second_leader) {
            MST_weight += cost;
            path.push_back(edges[i].second);
            dsu.UnionSets(first_leader, second_leader);
        }
    }

    return MST_weight;
}

int main() {
    uint32_t n = 0, m = 0;
    std::cin >> n >> m;

    ListGraph graph(n);
    std::vector<std::pair<uint32_t, std::pair<uint32_t, uint32_t>>> edges(m);
    for (uint32_t i = 0; i < m; ++i) {
        uint32_t from = 0, to = 0, cost = 0;
        std::cin >> from >> to >> cost;
        --from, --to;

        graph.AddEdge(from, to, cost);
        edges[i] = std::make_pair(cost, std::make_pair(from, to));
    }

    Comparator<std::pair<uint32_t, std::pair<uint32_t, uint32_t>>> cmp;
    sort(edges.begin(), edges.end(), cmp);

    // Additionally we can see edges of a minimum spanning tree in vector "path".
    std::vector<std::pair<uint32_t , uint32_t>> path;
    std::cout << FindMinSpanningTree(&graph, edges, path) << '\n';

    /*for (uint32_t i = 0; i < path.size(); ++i) {
        std::cout << path[i].first << ' ' << path[i].second << '\n';
    }*/

    return 0;
}

/*
IN
4 4
1 2 1
2 3 2
3 4 5
4 1 4
OUT
7
--------
IN
5 10
4 3 3046
4 5 90110
5 1 57786
3 2 28280
4 3 18010
4 5 61367
4 1 18811
4 2 69898
3 5 72518
3 1 85838
OUT
107923*/