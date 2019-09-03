/*Дан ориентированный граф.
Определите, какое минимальное число ребер необходимо добавить, чтобы граф стал сильносвязным.
В графе возможны петли.
Формат ввода
В первой строке указывается число вершин графа V, во второй – число ребер E, в последующих – E пар вершин, задающих ребра.

Формат вывода
Минимальное число ребер k.*/

#include <iostream>
#include <vector>
#include <unordered_set>
#include <stack>

struct IGraph {
	virtual ~IGraph() = default;

	// Adding edge from->to.
	virtual void AddEdge(const int from, const int to) = 0;

	virtual std::size_t VerticesCount() const = 0;

	// Returns vertices that can be reached by an edge from the current vertex.
	virtual void GetNextVertices(const int vertex, std::vector<int>& vertices) const = 0;
	// Returns vertices that have edge from them to the current vertex.
	virtual void GetPrevVertices(const int vertex, std::vector<int>& vertices) const = 0;

	// Increases number of stored elements.
	virtual void IncreaseNumberOfVertices() = 0;
};

class SetGraph : public IGraph {
public:
	explicit SetGraph(const int vertices_count) {
		_adjacent_vertices.resize(vertices_count);
	}
	explicit SetGraph(const IGraph* graph) {
		_adjacent_vertices.resize(graph->VerticesCount());

		std::vector<int> get_next_vertices;
		for (int i = 0; i < _adjacent_vertices.size(); ++i) {
			graph->GetNextVertices(i, get_next_vertices);

			for (int j = 0; j < get_next_vertices.size(); ++j) {
				_adjacent_vertices[i].insert(get_next_vertices[j]);
			}
		}
	}

	void AddEdge(const int from, const int to) override {
		_adjacent_vertices[from].insert(to);
	}

	std::size_t VerticesCount() const override {
		return _adjacent_vertices.size();
	}

	void GetNextVertices(const int vertex, std::vector<int>& vertices) const override {
		vertices.clear();

		for (auto j = _adjacent_vertices[vertex].cbegin(); j != _adjacent_vertices[vertex].cend(); ++j) {
			vertices.push_back(*j);
		}
	}
	void GetPrevVertices(const int vertex, std::vector<int>& vertices) const override {
		vertices.clear();

		for (int i = 0; i < _adjacent_vertices.size(); ++i) {
			auto found_iterator = _adjacent_vertices[i].find(vertex);

			if (found_iterator != _adjacent_vertices[i].end())
				vertices.push_back(*found_iterator);
		}
	}

	void IncreaseNumberOfVertices() override {
		_adjacent_vertices.resize(_adjacent_vertices.size() + 1);
	}

private:
	std::vector<std::unordered_set<int>> _adjacent_vertices;
};

void StraightDFS(const int vertex, std::vector<char>& used, std::vector<int>& order,
	const IGraph* graph) {
	std::stack<int> dfs_tack, way_stack;

	dfs_tack.push(vertex);
	way_stack.push(vertex);
	used[vertex] = true;

	while (!dfs_tack.empty()) {
		int cur_vertex = dfs_tack.top();
		dfs_tack.pop();

		std::vector<int> next_vertices;
		graph->GetNextVertices(cur_vertex, next_vertices);
		for (int i = 0; i < next_vertices.size(); ++i) {
			int to = next_vertices[i];

			if (!used[to]) {
				dfs_tack.push(to);
				way_stack.push(to);
				used[to] = true;
			}
		}
	}

	while (!way_stack.empty()) {
		int cur_vertex = way_stack.top();
		order.push_back(cur_vertex);
		way_stack.pop();
	}
}

void MainStraightDFS(std::vector<int>& order, const IGraph* graph) {
	std::vector<char> used(graph->VerticesCount(), false);

	for (int i = 0; i < graph->VerticesCount(); ++i) {
		if (!used[i])
			StraightDFS(i, used, order, graph);
	}
}

void TransposedDFS(const int vertex, std::vector<char>& used,
	std::vector<int>& component, const IGraph* graph) {
	std::stack<int> dfs_stack;

	dfs_stack.push(vertex);
	used[vertex] = true;

	while (!dfs_stack.empty()) {
		int cur_vertex = dfs_stack.top();
		dfs_stack.pop();
		component.push_back(cur_vertex);

		std::vector<int> next_vertices;
		graph->GetNextVertices(cur_vertex, next_vertices);
		for (int i = 0; i < next_vertices.size(); ++i) {
			int to = next_vertices[i];

			if (!used[to]) {
				used[to] = true;
				dfs_stack.push(to);
			}
		}
	}
}

