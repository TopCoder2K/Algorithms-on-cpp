/*Дан невзвешенный неориентированный граф. Найдите цикл минимальной длины.
Ввод: v:кол-во вершин(макс. 50000), n:кол-во ребер(макс. 200000), n пар реберных вершин
Вывод: одно целое число равное длине минимального цикла. Если цикла нет, то вывести -1.
*/

#include <iostream>
#include <vector>
#include <queue>
#define _CRT_INT_MAX 2147483647

struct IGraph {
	virtual ~IGraph() = default;

	// Adding edge from -> to.
	virtual void AddEdge(int from, int to) = 0;

	virtual std::size_t VerticesCount() const = 0;

	virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const = 0;
	virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const = 0;
};

class ListGraph : public IGraph {
public:
	explicit ListGraph(const int vertices_count) {
		_adjacent_vertices.resize(vertices_count);
	}
	explicit ListGraph(const IGraph* graph) {
		_adjacent_vertices.resize(graph->VerticesCount());
		for (int i = 0; i < _adjacent_vertices.size(); ++i) {
			graph->GetNextVertices(i, _adjacent_vertices[i]);
		}
	}

	void AddEdge(const int from, const int to) override {
		_adjacent_vertices[from].push_back(to);
	}

	std::size_t VerticesCount() const override {
		return _adjacent_vertices.size();
	}

	void GetNextVertices(const int vertex, std::vector<int>& vertices) const override {
		vertices.clear();
		vertices.resize(_adjacent_vertices[vertex].size());

		for (int i = 0; i < vertices.size(); ++i) {
			vertices[i] = _adjacent_vertices[vertex][i];
		}
	}
	void GetPrevVertices(const int vertex, std::vector<int>& vertices) const override {
		vertices.clear();

		for (int i = 0; i < _adjacent_vertices.size(); ++i) {
			for (int j = 0; j < _adjacent_vertices[i].size(); ++j) {
				if (_adjacent_vertices[i][j] == vertex)
					vertices.push_back(i);
			}
		}
	}

private:
	std::vector<std::vector<int>> _adjacent_vertices;
};

int BFS(const IGraph* graph, int from) {
	int cur_min_cycle = _CRT_INT_MAX;
	std::vector<char> used(graph->VerticesCount(), 0);
	std::vector<int> dist(graph->VerticesCount(), _CRT_INT_MAX);
	std::vector<int> parent(graph->VerticesCount(), -1);
	std::queue<int> bfs_queue;

	bfs_queue.push(from);
	used[from] = 1;
	dist[from] = 0;
	while (!bfs_queue.empty()) {
		int cur_vertex = bfs_queue.front();
		bfs_queue.pop();

		std::vector<int> next_vertices;
		graph->GetNextVertices(cur_vertex, next_vertices);
		for (int i = 0; i < next_vertices.size(); ++i) {
			int to = next_vertices[i];

			if (!used[to]) {
				bfs_queue.push(to);
				dist[to] = dist[cur_vertex] + 1;
				used[to] = 1;
				parent[to] = cur_vertex;
			}
			else if (to != parent[cur_vertex]) {
				if (cur_min_cycle > dist[to] + dist[cur_vertex] + 1)
					cur_min_cycle = dist[to] + dist[cur_vertex] + 1;
			}
		}
	}

	return cur_min_cycle;
}

int FindMinCycle(const IGraph* graph) {
	int min_cycle_length = _CRT_INT_MAX;

	for (std::size_t i = 0; i < graph->VerticesCount(); ++i) {
		int cur_min_cycle = BFS(graph, i);

		if (cur_min_cycle < min_cycle_length)
			min_cycle_length = cur_min_cycle;
	}

	return min_cycle_length;
}

int main() {
	int v = 0, n = 0;
	std::cin >> v >> n;

	ListGraph list_graph(v);

	for (int i = 0; i < n; ++i) {
		int from = 0, to = 0;
		std::cin >> from >> to;

		list_graph.AddEdge(from, to);
		list_graph.AddEdge(to, from);
	}

	int min_cycle_length = _CRT_INT_MAX;
	min_cycle_length = FindMinCycle(&list_graph);

	min_cycle_length == _CRT_INT_MAX ? std::cout << -1 : std::cout << min_cycle_length;

	return 0;
}

/*
IN
6
6
0 1
0 2
2 3
2 4
3 5
4 5
OUT
4*/