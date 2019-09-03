#ifndef SALESMANPROBLEM_2_CMATRIXGRAPH_H
#define SALESMANPROBLEM_2_CMATRIXGRAPH_H

class MatrixGraph : public IGraph {
public:
    explicit MatrixGraph(const uint32_t VerticesCount) {
        _matrix.resize(VerticesCount);
        for (int i = 0; i < VerticesCount; ++i) {
            _matrix[i].resize(VerticesCount, false);
        }
    }

    void AddEdge(const uint32_t from, const uint32_t to, const double cost) override {
        _matrix[from][to] = cost;
    }

    double GetCost(const uint32_t from, const uint32_t to) const {
        return _matrix[from][to];
    }

    uint32_t VerticesCount() const override {
        return _matrix.size();
    }

    void GetNextVertices(const uint32_t vertex, std::vector<uint32_t>& vertices) const {
        vertices.clear();

        for (int i = 0; i < _matrix[vertex].size(); ++i) {
            if (_matrix[vertex][i]) {
                vertices.push_back(i);
            }
        }
    }

private:
    std::vector<std::vector<double>> _matrix;
};

#endif //SALESMANPROBLEM_2_CMATRIXGRAPH_H
