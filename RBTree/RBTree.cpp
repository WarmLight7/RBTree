#include"RBTree.h"

template<typename T> BinNode<T>* RBTree<T>::insert(const T& e)
{// 将关键码e插入红黑树
	BinNode<T>* & x = search(e); 
	if (x) return x;// 找到了直接寄,找不到hot也会返回该插入的位置
	x = new BinNode<T>(e, this->_hot, nullptr, nullptr, -1);//以hot为父亲，黑高度为-1
	this->_size++;
	solveDoubleRed(x);// 插入的默认是红色的，所以会出现双红
	return x;
}
template<typename T> BinNode<T>*& RBTree<T>::search(const T& e)
{
	return searchIn(this->_root, e, this->_hot = nullptr);
}
template<typename T> BinNode<T>*& RBTree<T>::searchIn(BinNode<T>*& v, const T& e, BinNode<T>*& hot)
{
	if (!v || (e == v->data)) return v; // 找到找不到都寄了
	hot = v; // 存爹
	return searchIn(((e < v->data) ? v->lChild : v->rChild), e, hot); //深入一局，递归查找
}
template<typename T> bool RBTree<T>::remove(const T& e)
{
	BinNode<T>*& x = search(e); if (!x) return false;// 找不到直接寄，找到了hot能存他爹
	BinNode<T>* r = removeAt(x, this->_hot);// 进行一个除的删
	if (0 >= --(this->_size)) return true;// 如果删完了皆大欢喜
	if (!this->_hot) {// 如果删除的是根节点，把补上来的节点置黑，更新高度，这里也没人补它，所以不可能两边都有子树，所以直接更新高度返回就行
		this->_root->color = RB_BLACK; updateHeight(this->_root); return true;
	}
	// 现在r为真实所在位置，并且非根
	if (BlackHeightUpdated(this->_hot))return true; // 如果父亲还是平衡的，就不用修改，否则，相当于r所在子树黑高度少了1
	if (IsRed(r)) {// 如果r是红色，只需要将其转黑就可以平衡
		r->color = RB_BLACK; r->height++; return true;
	}
	// 现在是又不平衡，又双黑，所以要进行双黑调整
	solveDoubleBlack(r);
	return true;
}
template<typename T> void RBTree<T>::solveDoubleRed(BinNode<T>* x)
{
	if (x == this->_root) {// 如果是根节点直接染色 + 高度+1 起飞
		x->color = RB_BLACK;
		x->height++;
		return;
	}
	BinNode<T>* p = x->parent;
	if (IsBlack(p)) return;// 如果不需要双红修正直接寄！
	BinNode<T>* g = p->parent;
	BinNode<T>* u = uncle(x);
	if (IsBlack(u)) {// RR-1 进行一次转的旋，调整后根节点置为黑色，叶子节点置为红色
		if (IsLChild(x) == IsLChild(p)) { // 位于同侧的时候p为根节点 zig-zig或者 zag-zag
			p->color = RB_BLACK;
		}
		else {// 否则x为根节点
			x->color = RB_BLACK;
		}
		g->color = RB_RED;
		BinNode<T>* gg = g->parent;// 不要忘记连接祖父节点，FromParentTo已经处理了为根节点的情况，所以这一步不需要额外讨论
		BinNode<T>* r = FromParentTo(g) = rotateAt(x);
		r->parent = gg;
	}
	else {// RR-2
		p->color = RB_BLACK; p->height++;// 蚌埠住了要提升高度了，直接进行一个色的转，转完色去处理高层
		u->color = RB_BLACK; u->height++;
		if (!IsRoot(g)) g->color = RB_RED;// 这里如果是根节点是不能进行色的转的，就结束了
		solveDoubleRed(g);
	}
}
template<typename T>
void RBTree<T>::solveDoubleBlack(BinNode<T>* r)
{
	BinNode<T>* p = r ? r->parent : this->_hot; if (!p)return; // 这里直接this->hot不就行了吗，我不知道为啥会出现p空的情况
	BinNode<T>* s = (r == p->lChild) ? p->rChild : p->lChild; // s为r的兄弟节点
	if (IsBlack(s)) {//如果兄弟节点是黑的
		// 如果兄弟节点的子节点有红的
		BinNode<T>* t = nullptr;
		if (s->lChild && IsRed(s->lChild)) t = s->lChild;
		if (s->rChild && IsRed(s->rChild)) t = s->rChild;
		if (t) { //BB-1   既然兄弟节点有红的，那直接从红的那个往上转就行了
			RBColor oldcolor = p->color;
			BinNode<T>* b = FromParentTo(p) = rotateAt(t);
			if (b->lChild){
				b->lChild->color = RB_BLACK; updateHeight(b->lChild);
			}
			if (b->rChild) {
				b->rChild->color = RB_BLACK; updateHeight(b->rChild);
			}
			b->color = oldcolor;
		}
		else {// BB-2 兄弟的俩儿子都是黑的，此时兄弟肯定变红
			s->color = RB_RED; s->height--;
			if (IsRed(p)) { // BB-2-R 如果p是红色，那把p的颜色变黑就行,变黑维持高度不变
				p->color = RB_BLACK;
			}
			else {// BB-2-B 爹也是黑的，那就得降层数了，直接让xd变红，此时爹层数降低，再处理上层
				p->height--;
				solveDoubleBlack(p);
			}
		}
	}
	else {// BB-3 如果兄弟本来就是红色 兄弟那边还是高了，要往这边转的，选个同侧旋转 兄弟和父亲都变色
		s->color = RB_BLACK; p->color = RB_RED;
		BinNode<T>* t = IsLChild(s) ? s->lChild : s->rChild;
		this->_hot = p; //转完之后s为祖宗节点，那s不需要
		FromParentTo(p) = rotateAt(t);
		// 转完之后继续处理r
		solveDoubleBlack(r);
	}
}
template<typename T> int RBTree<T>::updateHeight(BinNode<T>* x)
{
	if (!x) return -1;
	x->height = max(stature(x->lChild), stature(x->rChild));
	return IsBlack(x) ? x->height++ : x->height;
}
template<typename T> BinNode<T>* RBTree<T>::connect34(BinNode<T>* a, BinNode<T>* b, BinNode<T>* c, BinNode<T>* t0, BinNode<T>* t1, BinNode<T>* t2, BinNode<T>* t3)
{// t0 a t1 b t2 c t3 的连，构建子树
	a->lChild = t0; if (t0) t0->parent = a;
	a->rChild = t1; if (t1) t1->parent = a;
	updateHeight(a);
	c->lChild = t2; if (t2) t2->parent = c;
	c->rChild = t3; if (t3) t3->parent = c;
	updateHeight(c);
	b->lChild = a; a->parent = b;
	b->rChild = c; c->parent = b;
	updateHeight(b);
	return b;
}

