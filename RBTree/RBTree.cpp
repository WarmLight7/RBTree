#include"RBTree.h"

template<typename T> BinNode<T>* RBTree<T>::insert(const T& e)
{// ���ؼ���e��������
	BinNode<T>* & x = search(e); 
	if (x) return x;// �ҵ���ֱ�Ӽ�,�Ҳ���hotҲ�᷵�ظò����λ��
	x = new BinNode<T>(e, this->_hot, nullptr, nullptr, -1);//��hotΪ���ף��ڸ߶�Ϊ-1
	this->_size++;
	solveDoubleRed(x);// �����Ĭ���Ǻ�ɫ�ģ����Ի����˫��
	return x;
}
template<typename T> BinNode<T>*& RBTree<T>::search(const T& e)
{
	return searchIn(this->_root, e, this->_hot = nullptr);
}
template<typename T> BinNode<T>*& RBTree<T>::searchIn(BinNode<T>*& v, const T& e, BinNode<T>*& hot)
{
	if (!v || (e == v->data)) return v; // �ҵ��Ҳ���������
	hot = v; // ���
	return searchIn(((e < v->data) ? v->lChild : v->rChild), e, hot); //����һ�֣��ݹ����
}
template<typename T> bool RBTree<T>::remove(const T& e)
{
	BinNode<T>*& x = search(e); if (!x) return false;// �Ҳ���ֱ�Ӽģ��ҵ���hot�ܴ�����
	BinNode<T>* r = removeAt(x, this->_hot);// ����һ������ɾ
	if (0 >= --(this->_size)) return true;// ���ɾ���˽Դ�ϲ
	if (!this->_hot) {// ���ɾ�����Ǹ��ڵ㣬�Ѳ������Ľڵ��úڣ����¸߶ȣ�����Ҳû�˲��������Բ��������߶�������������ֱ�Ӹ��¸߶ȷ��ؾ���
		this->_root->color = RB_BLACK; updateHeight(this->_root); return true;
	}
	// ����rΪ��ʵ����λ�ã����ҷǸ�
	if (BlackHeightUpdated(this->_hot))return true; // ������׻���ƽ��ģ��Ͳ����޸ģ������൱��r���������ڸ߶�����1
	if (IsRed(r)) {// ���r�Ǻ�ɫ��ֻ��Ҫ����ת�ھͿ���ƽ��
		r->color = RB_BLACK; r->height++; return true;
	}
	// �������ֲ�ƽ�⣬��˫�ڣ�����Ҫ����˫�ڵ���
	solveDoubleBlack(r);
	return true;
}
template<typename T> void RBTree<T>::solveDoubleRed(BinNode<T>* x)
{
	if (x == this->_root) {// ����Ǹ��ڵ�ֱ��Ⱦɫ + �߶�+1 ���
		x->color = RB_BLACK;
		x->height++;
		return;
	}
	BinNode<T>* p = x->parent;
	if (IsBlack(p)) return;// �������Ҫ˫������ֱ�Ӽģ�
	BinNode<T>* g = p->parent;
	BinNode<T>* u = uncle(x);
	if (IsBlack(u)) {// RR-1 ����һ��ת��������������ڵ���Ϊ��ɫ��Ҷ�ӽڵ���Ϊ��ɫ
		if (IsLChild(x) == IsLChild(p)) { // λ��ͬ���ʱ��pΪ���ڵ� zig-zig���� zag-zag
			p->color = RB_BLACK;
		}
		else {// ����xΪ���ڵ�
			x->color = RB_BLACK;
		}
		g->color = RB_RED;
		BinNode<T>* gg = g->parent;// ��Ҫ���������游�ڵ㣬FromParentTo�Ѿ�������Ϊ���ڵ�������������һ������Ҫ��������
		BinNode<T>* r = FromParentTo(g) = rotateAt(x);
		r->parent = gg;
	}
	else {// RR-2
		p->color = RB_BLACK; p->height++;// ����ס��Ҫ�����߶��ˣ�ֱ�ӽ���һ��ɫ��ת��ת��ɫȥ����߲�
		u->color = RB_BLACK; u->height++;
		if (!IsRoot(g)) g->color = RB_RED;// ��������Ǹ��ڵ��ǲ��ܽ���ɫ��ת�ģ��ͽ�����
		solveDoubleRed(g);
	}
}
template<typename T>
void RBTree<T>::solveDoubleBlack(BinNode<T>* r)
{
	BinNode<T>* p = r ? r->parent : this->_hot; if (!p)return; // ����ֱ��this->hot�����������Ҳ�֪��Ϊɶ�����p�յ����
	BinNode<T>* s = (r == p->lChild) ? p->rChild : p->lChild; // sΪr���ֵܽڵ�
	if (IsBlack(s)) {//����ֵܽڵ��Ǻڵ�
		// ����ֵܽڵ���ӽڵ��к��
		BinNode<T>* t = nullptr;
		if (s->lChild && IsRed(s->lChild)) t = s->lChild;
		if (s->rChild && IsRed(s->rChild)) t = s->rChild;
		if (t) { //BB-1   ��Ȼ�ֵܽڵ��к�ģ���ֱ�ӴӺ���Ǹ�����ת������
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
		else {// BB-2 �ֵܵ������Ӷ��Ǻڵģ���ʱ�ֵܿ϶����
			s->color = RB_RED; s->height--;
			if (IsRed(p)) { // BB-2-R ���p�Ǻ�ɫ���ǰ�p����ɫ��ھ���,���ά�ָ߶Ȳ���
				p->color = RB_BLACK;
			}
			else {// BB-2-B ��Ҳ�Ǻڵģ��Ǿ͵ý������ˣ�ֱ����xd��죬��ʱ���������ͣ��ٴ����ϲ�
				p->height--;
				solveDoubleBlack(p);
			}
		}
	}
	else {// BB-3 ����ֵܱ������Ǻ�ɫ �ֵ��Ǳ߻��Ǹ��ˣ�Ҫ�����ת�ģ�ѡ��ͬ����ת �ֵܺ͸��׶���ɫ
		s->color = RB_BLACK; p->color = RB_RED;
		BinNode<T>* t = IsLChild(s) ? s->lChild : s->rChild;
		this->_hot = p; //ת��֮��sΪ���ڽڵ㣬��s����Ҫ
		FromParentTo(p) = rotateAt(t);
		// ת��֮���������r
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
{// t0 a t1 b t2 c t3 ��������������
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
	BinNode<T>* succ = nullptr; // ʵ�ʱ�ɾ���ڵ�Ľ�����
	if (!(x->lChild)) {// û����������ʱ��̳нڵ�Ϊ������
		succ = x = x->rChild;
	}
	else if (!(x->rChild)) { // û����������ʱ��̳нڵ�Ϊ������
		succ = x = x->lChild;
	}
	else {// ��������������У�������������Ѱ��ֱ�Ӻ�̣�ֱ�Ӻ��һ��û��������
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
