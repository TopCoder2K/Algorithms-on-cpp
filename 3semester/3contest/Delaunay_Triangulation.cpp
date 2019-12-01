/* Даны точки, никакие 3 из которых не лежат на одной прямой.
 * Никакие 4 точки не лежат на одной окружности.
 * Кроме того, все точки имеют различные x-координаты.
 * Определите среднее число сторон в многоугольниках диаграммы Вороного этого множества точек.
 * Считаются только конечные многоугольники.
 * Если все многоугольники неограниченны, ответ полагается равным 0.
 * Число точек n ≤ 100000. Алгоритм должен иметь асимптотику O(n log n).
 *
 * Формат ввода
 * В каждой строке через пробел записаны действительные координаты точек xi yi.
 *
 * Формат вывода
 * Число - среднее число сторон в ограниченных многоугольниках диаграммы Вороного с точностью 10^(-6).
 * Если таких многоугольников нет - ответ 0.*/

#include <iostream>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <cassert>


// There might be some problems but it's precise enough.
constexpr double epsilon = 1e-12;

struct RadiusVector2D {
    RadiusVector2D(double x1, double y1) : x(x1), y(y1) {}
    RadiusVector2D() : x(0.0), y(0.0) {}

    RadiusVector2D operator/(const int32_t divider) const {
        return RadiusVector2D((*this).x / divider, (*this).y / divider);
    }
    RadiusVector2D operator*(const int32_t multiplier) const {
        return RadiusVector2D((*this).x * multiplier, (*this).y * multiplier);
    }
    RadiusVector2D operator+(const RadiusVector2D& rhs) const {
        return RadiusVector2D((*this).x + rhs.x, (*this).y + rhs.y);
    }
    RadiusVector2D operator-(const RadiusVector2D& rhs) const {
        return RadiusVector2D(x - rhs.x, y - rhs.y);
    }
    RadiusVector2D operator-() const {
        return RadiusVector2D(-x, -y);
    }

    bool operator<(const RadiusVector2D& rhs) const {
        if (x < rhs.x)
            return true;
        if (rhs.x < x)
            return false;
        return y < rhs.y;
    }

    double Distance(const RadiusVector2D& other) const {
        return sqrt((other.x - x) * (other.x - x) + (other.y - y) * (other.y - y));
    }
    double Length() const {
        return Distance(RadiusVector2D(0.0, 0.0));
    }
    // As we are on the plane, we can only get a module and a sign of a cross product.
    double CrossProduct(const RadiusVector2D& other) const {
        return (x * other.y - other.x * y);
    }
    double DotProduct(const RadiusVector2D& other) const {
        return (x * other.x + y * other.y);
    }

    double x, y;
};
//------------------------------------------------------------------------------------------------------

// Because in triangulation quadrangles are considered during reconstructing,
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

// And the second one.
struct OppositeVertices {
    OppositeVertices(int32_t index1, int32_t index2) : index1_(index1), index2_(index2) {}
    OppositeVertices() : index1_(-1), index2_(-1) {}

    int32_t Size() const {
        if (index1_ == -1 && index2_ == -1) return 0;
        if (index1_ == -1 || index2_ == -1) return 1;
        return 2;
    }

    void Insert(int32_t index) {
        // It's possible to insert equal indices, so we have to take it into account.
        if (index1_ == index || index2_ == index) return;
        // As we will insert only into incomplete quadrangles.
        assert(Size() < 2);

        (index1_ == -1 ? index1_ : index2_) = index;
    }

    int32_t Min() const {
        assert(index1_ != -1 || index2_ != -1);

        if (index1_ != -1 && index2_ != -1)
            return std::min(index1_, index2_);

        return index1_ != -1 ? index1_ : index2_;
    }
    // For testing!
    int32_t Max() const {
        assert(index1_ != -1 || index2_ != -1);

        return std::max(index1_, index2_);
    }

    void Replace(int32_t old_index, int32_t new_index) {
        if (index1_ == old_index) {
            index1_ = new_index;
        } else if (index2_ == old_index) {
            index2_ = new_index;
        } else {
            Insert(new_index);
        }
    }

    int32_t index1_, index2_;
};
//------------------------------------------------------------------------------------------------------

// In our implementation triangulation consists of more than 2 points.
class DelaunayTriangulation {
public:
    DelaunayTriangulation(std::vector<RadiusVector2D>& points);

    auto begin() { return triangulation_.begin(); }
    auto end() { return triangulation_.end(); }

    void AddPoint(int32_t new_point_i);
    void FixTriangulation(int32_t edge_left_i, int32_t edge_right_i, int32_t point_to_be_added_i);
    bool LocalDelaunayCriterion(int32_t edge_left_i, int32_t edge_right_i, int32_t inner_i, int32_t point_to_be_added_i) const;
    std::unordered_set<int32_t> GetConvexHull() const;

