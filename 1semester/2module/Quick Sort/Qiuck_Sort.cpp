/*Напишите различные оптимизации сортировки Хоара и замерьте времена работы.*/

#include <iostream>
#include <ctime>
//#include <stack>

/*
struct Pair {
	int begin, end;
};
*/

struct Comparator {
	bool operator()(const int a, const int b) { return a < b; }
};

// Primitive dynamic array.
template <typename T>
class Array {
public:
	Array();
	~Array() { delete[] _buffer; }

	Array& operator=(const Array& other);
	Array& operator=(Array&& other) noexcept;
	Array(const Array& other) : Array() { *this = other; }
	Array(Array&& other) noexcept : Array() { *this = std::forward<Array>(other); }

	T& operator[](int index) { return _buffer[index]; }
	const T operator[](int index) const { return _buffer[index]; }

	void PushBack(const T& value);
	int Size() const noexcept { return _array_size; }
private:
	void _Reallocate();
	T* _buffer;
	int _array_size;
	int _buffer_size;
};

template <typename T>
Array<T>::Array() {
	_buffer_size = 1;
	_array_size = 0;
	_buffer = new T[_buffer_size];
}

template <typename T>
Array<T>& Array<T>::operator=(const Array& other) {
	if (this == &other) { return *this; }

	delete[] _buffer;
	_buffer = new T[other._buffer_size];
	for (int i = 0; i < other._array_size; ++i) {
		_buffer[i] = other._buffer[i];
	}

	_array_size = other._array_size;
	_buffer_size = other._buffer_size;

	return *this;
}

template <typename T>
Array<T>& Array<T>::operator=(Array&& other) noexcept {
	std::swap(_buffer, other._buffer);
	std::swap(_buffer_size, other._buffer_size);
	std::swap(_array_size, other._array_size);

	return *this;
}

template <typename T>
void Array<T>::PushBack(const T& value) {
	if (_array_size == _buffer_size) _Reallocate();

	_buffer[_array_size] = value;
	++_array_size;
}

template <typename T>
void Array<T>::_Reallocate() {
	int new_buffer_size = _buffer_size * 2;

	T* new_buffer = new T[new_buffer_size];
	for (int i = 0; i < _array_size; i++)
		new_buffer[i] = _buffer[i];

	delete[] _buffer;
	_buffer = new_buffer;

	_buffer_size = new_buffer_size;
}

template<typename Type>
void FindMedian(Type& a, int left, int right, Comparator& less) {
	if (less(a[(left + right) / 2], a[left])) {
		std::swap(a[left], a[(left + right) / 2]);
	}
	if (less(a[right], a[(left + right) / 2])) {
		std::swap(a[right], a[(left + right) / 2]);
	}
	if (less(a[(left + right) / 2], a[left])) {
		std::swap(a[left], a[(left + right) / 2]);
	}

	std::swap(a[right], a[(left + right) / 2]);
}

/*template<typename Type>
void Merge(Type& a, int left1, int right1, int left2, int right2, int* c, Comparator& less) {
	int i = left1, j = left2;

	while (i <= right1 && j <= right2) {
		if (!less(a[j], a[i])) {
			c[i + j - left1 - left2] = a[i];
			++i;
		} else {
			c[i + j - left1 - left2] = a[j];
			++j;
		}
	}

	if (i > right1) {
		for (; j <= right2; ++j) {
			c[i + j - left1 - left2] = a[j];
		}
	} else {
		for (; i <= right1; ++i) {
			c[i + j - left1 - left2] = a[i];
		}
	}
}

template<typename Type>
void MergeSort(Type& a, int left, int right, Comparator& less) {
	if (right <= left) {
		return;
	}

	int middle = (right + left) / 2;
	MergeSort(a, left, middle, less);
	MergeSort(a, middle + 1, right, less);

	auto c = new int[right - left + 1];
	Merge(a, left, middle, middle + 1, right, c, less);
	for (int i = left; i <= right; ++i) {
		a[i] = c[i - left];
	}

	delete[] c;
}
*/

