/*Необходимо написать торгового советника для поиска арбитража.
Арбитраж - это торговля по цепочке различных валют в надежде заработать на небольших различиях в коэффициентах.
Например, есть следующие курсы валют:
GBP/USD: 0.67
RUB/GBP: 78.66
USD/RUB: 0.02
Имея 1$ и совершив цикл USD->GBP->RUB->USD, получим 1.054$. Таким образом заработав 5.4

Формат ввода
Первая строка содержит число N – количество возможных валют (определяет размер таблицы котировок).
Далее следует построчное представление таблицы. Диагональные элементы (i, i) пропущены (подразумевается,
что курс валюты к себе же 1.0).
В элементе таблицы (i, j) содержится обменный курс i->j.
Если обмен в данном направлении не производится, то -1.
Формат выходных данных.
Выведите YES, если арбитраж есть, и NO, иначе.
*/

#include <iostream>
#include <vector>
#include <queue>

struct IGraph {
    virtual ~IGraph() = default;

    virtual void AddEdge(const std::size_t from, const std::size_t to, const double cost) = 0;

    virtual std::size_t VerticesCount() const = 0;

    virtual void GetNextVertices(const std::size_t vertex, std::vector<std::pair<std::size_t, double>>& vertices) const = 0;
    virtual void GetPrevVertices(const std::size_t vertex, std::vector<std::pair<std::size_t, double>>& vertices) const = 0;
};

// Oriented graph.
class ListGraph : public IGraph {
public:
    explicit ListGraph(const std::size_t VerticesCount) {
        _adjacent_vertices.resize(VerticesCount);
    }
    explicit ListGraph(const IGraph *graph) {
        _adjacent_vertices.resize(graph->VerticesCount());
        for (std::size_t i = 0; i < _adjacent_vertices.size(); ++i) {
            graph->GetNextVertices(i, _adjacent_vertices[i]);
        }
    }

    void AddEdge(const std::size_t from, const std::size_t to, const double cost) override {
        std::pair<std::size_t, double> tmp(to, cost);
        _adjacent_vertices[from].push_back(tmp);
    }

    std::size_t VerticesCount() const override {
        return _adjacent_vertices.size();
    }

    void GetNextVertices(const std::size_t vertex,
                         std::vector<std::pair<std::size_t, double>>& vertices) const override {
        vertices.clear();

        vertices.resize(_adjacent_vertices[vertex].size());
        for (std::size_t i = 0; i < vertices.size(); ++i) {
            vertices[i] = _adjacent_vertices[vertex][i];
        }
    }
    void GetPrevVertices(const std::size_t vertex,
                         std::vector<std::pair<std::size_t, double>>& vertices) const override {
        vertices.clear();

        for (std::size_t i = 0; i < _adjacent_vertices.size(); ++i) {
            for (std::size_t j = 0; j < _adjacent_vertices[i].size(); ++j) {
                if (_adjacent_vertices[i][j].first == vertex)
                    vertices.push_back(std::make_pair(i, _adjacent_vertices[i][j].second));
            }
        }
    }

private:
    std::vector<std::vector<std::pair<std::size_t, double>>> _adjacent_vertices;
};

bool FindTradeArbitrage(const IGraph *graph) {
    // Modified BFS algorithm.
    // Initializing.
    std::vector<double> trades(graph->VerticesCount(), -1.0);

    // Find max path and check for cycle.
    for (std::size_t i = 0; i < trades.size(); ++i)
        if (static_cast<int>(trades[i]) == -1) {
            std::queue<std::size_t> queue;
            trades[i] = 1.0;

            queue.push(i);
            std::size_t counter = 0;

            while (!queue.empty()) {
                const std::size_t cur_vertex = queue.front();
                queue.pop();

                std::vector<std::pair<std::size_t, double>> next_vertices;
                graph->GetNextVertices(cur_vertex, next_vertices);

                for (std::size_t j = 0; j < next_vertices.size(); ++j) {
                    const std::size_t to = next_vertices[j].first;
                    const double cost = next_vertices[j].second;

                    if (trades[to] < trades[cur_vertex] * cost) {
                        trades[to] = trades[cur_vertex] * cost;
                        queue.push(to);

                        ++counter;
                        if (counter == graph->VerticesCount())
                            return true;
                    }
                }
            }
        }

    return false;
}

int main() {
    std::size_t n = 0;
    std::cin >> n;

    ListGraph graph(n);
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            if (j == i) continue;

            double cost = 1.0;
            std::cin >> cost;

            if (static_cast<int>(cost) != -1)
                graph.AddEdge(i, j, cost);
        }
    }

    FindTradeArbitrage(&graph) ? std::cout << "YES" : std::cout << "NO";

    return 0;
}

/*
IN
4
      32.1  1.50 78.66
0.03        0.04  2.43
0.67 21.22       51.89
0.01    -1  0.02

OUT
YES*/