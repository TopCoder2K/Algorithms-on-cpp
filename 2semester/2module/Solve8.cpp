/*«Восьминашки» – упрощенный вариант известной головоломки «Пятнашки».
 * Восемь костяшек, пронумерованных от 1 до 8,
 * расставлены по ячейкам игровой доски 3 на 3,
 * одна ячейка при этом остается пустой.
 * За один ход разрешается передвинуть одну из костяшек,
 * расположенных рядом с пустой ячейкой, на свободное место.
 * Цель игры – для заданной начальной конфигурации игровой доски
 * за минимальное число ходов получить выигрышную конфигурацию
 * (пустая ячейка обозначена нулем)
 * 1 2 3
 * 4 5 6
 * 7 8 0
 * Формат входного файла
 * Во входном файле содержится начальная конфигурация головоломки
 * – 3 строки по 3 числа в каждой.
 * Формат выходного файла
 * Если решение существует,
 * то в первой строке выходного файла выведите минимальное число перемещений
 * костяшек, которое нужно сделать, чтобы достичь выигрышной конфигурации,
 * а во второй строке выведите соответствующую последовательность ходов:
 * L означает, что в результате перемещения костяшки
 * пустая ячейка сдвинулась влево, R – вправо, U – вверх, D – вниз.
 * Если таких последовательностей несколько, то выведите любую из них.
 * Если же выигрышная конфигурация недостижима,
 * то выведите в выходной файл одно число −1.
*/

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <cassert>

class State {
public:
    explicit State(const std::string& state);
    ~State() = default;

    State& operator=(const State& other) = default;
    State& operator=(State&& other) = default;
    State(const State& other) = default;
    State(State&& other) = default;

    bool operator==(const State& other) const noexcept {return _state == other._state;}

    std::vector<State> GetNextVertices() const;

    uint8_t GetMove(const State& state) const noexcept {
        if(_Up() == state) return 'U';

        if(_Down() == state) return 'D';

        if(_Left() == state) return 'L';

        if(_Right() == state) return 'R';

        assert(false);
    }

    friend struct std::hash<State>;
private:
    // We won't store a whole string, we will pack it in int. Null position is 3 most significant digits.
    int _state;

    uint8_t _GetNullPosition() const noexcept { return (_state >> 27); }
    void _SetNullPosition(uint8_t position0) noexcept {
        _state &= ~(15 << 27);
        _state |= ((15 & position0) << 27);
    }

    // Returns 3-bit at i position.
    uint8_t _Get(uint8_t i) const { return ((_state >> 3 * (8 - i)) & 7); }
    // Changes 3-bit to val at i position.
    void _Set(uint8_t i, int val) noexcept {
        _state &=  ~(7 << 3 * (8 - i)); // Now there is 000 in null position.

        val &= 7;
        val <<= 3 * (8 - i);

        _state |= val;
    }

    // Moving through edges.
    State _Up() const noexcept {
        State up(*this);

        uint8_t null = _GetNullPosition();

        up._Set(null, _Get(null - 3));
        up._Set(null - 3, 0);

        up._SetNullPosition(null - 3);
        return up;
    }
    State _Down() const noexcept {
        State down(*this);

        uint8_t null = _GetNullPosition();

        down._Set(null, _Get(null + 3));
        down._Set(null + 3, 0);

        down._SetNullPosition(null + 3);
        return down;
    }
    State _Left() const noexcept {
        State left(*this);

        uint8_t null = _GetNullPosition();

        left._Set(null, _Get(null - 1));
        left._Set(null - 1, 0);

        left._SetNullPosition(null - 1);
        return left;
    }
    State _Right() const noexcept {
        State right(*this);

        uint8_t null = _GetNullPosition();

        right._Set(null, _Get(null + 1));
        right._Set(null + 1, 0);

        right._SetNullPosition(null + 1);
        return right;
    }
};

State::State(const std::string& state) : _state(0) {
    int digit = 0;
    int position0 = 0;

    for (std::size_t i = 0; i < state.size(); ++i) {
        digit = static_cast<int>(state[i]) - 48;

        if (digit == 0) {
            position0 = i;
            continue;
        }

        digit &= 7;
        digit <<= 3 * (state.size() - 1 - i);

        _state |= digit;
    }

    position0 &= 15;
    _state |= (position0 << 27);
}

const State finish = State("123456780");

std::vector<State> State::GetNextVertices() const {
    const uint8_t pos0 = _GetNullPosition();
    std::vector<State> result;

    if (pos0 > 2) result.push_back(_Up());
    if (pos0 < 6) result.push_back(_Down());
    if (pos0 % 3 != 2) result.push_back(_Right());
    if (pos0 % 3 != 0) result.push_back(_Left());

    return result;
}

namespace std {
    template<>
    struct hash<State> {
        int operator()(const State& state) const {
            return hash<int>()(state._state);
        }
    };
}

//Finds a path.
class Visit {
public:
    bool operator()(const State &state);

    std::string GetAnswer() const { return routes.at(finish); }
private:
    std::unordered_map<State, std::string> routes;
};

bool Visit::operator()(const State& state) {
    std::vector<State> prev = state.GetNextVertices();
    std::string route;

    for(std::size_t i = 0; i < prev.size(); ++i) {
        if(routes.count(prev[i])) {
            route = routes.at(prev[i]);
            route += prev[i].GetMove(state);
            break;
        }
    }

    routes[state] = route;
    return !(state == finish);
}

template <class TVertex, class TVisit>
void BFS(const TVertex& start, TVisit& visit) {
    std::unordered_set<TVertex> visited;
    std::queue<TVertex> q;

    visited.insert(start);
    q.push(start);

    while(!q.empty()) {
        TVertex v = q.front();
        q.pop();

        if(!visit(v)) {
            break;
        }

        auto next_vertices = v.GetNextVertices();
        for(const auto& u : next_vertices) {
            if (!visited.count(u)) {
                visited.insert(u);
                q.push(u);
            }
        }
    }
}

std::string Solve8(const State& start) {
    Visit visit;

    BFS(start, visit);

    return visit.GetAnswer();
}

uint8_t InversionCount(const std::string& string) {
    uint8_t inversions = 0;

    for (int i = 0; i < string.size(); ++i) {
        for (int j = i + 1; j < string.size(); ++j) {
            if (string[i] > string[j] && string[j] != '0') {
                ++inversions;
            }
        }
    }

    return inversions;
}

int main() {
    // It would be easier to work with string.
    std::string in_string;
    for(int i = 0; i < 9; ++i) {
        char tmp = 0;
        std::cin >> tmp;
        in_string += tmp;
    }

    if (InversionCount(in_string) % 2 != 0) {
        std::cout << -1;
        return 0;
    }

    State start(in_string);
    std::string answer = Solve8(start);
    std::cout << answer.size() << std::endl << answer;

    return 0;
}

/*
IN
0 1 6
4 3 2
7 5 8
OUT
8
RDRULDDR
--------------------
IN
0 1 2
3 4 5
6 7 8
OUT
22
RDLDRRULLDRUURDDLLURRD
--------------------
IN
1 2 3
8 0 4
7 6 5
OUT
-1
*/