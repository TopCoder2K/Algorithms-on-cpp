/*Дано N кубиков. Требуется определить каким количеством способов можно выстроить из этих кубиков пирамиду.
Формат входных данных:
На вход подается количество кубиков N.
Формат выходных данных:
Вывести число различных пирамид из N кубиков.
3_2. Широкая пирамида. Каждый вышележащий слой пирамиды должен быть строго меньше нижележащего.
N ≤ 300.*/

#include <iostream>
#include <vector>

long long CountingPyraminds(int cubes) {
	// i-th string is maximum number of cubes on the first level, j-th column is number of available cubes.
	std::vector<std::vector<long long>> table(cubes + 1, std::vector<long long>(cubes + 1, 0));

	table[0][0] = 1;
	for (int i = 1; i <= cubes; ++i) {
		table[0][i] = 0;
		table[i][0] = 1;
	}

	for (int i = 1; i <= cubes; ++i) {
		for (int j = 1; j <= cubes; ++j) {
			table[i][j] = table[i - 1][j];

			if (j - i >= 0)
				table[i][j] += table[i - 1][j - i];
		}
	}

	/*for (int i = 0; i <= cubes; ++i) {
		for (int j = 0; j <= cubes; ++j) {
			std::cout << table[i][j] << ' ';
		}
		std::cout << std::endl;
	}*/

	return table[cubes][cubes];
}

int main() {
	int n;
	std::cin >> n;

	std::cout << CountingPyraminds(n);

	return 0;
}

/*
IN
3
OUT
2
------
IN
5
OUT
3
------
IN
7
OUT
5*/