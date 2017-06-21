#include <string>
#include <vector>
#include <iostream>
#include "ast.h"

int main(int argc, char **argv) {
	std::string exp;
	while(1){
		std::cout << "enter an arithmetic expression { numbers, '+', '-', '*', '/', '(', ')' } " 
		  << std::endl << "or type 'q' to exit" << std::endl;
		std::getline(std::cin,exp);
		if(exp == "q") {
			std::cout << "Quitting program" << std::endl;
			break;
		}
		ast_wrapper::evaluate<double>(exp);
	}

	return 0;
}
