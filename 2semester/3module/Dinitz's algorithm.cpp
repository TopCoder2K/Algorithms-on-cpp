/*
 * Задан неориентированный граф, каждое ребро которого обладает целочисленной пропускной способностью.
 * Найдите максимальный поток из вершины с номером 1
 * в вершину с номером n.
 * Формат ввода
 * Первая строка входного файла содержит n и m — количество вершин и количество ребер графа
 * (2≤n≤100,1≤m≤1000). Следующие m строк содержат по три числа:
 * номера вершин, которые соединяет соответствующее ребро графа и его пропускную способность.
 * Пропускные способности не превосходят 10^5.
 * Формат вывода
 * В выходной файл выведите одно число — величину максимального потока из вершины с номером
 * 1 в вершину с номером n.
 * */

#include <iostream>
#include <vector>
#include <queue>

struct Vertex {
    Vertex(uint32_t from, uint32_t to, int32_t cost) : _from(from), _to(to), _cap(cost), _flow(0) {}
    Vertex() : _from(0), _to(0), _cap(0), _flow(0) {}

    uint32_t _from, _to;
    int32_t _flow, _cap;
};

struct IGraph {
    virtual ~IGraph() = default;

    virtual void AddEdge(const uint32_t from, const uint32_t to, const int32_t cost) = 0;

    virtual uint32_t VerticesCount() const = 0;

    virtual void GetNextVertices(const uint32_t vertex, std::vector<Vertex>& vertices) const = 0;
    virtual void ChangeFlow(uint32_t from, uint32_t to, int32_t flow) = 0;
};

// Oriented graph.
class ListGraph : public IGraph {
public:
    explicit ListGraph(const uint32_t VerticesCount) {
        _adjacent_vertices.resize(VerticesCount);
    }
    explicit ListGraph(const IGraph *graph) {
        _adjacent_vertices.resize(graph->VerticesCount());
        for (std::size_t i = 0; i < _adjacent_vertices.size(); ++i) {
            graph->GetNextVertices(i, _adjacent_vertices[i]);
        }
    }

    void AddEdge(const uint32_t from, const uint32_t to, const int32_t cost) override {
        Vertex tmp(from, to, cost);
        _adjacent_vertices[from].push_back(tmp);
    }

    uint32_t VerticesCount() const override {
        return _adjacent_vertices.size();
    }

    void GetNextVertices(const uint32_t vertex, std::vector<Vertex>& vertices) const override {
        if(!vertices.empty()) {
            vertices.clear();
        }

        vertices.resize(_adjacent_vertices[vertex].size());
        for (uint32_t i = 0; i < vertices.size(); ++i) {
            vertices[i] = _adjacent_vertices[vertex][i];
        }
    }

    void ChangeFlow(uint32_t from, uint32_t to, int32_t flow) {
        // Find to position in next vertices vector and changes flow.
        std::vector<Vertex> next_vertices;
        GetNextVertices(from, next_vertices);
        uint32_t to_pos = 0;

        for (; to_pos < next_vertices.size(); ++to_pos) {
            if (next_vertices[to_pos]._to == to)
                break;
        }
        _adjacent_vertices[from][to_pos]._flow += flow;

        // Analogically, but do not forget to add reverse edge if it does not exist.
        GetNextVertices(to, next_vertices);
        uint32_t from_pos = 0;
        for (; from_pos < next_vertices.size(); ++from_pos) {
            if (next_vertices[from_pos]._to == from)
                break;
        }

        if (from_pos == next_vertices.size())
            AddEdge(to, from, 0);

        _adjacent_vertices[to][from_pos]._flow -= flow;
    }

private:
    std::vector<std::vector<Vertex>> _adjacent_vertices;
};

bool BFS (const IGraph *graph, const uint32_t source, const uint32_t sink, std::vector<int32_t>& level) {
    std::queue<uint32_t> queue;
    for (uint32_t i = 0; i < level.size(); ++i) {
        level[i] = -1;
    }

    level[source] = 0;
    queue.push(source);

    while (!queue.empty() && level[sink] == -1) {
        uint32_t cur_vertex = queue.front();
        queue.pop();

        std::vector<Vertex> next_vertices;
        graph->GetNextVertices(cur_vertex, next_vertices);

        for (uint32_t i = 0; i < next_vertices.size(); ++i) {
            uint32_t to = next_vertices[i]._to;
            // Additional check on saturated edges.
            if (level[to] == -1 && next_vertices[i]._flow < next_vertices[i]._cap) {
                queue.push(to);
                level[to] = level[cur_vertex] + 1;
            }
        }
    }

    return level[sink] == -1;
}

int32_t DFS(IGraph *graph, const uint32_t vertex, const uint32_t sink, int32_t flow,
            std::vector<uint32_t>& ptr, std::vector<int32_t>& level) {
    if (vertex == sink || flow == 0)
        return flow;

    std::vector<Vertex> next_vertices;
    graph->GetNextVertices(vertex, next_vertices);

    for (; ptr[vertex] < next_vertices.size(); ++ptr[vertex]) {
        uint32_t to = next_vertices[ptr[vertex]]._to;

        if (level[to] != level[vertex] + 1) continue;

        int32_t pushed = DFS(graph, to, sink,
                             std::min(
                                     flow, static_cast<int32_t>(next_vertices[ptr[vertex]]._cap) - next_vertices[ptr[vertex]]._flow
                                     ),ptr, level);

        if (pushed) {
            graph->ChangeFlow(vertex, to, pushed);
            return pushed;
        }
    }

    return 0;
}

// Dinitz's algorithm.
uint32_t MaxFlow(IGraph *graph) {
    uint32_t max_flow = 0;
    // Source vertex and sink vertex from the task.
    const uint32_t source = 0, sink = graph->VerticesCount() - 1;
    // To make algorithm faster we won't go by edges, which do not lead to sink, for the second time.
    std::vector<uint32_t> start_edges(graph->VerticesCount(), 0);
    // To store shortest paths.
    std::vector<int32_t> level(graph->VerticesCount(), -1);

    while (!BFS(graph, source, sink, level)) {
        // Refresh start edges for every vertex.
        for (uint32_t i = 0; i < start_edges.size(); ++i) {
            start_edges[i] = 0;
        }

        int32_t pushed = DFS(graph, source, sink, INT32_MAX, start_edges, level);
        while (pushed) {
            max_flow += pushed;
            pushed = DFS(graph, source, sink, INT32_MAX, start_edges, level);
        }
    }

    return max_flow;
}

int main() {
    uint32_t n = 0, m = 0;
    std::cin >> n >> m;

    ListGraph graph(n);
    for (uint32_t i = 0; i < m; ++i) {
        uint32_t from = 0, to = 0, cost = 0;
        std::cin >> from >> to >> cost;

        --from, --to;
        graph.AddEdge(from, to, cost);
    }

    std::cout << MaxFlow(&graph);

    return 0;
}

/*
IN
4 5
1 2 1
1 3 2
3 2 1
2 4 2
3 4 1
OUT
3*/