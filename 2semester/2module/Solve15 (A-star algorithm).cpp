/*Написать алгоритм для решения игры в “пятнашки”.
 * Решением задачи является приведение к виду:
 * [ 1 2 3 4 ]
 * [ 5 6 7 8 ]
 * [ 9 10 11 12]
 * [ 13 14 15 0 ], где 0 задает пустую ячейку.
 * Достаточно найти хотя бы какое-то решение (но быстро: max time - 6 seconds).
 * Число перемещений костяшек не обязано быть минимальным.
 * Формат ввода
 * Начальная расстановка.

 * Формат вывода
 * Если вам удалось найти решение, то в первой строке файла выведите число перемещений, которое требуется сделать в вашем решении.
 * А во второй строке выведите соответствующую последовательность ходов:
 * L означает, что в результате перемещения костяшки пустая ячейка сдвинулась влево,
 * R – вправо, U – вверх, D – вниз.
 * Если же выигрышная конфигурация недостижима, то выведите в выходной файл одно число −1.*/

# include <iostream>
# include <string>
# include <vector>
# include <unordered_set>
# include <set>
# include <unordered_map>
# include <cmath>
# include <cassert>

class State {
public:
    explicit State(const std::vector<uint8_t>& state);
    ~State() = default;

    State& operator=(const State& other) = default;
    State& operator=(State&& other) = default;
    State(const State& other) = default;
    State(State&& other) = default;


    bool operator==(const State& other) const { return _state == other._state; }
    bool operator<(const State& other) const {
        if (_path_len + _heuristics == other._path_len + other._heuristics)
            return _state < other._state;

        return _path_len + _heuristics < other._path_len + other._heuristics;
    }

    std::vector<State> GetNextVertices() const;

    uint8_t GetMove(const State& state) const noexcept {
        if(_Up() == state) return 'D';

        if(_Down() == state) return 'U';

        if(_Left() == state) return 'R';

        if(_Right() == state) return 'L';

        assert(false);
    }

    // Heuristic influence can be adjusted by a coefficient.
    void UpdatePathLen() { _path_len += 1; _heuristics = 1 * _Heuristics(); }

    friend struct std::hash<State>;
private:
    // We won't store a whole vector, we will pack numbers into uint64.
    unsigned long long _state;
    unsigned long long _path_len;
    uint8_t _heuristics;

    // Returns 4 bits at i position.
    uint8_t _Get(uint8_t i) const { return ((_state >> 4 * (15 - i)) & 15); }
    // Changes 4 bits to val at i position.
    void _Set(uint8_t i, uint8_t val) noexcept {
        unsigned long long tmp = 15;

        tmp = tmp << 4 * (15 - i); // Now 4 bits at i position in tmp are 1111.
        _state &=  ~(tmp); // Now 4 bits at i position are 0000.

        tmp = val;
        tmp &= 15;
        tmp <<= 4 * (15 - i); // Now 4 bits at i position in tmp are having the necessary value.

        _state |= tmp;
    }

    uint8_t GetNullPosition() const noexcept {
        for (uint8_t i = 0; i < 16; ++i) {
            uint8_t cur_four_bits = _Get(i);

            if (cur_four_bits == 0) return i;
        }
    }

