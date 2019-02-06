#pragma once 

#include <iostream>

/**
 * 红黑树的五条性质：
 *	（1）所有的节点不是黑色就是红色
 *	（2）根节点的颜色一定是黑色
 *	（3）由根节点出发到叶子节点的任意一条简单路径上不能有连续的红色节点
 *	（4）由任意节点出发到叶子节点的任意一条简单路径上的黑色节点数目相同
 *	（5）所有的叶子节点一定是黑色（nullptr 节点也看作叶子节点）
 */

enum Color{RED, BLACK};

template <class V>
struct RBTreeNode
{
	RBTreeNode(const V& value = V(), Color color = RED)
		: _left(nullptr)
		, _right(nullptr)
		, _parent(nullptr)
		, _value(value)
		, _color(color)
	{}

	RBTreeNode<V>* _left;
	RBTreeNode<V>* _right;
	RBTreeNode<V>* _parent;
	V _value;
	Color _color;
};

template <class V>
class RBTree
{
	typedef RBTreeNode<V> Node;
	typedef Node* PNode;

public:
	RBTree()
	{
		_pHead = new Node;
		_pHead->_left = _pHead;
		_pHead->_right = _pHead;
		_pHead->_parent = nullptr;
	}

	bool Insert(const V& value)
	{
		PNode pRoot = GetRoot();

		if (pRoot == nullptr)
		{
			pRoot = new Node(value, BLACK);
			return true;
		}

		PNode pCur = pRoot;
		PNode pParent = nullptr;
		while (pCur)
		{
			pParent = pCur;
			if (pCur->_value > value)
				pCur = pCur->_left;
			else if (pCur->_value < value)
				pCur = pCur->_right;
			else
				return false;
		}

		pCur = new Node(value);
		if (pCur == pParent->_left)
			pParent->_left = pCur;
		else
			pParent->_right = pCur;

		pCur->_parent = pParent;
		

		while (pParent && pParent->_color == RED)
		{
			PNode pGrandFather = pParent->_parent;
			PNode pUncle = nullptr;

			if (pGrandFather->_left == pParent)
			{
				pUncle = pGrandFather->_right;

				// 情况一 叔叔节点存在并且颜色是红色
				if (pUncle && pUncle->_color == RED)
				{
						pParent->_color = BLACK;
						pUncle->_color = BLACK;
						pGrandFather->_color = RED;
						pCur = pGrandFather;
						pParent = pCur->_parent;
				}
				else
				{
					// 叔叔节点存在且为黑色
					// 情况二 当前节点为右孩子
					if (pCur == pParent->_right)
					{
						pCur = pParent;
						RotateL(pGrandFather);
					}


					//　情况三 当前节点为左孩子
					pParent->_color = BLACK;
					pGrandFather->_color = RED;
					RotateR(pGrandFather);
				}
	
			}
			else
			{
				pUncle = pGrandFather->_left;

				if (pUncle->_color == RED)
				{
					pParent->_color = BLACK;
					pGrandFather->_color = RED;
					pUncle->_color = BLACK;
					pCur = pGrandFather;
					pParent = pCur->_parent;
				}
				else
				{
					if (pCur = pParent->_left)
					{
						pCur = pParent;
						RotateR(pParent);
					}
					else
					{
						pParent->_color = BLACK;
						pGrandFather->_color = RED;
						RotateL(pGrandFather);
					}

				} // end of if ( pUncle->_color == RED )
	
			} // end of if ( pGrandFather->_left == pParent )

		} // end of while ( pParent && pParent->_color == RED )

		_pHead->_left = LeftMost();
		_pHead->_right = RightMost();
		_pHead->_parent->_color = BLACK;

		return true;
	}

	void InOrder()
	{
		_InOrder(GetRoot());
	}

