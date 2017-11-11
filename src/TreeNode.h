/*
 * TreeNode.h
 *
 *  Created on: 08.11.2017
 *      Author: Lars
 */

#include <iostream>
#include <vector>
#include "DataStructures.h"

using namespace std;

#ifndef TREENODE_H_
#define TREENODE_H_





class ErrorNode;
class TreeNode;


class TreeNode {

protected:

	LexerResult lexerResult;
	vector<TreeNode*> children;

public:
	bool isErrorNode = false;
	string errMsg = "";


	TreeNode() {

	}

	TreeNode(LexerResult token) {
		this->lexerResult = token;
	}

	~TreeNode() {
		for(TreeNode* child : children) {
			if(child)
				delete child;
		}
	}

	void addChild(TreeNode* n) {
		children.push_back(n);
	}


	string toString() {
		return toString(0);
	}

	string toString(int depth) {
		string result;
		if(isErrorNode) {
			result = " line[" + std::to_string(lexerResult.lineNumber) + "] Error: " + errMsg;
		} else {
			result = lexerResult.identifierStr;
		}
		result += "\n";

		for(TreeNode* child : children) {
			if(child) {
				result += getIntent(depth) + "\t->" + child->toString(depth+1);
			} else
				result += getIntent(depth) + "\t->(nullchild)\n";
		}
		return result;
	}

	string getIntent(int depth) {
		string result = "";
		for(int i = 0; i < depth; i++) {
			result += "\t";
		}
		return result;
	}

};


class ErrorNode : public TreeNode {

public:

	ErrorNode(string msg, LexerResult lr) {
		this->lexerResult = lr;
		this->isErrorNode = true;
		this->errMsg = msg;
	}

	~ErrorNode() {

	}
};




#endif /* TREENODE_H_ */
