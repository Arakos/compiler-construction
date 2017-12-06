/*
 * NewTreeNode.h
 *
 *  Created on: 08.11.2017
 *      Author: Lars
 */

#include <iostream>
#include <vector>


using namespace std;

#ifndef NewTreeNode_H_
#define NewTreeNode_H_





class NewTreeNode {

public:
	vector<NewTreeNode*> children;

	string type;
	
	string value;

	NewTreeNode() {

	}

	virtual ~NewTreeNode() {
		NewTreeNode* tmp = NULL;
//		cout << "delete: " << flatToString() << " - children: " << to_string(children.size()) << endl;
		while(children.size() > 0) {
			tmp = children[0];
			if(tmp) {
				children.erase(children.begin());
				delete tmp;
			}
		}
	}

	void addChild(NewTreeNode* n) {
		if(n) {
			children.push_back(n);
		}
		//else {
//			cout << "INFO: AddChild method tried to add null as child..." << endl;
//		}
	}

	string virtual toString() {
		return toString(0);
	}

	string flatToString() {
		return type + ": " + value;
	}

	string virtual toString(int depth) {
		string result = flatToString() + "\n";

		for(NewTreeNode* child : children) {
			if(child) {
				result += getIntent(depth) + "\t-> " + child->toString(depth+1);
			}
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


class IdentifierNode : public NewTreeNode {

public:

	IdentifierNode(string name) {
		type = "Id";
		value = name;
	}
	
	virtual ~IdentifierNode() {
	}

};

class ValueNode : public NewTreeNode {

public:
	
	ValueNode(string type, string value) {
		this->type = type;
		this->value = value;
	}
	
	virtual ~ValueNode() {
	}
};

class ContainerNode : public NewTreeNode {

public:

	ContainerNode(string name) {
		type = "Container";
		value = name;
	}
	
	virtual ~ContainerNode() {
	}
};

class ExpressionNode : public NewTreeNode {

public:

	ExpressionNode(NewTreeNode* left, NewTreeNode* right, string value) {
		type = "Expression";
		this->value = value;
		addChild(left);
		addChild(right);
	}
	
	virtual ~ExpressionNode() {
	}

};

class StatementNode : public NewTreeNode {
	
public:

	StatementNode(NewTreeNode* left, NewTreeNode* right, string type, string value) {
		this->type = type;
		this->value = value;
		addChild(left);
		addChild(right);
	}
	
	virtual ~StatementNode() {
	}


};




#endif /* NewTreeNode_H_ */
