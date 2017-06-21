#include <string>
#include <vector>
#include <iostream>
#include "node.h"

template <typename T>
NodeUPtr<T> NodeFactory<T>::create(const op::Type& operation, NodeUPtr<T> l, NodeUPtr<T> r) throw(std::bad_alloc){
	NodeUPtr<T> node;
	try {
		switch(operation) 
		{
			case op::Type::plus:
				node = std::make_unique<Plus<T>>(std::move(l), std::move(r)); break;
			case op::Type::minus:
				node = std::make_unique<Minus<T>>(std::move(l), std::move(r)); break;
			case op::Type::times:
				node = std::make_unique<Times<T>>(std::move(l), std::move(r)); break;
			case op::Type::divide:
				node = std::make_unique<Divide<T>>(std::move(l), std::move(r)); break;
			case op::Type::none:
				throw std::invalid_argument( "cannot create a 'none' node" );;
			default:
				throw std::invalid_argument( "Operative node not defined" );
		}
		
		return node;
	}
	catch(std::bad_alloc&) {
		std::cout << "memory allocation failure!!" << std::endl;
		exit(-1);
	}
}

template <typename T>
NodeUPtr<T> NodeFactory<T>::create(const op::Type& operation, T val) throw(std::bad_alloc){
	try {

		return std::make_unique<Num<T>> (val);
	}
	catch(std::bad_alloc&) {
		std::cout << "memory allocation failure!!" << std::endl;
		exit(-1);
	}
}

template <typename T>
Node<T>::Node(NodeUPtr<T> l, NodeUPtr<T> r): left(std::move(l)), right(std::move(r)) {}
template <typename T>
Node<T>::~Node() {
	std::cout << "Node<T> destructor" << std::endl;
	left.reset();
	right.reset();
}
 
template <typename T>
Plus<T>::Plus(NodeUPtr<T> l, NodeUPtr<T> r): Node<T>(std::move(l), std::move(r)) {}
template <typename T>
Plus<T>::~Plus() {
	std::cout << "Plus<T> destructor" << std::endl;
}
template <typename T>
T Plus<T>::eval() const { return this->left->eval() + this->right->eval(); }
template <typename T>
std::string Plus<T>::visit() const {
	return std::string(1,op::arith_rmap.find(op::Type::plus)->second);
}


template <typename T>
Minus<T>::Minus(NodeUPtr<T> l, NodeUPtr<T> r): Node<T>(std::move(l), std::move(r)) {}
template <typename T>
Minus<T>::~Minus() {
	std::cout << "Minus<T> destructor" << std::endl;
}
template <typename T>
T Minus<T>::eval() const {return this->left->eval() - this->right->eval(); }
template <typename T>
std::string Minus<T>::visit() const {
	return std::string(1,op::arith_rmap.find(op::Type::minus)->second);
}


template <typename T>
Times<T>::Times(NodeUPtr<T> l, NodeUPtr<T> r): Node<T>(std::move(l), std::move(r)) {}
template <typename T>
Times<T>::~Times() {
	std::cout << "Times<T> destructor" << std::endl;
}
template <typename T>
T Times<T>::eval() const {return this->left->eval() * this->right->eval(); }
template <typename T>
std::string Times<T>::visit() const {
	return std::string(1,op::arith_rmap.find(op::Type::times)->second);
}


template <typename T>
Divide<T>::Divide(NodeUPtr<T> l, NodeUPtr<T> r): Node<T>(std::move(l), std::move(r)) {}
template <typename T>
Divide<T>::~Divide() {
	std::cout << "Divide<T> destructor" << std::endl;
}
template <typename T>
T Divide<T>::eval() const {return this->left->eval() / this->right->eval(); }
template <typename T>
std::string Divide<T>::visit() const {
	return std::string(1,op::arith_rmap.find(op::Type::divide)->second);
}


template <typename T>
Num<T>::Num(T val): Node<T>(nullptr, nullptr) {num=val;}
template <typename T>
Num<T>::~Num() {
	std::cout << "Num<T> destructor" << std::endl;
}
template <typename T>
T Num<T>::eval() const {return num; }
template <typename T>
std::string Num<T>::visit() const {return std::to_string(num);}


template struct NodeFactory<int>;
template struct NodeFactory<double>;