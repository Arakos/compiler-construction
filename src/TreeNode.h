/*
 * TreeNode.h
 *
 *  Created on: 08.11.2017
 *      Author: Lars
 */

#include <iostream>
#include <vector>

#include "Util.h"

using namespace std;

#ifndef TREENODE_H_
#define TREENODE_H_





class ErrorNode;
class TreeNode;


class TreeNode {

public:
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

	virtual ~TreeNode() {
		for(TreeNode* child : children) {
			if(child)
				delete child;
		}
	}

	void addChild(TreeNode* n) {
		if(n) {
			children.push_back(n);
		} else {
			cout << "AddChild method tried to add null as child..." << endl;
		}
	}


	string virtual toString() {
		return toString(0);
	}

	string virtual toString(int depth) {
		string result = lexerResult.identifierStr + "\n";

		for(TreeNode* child : children) {
			if(child) {
				result += getIntent(depth) + "\t->" + child->toString(depth+1);
			} else
				result += getIntent(depth) + "\t->(nullchild!)\n";
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

	string toString() {
		return toString(0);
	}

	string toString(int depth) {
		return "line[" + std::to_string(lexerResult.lineNumber) + "]: " + errMsg;
	}
};




#endif /* TREENODE_H_ */
