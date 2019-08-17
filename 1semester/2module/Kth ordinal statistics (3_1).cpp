/*���� ��������������� ����� ����� n,k � ������ ����� ����� �� [0..10^9] ������� n.
��������� ����� k-� ���������� ����������. �.�. ���������� �����, ������� �� ������ �� ������� � �������� k (0..n-1) � ��������������� �������.
�������� ������������� ��������.
���������� � �������������� ������: O(n). ��������� ������� ����� ������: O(n).
������� Partition ������� ������������� ������� ������� ����� ����������� � ����� �����������.

3_1. ���������� ��������� ������ �������� �������� �������� ����.
������� Partition ���������� ������� ������� ����� ����������� �� ������ ������� � �����.*/

#include<iostream>

void FindMedian(int* a, int left, int right) {
	std::less<int> less;

	if (less(a[(left + right) / 2], a[left])) {
		std::swap(a[(left + right) / 2], a[left]);
	}
	if (less(a[right], a[(left + right) / 2])) {
		std::swap(a[right], a[(left + right) / 2]);
	}
	if (less(a[(left + right) / 2], a[left])) {
		std::swap(a[left], a[(left + right) / 2]);
	}

	// The least is in the middle, so we will put it to the end.
	std::swap(a[right], a[(left + right) / 2]);
}

int Partition(int* a, int left, int right) {
	int i = left, j = left;
	std::less<int> less;

	while (j < right) {
		if (less(a[right], a[j])) {
			++j;
		}
		else {
			std::swap(a[i], a[j]);
			++i;
			++j;
		}
	}

	std::swap(a[i], a[right]);
	return i;
}

int KOrdinalStatistics(int* a, int left, int right, int k) {
	std::less<int> less;

	while (!less(right, left)) {
		FindMedian(a, left, right);
		int i = Partition(a, left, right);

		if (i == k) {
			return a[i];
		}
		else if (i > k) {
			right = i - 1;
		}
		else {
			left = i + 1;
		}
	}
}

int main() {
	int n = 0, k = 0;
	std::cin >> n >> k;

	auto arr = new int[n];
	for (int i = 0; i < n; ++i) {
		std::cin >> arr[i];
	}

	std::cout << KOrdinalStatistics(arr, 0, n - 1, k);

	delete[] arr;
	return 0;
}

/*
IN
10 4
1 2 3 4 5 6 7 8 9 10
OUT
5
--------------------
IN
10 0
3 6 5 7 2 9 8 10 4 1
OUT
1
--------------------
IN
10 9
0 0 0 0 0 0 0 0 0 1
OUT
1
*/