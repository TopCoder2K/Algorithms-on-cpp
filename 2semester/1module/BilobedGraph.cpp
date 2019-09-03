/*Дан невзвешенный неориентированный граф. Определить, является ли он двудольным. Требуемая сложность O(V+E).
Ввод: v:кол-во вершин(макс. 50000), n:кол-во ребер(макс. 200000), n пар реберных вершин.
Вывод: YES если граф является двудольным, NO - если не является.
*/

#include <iostream>
#include <vector>
#include <queue>
#define _CRT_INT_MAX 2147483647

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

void BFS(std::size_t vertex, const IGraph *graph, std::vector<char>& part, bool& is_bilobed)
{
	std::vector<int> parent(graph->VerticesCount(), -1);
	std::queue<int> bfs_queue;

	bfs_queue.push(vertex);

	while (!bfs_queue.empty()) {
		std::size_t cur_vertex = bfs_queue.front();
		bfs_queue.pop();

		std::vector<int> next_vertices;
		graph->GetNextVertices(cur_vertex, next_vertices);
		for (std::size_t i = 0; i < next_vertices.size(); ++i) {
			std::size_t to = next_vertices[i];

			if (static_cast<char>(-1) == part[to]) {
				bfs_queue.push(to);
				part[cur_vertex] == 0 ? part[to] = 1 : part[to] = 0;
				parent[to] = cur_vertex;
			}
			else if (part[to] == part[cur_vertex]) {
				is_bilobed = false;
				break;
			}
		}
	}
}

bool DivisionIntoParts(const IGraph *graph) {
	bool is_bilobed = true;
	std::vector<char> part(graph->VerticesCount(), -1);

	for (std::size_t i = 0; i < graph->VerticesCount(); ++i) {
		if (part[i] == -1) {
			BFS(i, graph, part, is_bilobed);

			if (!is_bilobed) break;
		}
	}

	return is_bilobed;
}

int main() {
	std::size_t v = 0, n = 0;
	std::cin >> v >> n;
	ListGraph list_graph(v);

	for (std::size_t i = 0; i < n; ++i) {
		std::size_t from = 0, to = 0;
		std::cin >> from >> to;

		list_graph.AddEdge(from, to);
		list_graph.AddEdge(to, from);
	}

	bool is_bilobed = true;
	is_bilobed = DivisionIntoParts(&list_graph);

	is_bilobed ? std::cout << "YES" : std::cout << "NO";

	return 0;
}

/*
IN
3
3
0 1
1 2
0 2

OUT
NO
--------
IN
5
6
0 1
0 2
0 3
1 4
2 4
3 4

OUT
YES
*/