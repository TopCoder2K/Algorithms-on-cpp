/*Дан базовый интерфейс для представления ориентированного графа:
struct IGraph {
	virtual ~IGraph() {}
	
	virtual void AddEdge(int from, int to) = 0;

	virtual int VerticesCount() const  = 0;

	virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const = 0;
	virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const = 0;
};

Необходимо написать несколько реализаций интерфейса:
ListGraph, хранящий граф в виде массива списков смежности,
MatrixGraph, хранящий граф в виде матрицы смежности,
SetGraph, хранящий граф в виде массива хэш-таблиц,
ArcGraph, хранящий граф в виде одного массива пар {from, to}.
Также необходимо реализовать конструктор, принимающий const IGraph*. Такой конструктор должен скопировать переданный граф в создаваемый объект.
Для каждого класса создавайте отдельные h и cpp файлы.
Число вершин графа задается в конструкторе каждой реализации.

Дополнительно: засеките время получения всех смежных вершин для каждой из вершин для каждой реализации графа.
*/

#include "pch.h"
#include <iostream>
#include "SetGraph.h"
#include "ArcGraph.h"
#include "MatrixGraph.h"
#include "ListGraph.h"
#include <ctime>
#include <vector>
#include <string>

void TimeMeasurement(const IGraph* graph, int vertices, std::string graph_type) {
	std::vector<int> next_vertices;

	unsigned long long time_begin = clock();
	for (int i = 0; i < vertices; ++i) {
		graph->GetNextVertices(i, next_vertices);
	}
	unsigned long long time_end = clock();

	std::cout << "Operation: get next vertices;     Graph type: " << graph_type << 
		"; Time: " << static_cast<double>(time_end - time_begin) / 1000.0 << "\n";

	std::vector<int> previous_vertices;

	time_begin = clock();
	for (int i = 0; i < vertices; ++i) {
		graph->GetPrevVertices(i, previous_vertices);
	}
	time_end = clock();

	std::cout << "Operation: get previous vertices; Graph type: " << graph_type << 
		"; Time: " << static_cast<double>(time_end - time_begin) / 1000.0 << "\n\n";
}

void Testing(int vertices, int edges) {
	SetGraph set_graph(vertices);

	unsigned long long time_begin = clock();
	for (int i = 0; i < edges; ++i) {
		int from = std::rand() % vertices;
		int to = std::rand() % vertices;
		// std::cout << from << ' ' << to << "\n";

		set_graph.AddEdge(from, to);
	}
	ArcGraph arc_graph(&set_graph);
	MatrixGraph matrix_graph(&set_graph);
	ListGraph list_graph(&set_graph);

	unsigned long long time_end = clock();
	std::cout << "Graphs have formed successfully: " << static_cast<double>(time_end - time_begin) / 1000.0 << "\n\n";

	TimeMeasurement(&arc_graph, vertices, "ArcGraph");
	TimeMeasurement(&matrix_graph, vertices, "MatrixGraph");
	TimeMeasurement(&set_graph, vertices, "SetGraph");
	TimeMeasurement(&list_graph, vertices, "ListGraph");
}

int main()
{
	std::cout << "Hello, Graphs!\n";

	int vertices = 0, edges_not_rarefied = 0, edges_rarefied = 0;
	std::cin >> vertices >> edges_not_rarefied >> edges_rarefied;

	// Not rarefied graph.
	Testing(vertices, edges_not_rarefied);
	std::cout << "Not rarefied graph testing is over.\n\n\n\n\n";

	//Rarefied Graph
	Testing(vertices, edges_rarefied);
	std::cout << "Rarefied graph testing is over.\n";

	return 0;
}