    int64_t Size() { return points_.size(); }

private:
    struct ListNode_{
        ListNode_() : left_(-1), right_(-1) {}
        ListNode_(int32_t left, int32_t right) : left_(left), right_(right) {}

        int32_t left_, right_;
    };

    std::unordered_map<Edge, OppositeVertices> triangulation_;
    std::vector<RadiusVector2D> points_;
    std::vector<ListNode_> convex_hull_;
};

std::unordered_set<int32_t> DelaunayTriangulation::GetConvexHull() const {
    std::unordered_set<int32_t> convex_hull;
    int32_t start_point = 0;
    int32_t next_point = convex_hull_[start_point].right_;

    // "Counter-clockwise" bypass.
    convex_hull.insert(start_point);
    while (next_point != start_point) {
        convex_hull.insert(next_point);
        next_point = convex_hull_[next_point].right_;
    }

    return convex_hull;
}

bool DelaunayTriangulation::LocalDelaunayCriterion(
        int32_t edge_left_i,
        int32_t edge_right_i,
        int32_t inner_i,
        int32_t point_to_be_added_i
) const {
    // Check if the four points form a quadrangle.
    if (inner_i == point_to_be_added_i) return true;

    const RadiusVector2D& left = points_[edge_left_i];
    const RadiusVector2D& right = points_[edge_right_i];
    const RadiusVector2D& inner = points_[inner_i];
    const RadiusVector2D& to_be_added = points_[point_to_be_added_i];

    // Convexity test: if it's non convex, return true.
    if ((left - to_be_added).CrossProduct(inner - to_be_added) *
        (right - to_be_added).CrossProduct(inner - to_be_added) > 0
            ) {
        return true;
    }

    // A test from the book from the article.
    const double Sa = (right - inner).DotProduct(left - inner);
    const double Sb = (to_be_added - right).DotProduct(to_be_added - left);

    if (Sa < epsilon && Sb < epsilon) {
        return false;
    } else if (Sa > -epsilon && Sb > -epsilon) {
        return true;
    } else {
        const double Sg = std::abs((right - inner).CrossProduct(left - inner));
        const double Sd = std::abs((to_be_added - right).CrossProduct(to_be_added - left));

        return Sg * Sb + Sd * Sa > -epsilon;
    }
}

void DelaunayTriangulation::FixTriangulation(int32_t edge_left_i, int32_t edge_right_i, int32_t point_to_be_added_i) {
    std::stack<Edge> stack;
    stack.push(Edge(edge_left_i, edge_right_i));

    while(!stack.empty()) {
        edge_left_i = stack.top().index1_;
        edge_right_i = stack.top().index2_;
        stack.pop();

        int32_t inner_i = triangulation_[Edge(std::min(edge_left_i, edge_right_i),
                                         std::max(edge_left_i, edge_right_i))].Min();
        if (LocalDelaunayCriterion(edge_left_i, edge_right_i, inner_i, point_to_be_added_i)) {
            // Add missing edges and continue.
            triangulation_[Edge(edge_left_i, point_to_be_added_i)].Insert(edge_right_i);
            triangulation_[Edge(edge_right_i, point_to_be_added_i)].Insert(edge_left_i);

            if (edge_left_i > edge_right_i) std::swap(edge_left_i, edge_right_i);
            triangulation_[Edge(edge_left_i, edge_right_i)].Insert(point_to_be_added_i);

            continue;
        }

        // Reconstruct quadrangle.
        triangulation_[Edge(edge_left_i, point_to_be_added_i)].Replace(edge_right_i, inner_i);
        triangulation_[Edge(edge_right_i, point_to_be_added_i)].Replace(edge_left_i, inner_i);
        triangulation_[Edge(std::min(inner_i, edge_left_i), std::max(inner_i, edge_left_i))].Replace(
                edge_right_i, point_to_be_added_i
        );
        triangulation_[Edge(std::min(inner_i, edge_right_i), std::max(inner_i, edge_right_i))].Replace(
                edge_left_i, point_to_be_added_i
        );

        triangulation_.erase(Edge(std::min(edge_left_i, edge_right_i), std::max(edge_left_i, edge_right_i)));

        stack.push(Edge(edge_left_i, inner_i));
        stack.push(Edge(inner_i, edge_right_i));
    }
}

