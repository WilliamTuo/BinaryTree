#pragma once

#include <iostream>

template <class K, class V>
struct AVLTreeNode
{
	AVLTreeNode(const K& key = K(), const V& value = V())
		: _left(nullptr)
		, _right(nullptr)
		, _parent(nullptr)
		, _key(key)
		, _value(value)
		, _pb(0)
	{}

	
	AVLTreeNode<K, V>* _left;
	AVLTreeNode<K, V>* _right;
	AVLTreeNode<K, V>* _parent;
	K _key;
	V _value;
	int _pb;		// 记录节点的平衡因子
};

template <class K, class V>
class AVLTree
{
	typedef AVLTreeNode<K, V> Node;
	typedef Node* PNode;

public:
	AVLTree()
		: _pRoot(nullptr)
	{}

	bool Insert(const K& key, const V& value)
	{
		if (_pRoot == nullptr)
		{
			_pRoot = new Node(key, value);
			return true;
		}

		PNode cur = _pRoot;
		PNode parent = nullptr;

		while (cur)
		{
			parent = cur;

			if (cur->_key > key)
			{
				cur = cur->_left;
			}
			else if (cur->_key < key)
			{
				cur = cur->_right;
			}
			else
			{
				return false;
			}
		}
		
		cur = new Node(key, value);
	
		if (key < parent->_key)
		{
			parent->_left = cur;
		}
		else
		{
			parent->_right = cur;
		}

		cur->_parent = parent;  // 更新新插入节点的 parent 节点


		while (parent)
		{
			if (parent->_left == cur)
			{
				parent->_pb--;
			}
			else
			{
				parent->_pb++; 
			}

			if (parent->_pb == 0)
			{
				break;
			}
			else if (parent->_pb == 1 || parent->_pb == -1)
			{
				cur = parent;
				parent = cur->_parent;
			}
			else
			{
				if (parent->_pb == 2)
				{
					if (1 == cur->_pb)
					{
						RotateL(parent);
					}
					else
					{
						RotateRL(parent);
					}
				}
				else
				{
					if (-1 == cur->_pb)
					{
						RotateR(parent);
					}
					else
					{
						RotateLR(parent);
					}
				}

				break;
			}
		}

		return true;
	}

	void InOrder()
	{
		_InOrder(_pRoot);
	}

	bool IsBalance()
	{
		return _IsBalance(_pRoot);
	}

	~AVLTree()
	{
		_Destroy(_pRoot);
	}

private:
	void _InOrder(PNode pRoot)
	{
		if (pRoot)
		{
			_InOrder(pRoot->_left);
			std::cout << "<" << pRoot->_key << ", " << pRoot->_value << "> " << std::endl;
			std::cout << ">" << pRoot->_pb << std::endl;
			_InOrder(pRoot->_right);
		}
	}

	bool _IsBalance(PNode pRoot)
	{
		if (pRoot == nullptr)
			return true;;
		
		int left = GetHight(pRoot->_left);
		int right = GetHight(pRoot->_right);
		int diff = right - left;

		if (abs(diff) > 1 || diff != pRoot->_pb)
			return false;

		return _IsBalance(pRoot->_left) && _IsBalance(pRoot->_right);
	}

	int GetHight(PNode pRoot)
	{
		if (pRoot == nullptr)
			return 0;

		int left = GetHight(pRoot->_left);
		int right = GetHight(pRoot->_right);

		return (left > right) ? (left + 1) : (right + 1);
	}

	void _Destroy(PNode pRoot)
	{
	}


	void RotateL(PNode parent)
	{
		PNode subR = parent->_right;
		PNode subRL = subR->_left;

		subR->_left = parent;
		parent->_right = subRL;

		if (subRL)
			subRL->_parent = parent;

		PNode pparent = parent->_parent;
		subR->_parent = pparent;
		parent->_parent = subR;

		if (pparent == nullptr)
			_pRoot = subR;
		else
		{
			if (pparent->_left == parent)
				pparent->_left = subR;
			else 
				pparent->_right = subR;
		}

		parent->_pb = subR->_pb = 0;
	}

	void RotateR(PNode parent)
	{
		PNode subL = parent->_left;
		PNode subLR = subL->_right;

		subL->_right = parent;
		parent->_left = subLR;

		if (subLR)
			subLR->_parent = parent;

		PNode pparent = parent->_parent;
		subL->_parent = pparent;
		parent->_parent = subL;

		if (pparent == nullptr)
		{
			_pRoot = subL;
		}
		else
		{
			if (parent == pparent->_left)
				pparent->_left = subL;
			else
				pparent->_right = subL;
		}

		parent->_pb = subL->_pb = 0;
	}

	void RotateRL(PNode parent)
	{
		PNode subR = parent->_right;
		PNode subRL = subR->_left;
		int pb = subRL->_pb;

		RotateR(parent->_right);
		RotateL(parent);

		if (pb == 1)
			parent->_pb = -1;
		else if (pb == -1)
			subR->_pb = 1;
	}

	void RotateLR(PNode parent)
	{
		PNode subL = parent->_left;
		PNode subLR = subL->_right;
		int pb = subLR->_pb;

		RotateL(parent->_left);
		RotateR(parent);

		if (pb == 1)
			subL->_pb = -1;
		else if (pb == -1)
			parent->_pb = 1;
	}

private:
	PNode _pRoot;
};


void TestAVLTree()
{
	int array[] = {16, 3, 7, 11, 9, 26, 18, 14, 15};
	//int array[] = {4, 2, 6, 1, 3, 5, 15, 7, 16, 14};

	AVLTree<int, int> t;

	for (auto e : array)
		t.Insert(e, e);

	t.InOrder();

	if (t.IsBalance())
		std::cout << "t is a Balance tree" << std::endl;
	else
		std::cout << "t is not a Balance tree" << std::endl;

}

