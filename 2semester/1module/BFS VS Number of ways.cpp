/*Дан невзвешенный неориентированный граф. В графе может быть несколько кратчайших путей между какими-то вершинами.
Найдите количество различных кратчайших путей между заданными вершинами. Требуемая сложность O(V+E).
Ввод: v:кол-во вершин(макс. 50000), n:кол-во ребер(макс. 200000), n пар реберных вершин, пара вершин v, w для запроса.
Вывод:количество кратчайших путей от v к w
*/

#include <iostream>
#include <vector>
#include <queue>

struct IGraph {
	virtual ~IGraph() = default;

	// Adding edge from->to.
	virtual void AddEdge(const int from, const int to) = 0;

	virtual std::size_t VerticesCount() const = 0;

	// Returns vertices that can be reached by an edge from the current vertex.
	virtual void GetNextVertices(const int vertex, std::vector<int>& vertices) const = 0;
	// Returns vertices that have edge from them to the current vertex.
	virtual void GetPrevVertices(const int vertex, std::vector<int>& vertices) const = 0;
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

int BFS(const IGraph* graph, int from, int to)
{
	std::vector<char> used(graph->VerticesCount(), 0);
	std::vector<int> dist(graph->VerticesCount(), INT_MAX);
	std::vector<int> ways_count(graph->VerticesCount(), 0);
	std::queue<int> bfs_queue;

	bfs_queue.push(from);
	used[from] = 1;
	dist[from] = 0;
	ways_count[from] = 1;
	while (!bfs_queue.empty()) {
		int cur_vertex = bfs_queue.front();
		bfs_queue.pop();

		std::vector<int> next_vertices;
		graph->GetNextVertices(cur_vertex, next_vertices);
		for (int i = 0; i < next_vertices.size(); ++i) {
			int next_vertex = next_vertices[i];

			if (!used[next_vertex]) {
				bfs_queue.push(next_vertex);
				dist[next_vertex] = dist[cur_vertex] + 1;
				ways_count[next_vertex] = ways_count[cur_vertex];
				used[next_vertex] = 1;
			}
			else if (dist[next_vertex] == dist[cur_vertex] + 1) {
				ways_count[next_vertex] += ways_count[cur_vertex];
			}
		}
	}

	return ways_count[to];
}

int main() {
	int v = 0, n = 0;
	std::cin >> v >> n;
	ListGraph list_graph(v);

	int from = 0, to = 0;
	for (int i = 0; i < n; ++i) {
		std::cin >> from >> to;

		list_graph.AddEdge(from, to);
		list_graph.AddEdge(to, from);
	}

	std::cin >> from >> to;

	int ways_count = 0;
	ways_count = BFS(&list_graph, from, to);
	std::cout << ways_count;

	return 0;
}

/*
IN
4
5
0 1
0 2
1 2
1 3
2 3

0 3
OUT
2
*/