void DelaunayTriangulation::AddPoint(int32_t new_point_i) {
    int32_t hull_cur_point_i = new_point_i - 1;
    int32_t cur_right_point_i = convex_hull_[hull_cur_point_i].right_;
    RadiusVector2D difference = points_[hull_cur_point_i] - points_[new_point_i];
    RadiusVector2D next_difference = points_[cur_right_point_i] - points_[new_point_i];

    // Find the "right" part of edges visible from the points_[index].
    while (difference.CrossProduct(next_difference) < epsilon) {
        FixTriangulation(hull_cur_point_i, cur_right_point_i, new_point_i);

        hull_cur_point_i = cur_right_point_i;
        cur_right_point_i = convex_hull_[hull_cur_point_i].right_;
        difference = next_difference;
        next_difference = points_[cur_right_point_i] - points_[new_point_i];
    }
    convex_hull_[new_point_i].right_ = hull_cur_point_i;

    // Similarly to the "left".
    hull_cur_point_i = new_point_i - 1;
    int32_t cur_left_point_i = convex_hull_[hull_cur_point_i].left_;
    difference = points_[hull_cur_point_i] - points_[new_point_i];
    next_difference = points_[cur_left_point_i] - points_[new_point_i];

    // Find the "right" (relative to difference) part of edges visible from the points_[index].
    while (difference.CrossProduct(next_difference) > -epsilon) {
        FixTriangulation(cur_left_point_i, hull_cur_point_i, new_point_i);

        hull_cur_point_i = cur_left_point_i;
        cur_left_point_i = convex_hull_[hull_cur_point_i].left_;
        difference = next_difference;
        next_difference = points_[cur_left_point_i] - points_[new_point_i];
    }
    convex_hull_[new_point_i].left_ = hull_cur_point_i;

    convex_hull_[convex_hull_[new_point_i].right_].left_ = new_point_i;
    convex_hull_[hull_cur_point_i].right_ = new_point_i;
}

DelaunayTriangulation::DelaunayTriangulation(std::vector<RadiusVector2D> &points) {
    points_ = std::move(points);
    // As number of points in the convex hull is less or equal to the number of points.
    convex_hull_.resize(points_.size());

    std::sort(points_.begin(), points_.end(), std::less<RadiusVector2D>());

    // Initialization of the first triangle (Test is necessary for a correct orientation.)
    if ((RadiusVector2D(points_[1]) - RadiusVector2D(points_[0])).CrossProduct(RadiusVector2D(points_[2]) - RadiusVector2D(points_[0])) < epsilon) {
        convex_hull_[0] = ListNode_(1 ,2);
        convex_hull_[2] = ListNode_(0, 1);
        convex_hull_[1] = ListNode_(2, 0);
    } else {
        convex_hull_[0] = ListNode_(2 ,1);
        convex_hull_[1] = ListNode_(0, 2);
        convex_hull_[2] = ListNode_(1, 0);
    }
    triangulation_[Edge(0, 1)].Insert(2);
    triangulation_[Edge(1, 2)].Insert(0);
    triangulation_[Edge(0, 2)].Insert(1);

    for (int32_t i = 3; i < points_.size(); ++i) {
        AddPoint(i);
    }
}
//------------------------------------------------------------------------------------------------------

// Returns average number of sides of bounded polygons of the Voronoi's diagram.
double CalculateSides(std::vector<RadiusVector2D>& points) {
    if (points.size() < 3) return 0;

    DelaunayTriangulation triangulation(points);
    std::unordered_set<int32_t> convex_hull = triangulation.GetConvexHull();

    // Number of sides of bounded polygons is equal to degree of appropriate vertex.
    int64_t summary_degree = 0;
    // int64_t sum_deg_test = 0;    // For testing.
    for (const auto& [edge, opposite_points]: triangulation) {
        if (!convex_hull.count(edge.index1_))
            ++summary_degree;
        if (!convex_hull.count(edge.index2_))
            ++summary_degree;

        // sum_deg_test += 2;
    }

    // For testing!!!
    /*bool check = true;
    for (const auto& [edge, vertices] : triangulation) {
        if (vertices.Size() == 2) {
            check = triangulation.LocalDelaunayCriterion(edge.index1_, edge.index2_, vertices.Min(), vertices.Max());
        }
    }
    std::cout << check << std::endl;
    if (sum_deg_test / 2 + convex_hull.size() != 3 * (triangulation.Size() - 1)) {
        std::cout << "ERROR!";
    }*/

    if (triangulation.Size() == convex_hull.size()) {
        return static_cast<double>(0);
    }

    return summary_degree / static_cast<double>(triangulation.Size() - convex_hull.size());
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    std::vector<RadiusVector2D> points;

    double x, y;
    while((std::cin >> x) && (std::cin >> y)) {
        points.emplace_back(x, y);
    }

    std::cout.precision(9);
    std::cout << std::fixed << CalculateSides(points);

    return 0;
}

/*
IN
0 0
3 -3
4 1
2 -1
OUT
3
----------
IN
0 0
2 1
3 3
4 0
OUT
3
----------
IN
-2 0
0 2
0.001 -2
0.002 0
2 1
OUT
4
----------
IN
404 315
426 295
443 319
422 353
491 290
364 284
436 191
672 450
131 433
OUT
4.8333333
----------
IN
-2 0
0 -2
1 2
3 1
4 0
OUT
0
----------
IN
test005
OUT
27.763157895*/