template <typename Type>
void InsertionSort(Type& arr, int left, int right, Comparator& less) {
	for (int i = left + 1; i <= right; ++i) {
		int tmp = arr[i];
		int j = i - 1;
		for (; j >= left && less(tmp, arr[j]); --j) {
			arr[j + 1] = arr[j];
		}
		arr[j + 1] = tmp;
	}
}

template<typename Type>
int Partition(Type& arr, int left, int right, Comparator& less) {
	// Pivot is last element.
	// const int& pivot = arr[right];

	// Pivot is random element.
	// int index = rand() % (right - left + 1) + left;
	// std::swap(arr[index], arr[right]);

	// Pivot is median of three.
	FindMedian(arr, left, right, less);

	const int& pivot = arr[right];

	// Indices pass towards each other.
	int i = left, j = right - 1;
	while (i <= j) {
		for (; less(arr[i], pivot); ++i) {}
		for (; j >= left && !less(arr[j], pivot) && j >= i; --j) {}

		if (i < j) std::swap(arr[i++], arr[j--]);
	}

	std::swap(arr[i], arr[right]);

	// Indices pass one way.
/*	int i = left;
	for (; i < right && arr[i] <= pivot; ++i) {}
	int j = i + 1;
	for (; j < right; ++j) {
		if (arr[j] <= pivot) {
			std::swap(arr[i++], arr[j]);
		}
	}

	std::swap(arr[i], arr[right]);*/

	return i;
}

template<typename Type>
void QuickSort(Type& arr, int left, int right, Comparator& less)
{
	// 20 is number of elements when insertion sort is approximately faster than quick sort.
	if (right - left + 1 > 20)
	{
		// Two branches of recursion.
		if (right > left) {
			int part = Partition(arr, left, right, less);

			if (part > left + 1) {
				QuickSort(arr, left, part - 1, less);
			}
			if (part < right - 1) {
				QuickSort(arr, part + 1, right, less);
			}
		}
	}
	else {
		InsertionSort(arr, left, right, less);
	}

	/*	// One branch of recursion.
			while (right > left) {
				int part = Partition(arr, left, right, less);

				if (part - 1 > left)
					QuickSort(arr, left, part - 1, less);

				left = part + 1;
			}

		// Without recursion.
		std::stack <Pair> call_stack;
		Pair tmp;
		tmp.begin = left, tmp.end = right;
		call_stack.push(tmp);
		while (!call_stack.empty()) {
			tmp.begin = call_stack.top().begin;
			tmp.end = call_stack.top().end;
			call_stack.pop();
			int part = Partition(arr, tmp.begin, tmp.end);
			if (part - 1 > tmp.begin) {
				int temp_index = tmp.end;
				tmp.end = part - 1;
				call_stack.push(tmp);
				tmp.end = temp_index;
			}
			if (part + 1 < tmp.end) {
				tmp.begin = part + 1;
				call_stack.push(tmp);
			}
		}*/
}

int main()
{
	unsigned long long time_begin = clock();
	Array<int> arr;

	/*	int tmp = 0;
	while (scanf("%d", tmp)) {
	arr.Add(tmp);
	}*/

	// Randomly generated 5 000 000 numbers.
	freopen("input.txt", "r", stdin);

	//	std::ios_base::sync_with_stdio(false);
	//	std::cin.tie(NULL);

	int n = 0;
	//	std::cin >> n;
	scanf("%d", &n);
	for (int i = 0; i < n; ++i) {
		int tmp = 0;
		//		std::cin >> tmp;
		scanf("%d", &tmp);
		arr.PushBack(tmp);
	}

	Comparator less;
	QuickSort(arr, 0, arr.Size() - 1, less);

	//	for (int i = 0; i < arr.Size(); ++i) {
	//		std::cout << arr[i] << ' ';
	//	}
	freopen("output.txt", "w", stdout);
	for (int i = 9; i < arr.Size(); i += 10) {
		printf("%d ", arr[i]);
		//std::cout << arr[i] << ' ';
	}

	unsigned long long time_end = clock();
	std::cout << std::endl << (time_end - time_begin) / 1000;

	return 0;
}
