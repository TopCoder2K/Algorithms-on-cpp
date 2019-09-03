/*
 Полный ориентированный взвешенный граф задан матрицей смежности.
 Постройте матрицу кратчайших путей между его вершинами.
 Гарантируется, что в графе нет циклов отрицательного веса.

 Формат ввода
 В первой строке вводится единственное число N (1 < N < 100) — количество вершин графа.
 В следующих N строках по N чисел задается матрица смежности графа
 (j-ое число в i-ой строке — вес ребра из вершины i в вершину j).
 Все числа по модулю не превышают 100. На главной диагонали матрицы — всегда нули.
 Формат выходного файла
 Выведите 𝑁 строк по 𝑁 чисел — матрицу расстояний между парами вершин, где 𝑗-ое число в 𝑖-ой строке равно весу кратчайшего пути из вершины 𝑖 в j.
*/

#include <iostream>
#include <vector>
#define MAX_INT 2147483648

struct IGraph {
    virtual ~IGraph() = default;

    virtual void AddEdge(const std::size_t from, const std::size_t to, const int cost) = 0;

    virtual std::size_t VerticesCount() const = 0;
    virtual int GetCost(const std::size_t from, const std::size_t to) const = 0;

    virtual void GetNextVertices(const std::size_t vertex, std::vector<std::pair<std::size_t, int>> &vertices) const = 0;
    virtual void GetPrevVertices(const std::size_t vertex, std::vector<std::pair<std::size_t, int>> &vertices) const = 0;
};

class MatrixGraph : public IGraph {
public:
    explicit MatrixGraph(const std::size_t VerticesCount) {
        _matrix.resize(VerticesCount);
        for (std::size_t i = 0; i < VerticesCount; ++i) {
            _matrix[i].resize(VerticesCount, MAX_INT);
        }
    }
    explicit MatrixGraph(const IGraph *graph) {
        _matrix.resize(graph->VerticesCount());
        for (std::size_t i = 0; i < _matrix.size(); ++i) {
            _matrix[i].resize(_matrix.size(), MAX_INT);
        }

        std::vector<std::pair<std::size_t, int>> next_vertices;
        for (std::size_t i = 0; i < _matrix.size(); ++i) {
            graph->GetNextVertices(i, next_vertices);
            for (std::size_t j = 0; j < next_vertices.size(); ++j) {
                _matrix[i][next_vertices[j].first] = next_vertices[j].second;
            }
        }
    }

    void AddEdge(const std::size_t from, const std::size_t to, const int cost) override {
        _matrix[from][to] = cost;
    }

    std::size_t VerticesCount() const override {
        return _matrix.size();
    }

    int GetCost(const std::size_t from, const std::size_t to) const {
        return _matrix[from][to];
    }

    void GetNextVertices(const std::size_t vertex,
            std::vector<std::pair<std::size_t, int>> &vertices) const override {
        vertices.clear();

        for (int i = 0; i < _matrix[vertex].size(); ++i) {
            if(_matrix[vertex][i] != MAX_INT) {
                std::pair<std::size_t, int> tmp(i, _matrix[vertex][i]);
                vertices.push_back(tmp);
            }
        }
    }
    void GetPrevVertices(const std::size_t vertex,
            std::vector<std::pair<std::size_t, int>>& vertices) const override{
        vertices.clear();

        for (int i = 0; i < _matrix[vertex].size(); ++i) {
            if(_matrix[i][vertex] != MAX_INT) {
                std::pair<std::size_t, int> tmp(i, _matrix[i][vertex]);
                vertices.push_back(tmp);
            }
        }
    }

private:
    std::vector<std::vector<int>> _matrix;
};

void FindMinPaths(const IGraph *graph, std::vector<std::vector<int>> &distances) {
    // Initializing.
    for (std::size_t i = 0; i < graph->VerticesCount(); ++i) {
        for (std::size_t j = 0; j < graph->VerticesCount(); ++j) {
            distances[i][j] = graph->GetCost(i, j);
        }
    }

    // Floid algorithm.
    for (std::size_t k = 0; k < graph->VerticesCount(); ++k) {
        for (std::size_t i = 0; i < graph->VerticesCount(); ++i) {
            for (std::size_t j = 0; j < graph->VerticesCount(); ++j) {
                if (distances[i][k] < MAX_INT && distances[k][j] < MAX_INT)
                    distances[i][j] = std::min(distances[i][j], distances[i][k] + distances[k][j]);
            }
        }
    }
}

int main() {
    std::size_t n = 0;
    std::cin >> n;

    MatrixGraph graph(n);
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            int cost = 0;
            std::cin >> cost;
            graph.AddEdge(i, j, cost);
        }
    }

    std::vector<std::vector<int>> distances(n, std::vector<int>(n));
    FindMinPaths(&graph, distances);

    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            std::cout << distances[i][j] << ' ';
        }
        std::cout << std::endl;
    }

    return 0;
}

/*
IN
4
0 5 9 100
100 0 2 8
100 100 0 7
4 100 100 0
OUT
0 5 7 13
12 0 2 8
11 16 0 7
4 9 11 0
*/