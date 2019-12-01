/* Даны n точек в пространстве. Никакие 4 точки не лежат в одной полуплоскости.
 * Найдите выпуклую оболочку этих точек за O(n*log(n)).
 *
 * Формат ввода
 * Первая строка содержит число m - количество тестов. В последующих строках описаны сами тесты.
 * Каждый тест начинается со строки, содержащей n (n ≤ 1000) - число точек.
 * Далее, в n строках даны по три числа - координаты точек. Все координаты целые, не превосходят по модулю 500.
 *
 * Формат вывода
 * Для каждого теста выведите следующее. В первую строку выведите количество граней m.
 * Далее в последующие m строк выведите описание граней:
 * количество точек грани (=3) и номера точек в исходном множестве.
 * Точки нумеруются в том же порядке, в котором они даны во входном файле.
 * Точки в пределах грани должны быть отсортированы
 * в порядке против часовой стрелки относительно внешней нормали к грани.
 * Первая точка – точка с минимальным номером. Порядок граней лексикографический.*/

#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <cassert>



constexpr double epsilon = 1e-9;

// We will have only radius vectors.
struct Vector3D {
    Vector3D(double x1, double y1, double z1) : x(x1), y(y1), z(z1) {}
    Vector3D() : Vector3D(0.0, 0.0, 0.0) {}

    Vector3D operator/(const double divider) const {
        return Vector3D((*this).x / divider, (*this).y / divider, (*this).z / divider);
    }
    Vector3D operator*(const double multiplier) const {
        return Vector3D((*this).x * multiplier, (*this).y * multiplier, (*this).z * multiplier);
    }
    Vector3D operator+(const Vector3D& other) const {
        return Vector3D((*this).x + other.x, (*this).y + other.y, (*this).z + other.z);
    }
    Vector3D operator-() const {
        return Vector3D(-x, -y, -z);
    }
    Vector3D operator-(const Vector3D& other) const {
        return -other + *this;
    }

    bool operator<(const Vector3D& rhs) const {
        if (z == rhs.z) {
            if (y == rhs.y) {
                return x < rhs.x;
            }
            return y < rhs.y;
        }
        return z < rhs.z;
    }

    double Distance(const Vector3D& other) const {
        return sqrt((other.x - x) * (other.x - x) + (other.y - y) * (other.y - y) + (other.z - z) * (other.z - z));
    }
    double Length() const {
        return Distance(Vector3D(0.0, 0.0, 0.0));
    }

    double DotProduct(const Vector3D& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    Vector3D CrossProduct(const Vector3D& other) const {
        return Vector3D(y * other.z - z * other.y, other.x * z - x * other.z, x * other.y - other.x * y);
    }


    double x, y, z;
};
//-----------------------------------------------------------------------------------------

// Because convex hull always consists of triangles (due to the task),
// it's convenient to store them by one diagonal and vertices opposite to it.
// So the first one is Edge structure.
struct Edge {
    Edge(int32_t index1, int32_t index2) : index1_(index1), index2_(index2) {}
    Edge() : index1_(-1), index2_(-1) {}

    bool operator==(const Edge& rhs) const {
        return (index1_ == rhs.index1_) && (index2_ == rhs.index2_);
    }

    int32_t index1_, index2_;
};

template<>
struct std::hash<Edge> {
    size_t operator()(const Edge& edge) const {
        return (std::hash<int32_t>()(edge.index1_)) ^ (std::hash<int32_t>()(edge.index2_));
    }
};
//-----------------------------------------------------------------------------------------

struct Face {
    Face(int32_t first_i, int32_t second_i, int32_t third_i) noexcept : first_(first_i), second_(second_i), third_(third_i) {}
    Face() : Face(-1, -1, -1) {}

    bool operator<(const Face& rhs) const noexcept {
        if (first_ == rhs.first_) {
            if (second_ == rhs.second_) {
                return third_ < rhs.third_;
            }
            return second_ < rhs.second_;
        }
        return first_ < rhs.first_;
    }
    bool operator==(const Face& rhs) const noexcept {
        return first_ == rhs.first_ && second_ == rhs.second_ && third_ == rhs.third_;
    }

    void InitializeFace(const std::vector<Vector3D>& points, int32_t first_i, int32_t second_i, int32_t third_i);

