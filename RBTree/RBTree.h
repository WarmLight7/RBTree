#pragma once
#include "BinNode.h"
#include<iostream>
#define stature(p) ((p)?(p)->height:-1)
#define IsRoot(x) (!((x)->parent))
#define BlackHeightUpdated(x)(\
	(stature((x)->lChild) == stature((x)->rChild)) && \
	((x)->height == (IsRed(x) ? stature((x)->lChild) : stature((x)->lChild) + 1)) \
)
#define IsBlack(p) (!(p) || (RB_BLACK == (p)->color))
#define IsRed(p) (!IsBlack(p)) //非黑即红
#define IsLChild(x) (!IsRoot(x) && ((x) == (x)->parent->lChild))
#define uncle(x) ( IsLChild((x)->parent) ? \
(x)->parent->parent->rChild : \
(x)->parent->parent->lChild \
) //叔叔节点
#define FromParentTo(x)(\
	IsRoot(x) ? this->_root:(\
	IsLChild(x)? (x)->parent->lChild : (x)->parent->rChild \
	)\
)//父亲节点到当前节点的指针
#define max(a, b) ((a)>(b)? (a):(b))
template<typename T> class RBTree
{
public:
	RBTree() : _size(0), _root(nullptr), _hot(nullptr) {}
	~RBTree() { delete _root; delete _hot; }
	BinNode<T>* insert(const T& );
	BinNode<T>* & search(const T& );
	BinNode<T>*& searchIn(BinNode<T>*& v, const T& e, BinNode<T>*& hot);
	bool remove(const T& );
	void solveDoubleRed(BinNode<T>* x);
	void solveDoubleBlack(BinNode<T>* r);
	int updateHeight(BinNode<T>* x);
	BinNode<T>* connect34(BinNode<T>*, BinNode<T>*, BinNode<T>*,
		BinNode<T>*, BinNode<T>*, BinNode<T>*, BinNode<T>*);
	BinNode<T>* rotateAt(BinNode<T>* v);
	BinNode<T>* removeAt(BinNode<T>*& x, BinNode<T>*& hot);
	int size() { return _size; }
	BinNode<T>* root() { return _root; }
	void swap(T& a, T& b) { T c = a; a = b; b = c; }
	void trav() {
		travIn(_root); 
		std::cout << std::endl;
	}
	void travIn(BinNode<T>* x) {
		if (!x) return;
		travIn(x->lChild);
		std::cout << x->data << " ";
		travIn(x->rChild);
	}

private:
	int _size;
	BinNode<T>* _root;
	BinNode<T>* _hot;
};

