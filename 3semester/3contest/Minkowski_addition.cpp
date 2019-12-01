/* Даны два выпуклых многоугольника на плоскости. В первом n точек, во втором m.
 * Определите, пересекаются ли они за O(n + m).
 * Указание. Используйте сумму Минковского.
 *
 * Формат ввода
 * Первая строка содержит число n точек первого многоугольника.
 * Затем идут n строчек с координатами точек первого многоугольника по часовой стрелке
 * (координаты — действительные числа, double).
 * Второй прямоугольник задается аналогично. n, m ≤ 80000.
 *
 * Формат вывода
 * YES/NO.*/

#include <iostream>
#include <cmath>
#include <vector>



// We will have only radius vectors.
struct RadiusVector {
    RadiusVector(double x1, double y1) : x(x1), y(y1) {}
    RadiusVector() : x(0.0), y(0.0) {}

    RadiusVector operator/(const int32_t divider) const {
        return RadiusVector((*this).x / divider, (*this).y / divider);
    }
    RadiusVector operator*(const int32_t multiplier) const {
        return RadiusVector((*this).x * multiplier, (*this).y * multiplier);
    }
    RadiusVector operator+(const RadiusVector& rhs) const {
        return RadiusVector((*this).x + rhs.x, (*this).y + rhs.y);
    }
    RadiusVector operator-(const RadiusVector& rhs) const {
        return RadiusVector(x - rhs.x, y - rhs.y);
    }
    RadiusVector operator-() const {
        return RadiusVector(-x, -y);
    }

    bool operator<(const RadiusVector& rhs) const {
        if (y < rhs.y)
            return true;
        if (rhs.y < y)
            return false;
        return x < rhs.x;
    }
    bool operator<=(const RadiusVector &rhs) const {
        return !(rhs < *this);
    }

    double Distance(const RadiusVector& other) const {
        return sqrt((other.x - x) * (other.x - x) + (other.y - y) * (other.y - y));
    }
    double Length() const {
        return Distance(RadiusVector(0.0, 0.0));
    }
    // As we are on the plane, we can only get a module and a sign of a cross product.
    double CrossProduct(const RadiusVector& other) const {
        return (x * other.y - other.x * y);
    }
    // Returns cosine of polar angle.
    double PolarAngle() const;

    double x, y;
};

double RadiusVector::PolarAngle() const {
    if (x == 0) {
        if (y > 0) return M_PI / 2;
        if (y < 0) return 3 * M_PI / 2;
        throw "Polar angle is not defined.";
    } else if (x > 0) {
        if (y >= 0) return atan(y / x);
        return atan(y / x) + 2 * M_PI;
    } else {
        return atan(y / x) + M_PI;
    }
}


//------------------------------------------------------------------------------------------------------

// All faces are a set of points, that are sorted in counter-clockwise order from the lowest left point.
// Vector should be sorted in counter-clockwise order.
class Face {
public:
    explicit Face(const std::vector<RadiusVector>& set);
    Face() = default;

    Face operator-() const;

    bool IsInFace(const RadiusVector& point) const;
    Face MinkowskiAddition(const Face& face2);

private:
    std::vector<RadiusVector> face_;
};

Face::Face(const std::vector<RadiusVector>& set) {
    RadiusVector lowest_left(set[0]);
    int32_t j = 0, size = set.size();
    face_.resize(size);

    for (int32_t i = 1; i < size; ++i) {
        if (set[i] < lowest_left) {
            lowest_left = set[i];
            j = i;
        }
    }

    for (int32_t i = 0; i < size; ++i) {
        face_[i] = set[(i + j) % size];
    }
}

Face Face::operator-() const {
    int32_t size = face_.size();
    std::vector<RadiusVector> tmp(size);
    for(int32_t i = 0; i < size; ++i) {
        tmp[i] = -face_[i];
    }

    return Face(tmp);
}

bool Face::IsInFace(const RadiusVector& point) const {
    bool IsInRightHalfPlane = true;

    // Check that the point is in the half-plane of every vertex of the face.
    int32_t size = face_.size();
    for (int32_t i = 1; i <= size; ++i) {
        RadiusVector diff(face_[i - 1] - point), edge(face_[i % size] - face_[i - 1]);

        if (diff.CrossProduct(edge) < 0) {
            IsInRightHalfPlane = false;
            break;
        }
    }

    return IsInRightHalfPlane;
}

Face Face::MinkowskiAddition(const Face& face2) {
    Face result;
    int32_t size1 = face_.size(), size2 = face2.face_.size();
    // result.face_.resize(size1 + size2);
    int32_t iter1 = 0, iter2 = 0, iter3 = 0;

    // We have to return at the start position and (n; n + 1) = (0; 1) edge will be considered.
    while (iter1 < size1 || iter2 < size2) {
        result.face_.push_back(face_[iter1 % size1] + face2.face_[iter2 % size2]);
        ++iter3;

        double angle1 = 0, angle2 = 0;
        try {
            angle1 = (face_[(iter1 + 1) % size1] - face_[iter1 % size1]).PolarAngle();
            angle2 = (face2.face_[(iter2 + 1) % size2] - face2.face_[iter2 % size2]).PolarAngle();
        } catch (const char *msg) {
            std::cerr << msg << std::endl;
        }

        // If one of the faces looped, make the angle > 2 * pi.
        iter1 == size1 ? angle1 += 2 * M_PI : angle1;
        iter2 == size2 ? angle2 += 2 * M_PI : angle2;

        if (angle1 < angle2) {
            ++iter1;
        } else if (angle2 < angle1) {
            ++iter2;
        } else {
            ++iter1, ++iter2;
        }
    }

    return result;
}
//------------------------------------------------------------------------------------------------------


int main() {
    int32_t n = 0;
    std::cin >> n;
    std::vector<RadiusVector> polygon1(n);
    for (int32_t i = n - 1; i >= 0; --i) {
        double x, y;
        std::cin >> x >> y;

        polygon1[i] = RadiusVector(x, y);
    }

    int32_t m = 0;
    std::cin >> m;
    std::vector<RadiusVector> polygon2(m);
    for (int32_t i = m - 1; i >= 0; --i) {
        double x, y;
        std::cin >> x >> y;

        polygon2[i] = RadiusVector(x, y);
    }

    Face face1(polygon1), face2(polygon2);

    (face1.MinkowskiAddition(-face2)).IsInFace(RadiusVector(0.0, 0.0)) ? std::cout << "YES" : std::cout << "NO";

    return 0;
}

/*
IN
5
0 0
0 1
1 2
2 1
1 0
3
3 0
2 0
1 1
OUT
YES
------
IN
4
0 0
0 1
1 1
1 0
3
2 0
3 1
3 0
OUT
NO
------
IN
3
0 0
0 1
1 1
3
0 0
0 -1
-1 -1
OUT
YES*/