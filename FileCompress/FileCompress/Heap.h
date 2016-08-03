#pragma once 
#include <iostream>
#include <vector>
#include <assert.h>
using namespace std;
//建最小堆，使用仿函数
template <class T>
struct Less
{
	bool operator()(const T& l, const T& r)
	{
		return l < r;
	}
};

//template <class T>
//struct Less<CharInfo<T>>
//{
//	bool operator()(const CharInfo& l, const CharInfo& r)
//	{
//		return l->_weight < r->_weight;
//	}
//};

template <class T,class Compare>
class MinHeap
{
public:
	MinHeap(T *a,size_t size)
		: _size(size)
	{
		_a.resize(size);
		for (int i = 0; i < size; i++)
			_a[i] = a[i];
		for (int i = (size - 2) / 2; i >= 0; i--)
			_AdjustDown(i);
	}
	void Push(const T& data)
	{
		_a.push_back(data);
		_AdjustUp(_size++);
	}

	void Pop()
	{
		swap(_a[0], _a[--_size]);
		_a.pop_back();
		_AdjustDown(0); 
	}
	T& Top()
	{
		return _a[0];
	}
	void Print()
	{
		for (int i = 0; i < _size; i++)
			cout << _a[i] << " ";
		cout << endl;
	}
	size_t Size()
	{
		return _a.size();
	}
protected:
	void _AdjustDown(int parent)
	{
		Compare com;
		int child = parent * 2 + 1;
		while (child < _size)
		{
			if (child + 1 < _size && com(_a[child + 1], _a[child]))
				child++;
			if (com(_a[child], _a[parent]))
			{
				swap(_a[child], _a[parent]);
				parent = child;
				child = parent * 2 + 1;
			}
			else
				break;
		}
	}
	void _AdjustUp(int child)
	{
		Compare com;
		int parent = (child - 1) / 2;
		while (child > 0)
		{
			if (com(_a[child], _a[parent]))
			{
				swap(_a[child], _a[parent]);
				child = parent;
				parent = (child - 1) / 2;
			}
			else break;
		}
	}
protected:
	vector<T> _a;
	size_t _size;
};


void TestHeap()
{
	int a[] = { 6, 5, 4, 3, 8, 7, 1, 0, 9, 2 };
	MinHeap<int,Less<int>> mh(a,sizeof(a)/sizeof(a[0]));
	mh.Print();
	mh.Push(33);
	mh.Print();
	mh.Pop();
	mh.Print();
}