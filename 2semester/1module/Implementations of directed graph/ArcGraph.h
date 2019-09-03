#pragma once
#include "GraphInterface.h"
#include <vector>

class ArcGraph : public IGraph {
public:
	explicit ArcGraph(const int vertices_count) : _vertices_count(vertices_count) {}
	explicit ArcGraph(const IGraph *graph) {
		_vertices_count = graph->VerticesCount();

		std::vector<int> get_next_vertices;
		for (int i = 0; i < _vertices_count; ++i) {
			graph->GetNextVertices(i, get_next_vertices);
			for (int j = 0; j < get_next_vertices.size(); ++j) {
				_edges.push_back(std::make_pair(i, get_next_vertices[j]));
			}
		}
	}

	void AddEdge(const int from, const int to) override {
		_edges.push_back(std::make_pair(from, to));
	}

	std::size_t VerticesCount() const override {
		return _vertices_count;
	}

	void GetNextVertices(const int vertex, std::vector<int>& vertices) const override {
		vertices.clear();

		for (int i = 0; i < _edges.size(); ++i) {
			if (_edges[i].first == vertex) {
				vertices.push_back(_edges[i].second);
			}
		}
	}
	void GetPrevVertices(const int vertex, std::vector<int>& vertices) const override {
		vertices.clear();

		for (int i = 0; i < _edges.size(); ++i) {
			if (_edges[i].second == vertex) {
				vertices.push_back(_edges[i].first);
			}
		}
	}

private:
	std::vector<std::pair<int, int>> _edges;
	std::size_t _vertices_count;
};