    // Heuristics accounting.
    uint8_t _Heuristics() const {
        uint8_t heuristics = 0;
        std::vector<std::vector<uint8_t>> line_conflict(4, std::vector<uint8_t>());
        std::vector<std::vector<uint8_t>> column_conflict(4, std::vector<uint8_t>());

        for (uint8_t i = 0; i < 16; ++i) {
            uint8_t cur_four_bits = this->_Get(i);

            if (cur_four_bits != 0) {
                uint8_t true_pos_i = (cur_four_bits - 1) / 4, true_pos_j = (cur_four_bits - 1) % 4;
                uint8_t real_pos_i = i / 4, real_pos_j = i % 4;

                // Manhattan distance.
                heuristics += abs(real_pos_i - true_pos_i) + abs(real_pos_j - true_pos_j);

                // Linear conflict analysis preparation.
                if (real_pos_i == true_pos_i)
                    line_conflict[i / 4].push_back(cur_four_bits);

                if (real_pos_j == true_pos_j)
                    column_conflict[i % 4].push_back(cur_four_bits);
            }
        }

        // Linear conflict.
        // String linear conflict.
        std::unordered_set<uint8_t> corner_conflict;
        for (uint8_t line = 0; line < line_conflict.size(); ++line) {
            for (uint8_t i = 0; i < line_conflict[line].size(); ++i) {
                for (uint8_t j = i + 1; j < line_conflict[line].size(); ++j) {
                    if (line_conflict[line][i] > line_conflict[line][j]) {
                        heuristics += 2;

                        corner_conflict.insert(line_conflict[line][i]);
                        corner_conflict.insert(line_conflict[line][j]);
                    }
                }
            }
        }
        // Column linear conflict.
        for (uint8_t column = 0; column < column_conflict.size(); ++column) {
            for (uint8_t i = 0; i < column_conflict[column].size(); ++i) {
                for (uint8_t j = i + 1; j < column_conflict[column].size(); ++j) {
                    if (column_conflict[column][i] > column_conflict[column][j]) {
                        heuristics += 2;

                        corner_conflict.insert(column_conflict[column][i]);
                        corner_conflict.insert(column_conflict[column][j]);
                    }
                }
            }
        }

        // Corner tiles.
        // Right top corner.
        if (_Get(3) != 4 && _Get(3) != 0) {
            if (_Get(2) == 3 && (!corner_conflict.count(3))) {
                heuristics += 2;
            } else if (_Get(1 * 4 + 3) == 8 && (!corner_conflict.count(8))) {
                heuristics += 2;
            }
        }
        // Left top corner.
        if (_Get(0) != 1 && _Get(0) != 0) {
            if (_Get(1) == 2 && (!corner_conflict.count(2))) {
                heuristics += 2;
            } else if (_Get(1 * 4) == 5 && (!corner_conflict.count(5))) {
                heuristics += 2;
            }
        }
        // Left bottom corner.
        if (_Get(3 * 4) != 13 && _Get(3 * 4) != 0) {
            if (_Get(2 * 4) == 9 && (!corner_conflict.count(9))) {
                heuristics += 2;
            } else if (_Get(3 * 4 + 1) == 14 && (!corner_conflict.count(14))) {
                heuristics += 2;
            }
        }

        // Last move.
        if (_Get(3 * 4 + 3) != 12 && _Get(3 * 4 + 3) != 15 && _Get(3 * 4 + 3) != 0) {
            heuristics += 2;
        }

        return heuristics;
    }

    // Moving through edges.
    State _Up() const {
        State up(*this);

        uint8_t null = GetNullPosition();

        // Shift 0 one line up <=> null position has to decrease by 4 (field 4x4).
        up._Set(null, _Get(null - 4));
        up._Set(null - 4, 0);

        up.UpdatePathLen();

        return up;
    }
    State _Down() const {
        State down(*this);

        uint8_t null = GetNullPosition();

        // Similarly to up.
        down._Set(null, _Get(null + 4));
        down._Set(null + 4, 0);

        down.UpdatePathLen();

        return down;
    }
    State _Left() const {
        State left(*this);

        uint8_t null = GetNullPosition();

        // Similarly to up.
        left._Set(null, _Get(null - 1));
        left._Set(null - 1, 0);

        left.UpdatePathLen();

        return left;
    }
    State _Right() const {
        State right(*this);

        uint8_t null = GetNullPosition();

        // Similarly to up.
        right._Set(null, _Get(null + 1));
        right._Set(null + 1, 0);

        right.UpdatePathLen();

        return right;
    }
};

State::State(const std::vector<uint8_t>& state) {
    _state = 0;
    unsigned long long digit = 0;

    for (std::size_t i = 0; i < state.size(); ++i) {
        digit = static_cast<unsigned long long>(state[i]);
        digit &= 15;
        digit <<= 4 * (state.size() - 1 - i);

        _state |= digit;
    }

    _path_len = _Heuristics();
}

std::vector<State> State::GetNextVertices() const {
    const uint8_t pos0 = GetNullPosition();

    std::vector<State> result;
    if (pos0 > 3) result.push_back(_Up());

    if (pos0 < 12) result.push_back(_Down());

    if (pos0 % 4 != 3) result.push_back(_Right());

    if (pos0 % 4 != 0) result.push_back(_Left());

    return result;
}

std::vector<uint8_t> FinishState() {
    std::vector<uint8_t> tmp(16, 0);
    for(std::size_t i = 1; i <= 16; ++i) {
        tmp[i - 1] = i;
    }

    return tmp;
}
const State finish = State(FinishState());

// Needs operator< for type!
template <typename Type>
class Comparator {
public:
    bool operator()(const Type& a, const Type& b) {
        return a < b;
    }
};

namespace std {
    template<>
    struct hash<State> {
        unsigned long long operator()(const State& state) const {
            return hash<unsigned long long>()(state._state);
        }
    };
}

