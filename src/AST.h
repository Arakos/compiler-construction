/*
 * AST.h
 *
 *  Created on: 15.11.2017
 *      Author: Lars
 */

#ifndef AST_H_
#define AST_H_

#include <iostream>

class ExprAST {
public:
	virtual ~ExprAST() {}
};


class NumberExprAST : public ExprAST {
	double value;

public:
	NumberExprAST(double value) {
		this->value = value;
	}
};

class VariableExprAST : public ExprAST {
	std::string name;

public:
	VariableExprAST(std::string &name) {
		this->name = name;
	}
};

class BinaryExprAST : public ExprAST {
public:
	BinaryExprAST(char op, ExprAST* lhs, ExprAST* rhs) {

	}
};



#endif /* AST_H_ */
