#pragma once
typedef enum{RB_RED, RB_BLACK} RBColor;
template<typename T> struct BinNode
{
	T data;
	int height;
	BinNode<T>* parent;
	BinNode<T>* lChild;
	BinNode<T>* rChild;
	RBColor color;
	BinNode():parent(nullptr),lChild(nullptr),rChild(nullptr), height(-1), color(RB_RED) {}
	BinNode(T e, BinNode<T>* p = nullptr, BinNode<T>* lc = nullptr, BinNode<T>* rc = nullptr,
		int h = -1, RBColor c = RB_RED)
		: data(e), parent(p), lChild(lc), rChild(rc), height(h), color(c) { }
	BinNode<T>* succ();
	bool operator<(BinNode const& bn) { return data < bn.data; } //小于
	bool operator==(BinNode const& bn) { return data == bn.data; } //等于
};

template<typename T>
BinNode<T>* BinNode<T>::succ()
{
	BinNode<T>* s = this;
	if (rChild) {
		s = rChild;
		while (s->lChild) {
			s = s->lChild;
		}
	}
	else {
		while ((s->parent) && (s->parent->rChild)) s = s->parent;
		s = s->parent;
	}
	return nullptr;
}
