#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>
#include <boost/lexical_cast.hpp>
#include "node.h"

/* wrapper */
template <typename T> class AST;
namespace ast_wrapper {
	template <typename T>
	T evaluate(const std::string& exp) {
		try {
			AST<T> ast(exp);
			std::cout << "inorder traversal of the AST: ";
			ast.print_inorder();
			std::cout << "postorder traversal of the AST: ";
			ast.print_postorder();
			T res = ast.evaluate();
			std::cout << "***** result is: " << res << " *****" << std::endl;
		}
		catch(std::exception& e) {
			std::cout << e.what() << std::endl;
		}
	}
};

template <typename T>
using pair_vec = std::vector<std::pair<op::Type, T>>;

template <typename T>
class AST {
	public:
		explicit AST(const std::string& ifix_exp);
		AST() = delete;
		~AST();
		T evaluate() const;
		void print_inorder() const;
		void print_postorder() const;
	
	private:	
		NodeUPtr<T> root;
		auto tokenize(const std::string& ifix_exp) const throw(boost::bad_lexical_cast, std::invalid_argument);
		void verify_parens(const pair_vec<T>& ifix_tokens) const throw(std::runtime_error);
		auto to_postfix(const pair_vec<T>& ifix_tokens) const throw(std::logic_error);
		NodeUPtr<T> buildAST(const pair_vec<T>& pfix_tokens) const throw(std::runtime_error);
};
#endif