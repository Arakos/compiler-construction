/*
 * Parser.h
 *
 *  Created on: 10.11.2017
 *      Author: Lars
 */

#ifndef PARSER_H_
#define PARSER_H_

#include "TreeNode.h"
#include "Lexer.h"
#include "DataStructures.h"

class Parser {

private:
	Lexer* lexer;

	LexerResult current;
	LexerResult last;

public:
	Parser(Lexer* lexer) {
		this->lexer = lexer;
	}

	~Parser() {
		delete lexer;
	}

	void step() {
		last = current;
		current = lexer->gettok();
//		cout << "stepTo: " + current.identifierStr << " id: " << std::to_string(current.token) << endl;
	}

	TreeNode* createAST() {
		LexerResult rootLexRes;
		rootLexRes.identifierStr = "root";
		TreeNode* root = new TreeNode(rootLexRes);

		TreeNode* result = chooseMethod(root);
		if(result) {
			return result;
		}
		delete result;
		rootLexRes.identifierStr = "No result available";
		return new TreeNode(rootLexRes);
	}

	TreeNode* chooseMethod(TreeNode* root) {
		bool error = false;
		while(!error && current.token != tok_eof) {
			step();
			TreeNode* result;
			switch(current.token) {
				case tok_package:
					result = package();
					break;

				case tok_import:
					result = import();
					break;

				case tok_func:
					result = funcDefinition();
					if(!result->isErrorNode && current.identifierStr == "}") {
						current.token = tok_endl;
					}
					break;

				case tok_eof:
					return root;

				default:
					cout << "unknown symbol: '" + current.identifierStr + "' at line: " + std::to_string(current.lineNumber) << endl;
					return NULL;
			}
			if(result) {
				root->addChild(result);
			} else {
				cout << "a method returned null. this is an internal error :/" << endl;
				return NULL;
			}

			if(result->isErrorNode) {
				cout << "Error occured: " + result->errMsg << endl;
				return NULL;
			}

			if(current.token != tok_endl) {
				cout << "unknown symbol: '" + current.identifierStr + "' at line: " + std::to_string(current.lineNumber)  << " - expected >;<"<< endl;
				return NULL;
			}
		}
		return root;
	}


	TreeNode* package() {

		if(current.token != tok_package) {
			cout << "Invalid call to function package! current token is not package!" << endl;
			return NULL;
		}

		TreeNode* result = new TreeNode(current);

		step();

		TreeNode* packageName;
		if(current.token == tok_identifier) {
			packageName = new TreeNode(current);

		} else {
			packageName = new ErrorNode("Error! Illegal package name: " + current.identifierStr, current);
		}
		result->addChild(packageName);
		step();	// step to ;

		return result;
	}

	TreeNode* import() {
		string msg = " ";
		TreeNode* result = NULL ;
		TreeNode* name = NULL;

		if(current.token != tok_import) {
			cout << "Invalid call to importName function! Last token must be the import keyword!" << endl;
			return NULL;

		} else {
			result = new TreeNode(current);
			step();
			if(current.token == tok_quot_mark) {
				step();
				if(current.token == tok_identifier) {
					step();
					if(current.token == tok_quot_mark) {
						name = new TreeNode(last);	// identifier name is safed in last
						step(); // consume second >"<
					} else {
						msg += "missing >\"< in import statement!";
					}
				} else {
					msg += "invalid import identifier!";
				}
			} else {
				msg += "missing >\"< in import statement!";
			}
		}

		if(result == NULL || name == NULL) {
			delete result;
			delete name;
			result = new ErrorNode(msg, current);
		} else {
			result->addChild(name);
		}
		return result;
	}

	// func name ( ) { funcBody }, else returns error or null
	TreeNode* funcDefinition() {
		TreeNode* result = NULL;
		// previous call to gettok in doimport is still stored in lastResult. so its the next to care about...
		if(current.token == tok_func) {
			result = new TreeNode(current);
			step();
			if(current.token == tok_identifier) {
				TreeNode* funcName = new TreeNode(current);
				step();
				if(current.identifierStr == "(") {
					// add function arguments as child of funcName here
					step();
					if(current.identifierStr == ")"){
						step();
						if(current.identifierStr == "{") {
							TreeNode* functionBody = funcBody();

							if(current.identifierStr == "}") {

								funcName->addChild(functionBody);

								result->addChild(funcName);

							} else {
								result = new ErrorNode("Expected >}< at the end of func definition: " + current.identifierStr, current);
							}

						} else {
							result = new ErrorNode("Expected >{< after function name definition: " + current.identifierStr + "(...)", current);
						}
					} else {
						result = new ErrorNode("Expected >)< after function name definition: " + current.identifierStr + "(", current);
					}
				} else {
					result = new ErrorNode("Expected >(< after function name definition: " + current.identifierStr, current);
				}

			} else {
				result = new ErrorNode("Invalid functionname: " + current.identifierStr, current);
			}


		} else {
			result = new ErrorNode("Expected keyword 'func', got: " + current.identifierStr, current);
		}

		return result;
	}

	TreeNode* funcBody() {
		while(current.identifierStr != "}" && current.token != tok_eof) {
			step();
		}
		return NULL;
	}

};



#endif /* PARSER_H_ */
