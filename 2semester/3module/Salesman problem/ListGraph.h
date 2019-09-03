#ifndef SALESMANPROBLEM_2_CLISTGRAPH_H
#define SALESMANPROBLEM_2_CLISTGRAPH_H

// Not oriented graph.
class ListGraph : public IGraph {
public:
    explicit ListGraph(const uint32_t VerticesCount) {
        _adjacent_vertices.resize(VerticesCount);
    }

    /*explicit ListGraph(const IGraph *graph) {
        _adjacent_vertices.resize(graph->VerticesCount());
        for (std::size_t i = 0; i < _adjacent_vertices.size(); ++i) {
            graph->GetNextVertices(i, _adjacent_vertices[i]);
        }
    }*/

    void AddEdge(const uint32_t from, const uint32_t to, const double cost) override {
        std::pair<std::size_t, double> tmp(to, cost);
        _adjacent_vertices[from].push_back(tmp);

        tmp.first = from;
        _adjacent_vertices[to].push_back(tmp);
    }

    uint32_t VerticesCount() const override {
        return _adjacent_vertices.size();
    }

    double GetCost(const uint32_t from, const uint32_t to) const override {
        uint32_t to_pos = 0;
        for (; to_pos < _adjacent_vertices[from].size(); ++to_pos) {
            if (_adjacent_vertices[from][to_pos].first == to)
                break;
        }

        if (to_pos == _adjacent_vertices[from].size())
            return -1.0;

        return _adjacent_vertices[from][to_pos].second;
    }

    void GetNextVertices(const uint32_t vertex, std::vector<std::uint32_t>& vertices) const {
        if(vertices.size() > 0)
            vertices.clear();

        vertices.resize(_adjacent_vertices[vertex].size());
        for (std::size_t i = 0; i < vertices.size(); ++i) {
            vertices[i] = _adjacent_vertices[vertex][i].first;
        }
    }

private:
    std::vector<std::vector<std::pair<std::size_t, double>>> _adjacent_vertices;
};

#endif //SALESMANPROBLEM_2_CLISTGRAPH_H
