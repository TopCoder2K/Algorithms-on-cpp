#pragma once
#include <vector>

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
