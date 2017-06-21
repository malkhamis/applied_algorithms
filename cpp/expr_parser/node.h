#ifndef NODE_H
#define NODE_H
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>

template <typename T> class Node; //forward declaration
template <typename T> using NodeUPtr = std::unique_ptr<Node<T>>;

namespace op{
	enum class Type {plus, minus, times, divide, exp, number, l_paren, r_paren, none};
	//const std::set<char> ops = {'+','-','*','/'};
	const std::map<Type,int> prec_map = {
		{Type::plus,1}, {Type::minus,1},
		{Type::times,2}, {Type::divide,2},
		{Type::exp,3}	};
	const std::map<char,Type> arith_map = {
		{'+',Type::plus}, {'-',Type::minus},
		{'*',Type::times}, {'/',Type::divide},
		{'^',Type::exp}	};
	const std::map<Type,char> arith_rmap = {
		{Type::plus,'+'}, {Type::minus,'-'},
		{Type::times, '*'}, {Type::divide, '/'},
		{Type::exp, '^'} };
	const std::vector<char> l_parens= {'('};
	const std::vector<char> r_parens= {')'};
};	

template <typename T>
struct NodeFactory {
	NodeUPtr<T> create(const op::Type& operation, NodeUPtr<T> l, NodeUPtr<T> r) throw(std::bad_alloc);
	NodeUPtr<T> create(const op::Type& operation, T val) throw(std::bad_alloc);
};



template <typename T>
class Node {
	public:
		virtual ~Node();
		virtual T eval() const = 0;
		virtual std::string visit() const = 0;
		const NodeUPtr<T>& get_left() const {return this->left;};
		const NodeUPtr<T>& get_right() const {return this->right;};
	protected:
		NodeUPtr<T> left;
		NodeUPtr<T> right;
		explicit Node(NodeUPtr<T> l, NodeUPtr<T> r);
};

template <typename T>
class Plus: public Node<T> {
	public:
		Plus() = delete;
		explicit Plus(NodeUPtr<T> l, NodeUPtr<T> r);
		~Plus();
		std::string visit() const;
	private:
		T eval() const;
};

template <typename T>
class Minus: public Node<T> {
	public:
		Minus() = delete;
		Minus(NodeUPtr<T> l, NodeUPtr<T> r);
		~Minus();
		T eval() const;
		std::string visit() const;
};

template <typename T>
class Times: public Node<T> {
	public:
		Times() = delete;
		~Times();
		Times(NodeUPtr<T> l, NodeUPtr<T> r);
		T eval() const;
		std::string visit() const;
};

template <typename T>
class Divide: public Node<T> {
	public:
		Divide() = delete;
		Divide(NodeUPtr<T> l, NodeUPtr<T> r);
		~Divide();
		T eval() const;
		std::string visit() const;
};

template <typename T>
class Num: public Node<T> {
	public:
		Num() = delete;
		explicit Num(T n);
		~Num();
		T eval() const;
		std::string visit() const ;
	private:
		T num; 
};

#endif
