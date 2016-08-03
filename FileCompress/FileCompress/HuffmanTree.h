
#pragma once
#include <iostream>
#include <assert.h>
#include <stack>
#include "Heap.h"
using namespace std;
//Node
//HuffmanTree
//构造
//+
//>
//Heap<Node*>

typedef unsigned long LONG;
struct CharInfo
{
	unsigned char _ch;
	LONG _count;//权值
	string code;//编码 

	CharInfo(const LONG& w = 0)
		:_count(w)
		, _ch(w)
		, code("0")
	{}
	CharInfo& operator=(const CharInfo& a)
	{
		if (this != &a)
		{
			_count = a._count;
			_ch = a._ch;
			code = a.code;
		}
		return *this;
	}
	CharInfo operator+(const CharInfo& a) const
	{
		return CharInfo(this->_count + a._count);
	}

	bool operator > (const CharInfo& a)const
	{
		return this->_count > a._count;
	}

	bool operator < (const CharInfo& a)const
	{
		return this->_count < a._count;
	}
	bool operator!=(const CharInfo& a)const
	{
		return this->_count != a._count;
	}
};

template <class T>
struct HuffmanTreeNode
{
	HuffmanTreeNode* _left;
	HuffmanTreeNode* _right;
	T _weight;
	HuffmanTreeNode()
		: _left(NULL)
		, _right(NULL)
	{}
	HuffmanTreeNode(const T& w)
		:_weight(w)
		, _left(NULL)
		, _right(NULL)
	{}
};

class HuffmanTree
{
public:
	HuffmanTreeNode<CharInfo>* CreateTree(CharInfo *_infos,size_t size,CharInfo invaild)
	{
		struct Compare
		{
			bool operator()(const HuffmanTreeNode<CharInfo>* l, const HuffmanTreeNode<CharInfo>* r)
			{
				return l->_weight < r->_weight;
			}
		};

		//求出文件中每个字符出现的次数
		//CharInfo _infos[256];
		/*int size = 0;
		Get_infos_Com(filename, _infos, size);*/

		//确定出现字符并构建最小堆
		HuffmanTreeNode<CharInfo> **a = new HuffmanTreeNode<CharInfo>*[size];
		for (int i = 0,j = 0; i < 256; i++)
		{
			if (_infos[i] != invaild)
			{
				a[j] = new HuffmanTreeNode<CharInfo>;
				a[j++]->_weight = _infos[i];
			}
		}
		MinHeap<HuffmanTreeNode<CharInfo>*, Compare> mh(a, size);
		//取出最小的两个数
		Compare com;
		while (mh.Size() > 1)
		{
			HuffmanTreeNode<CharInfo> *left = mh.Top();
			mh.Pop();

			HuffmanTreeNode<CharInfo> *right = mh.Top();
			mh.Pop();

			HuffmanTreeNode<CharInfo> *newNode = new HuffmanTreeNode<CharInfo>(left->_weight + right->_weight);
			if (com(right, left))
				swap(left, right);
			newNode->_left = left;
			newNode->_right = right;
			mh.Push(newNode);
		}
		_root = mh.Top();
		return _root;
	}
	HuffmanTreeNode<CharInfo>* GetRoot()
	{
		return _root;
	}
protected:
	void Code(CharInfo& ci,int num)
	{
		int temp = num;
		int size = 0;
		if (temp == 0)
			size = 1;
		while (temp)
		{
			temp >>= 1;
			size++;
		}
		char *a = new char[size+1];
		memset(a, 0, size + 1);
		for (int i = size; i > 0; i--)
		{
			a[i - 1] =  '0' + (num & 1);
			num >>= 1;
		}
		string tp(a);
		swap(tp, ci.code);
	}
	void GetCode(CharInfo *_infos)
	{
		assert(_root);
		stack<HuffmanTreeNode<CharInfo>*> s;
		s.push(_root);
 		int num = 0;
		HuffmanTreeNode<CharInfo>* p = s.top();
		while (!s.empty())
		{
			while (p->_left != NULL)
			{
				s.push(p->_left);
				p = p->_left;
				num <<= 1;
			}
			p = s.top();
			s.pop();
			if (strcmp(p->_weight.code.c_str(), "0") == 0)////////////////////////////////
			{
				Code(p->_weight, num);
				_infos[p->_weight._ch].code = p->_weight.code.c_str();
			}
			else
				break;
			num >>= 1;
			p = s.top();
			if (p->_right != NULL)
			{
				s.push(p->_right);
				p = p->_right;
				num = (num << 1) | 1;
			}
		}
	}
	
protected:
	HuffmanTreeNode<CharInfo>* _root;
};

void TestHuffmanTree()
{
//	HuffmanTree ht;
	cout << "1";
} 