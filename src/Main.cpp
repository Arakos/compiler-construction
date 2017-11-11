//============================================================================
// Name        : test.cpp
// Author      : Lars
// Version     :
// Copyright   : Your copyright notice
// Description : Compiler Construction practical work ...
//============================================================================

#include <iostream>
#include "Lexer.h"
#include "TreeNode.h"
#include "Parser.h"
using namespace std;


int main() {

	Lexer* l = new Lexer("package main;\n\nimport \"file1\";\nimport \"file2\";\n\n\n#this is a comment ...\nfunc main() {\n\tevery possible stuff except some breakeds ... \n}\nEOF");//"package main;\nimport \"aaa\";\nimport \"bbbb;\nimport ;EOF");// import asf import \"test\" func main() { func test() {} }");

	Parser* p = new Parser( l );
	TreeNode* root = p->createAST();
	cout << root->toString() << endl;

	delete p;
	cout << "DONE" << endl;
return 0;
}