	bool IsValidRBTree()
	{
		PNode& pRoot = GetRoot();

		if (pRoot == nullptr)
			return true;

		if (pRoot->_color == RED)
		{
			std::cout << "违反性质二 根节点的颜色一定是黑色" << std::endl;
			return false;
		}

		std::size_t blackCount = 0;
		PNode pCur = pRoot;

		while (pCur)
		{
			if (pCur->_color == BLACK)
				blackCount++;

			pCur = pCur->_left;
		}

		std::size_t pathCount = 0;

		return _IsValidRBTree(pRoot, pathCount, blackCount);
	}

private:
	void RotateL(PNode pParent)
	{
		PNode pSubR = pParent->_right;
		PNode pSubRL = pSubR->_left;

		pParent->_right = pSubRL;
		pSubR->_left = pParent;
		if (pSubRL)
			pSubRL->_parent = pParent;

		PNode ppParent = pParent->_parent;
		pParent->_parent = pSubR;
		pSubR->_parent = ppParent;

		PNode& pRoot = GetRoot();

		if (pParent == pRoot)
		{
			pRoot = pSubR;
		}
		else
		{
			if (ppParent->_left == pParent)
			{
				ppParent->_left = pSubR;
			}
			else
			{
				ppParent->_right = pSubR;
			}
		}
	}


	void RotateR(PNode pParent)
	{
		PNode pSubL = pParent->_left;
		PNode pSubLR = pSubL->_right;

		pSubL->_right = pParent;
		pParent->_left = pSubLR;

		if (pSubLR)
			pSubLR->_parent = pParent;

		PNode ppParent = pParent->_parent;
		pParent->_parent = pSubL;
		pSubL->_parent = ppParent;

		PNode& pRoot = GetRoot();
		if (pParent == pRoot)
		{
			pRoot = pSubL;
		}
		else 
		{
			if (pParent == ppParent->_left)
			{
				ppParent->_left = pSubL;
			}
			else
			{
				ppParent->_right = pSubL;
			}
		}
	}

	void _InOrder(PNode pRoot)
	{
		if (pRoot == nullptr)
			return;

		_InOrder(pRoot->_left);
		std::cout << pRoot->_value << std::endl;
		_InOrder(pRoot->_right);
	}

	PNode& GetRoot()
	{
		return _pHead->_parent;
	}

	PNode LeftMost()
	{
		PNode pRoot = GetRoot();

		if (pRoot == nullptr)
			return _pHead;

		PNode pCur = pRoot;
		while (pCur->_left)
			pCur = pCur->_left;

		return pCur;
	}

	PNode RightMost()
	{
		PNode pRoot = GetRoot();

		if (pRoot == nullptr)
			return _pHead;

		PNode pCur = pRoot;
		while (pCur->_right)
			pCur = pCur->_right;

		return pCur;
	}

	bool _IsValidRBTree(PNode pRoot, std::size_t pathCount, std::size_t blackCount)
	{
		if (pRoot == nullptr)
			return true;

		PNode pParent = pRoot->_parent;

		if (pParent && pParent->_color == RED && pRoot->_color == RED)
		{
			std::cout << "违反性质三：由根节点出发至叶子节点的同一条简单路径上不能有连续的红色节点" << std::endl;
			return false;
		}

		if (pRoot->_color = BLACK)
		{
			pathCount++;
		}

		if (pRoot->_left == nullptr && pRoot->_right == nullptr)
		{
			if (pathCount != blackCount)
			{
				std::cout << "违反性质四：由任意节点出发到叶子节点的简单路径上的黑色节点数目必须相同" << std::endl;
				return false;
			}
		}

		return _IsValidRBTree(pRoot->_left, pathCount, blackCount) &&
			   _IsValidRBTree(pRoot->_right, pathCount, blackCount);
	}

private:
	PNode _pHead;
};

void TestRBTree()
{
	int array[] = {5, 3, 4, 1, 7, 8, 2, 6, 0, 9};
	RBTree<int> t;

	for (auto e: array)
		t.Insert(e);

	t.InOrder();

	if (t.IsValidRBTree())
	{
		std::cout << "t is a valid rbtree" << std::endl;
	}
	else
	{
		std::cout << "t is not a valid rbtree" << std::endl;
	}
}

