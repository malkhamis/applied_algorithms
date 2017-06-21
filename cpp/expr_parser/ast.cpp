#include <memory>
#include <string>
#include <iostream>
#include <stdexcept>
#include <regex>
#include <algorithm>
#include "node.h"
#include "ast.h"

template <typename T>
AST<T>::AST(const std::string& ifix_exp) {
	auto stripped_exp(ifix_exp); 
	stripped_exp.erase (
	  std::remove(stripped_exp.begin(), stripped_exp.end(), ' '), stripped_exp.end()
	);
	
	const auto& ifix_tokens = AST<T>::tokenize(stripped_exp);
	std::cout << "the tokenized infix-expression you entered: ";
	for(auto& t: ifix_tokens){
		switch(t.first) {
			case op::Type::number: std::cout << t.second; break;
			case op::Type::l_paren: std::cout << '('; break;
			case op::Type::r_paren: std::cout << ')'; break;
			default: std::cout << op::arith_rmap.at(t.first);
		}
	}
	std::cout << std::endl;
	
	verify_parens(ifix_tokens);
	
	const auto& pfix_tokens = AST<T>::to_postfix(ifix_tokens);
	std::cout << "the postfix expression: ";
	for(auto rit = pfix_tokens.begin();
	  rit != pfix_tokens.end(); ++rit){
		switch((*rit).first) {
			case op::Type::number: std::cout << (*rit).second << ' '; break;
			default: std::cout << op::arith_rmap.at((*rit).first) << ' ';
		}
	}
	std::cout << std::endl;

	root = AST<T>::buildAST(pfix_tokens);	
}

template <typename T>
AST<T>::~AST() { AST<T>::root.reset(); }

template <typename T>
T AST<T>::evaluate() const {
	return root->eval();
}
template <typename T>
auto AST<T>::tokenize(const std::string& ifix_exp) const throw(boost::bad_lexical_cast, std::invalid_argument){
	
	pair_vec<T> tokens;
	std::string num = "";
	
	for(auto& c : ifix_exp){
		if(isdigit(c) or c=='.') 
			num += c;
			
		else {
			if(num != ""){
				std::regex numPattern("([[:digit:]]+)(\\.([[:digit:]]+))?");
				if(std::regex_match(num,numPattern)) {
					T casted_num;
					try{ // in case user provides floating-point number while expecting integers
						casted_num = boost::lexical_cast<T>(num);
					}
					catch(boost::bad_lexical_cast& blc){
						num = num.substr(0,num.find("."));
						casted_num = boost::lexical_cast<T>(num);
					}
					tokens.push_back(std::make_pair(op::Type::number, casted_num));
					num = "";
				} else throw std::invalid_argument( "numeric regex mismatch" );
			}
			
			auto isParen = [](char t) {
			  if(std::find(op::l_parens.begin(), op::l_parens.end(), t) != op::l_parens.end())
				  return op::Type::l_paren;
			  if(std::find(op::r_parens.begin(), op::r_parens.end(), t) != op::r_parens.end())
				  return op::Type::r_paren;
			  return op::Type::none;
			};
			auto isOp = [](char t) {
				if(op::arith_map.find(t) != op::arith_map.end())
					return op::arith_map.at(t);
				return op::Type::none;
			};
			
			auto p=isParen(c);
			if(p != op::Type::none){
				tokens.push_back(std::make_pair(p, T(0)));
				continue;
			}
			
			auto o=isOp(c);
			if(o != op::Type::none) {
				tokens.push_back(std::make_pair(o, boost::lexical_cast<T>(op::prec_map.at(o))));
				continue;
			}
			// if this point was reched, then this is invalid token
			throw std::invalid_argument( "unknown/undefined operation \'" + std::string(1,c) + "\'");
		}
	}
	if(num!="")  // last number isn't accounted for in the loop above
		tokens.push_back(std::make_pair(op::Type::number, boost::lexical_cast<T>(num)));

	return tokens;
}

