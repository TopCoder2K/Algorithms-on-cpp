/*Напишите алгоритм порязрядной сортировки от минимальных разрядов к старшим.
На вход подаётся массив из целых чисел (не превосходящих 10^9), который нужно отсортировать.*/

#include <iostream>
#include <vector>

// Byte counting sort (digits are bytes).
void CountingSort(std::vector<int>& arr, int size, int digit) {
	auto baskets = new int[256];
	for (int i = 0; i < 256; ++i) {
		baskets[i] = 0;
	}

	for (int i = 0; i < size; ++i) {
		int value = arr[i];
		value >>= 8 * digit;
		++baskets[value & 255];
	}

	// Beginnings of groups of identical elements.
	int sum = 0;
	for (int i = 0; i < 256; ++i) {
		int tmp = baskets[i];
		baskets[i] = sum;
		sum += tmp;
	}

	// After placing an element an index of beginning of a group must be increased.
	auto sorted_arr = new int[size];
	for (int i = 0; i < size; ++i) {
		int value = arr[i];
		value >>= 8 * digit;
		if (baskets[value & 255] < size)
			sorted_arr[baskets[value & 255]++] = arr[i];
	}

	for (int i = 0; i < size; ++i) {
		arr[i] = sorted_arr[i];
	}

	delete[] sorted_arr;
}

void LSDSort(std::vector<int>& arr, int size, int digit) {
	for (int i = 0; i < 4; ++i) {
		CountingSort(arr, size, digit + i);
	}
}

int main() {
	std::vector<int> arr;

	int n = 0;
	std::cin >> n;
	for (int i = 0; i < n; ++i) {
		int tmp;
		std::cin >> tmp;
		arr.push_back(tmp);
	}

	LSDSort(arr, arr.size(), 0);

	for (int i = 0; i < arr.size(); ++i) {
		printf("%d ", arr[i]);
	}

	return 0;
}

/*
IN
5
1235 1234 103 111 112
OUT
103 111 112 1234 1235*/