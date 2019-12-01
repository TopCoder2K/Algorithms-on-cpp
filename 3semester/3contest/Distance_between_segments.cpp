/* Даны два отрезка в пространстве (x1, y1, z1) - (x2, y2, z2) и (x3, y3, z3) - (x4, y4, z4).
 * Найдите расстояние между отрезками.
 *
 * Формат ввода
 * Заданы целые x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4.
 * Координаты по модулю не превосходят 1000.
 *
 * Формат вывода
 * Выведите искомую величину c точностью не менее 6 знаков после десятичной точки.*/

#include <iostream>
#include <cmath>



constexpr double epsilon = 0.000000000001;

// We will have only radius vectors.
struct Vector {
    Vector(double x1, double y1, double z1) : x(x1), y(y1), z(z1) {}

    Vector operator/(const int32_t divider) const {
        return Vector((*this).x / divider, (*this).y / divider, (*this).z / divider);
    }
    Vector operator*(const int32_t multiplier) const {
        return Vector((*this).x * multiplier, (*this).y * multiplier, (*this).z * multiplier);
    }
    Vector operator+(const Vector& other) const {
        return Vector((*this).x + other.x, (*this).y + other.y, (*this).z + other.z);
    }

    double Distance(const Vector& other) const {
        return sqrt((other.x - x) * (other.x - x) + (other.y - y) * (other.y - y) + (other.z - z) * (other.z - z));
    }
    double Length() const {
        return Distance(Vector(0.0, 0.0, 0.0));
    }

    double x, y, z;
};

// Ternary search for min distance from the point.
Vector min_dist_between_dot_and_segment(const Vector& point, Vector left, Vector right) {
    while (left.Distance(right) > epsilon) {
        Vector one_third = (left * 2 + right) / 3, second_third = (left + right * 2) / 3;

        if (point.Distance(one_third) < point.Distance(second_third)) {
            right = second_third;
        } else {
            left = one_third;
        }
    }

    return (left + right) / 2;
}

// Ternary search for a point of the first segment from which the min distance will be found.
double min_dist_between_segments(Vector& left1, Vector& right1, const Vector& left2, const Vector& right2) {
    while (left1.Distance(right1) > epsilon) {
        Vector one_third = (left1 * 2 + right1) / 3, second_third = (left1 + right1 * 2) / 3;

        if (one_third.Distance(min_dist_between_dot_and_segment(one_third, left2, right2)) <
            second_third.Distance(min_dist_between_dot_and_segment(second_third, left2, right2))) {
            right1 = second_third;
        } else {
            left1 = one_third;
        }
    }

    Vector final_point = (left1 + right1) / 2;
    return final_point.Distance(min_dist_between_dot_and_segment(final_point, left2, right2));
}

int main() {
    double x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4;
    std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3 >> x4 >> y4 >> z4;

    Vector start1(x1, y1, z1), end1(x2, y2, z2), start2(x3, y3, z3), end2(x4, y4, z4);
    std::cout << printf("%.12lf", min_dist_between_segments(start1, end1, start2, end2));

    return 0;
}

/*
IN
0 0 0
1 1 1
0 0 1
0 1 0
OUT
0.408248291
-----------------
IN
2 3 0
5 0 0
2 0 0
5 3 0
OUT
0.00000000821714
-----------------
IN
0 0 0
2 2 0
2 2 0
2 3 0
OUT
0.00000000000114
-----------------
IN
0 0 0
2 2 0
1 1 0
0 2 0
OUT
0.00000000624114
----------------
IN
1000 1000 1000
-1000 -1000 -1000
0 0 0
1 1 1
OUT
0.00000000000014
*/