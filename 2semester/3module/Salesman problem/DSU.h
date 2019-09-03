#ifndef SALESMANPROBLEM_2_CDSU_H
#define SALESMANPROBLEM_2_CDSU_H

template <typename T>
class DSU {
public:
    explicit DSU(const uint32_t size) { _parent.resize(size); _rank.resize(size); }

    void MakeSet(const T& new_set) { _parent[new_set] = new_set; _rank[new_set] = 0; }

    void UnionSets(const T& first_element, const T& second_element) {
        T first_leader = FindSet(first_element);
        T second_leader = FindSet(second_element);

        if (first_leader != second_leader) {
            if (_rank[first_leader] < _rank[second_leader])
                std::swap(first_leader, second_leader);

            _parent[second_leader] = first_leader;
            if (_rank[first_leader] == _rank[second_leader]) {
                ++_rank[first_leader];
            }
        }
    }

    T FindSet(const T& element) {
        if (element == _parent[element])
            return element;

        _parent[element] = FindSet(_parent[element]);
        return _parent[element];
    }
private:
    std::vector<T> _parent;
    std::vector<uint32_t> _rank; // For rank union heuristics.
};

#endif //SALESMANPROBLEM_2_CDSU_H