template<typename T> BinNode<T>* RBTree<T>::rotateAt(BinNode<T>* v)
{
	BinNode<T>* p = v->parent;
	BinNode<T>* g = p->parent;
	if (IsLChild(v)) {
		if (IsLChild(p)) {// zig-zig
			p->parent = g->parent;
			return connect34(v, p, g, v->lChild, v->rChild, p->rChild, g->rChild);
		}
		else {// zig-zag
			v->parent = g->parent;
			return connect34(p, v, g, p->lChild, v->lChild, v->rChild, g->rChild);
		}
	}
	else {
		if (IsLChild(p)) {// zag-zig
			v->parent = g->parent;
			return connect34(g, v, p, g->lChild, v->lChild, v->rChild, p->rChild);
		}
		else {// zag-zag
			p->parent = g->parent;
			return connect34(g, p, v, g->lChild, p->lChild, v->lChild, v->rChild);
		}
	}
}

template<typename T> BinNode<T>* RBTree<T>::removeAt(BinNode<T>*& x, BinNode<T>*& hot)
{
	BinNode<T>* w = x;
	BinNode<T>* succ = nullptr; // 实际被删除节点的接替者
	if (!(x->lChild)) {// 没有左子树的时候继承节点为右子树
		succ = x = x->rChild;
	}
	else if (!(x->rChild)) { // 没有右子树的时候继承节点为左子树
		succ = x = x->lChild;
	}
	else {// 如果左右子树都有，就在右子树中寻找直接后继，直接后继一定没有左子树
		x = x->succ();
		swap(x->data, w->data);
		BinNode<T>* u = w->parent;
		((u == x) ? u->rChild : u->lChild) = succ = w->rChild;
	}
	hot = w->parent;
	if (succ) {
		succ->parent = hot;
	}
	delete w;
	return succ;
}
