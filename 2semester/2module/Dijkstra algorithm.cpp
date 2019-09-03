/*Требуется отыскать самый выгодный маршрут между городами.
Требуемое время работы O((N+M)logN), где N-количество городов, M-известных дорог между ними.
Оптимизируйте ввод.

Формат входных данных.
Первая строка содержит число N – количество городов.
Вторая строка содержит число M - количество дорог.
Каждая следующая строка содержит описание дороги (откуда, куда, время в пути).
Последняя строка содержит маршрут (откуда и куда нужно доехать).
Формат выходных данных.
Вывести длину самого выгодного маршрута.
*/

#include <iostream>
#include <vector>
#include <set>
#define MAX_INT 2147483648

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

// Needs operator < for type!
template <class Type>
class Comparator {
public:
	bool operator()(const std::pair<std::size_t, Type>& a, const std::pair<std::size_t, Type>& b) const {
		if (a.second == b.second) {
			return a.first < b.first;
		}
		else {
			return a.second < b.second;
		}
	}
};

std::size_t FindMinPath(const std::size_t start, const std::size_t end, const IGraph* graph) {
	// Dijkstra algorithm.
	std::set<std::pair<std::size_t, std::size_t>, Comparator<std::size_t>> min_tree;
	std::vector<std::size_t> cur_min_path(graph->VerticesCount(), MAX_INT);

	cur_min_path[start] = 0;
	for (std::size_t i = 0; i < graph->VerticesCount(); ++i) {
		min_tree.insert(std::make_pair(i, cur_min_path[i]));
	}

	std::pair<std::size_t, std::size_t> tmp;
	tmp.first = MAX_INT;
	while (tmp.first != end) {
		tmp = *min_tree.begin();
		min_tree.erase(tmp);

		std::vector<std::pair<std::size_t, std::size_t>> next_vertices;
		graph->GetNextVertices(tmp.first, next_vertices);

		for (std::size_t i = 0; i < next_vertices.size(); ++i) {
			std::size_t to = next_vertices[i].first, cost = next_vertices[i].second;

			if (tmp.second + cost < cur_min_path[to]) {
				std::pair<std::size_t, std::size_t> cur_vertex(to, cur_min_path[to]);
				min_tree.erase(cur_vertex);

				cur_vertex.second = cur_min_path[to] = tmp.second + cost;
				min_tree.insert(cur_vertex);
			}
		}
	}

	return cur_min_path[end];
}

int main() {
	std::size_t n = 0, m = 0;
	std::cin >> n >> m;

	ListGraph graph(n);
	for (std::size_t i = 0; i < m; ++i) {
		std::size_t from, to, cost;
		std::cin >> from >> to >> cost;

		graph.AddEdge(from, to, cost);
	}

	std::size_t start = 0, end = 0;
	std::cin >> start >> end;

	std::cout << FindMinPath(start, end, &graph);
	return 0;
}

/*
IN
6
9
0 3 1
0 4 2
1 2 7
1 3 2
1 4 3
1 5 3
2 5 3
3 4 4
3 5 6
0 2
OUT
9*/