template <typename T>
void AST<T>::verify_parens(const pair_vec<T>& ifix_tokens) const throw(std::runtime_error) {
	std::cout << "grammar for use of parentheses is not checked!!" << std::endl;
	int l_parens=0, r_parens=0; //check the balance only
	for(auto t = ifix_tokens.begin(); t < ifix_tokens.end(); ++t) {
		if((*t).first == op::Type::l_paren) {
			l_parens++;
			continue;
		}
		if((*t).first == op::Type::r_paren)
			if(++r_parens > l_parens)
				throw std::runtime_error("unbalanced parentheses");
	}
	if(l_parens > r_parens)
		throw std::runtime_error("unbalanced parentheses");
}

template <typename T>
auto AST<T>::to_postfix(const pair_vec<T>& ifix_tokens) const throw(std::logic_error){
	pair_vec<T> pfix_tokens;
	const auto& stack_divider = std::make_pair(op::Type::none, T(0));
	pair_vec<T> op_stack = {stack_divider};
	for(auto& token : ifix_tokens) {
		switch(token.first) {
			case op::Type::none:
				throw std::logic_error( "I a not supposed to get a 'none' token here. Debug!!" );
			case op::Type::number: 
				pfix_tokens.push_back(token); break;
			case op::Type::l_paren: 
				op_stack.push_back(stack_divider); break;
			case op::Type::r_paren:
				for(auto top=op_stack.back(); 
				  top.first != op::Type::none; // didn't hit the stack divider?
				  op_stack.pop_back(), top=op_stack.back())
					pfix_tokens.push_back(top);
				op_stack.pop_back(); //remove the last stack divider
				break;
			default: //operators
				for(auto top=op_stack.back();
				  (top.first != op::Type::none) and 
				  (op::prec_map.find(token.first)->second <= op::prec_map.find(top.first)->second);
				  op_stack.pop_back(), top=op_stack.back())
					pfix_tokens.push_back(top);
				
				op_stack.push_back(token);
		}
	}
	for(auto top=op_stack.back(); op_stack.size() > 1; op_stack.pop_back(), top=op_stack.back())
		pfix_tokens.push_back(top);
	
	if (op_stack.size() > 1)
		throw std::logic_error("something went horribly wrong. Debug!!");
	return pfix_tokens;
}

template <typename T>
NodeUPtr<T> AST<T>::buildAST(const pair_vec<T>& pfix_tokens) const throw(std::runtime_error){
	
	static typename NodeFactory<T>::NodeFactory factory;

	auto tokens(pfix_tokens);
	std::function<NodeUPtr<T> ()> recurse =
	[tokens, &recurse] () mutable {
		if(tokens.empty()) 
			throw std::runtime_error("malformed expression!!");
		NodeUPtr<T> root;
		auto token = tokens.back();
		tokens.pop_back();
		const auto& op_type = token.first;
		if(op_type == op::Type::number)
			root = factory.create(op_type, token.second);
		else
			root = factory.create(op_type, recurse(), recurse());
		return root;		
	};
	
	return recurse();
}

template <typename T>
void AST<T>::print_inorder() const {
	std::function<void(const NodeUPtr<T>&)> recurse = 
	[&recurse] (const NodeUPtr<T>& node) {
		if(auto& left = node->get_left()) recurse(left);
		std::cout << node->visit() << " ";
		if(auto& right = node->get_right()) recurse(right);
	};
	recurse(AST<T>::root);
	std::cout << std::endl;
}

template <typename T>
void AST<T>::print_postorder() const {
	std::function<void(const NodeUPtr<T>&)> recurse = 
	[&recurse] (const NodeUPtr<T>& node) {
		if(auto& left = node->get_left()) recurse(left);
		if(auto& right = node->get_right()) recurse(right);
		std::cout << node->visit() << " ";
	};
	recurse(AST<T>::root);
	std::cout << std::endl;
}


template class AST<int>;
template class AST<double>;
