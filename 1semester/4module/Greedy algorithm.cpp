/*2_2. Заявки на переговоры.
В большой IT-фирме есть только одна переговорная комната. Желающие посовещаться заполняют заявки с желаемым временем начала и конца. Ваша задача определить максимальное количество заявок, которое может быть удовлетворено.
Число заявок ≤ 100000.
Формат входных данных:
Вход содержит число заявок и пары целых чисел — начала и концы заявок.
Формат выходных данных:
Выход должен содержать натуральное число — максимальное число заявок.
*/

#include <iostream>
#include <vector>
#include <algorithm>

// Main idea is using the smallest intervals, so we sort all intervals by ends in descending order (because it is easy to find intersections)
// and sort intervals by begins in ascending order in each group.
bool compare(std::pair <int, int> a, std::pair <int, int> b) {
	if (a.second == b.second) {
		return a.first > b.first;
	}
	return a.second < b.second;
}

int main() {
	int n = 0;
	std::cin >> n;

	std::vector<std::pair<int, int>> applications(n);
	for (int i = 0; i < n; ++i) {
		int begin = 0, end = 0;
		std::cin >> begin >> end;
		applications[i] = std::make_pair(begin, end);
	}

	std::sort(applications.begin(), applications.end(), compare);

	int count = 1, last_end = applications[0].second;
	for (int i = 1; i < n; ++i) {
		if (applications[i].first >= last_end) {
			++count;
			last_end = applications[i].second;

			while (i < n && last_end == applications[i].second) {
				++i;
			}
			--i;
		}
	}
	std::cout << count;

	return 0;
}

/*
IN
10
1 2
2 5
7 10
2 3
6 7
4 7
3 6
3 5
1 2
4 5
OUT
5*/