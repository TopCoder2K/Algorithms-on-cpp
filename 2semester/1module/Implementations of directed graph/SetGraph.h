#pragma once
#include "GraphInterface.h"
#include <vector>
#include <unordered_set>

class SetGraph : public IGraph {
public:
	explicit SetGraph(const int vertices_count) {
		_adjacent_vertices.resize(vertices_count);
	}
	explicit SetGraph(const IGraph *graph) {
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

private:
	std::vector<std::unordered_set<int>> _adjacent_vertices;
};