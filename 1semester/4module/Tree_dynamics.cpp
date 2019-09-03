/*Дано невзвешенное дерево.
Расстоянием между двумя вершинами будем называть количество ребер в пути, соединяющем эти две вершины.
Для каждой вершины определите расстояние до самой удаленной от нее вершины.
4_1. Самая удаленная вершина.
Для каждой вершины определите расстояние до самой удаленной от нее вершины. Время работы должно быть O(n).

Формат входных данных:
В первой строке записано количество вершин n ≤ 10000. Затем следует n – 1 строка, описывающая ребра дерева.
Каждое ребро – это два различных целых числа – индексы вершин в диапазоне [0, n–1].
Индекс корня – 0. В каждом ребре родительской вершиной является та, чей номер меньше.
Формат выходных данных:
Выход должен содержать n строк. В i-ой строке выводится расстояние от i-ой вершины до самой удаленной от нее.
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

class Nodes {
public:
	explicit Nodes(const int size);
	~Nodes() = default;

	Nodes& operator=(const Nodes& other) = delete;
	Nodes& operator=(Nodes&& other) = delete;
	Nodes(const Nodes& other) = delete;
	Nodes(Nodes&& other) = delete;

	void Insert(const int a, const int b);
	void FindHeights();
	void FindDistances();
	void PrintDistances() const;
private:
	std::vector<std::vector<int>>_graph;
	std::vector<int> _parents;
	std::vector<int> _degree;
	// _height stores first and second maximum heights and children indices where the heights are reached.
	std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> _height;
	std::vector<int> _distances;
	int _size;
};

Nodes::Nodes(const int size) {
	_size = size;
	_parents.resize(size, -1);
	_degree.resize(size, 0);
	_distances.resize(size, 0);

	// Two pairs consists of vertex and height.
	std::pair<std::pair<int, int>, std::pair<int, int>> double_pair;
	double_pair.first.first = -1, double_pair.first.second = 0;
	double_pair.second.first = -1, double_pair.second.second = 0;

	_height.resize(size, double_pair);
	_graph.resize(size, std::vector<int>());
}

void Nodes::Insert(const int a, const int b) {
	int parent = std::min(a, b);
	int child = std::max(a, b);

	_parents[child] = parent;
	++_degree[child];
	++_degree[parent];
	_graph[parent].push_back(child);
}

// Find two heights (maximum and a little bit less) and nodes, where these heights are achieved, for each node.
void Nodes::FindHeights() {
	std::queue<int> queue;
	// Find all leaves.
	for (int i = 0; i < _size; ++i) {
		if (_degree[i] == 1)
			queue.push(i);
	}

	// It seems like DFS but we go from the leaves, not from the root.
	// We "delete" current leaves and try to add their parents as new leaves.
	while (!queue.empty()) {
		int current_index = queue.front(), parent_index = _parents[current_index];
		queue.pop();

		// Not a root case.
		if (current_index != 0) {
			--_degree[current_index];
			--_degree[parent_index];

			if (_height[current_index].first.second + 1 > _height[parent_index].first.second) {
				_height[parent_index].second.second = _height[parent_index].first.second;
				_height[parent_index].second.first = _height[parent_index].first.first;
				_height[parent_index].first.second = _height[current_index].first.second + 1;
				_height[parent_index].first.first = current_index;
			}
			else if (_height[current_index].first.second + 1 > _height[parent_index].second.second) {
				_height[parent_index].second.second = _height[current_index].first.second + 1;
				_height[parent_index].second.first = current_index;
			}

			// Do not forget to add parent as a leaf when parent degree is 1.
			if (_degree[parent_index] == 1) {
				queue.push(parent_index);
			}
		}
		// In a root case we do nothing.
	}
}

void Nodes::FindDistances() {
	// A root case.
	_distances[0] = _height[0].first.second;

	// Max distance is either height (_height[index].first.second) or parent other way + 1 in case if a node is in the deepest branch.
	std::queue<std::pair<int, int>> queue;
	std::pair<int, int> child_and_other_branch_dist = std::make_pair(0, _height[0].second.second);

	queue.push(child_and_other_branch_dist);
	while (!queue.empty()) {
		child_and_other_branch_dist = queue.front();
		queue.pop();

		int parent_index = child_and_other_branch_dist.first, other_branch_dist = child_and_other_branch_dist.second;
		for (int i = 0; i < _graph[parent_index].size(); ++i) {
			int child_index = _graph[parent_index][i];

			// The node is in the deepest branch.
			// But we should consider an one important moment: other_branch_dist contains either a parent of the node or a node's child in an other branch.
			if (child_index == _height[parent_index].first.first) {
				_distances[child_index] = std::max(other_branch_dist + 1, _height[child_index].first.second);
				child_and_other_branch_dist.second = std::max(other_branch_dist + 1, _height[child_index].second.second);
			}
			else { // Otherwise we just increase parent distance.
				_distances[child_index] = _distances[parent_index] + 1;
				child_and_other_branch_dist.second = _distances[parent_index] + 1;
			}

			child_and_other_branch_dist.first = child_index;
			queue.push(child_and_other_branch_dist);
		}
	}
}

void Nodes::PrintDistances() const {
	for (int i = 0; i < _size; ++i) {
		std::cout << _distances[i] << std::endl;
	}
}

int main() {
	int n = 0;
	std::cin >> n;

	Nodes nodes(n);
	for (int i = 0; i < n - 1; ++i) {
		int tmp1 = 0, tmp2 = 0;
		std::cin >> tmp1 >> tmp2;
		nodes.Insert(tmp1, tmp2);
	}

	nodes.FindHeights();
	nodes.FindDistances();
	nodes.PrintDistances();

	return 0;
}

/*
IN
3
0 1
1 2
OUT
2
1
2
----------
IN
6
0 1
0 2
1 3
1 4
3 5
OUT
3
2
4
3
3
4
----------
IN
7
0 1
1 2
1 3
1 4
2 5
3 6
OUT
3
2
3
3
3
4
4
*/