    Edge GetFirstEdge() const noexcept { return Edge(first_, second_); }
    Edge GetSecondEdge() const  noexcept { return Edge(second_, third_); }
    Edge GetThirdEdge() const noexcept { return Edge(third_, first_); }

    void Sort() noexcept {
        if (second_ < third_ && second_ < first_) {
            std::swap(first_, second_);
            std::swap(second_, third_);
        } else if (third_ < second_ && third_ < first_) {
            std::swap(first_, third_);
            std::swap(second_, third_);
        }
    }

    int32_t first_;
    int32_t second_;
    int32_t third_;

    friend std::hash<Face>;
};

// Make the counter-clockwise orientation for the first face.
void Face::InitializeFace(const std::vector<Vector3D>& points, int32_t first_i, int32_t second_i, int32_t third_i) {
    // Sort indices.
    Face tmp_face(first_i, second_i, third_i);
    tmp_face.Sort();
    first_i = tmp_face.first_;
    second_i = tmp_face.second_;
    third_i = tmp_face.third_;

    Vector3D basis1(points[second_i] - points[first_i]);
    Vector3D basis2(points[third_i] - points[second_i]);

    // Calculate the normal vector to the face.
    Vector3D normal_vector = basis1.CrossProduct(basis2);
    normal_vector = normal_vector / normal_vector.Length();

    // Find the right direction of the normal vector (inside of a hull).
    // And check if face contains points in counter-clockwise order.
    int32_t different_point_i = -1;
    int32_t size = points.size();
    for (int32_t i = 0; i < size; ++i) {
        if (i != first_i && i != second_i && i != third_i) {
            different_point_i = i;
            break;
        }
    }

    bool counter_clockwise = true;
    if (normal_vector.DotProduct(points[different_point_i] - points[first_i]) > -epsilon) {
        normal_vector = -normal_vector;
        counter_clockwise = false;
    }

    // Change order if not.
    if (!counter_clockwise) {
        first_ = first_i;
        second_ = third_i;
        third_ = second_i;
    } else {
        first_ = first_i;
        second_ = second_i;
        third_ = third_i;
    }
}

template<>
struct std::hash<Face> {
    size_t operator()(const Face& face) const {
        return (std::hash<int32_t>()(face.first_)) ^
               (std::hash<int32_t>()(face.second_) ^
                (std::hash<int32_t>()(face.third_)));
    }
};
//-------------------------------------------------------------------------------------------------------

int32_t find_first_point(const std::vector<Vector3D>& points) {
    int32_t min_i = 0, size = points.size();

    for (int32_t i = 1; i < size; ++i) {
        if (points[i] < points[min_i]) {
            min_i = i;
        }
    }

    return min_i;
}

int32_t find_second_point(const std::vector<Vector3D>& points, int32_t first_i) {
    int32_t size = points.size();
    int32_t second_i = -1;
    double max_cos = -2.0;

    for (int32_t i = 0; i < size; ++i) {
        if (i != first_i) {
            Vector3D diff(points[i] - points[first_i]);
            double cur_cos = sqrt(diff.x * diff.x + diff.y * diff.y) / diff.Length();

            // We have to find a point with the minimum cosine (=> with the minimum angle).
            if (max_cos < cur_cos) {
                max_cos = cur_cos;
                second_i = i;
            }
        }
    }

    return second_i;
}

int32_t find_point(const std::vector<Vector3D>& points, int32_t first_i, int32_t second_i, int32_t third_i = -1) {
    int32_t size = points.size();
    int32_t new_point = -1;
    Vector3D edge_vector(points[second_i] - points[first_i]);

    // We will rotate the vector to a point and check a direction of the rotation.
    for (int32_t i = 0; i < size; ++i) {
        if (i != first_i && i != second_i && i != third_i) {
            if (new_point == -1) {
                new_point = i;
                continue;
            }

            Vector3D cur_outer(points[new_point] - points[first_i]);
            Vector3D cur_test(points[i] - points[first_i]);

            Vector3D normal_vector = cur_outer.CrossProduct(cur_test);
            if (normal_vector.DotProduct(edge_vector) > -epsilon) {
                new_point = i;
            }
        }
    }

    assert(new_point != -1);

    return new_point;
}
//-------------------------------------------------------------------------------------------------------

void insert_edges(std::unordered_set<Edge>& processed_edges, const Face& face) {
    processed_edges.insert(face.GetFirstEdge());
    processed_edges.insert(face.GetSecondEdge());
    processed_edges.insert(face.GetThirdEdge());
}

Face TryToAddNewFace(std::unordered_set<Edge>& processed_edges,
        const std::vector<Vector3D>& points,
        Edge edge,
        int32_t third_i) {
    std::swap(edge.index1_, edge.index2_);
    Face result_face;

    if (!processed_edges.count(edge)) {
        int32_t point_to_add_i = find_point(points, edge.index1_, edge.index2_, third_i);

        result_face = Face(edge.index1_, edge.index2_, point_to_add_i);
        result_face.Sort();
    }

    return result_face;
}

std::vector<Face> FindConvexHull(const std::vector<Vector3D>& points) {
    // Make first face.
    int32_t first_i = find_first_point(points);
    int32_t second_i = find_second_point(points, first_i);
    int32_t third_i = find_point(points, first_i, second_i);

    // Make the necessary orientation.
    Face initial_face;
    initial_face.InitializeFace(points, first_i, second_i, third_i);


    // Start wrapping.
    std::queue<Face> queue;
    queue.push(initial_face);
    std::unordered_set<Edge> processed_edges;
    initial_face.Sort();
    insert_edges(processed_edges, initial_face);
    // Faces won't repeat during the pass, so we can store them in the vector.
    std::vector<Face> faces;

    while(!queue.empty()) {
        Face cur_face = queue.front();
        queue.pop();
        faces.push_back(cur_face);

        Edge edge1 = cur_face.GetFirstEdge();
        Edge edge2 = cur_face.GetSecondEdge();
        Edge edge3 = cur_face.GetThirdEdge();

        Face tmp_face;
        Face tmp_result;

        tmp_result = TryToAddNewFace(processed_edges, points, edge1, edge2.index2_);
        if (!(tmp_result == tmp_face)) {
            queue.push(tmp_result);
            insert_edges(processed_edges, tmp_result);
        }
        tmp_result = TryToAddNewFace(processed_edges, points, edge2, edge3.index2_);
        if (!(tmp_result == tmp_face)) {
            queue.push(tmp_result);
            insert_edges(processed_edges, tmp_result);
        }
        tmp_result = TryToAddNewFace(processed_edges, points, edge3, edge1.index2_);
        if (!(tmp_result == tmp_face)) {
            queue.push(tmp_result);
            insert_edges(processed_edges, tmp_result);
        }
    }

    // Make the necessary order.
    std::sort(faces.begin(), faces.end(), std::less<Face>());

    return faces;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int32_t m;
    std::cin >> m;

    for (int32_t i = 0; i < m; ++i) {
        int32_t n = 0;
        std::cin >> n;

        std::vector<Vector3D> points(n);
        for (int32_t j = 0; j < n; ++j) {
            double x, y, z;
            std::cin >> x >> y >> z;
            points[j] = Vector3D(x, y, z);
        }

        std::vector<Face> faces = FindConvexHull(points);

        int32_t size = faces.size();
        std::cout << size << std::endl;

        const int32_t number_of_points_in_face = 3;
        for (const auto& item : faces) {
            std::cout << number_of_points_in_face << ' '
                      << item.first_ << ' ' << item.second_ << ' ' << item.third_ << std::endl;
        }
    }

    return 0;
}

/*
IN
1
4
0 0 0
1 0 0
0 1 0
0 0 1
OUT
4
3 0 1 3
3 0 2 1
3 0 3 2
3 1 2 3
-----------
IN
1
5
1 0 0
0 1 0
0 0 1
-1 0 0
0 -1 1
OUT
6
3 0 1 2
3 0 2 4
3 0 3 1
3 0 4 3
3 1 3 2
3 2 3 4
-----------
IN
1
6
1 0 0
0 1 0
0 0 1
-1 0 0
0 -1 1
0 2 2
OUT
6
3 0 1 5
3 0 3 1
3 0 4 3
3 0 5 4
3 1 3 5
3 3 4 5
*/