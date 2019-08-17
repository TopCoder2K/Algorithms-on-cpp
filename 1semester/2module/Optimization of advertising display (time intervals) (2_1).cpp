/*¬о всех задачах данного раздела необходимо реализовать и использовать локальную пирамидальную сортировку.
ќбщее врем€ работы алгоритма O(n log n).

¬ супермаркете решили оптимизировать показ рекламы.
»звестно расписание прихода и ухода покупателей (два целых числа).
 аждому покупателю необходимо показать минимум 2 рекламы.
–екламу можно транслировать только в целочисленные моменты времени.
ѕокупатель может видеть рекламу от момента прихода до момента ухода из магазина.
¬ каждый момент времени может показыватьс€ только одна реклама.
—читаетс€, что реклама показываетс€ мгновенно.
≈сли реклама показываетс€ в момент ухода или прихода, то считаетс€, что посетитель успел еЄ посмотреть.
“ребуетс€ определить минимальное число показов рекламы.*/

#include <iostream>
#include <cassert>

struct Person {
	Person() = default;
	int come = 0, leave = 0;
};

template <typename T>
void SiftDown(T* arr, int index, int size) {
	std::less<int> less;

	while (true) {
		int left = index * 2 + 1, right = index * 2 + 2, least = index;

		if (less(left, size) && less(arr[left].leave, arr[least].leave)) {
			least = left;
		}
		else if (less(left, size) && !less(arr[left].leave, arr[least].leave) && !less(arr[least].leave, arr[left].leave)) {
			if (less(arr[least].come, arr[left].come)) {
				least = left;
			}
		}

		if (less(right, size) && less(arr[right].leave, arr[least].leave)) {
			least = right;
		}
		else if (less(right, size) && !less(arr[right].leave, arr[least].leave) && !less(arr[least].leave, arr[right].leave)) {
			if (less(arr[least].come, arr[right].come)) {
				least = right;
			}
		}

		if (least != index) {
			std::swap(arr[least], arr[index]);
			index = least;
		}
		else {
			break;
		}
	}
}

template <typename T>
void BuildHeap(T* arr, int size) {
	for (int i = (size - 1) / 2; i >= 0; --i) {
		SiftDown(arr, i, size);
	}
}

template <typename T>
void ExtractMax(T* arr, int size) {
	assert(size > 0);

	std::swap(arr[0], arr[size - 1]);
	--size;
	SiftDown(arr, 0, size);
}

template <typename T>
void HeapSort(T* arr, int n) {
	int heap_size = n;

	BuildHeap(arr, heap_size);
	while (heap_size > 1) {
		ExtractMax(arr, heap_size);
		--heap_size;
	}
}

template<typename T>
int Advert(T* arr, int size) {
	int number = 2, last_adv = arr[size - 1].leave, penultimate_adv = arr[size - 1].leave - 1;
	std::less<int> less;

	for (int i = size - 2; i >= 0; --i) {
		if (less(arr[i].leave, arr[i + 1].leave) || less(arr[i + 1].leave, arr[i].leave)) {
			ShowAdvert(arr, i, number, size, last_adv, penultimate_adv);
		}
	}

	return number;
}

template <typename T>
void ShowAdvert(T* arr, int index, int& number, int size, int& last_adv, int& penultimate_adv) {
	std::less<int> less;

	if (less(last_adv, arr[index].come)) {
		number += 2;
		last_adv = arr[index].leave;
		penultimate_adv = arr[index].leave - 1;
	}
	else if (!less(last_adv, arr[index].come) && !less(arr[index].come, last_adv)) {
		++number;
		penultimate_adv = last_adv;
		last_adv = arr[index].leave;
	}
	else {
		if (less(penultimate_adv, arr[index].come)) {
			++number;
			penultimate_adv = last_adv;
			last_adv = arr[index].leave;
		}
	}
}

int main() {
	int n = 0;
	std::cin >> n;

	auto ans = new Person[n];
	for (int i = 0; i < n; ++i) {
		std::cin >> ans[i].come >> ans[i].leave;
	}

	HeapSort(ans, n);

	std::cout << Advert(ans, n);

	delete[] ans;
	return 0;
}

/*
IN
5
1 10
10 12
1 10
1 10
23 24
OUT
5*/