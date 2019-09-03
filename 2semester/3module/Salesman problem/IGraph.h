#ifndef SALESMANPROBLEM_2_IGRAPH_H
#define SALESMANPROBLEM_2_IGRAPH_H

struct IGraph {
    virtual ~IGraph() = default;

    virtual void AddEdge(const uint32_t from, const uint32_t to, const double cost) = 0;
    virtual double GetCost(const uint32_t from, const uint32_t to) const = 0;

    virtual uint32_t VerticesCount() const = 0;

    virtual void GetNextVertices(const uint32_t vertex, std::vector<uint32_t> &vertices) const = 0;
};

#endif //SALESMANPROBLEM_2_IGRAPH_H
