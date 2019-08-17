#include<iostream>

void Merge(int*& a, int left1, int right1, int left2, int right2)
{

}

void MergeSort(int*& a, int left, int right)
{
	if (right <= left) {
		return;
	}
	int middle = (right + left) / 2;
	MergeSort(a, left, middle);
	MergeSort(a, middle + 1, right);
	Merge(a, left, middle, middle + 1, right);
}

void Divide(int*& a, int size, int k)
{
	for (int i = 0; i < size/k; ++i) {
		MergeSort(a, i*k, (i + 1)*k - 1);
	}
	if (size%k != 0) {
		MergeSort(a, (size / k)*k, size - 1);
	}
}

int main()
{
	int n, k;
	std::cin >> n >> k;
	int* arr = new int[n];
	for (int i = 0; i < n; ++i) {
		std::cin >> arr[i];
	}
	Divide(arr, n, k);
	return 0;
}