// Finds a path.
class Visit {
public:
    // This operator updates letter description of a way and check if we reached the finish position.
    bool operator()(const State& state);

    std::string GetAnswer() const { return routes.at(finish); }
private:
    std::unordered_map<State, std::string> routes;
};

bool Visit::operator()(const State& state) {
    // There is the only one previous position among these next positions because of type of movement of zero.
    std::vector<State> next_vertices = state.GetNextVertices();
    std::string route;

    for(std::size_t i = 0; i < next_vertices.size(); ++i)
        if(routes.count(next_vertices[i])) {
            route = routes.at(next_vertices[i]);
            route += next_vertices[i].GetMove(state);

            break;
        }

    routes[state] = route;
    return !(state == finish);
}

template <typename TVertex, typename TVisit>
void FindPath(const TVertex& start, TVisit& visit) {
    // A* algorithm.
    std::set<TVertex, Comparator<State>> min_tree;
    std::unordered_set<TVertex> visited;

    min_tree.insert(start);
    visited.insert(start);

    while (true) {
        // Size limit on std::set size, because we strictly limited in time and don't need the least path length.
        // If we delete this block, we get the solve which searches min path lenght but many times slower.
        if(min_tree.size() > 20000) {
            auto iter = min_tree.end();

            while (min_tree.size() > 20000) {
                min_tree.erase(*iter);
                --iter;
            }
        }

        State v = *min_tree.begin();
        min_tree.erase(v);

        // Finish check.
        if(!visit(v)) {
            break;
        }

        auto next = v.GetNextVertices();
        for(const auto &u : next) {
            if (!visited.count(u)) {
                visited.insert(u);
                min_tree.insert(u);
            }
        }
    }
}

std::string Solve15(const State& start) {
    Visit visit;

    FindPath(start, visit);

    return visit.GetAnswer();
}

uint8_t InversionCount(const std::vector<uint8_t> &input) {
    uint8_t inversions = 0;

    // Inversions should be counted by snake pass.
    std::vector<uint8_t> inv_count(input.size());
    for (std::size_t i = 0; i <= 3; ++i) {
        inv_count[i] = input[i];
    }
    for (std::size_t i = 7; i > 3; --i) {
        inv_count[11 - i] = input[i];
    }
    for (std::size_t i = 8; i <= 11; ++i) {
        inv_count[i] = input[i];
    }
    for (std::size_t i = 15; i > 11; --i) {
        inv_count[27 - i] = input[i];
    }

    for (std::size_t i = 0; i < inv_count.size(); ++i) {
        for (std::size_t j = i + 1; j < inv_count.size(); ++j) {
            if (inv_count[i] > inv_count[j] && inv_count[j] != 0)
                ++inversions;
        }
    }

    return inversions;
}

int main() {
    std::vector<uint8_t > input;
    for(std::size_t i = 0; i < 16; ++i) {
        int tmp = 0;
        std::cin >> tmp;
        input.push_back(static_cast<uint8_t>(tmp));
    }

    if (InversionCount(input) % 2 != 1) {
        std::cout << -1;
        return 0;
    }

    State start(input);
    std::string answer = Solve15(start);
    std::cout << answer.size() << std::endl << answer;


    return 0;
}

/*
 * IN
 * 2 3 1 4 5 6 7 8 9 10 11 12 13 14 15 0
 * OUT
 * 20
 * DRDDRULURRDLDRUULLLU
-----------------------------------------
 * IN
 * 1 2 4 3 7 5 6 0 15 12 9 10 8 11 13 14
 * OUT
 * 52
 * DRRULURDLLURRULLDRRRDLDLUURRULLDRRDLULLDRRULLDRRULLU
-----------------------------------------
 * IN
 * 5 1 2 3 6 10 7 4 9 14 11 8 13 15 0 12
 * OUT
 * 11
 * RDDRDLLLUUU
-----------------------------------------
 * IN
 * 1 2 3 0 5 6 7 4 9 10 11 8 13 14 15 12
 * OUT
 * 3
 * UUU
-----------------------------------------
 * IN
 * 0 1 2 3 5 6 7 4 9 10 11 8 13 14 15 12
 * OUT
 * 6
 * LLLUUU
-----------------------------------------
 * IN
 * 0 2 3 4 1 6 7 8 5 10 11 12 9 13 14 15
 * OUT
 * 6
 * UUULLL
-----------------------------------------
 * IN
 * 0 2 3 4 1 5 6 8 9 10 7 12 13 14 11 15
 * OUT
 * 6
 * ULLUUL*/