#pragma once

#include <iostream>

template <class T>
struct BSTreeNode
{
	BSTreeNode(const T& data = T())
		: _left(nullptr)
		, _right(nullptr)
		, _data(data)
	{}

	~BSTreeNode()
	{}

	BSTreeNode* _left;
	BSTreeNode* _right;
	T _data;
};

template <class T>
class BSTree
{
	typedef BSTreeNode<T> Node;
	typedef Node* PNode;

public:
	BSTree()
		: _pRoot(nullptr)
	{}

	bool Insert(const T& data)
	{
		if (_pRoot == nullptr)
		{
			_pRoot= new Node(data);
			return true;
		}
		PNode parent = _pRoot;
		PNode cur = _pRoot;

		while (cur)
		{
			if (cur->_data > data)
			{
				parent = cur;
				cur = cur->_left;
			}
			else if (cur->_data < data)
			{
				parent = cur;
				cur = cur->_right;
			}
			else
			{
				return false;
			}
		}

		cur = new Node(data);

		if (cur->_data > parent->_data)
			parent->_right = cur;
		else
			parent->_left = cur;

		return true;
	}

	bool Release(const T& data)
	{
		if (_pRoot == nullptr)
			return false;

		PNode parent = _pRoot;
		PNode cur = _pRoot;

		while (cur)
		{
			if (cur->_data > data)
			{
				parent = cur;
				cur = cur->_left;
			}
			else if (cur->_data < data)
			{
				parent = cur;
				cur = cur->_right;
			}
			else
			{
				break;
			}
		}

		if (cur == nullptr)
			return false;

		PNode pDel = cur;

		// 待删除节点的左为空
		if (cur->_left == nullptr)
		{
			// 如果是当前节点是根节点
			if (cur == _pRoot)
			{
				_pRoot = cur->_right;
			}
			else
			{
				// 如果是双亲的左孩子
				if (cur == parent->_left)
				{
					 parent->_left = cur->_right;
				}
				// 如果是双亲的右孩子
				else
				{
					parent->_right = cur->_right;
				}
			}
		}
		// 待删除节点的右为空
		else if (cur->_right == nullptr)
		{
			if (cur == _pRoot)
			{
				_pRoot = cur->_left;
			}
			else
			{
				if (cur == parent->_left)
				{
					parent->_left = cur->_left; 
				}
				else
				{
					parent->_right = cur->_left;
				}
			}
		}
		// 待删除节点的左右均 不为空
		else
		{
			parent = cur;
			cur = cur->_right;
			
			while (cur->_left)
			{
				parent = cur;
				cur = cur->_left;
			}

			pDel->_data = cur->_data;
			pDel = cur;

			if (cur == parent->_left)
				parent->_left = cur->_right;
			else
				parent->_right = cur->_right;
		}

		delete pDel;
		return true;
	}

	PNode Find(const T& data)
	{
		PNode cur = _pRoot;

		while (cur)
		{
			if (cur->_data > data)
			{
				cur = cur->_left;
			}
			else if (cur->_data < data)
			{
				cur = cur->_right;
			}
			else
			{
				return cur;
			}
		}

		return nullptr;
	}

	void InOrder()
	{
		_InOrder(_pRoot);
	}

	~BSTree()
	{
		_Destroy(_pRoot);
	}

private:
	void _InOrder(PNode pRoot)
	{
		if (pRoot)
		{
			_InOrder(pRoot->_left);
			std::cout << pRoot->_data << " ";
			_InOrder(pRoot->_right);
		}
	}

	void _Destroy(PNode pRoot)
	{
		if (pRoot)
		{
			_Destroy(pRoot->_left);
			_Destroy(pRoot->_right);
			pRoot = nullptr;
		}
	}

private:
	PNode _pRoot;
};

void TestBSTree()
{
	int array[] = {5, 3, 7, 1, 4, 6, 8, 0, 2, 9};
	BSTree<int> bt;

	for (auto e: array)
		bt.Insert(e);

	BSTreeNode<int>* ret = bt.Find(5);
	std::cout << ret->_data << std::endl;

	bt.InOrder();
	std::cout << std::endl;
}

