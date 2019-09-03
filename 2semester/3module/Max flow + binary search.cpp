/*
 * В одном из отделов крупной организации работает n человек.
 * Как практически все сотрудники этой организации, они любят пить чай в перерывах между работой.
 * При этом они достаточно дисциплинированы и делают в день ровно один перерыв, во время которого пьют чай.
 * Для того, чтобы этот перерыв был максимально приятным,
 * каждый из сотрудников этого отдела обязательно пьет чай одного из своих любимых сортов.
 * В разные дни сотрудник может пить чай разных сортов. Для удобства пронумеруем сорта чая числами от 1 до m.
 * Недавно сотрудники отдела купили себе большой набор чайных пакетиков,
 * который содержит a1 пакетиков чая сорта номер 1, a2 пакетиков чая сорта
 * номер 2, ..., am пакетиков чая сорта номер m.
 * Теперь они хотят знать, на какое максимальное число дней им может хватить купленного набора так,
 * чтобы в каждый из дней каждому из сотрудников доставался пакетик чая одного из его любимых сортов.
 * Каждый сотрудник отдела пьет в день ровно одну чашку чая, которую заваривает из одного пакетика.
 * При этом пакетики чая не завариваются повторно.
 *
 * Входные данные
 * Первая строка содержит два целых числа n и m (1 ≤ n, m ≤ 50).
 * Вторая строка содержит m целых чисел a1, ..., am (1 ≤ ai ≤ 10^6 для всех i от 1 до m).
 * Далее следуют n строк — i-я из этих строк описывает любимые сорта i-го сотрудника отдела
 * и имеет следующий формат: сначала следует положительное число ki —
 * количество любимых сортов чая этого сотрудника, а затем идут ki различных чисел от 1 до m — номера этих сортов.
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
uint32_t MaxFlow(IGraph *graph, uint32_t source, uint32_t sink) {
    uint32_t max_flow = 0;
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

void AddSource(IGraph *graph, uint32_t days, uint32_t num_of_workers, uint32_t num_of_tea_types) {
    for (uint32_t i = 0; i < num_of_workers; ++i) {
        graph->AddEdge(num_of_workers + num_of_tea_types, i, days);
    }
}

uint32_t FindDaysNum(const IGraph *graph, uint32_t upper_bound, uint32_t num_of_workers, uint32_t num_of_tea_types) {
    uint32_t lower_bound = 0;
    uint32_t max_num_of_days = 0;

    // Binary search by days.
    while (upper_bound >= lower_bound) {
        uint32_t middle = (upper_bound + lower_bound) / 2;

        ListGraph cur_graph(graph);

        // Add edges from source to all workers and make edge weight is number of days.
        AddSource(&cur_graph, middle, num_of_workers, num_of_tea_types);

        // Dinitz's algorithm for searching max flow.
        uint32_t cur_flow = MaxFlow(&cur_graph, num_of_workers + num_of_tea_types, num_of_workers + num_of_tea_types + 1);

        if (cur_flow == num_of_workers * middle) {
            lower_bound = middle + 1;

            if (middle > max_num_of_days)
                max_num_of_days = middle;
        } else {
            upper_bound = middle - 1;
        }
    }

    return max_num_of_days;
}

int main() {
    uint32_t n = 0, m = 0;
    std::cin >> n >> m;

    ListGraph graph(n + m + 2);
    // From 0 to n - 1 vertices are workers, from n to n + m - 1 vertices are tea bags.
    // Penultimate vertex is source. Last vertex is sink.
    uint32_t tea_bags_amount = 0;
    for (uint32_t i = n; i < n + m; ++i) {
        uint32_t tea_bags = 0;
        std::cin >> tea_bags;
        tea_bags_amount += tea_bags;
        graph.AddEdge(i, n + m + 1, tea_bags);
    }

    // Add relationships between workers and their preferable tea.
    for (uint32_t i = 0; i < n; ++i) {
        uint32_t types = 0;
        std::cin >> types;

        for (uint32_t  j = 0; j < types; ++j) {
            uint32_t preferable_tea = 0;
            std::cin >> preferable_tea;
            graph.AddEdge(i, n + preferable_tea - 1, INT32_MAX);
        }
    }

    std::cout << FindDaysNum(&graph, tea_bags_amount / n + 1, n, m);

    return 0;
}

/*
IN
2 3
3 2 1
2 1 2
2 1 3
OUT
3
--------
IN
3 3
2 7 4
2 1 2
1 2
2 2 3
OUT
4
*/