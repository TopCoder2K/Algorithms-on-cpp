#pragma once
#include "GraphInterface.h"
#include <vector>

class MatrixGraph : public IGraph {
public:
	explicit MatrixGraph(const int vertices_count) {
		_matrix.resize(vertices_count);
		for (int i = 0; i < vertices_count; ++i) {
			_matrix[i].resize(vertices_count, false);
		}
	}
	explicit MatrixGraph(const IGraph *graph) {
		_matrix.resize(graph->VerticesCount());
		for (int i = 0; i < _matrix.size(); ++i) {
			_matrix[i].resize(_matrix.size(), false);
		}

		std::vector<int> get_next_vertices;
		for (int i = 0; i < _matrix.size(); ++i) {
			graph->GetNextVertices(i, get_next_vertices);
			for (int j = 0; j < get_next_vertices.size(); ++j) {
				_matrix[i][get_next_vertices[j]] = true;
			}
		}
	}

	void AddEdge(const int from, const int to) override {
		_matrix[from][to] = true;
	}

	std::size_t VerticesCount() const override {
		return _matrix.size();
	}

	void GetNextVertices(const int vertex, std::vector<int>& vertices) const override {
		vertices.clear();

		for (int i = 0; i < _matrix[vertex].size(); ++i) {
			if (_matrix[vertex][i]) vertices.push_back(i);
		}
	}
	void GetPrevVertices(const int vertex, std::vector<int>& vertices) const override {
		vertices.clear();

		for (int i = 0; i < _matrix[vertex].size(); ++i) {
			if (_matrix[i][vertex]) vertices.push_back(i);
		}
	}

private:
	std::vector<std::vector<bool>> _matrix;
};