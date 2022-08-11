#include"RBTree.cpp"
#include<iostream>
int main() {
	RBTree<int> myTree;
	std::cout << "insert" << std::endl;
	for (int i = 0; i < 10; i++) {
		myTree.insert(i);
	}
	std::cout << myTree.size() << std::endl;
	myTree.trav();
	std::cout << "remove" << std::endl;
	for (int i = 0; i < 5; i++) {
		myTree.remove(i);
	}
	std::cout << myTree.size() << std::endl;
	myTree.trav();



}