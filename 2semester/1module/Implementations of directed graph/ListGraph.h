#pragma once
#include "GraphInterface.h"
#include <vector>

class ListGraph : public IGraph {
public:
	explicit ListGraph(const int vertices_count) {
		_adjacent_vertices.resize(vertices_count);
	}
	explicit ListGraph(const IGraph *graph) {
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