void MainTransposedDFS(std::vector<int>& accessory_to_component, std::vector<int>& order,
	const IGraph* reverse_graph, IGraph* condensed_graph) {
	const int vertices_count = reverse_graph->VerticesCount();
	std::vector<char> used(vertices_count, false);
	std::vector<int> component;

	for (int i = vertices_count - 1; i >= 0; --i) {
		int to = order[i];

		if (!used[to]) {
			TransposedDFS(to, used, component, reverse_graph);
			for (int j = 0; j < component.size(); ++j) {
				accessory_to_component[component[j]] = condensed_graph->VerticesCount();
			}

			// Check for uniqueness of the first component.
			// If after first running of TransposedDFS for each index used[index] == true,
			// then the graph is a single component.
			bool only_one_component = false;
			if (i == vertices_count - 1) {
				only_one_component = true;

				for (int j = 0; j < vertices_count; ++j) {
					if (!used[j]) {
						only_one_component = false;
						break;
					}
				}
			}

			if (!only_one_component) {
				condensed_graph->IncreaseNumberOfVertices();
			}

			/*for (int j = 0; j < component.size(); ++j) {
				std::cout << component[j] << ' ';
			}
			std::cout << std::endl;*/

			component.clear();
		}
	}
}

void GetCondencedGraph(std::vector<int>& accessory_to_component, const IGraph* graph,
	IGraph* condensedGraph) {
	const int vertices_count = graph->VerticesCount();

	for (int i = 0; i < vertices_count; ++i) {
		std::vector<int> next_vertices;
		graph->GetNextVertices(i, next_vertices);

		for (int j = 0; j < next_vertices.size(); ++j) {
			int to = next_vertices[j];

			if (accessory_to_component[to] != accessory_to_component[i])
				condensedGraph->AddEdge(accessory_to_component[i], accessory_to_component[to]);
		}
	}
}

int DFSAndSinksCount(const int vertex, std::vector<char>& used,
	std::vector<int>& parent, const IGraph* graph) {
	std::stack<int> dfs_stack;
	int sinks = 0;

	dfs_stack.push(vertex);
	used[vertex] = true;

	while (!dfs_stack.empty()) {
		int cur_vertex = dfs_stack.top();
		dfs_stack.pop();

		std::vector<int> next_vertices;
		graph->GetNextVertices(cur_vertex, next_vertices);
		if (next_vertices.size() == 0) ++sinks;

		for (int i = 0; i < next_vertices.size(); ++i) {
			int to = next_vertices[i];

			if (!used[to]) {
				dfs_stack.push(to);
				parent[to] = cur_vertex;
				used[to] = true;
			}
		}
	}

	return sinks;
}

int MainDFSAndSinksCount(std::vector<int>& parent, const IGraph* condensed_graph) {
	int sinks = 0;
	const int vertices_count = condensed_graph->VerticesCount();
	std::vector<char> used(vertices_count, false);

	for (int i = 0; i < vertices_count; ++i) {
		if (!used[i])
			sinks += DFSAndSinksCount(i, used, parent, condensed_graph);
	}

	return sinks;
}

int SourcesCount(std::vector<int>& parent) {
	int sources = 0;

	for (int i = 0; i < parent.size(); ++i) {
		if (parent[i] == -1) ++sources;
	}

	return sources;
}

int main() {
	int v = 0, e = 0;
	std::cin >> v >> e;

	SetGraph graph(v), reverse_graph(v);
	for (int i = 0; i < e; ++i) {
		int from = 0, to = 0;
		std::cin >> from >> to;
		--from, --to;

		graph.AddEdge(from, to);
		reverse_graph.AddEdge(to, from);
	}

	// Topological sorting, but the vector "order" will contain vertices in reversed order.
	std::vector<int> order;
	MainStraightDFS(order, &graph);

	SetGraph condensed_graph(0);
	std::vector<int> accessory_to_component(v);
	MainTransposedDFS(accessory_to_component, order, &reverse_graph, &condensed_graph);


	// Get condenced graph.
	GetCondencedGraph(accessory_to_component, &graph, &condensed_graph);

	// Determine number of sources and sinks.
	std::vector<int> parent(condensed_graph.VerticesCount(), -1);
	int sinks = 0;
	sinks = MainDFSAndSinksCount(parent, &condensed_graph);

	int sources = SourcesCount(parent);

	sources > sinks ? std::cout << sources : std::cout << sinks;

	return 0;
}

/*
IN
4
4
1 2
2 3
3 1
3 4

OUT
1
---------
IN
5
4
1 2
3 4
4 5
5 3

OUT
2*/