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
#include <map>
#include "Util.h"

class Parser {

private:
	Lexer* lexer;

	LexerResult current;

	ErrorNode* unknowSymbolError() {
		return customError("unknown symbol '" + current.identifierStr + "'");
	}

	ErrorNode* unexpectedSymbolError(string expectedSymbol) {
		return customError("unexpected symbol '" + current.identifierStr + "' - expected: " + expectedSymbol);
	}

	ErrorNode* customError(string msg) {
//		cout << "Error:\n" << printLexerResult(current) << endl;
		return new ErrorNode(msg, current);
	}

	string printLexerResult(LexerResult lr) {
		string res = "Identifier: " + lr.identifierStr + "\nLine: " + to_string(lr.lineNumber) + "\nOperator: " + lr.op + "\nValue: " + to_string(lr.numValue) + "\nToken: " + to_string(lr.token);
		return res;
	}

public:
	Parser(Lexer* lexer) {
		this->lexer = lexer;
		this->current.token = 0;
	}

	~Parser() {
		delete lexer;
	}

	TreeNode* createAST() {
		LexerResult rootLexRes;
		rootLexRes.identifierStr = "root";
		TreeNode* root = new TreeNode(rootLexRes);

		TreeNode* result = topLevelSwitcher(root);
		if(result) {
			return result;
		}
		delete result;
		rootLexRes.identifierStr = "Internal Error. No parser result available";
		return customError("Internal Error. No parser result available");
	}

private:

	LexerResult step() {
//		cout << endl << "old:" << endl << printLexerResult(current) << endl;
		current = lexer->gettok();
//		cout << "new:" << endl << printLexerResult(current) << endl << endl;
		return current;
	}



	TreeNode* topLevelSwitcher(TreeNode* root) {
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
					if(result && !result->isErrorNode && current.identifierStr == "}") {
						current.token = tok_endl;
					}
					break;

				case tok_var:
					result = varDecl();
					break;

				case tok_eof:
					return root;

				default:
//					cout << "unknown symbol: '" + current.identifierStr + "' at line: " + std::to_string(current.lineNumber) << endl;
					result = unknowSymbolError();
			}

			if(result) {
				if(!result->isErrorNode) {
					root->addChild(result);
				} else {
					return result;
				}
			} else {
				cout << "a method returned null. this is an internal error :/" << endl;
				return NULL;
			}

			if(current.token != tok_endl) {
//				cout << "unknown symbol: '" + current.identifierStr + "' at line: " + std::to_string(current.lineNumber)  << " - expected >;<"<< endl;
				return unexpectedSymbolError(";");
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
		if(current.token != tok_identifier) {
			delete result;
			return unexpectedSymbolError("package identifier");
		}
		packageName = new TreeNode(current);
		result->addChild(packageName);
		step();	// step to ;
		return result;
	}

	TreeNode* import() {
		string msg = "";
		TreeNode* result = NULL ;
		TreeNode* name = NULL;

		if(current.token != tok_import) {
			cout << "Invalid call to importName function! Last token must be the import keyword!" << endl;
			return NULL;
		}

		result = new TreeNode(current);
		step();
		if(current.token == tok_quot_mark) {
			step();
			if(current.token == tok_identifier) {
				name = new TreeNode(current);	// identifier name is safed in last
				step();
				if(current.token == tok_quot_mark) {
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

		if(msg.length() > 0) {
			delete result;
			delete name;
			result = customError(msg);
		} else {
			result->addChild(name);
		}
		return result;
	}

	// func name ( ) { funcBody }, else returns error or null
	TreeNode* funcDefinition() {
		TreeNode* result = NULL;
		// previous call to gettok in doimport is still stored in lastResult. so its the next to care about...
		if(current.token != tok_func) {
			cout << "Invalid call of funcDefinition()! current token must be 'func' token!" << endl;
			return NULL;
		}

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
						TreeNode* functionBody = funcBodySwitcher();	// returns tmp node
						if(!functionBody) {
							return NULL;
						}
						if(functionBody->isErrorNode) {
							return functionBody;
						}
						if(current.identifierStr == "}") {
							for(TreeNode* node : functionBody->children) {
								funcName->addChild(node);
							}

							result->addChild(funcName);

						} else {
							result = unexpectedSymbolError("}");
						}

					} else {
						result = unexpectedSymbolError("{");
					}
				} else {
					result = unexpectedSymbolError(")");
				}
			} else {
				result = unexpectedSymbolError("(");
			}

		} else {
			result = customError("Invalid functionname: " + current.identifierStr);
		}
		return result;
	}

	TreeNode* funcBodySwitcher() {
		TreeNode* root = new TreeNode();
		TreeNode* errNode = NULL;
		int breakedCounter = 1;
		while((current.identifierStr != "}" || breakedCounter > 0) && !errNode && current.token != tok_eof) {
			step();
			TreeNode* result = NULL;
			switch(current.token) {
				case tok_var:
					result = varDecl();
					break;

				case tok_eof:
					errNode = customError("Unexpected end of file in the middle of function!");
					break;

				case tok_breacked:
					if(current.identifierStr == "{") {
						breakedCounter++;
					} else if(current.identifierStr == "}") {
						breakedCounter--;
						if(breakedCounter < 0) {
							errNode = unexpectedSymbolError("}");
						}
					} else {
						errNode = unexpectedSymbolError("{ or }");
						break;
					}
					continue;

				default:
					errNode = unknowSymbolError();
			}

			if(result) {
				if(!result->isErrorNode) {
					root->addChild(result);
				} else {
					errNode = result;
				}
			} else if(errNode) {
				break;

			} else {
				return NULL;// sth went terribly wrong...
			}

			if(breakedCounter == 0 && current.identifierStr == "}") {
				break;
			}

			if(current.token != tok_endl) {
				errNode = unexpectedSymbolError(";");
			}
		}
		if(errNode) {
			delete root;
			return errNode;
		}
		return root;
	}

	TreeNode* varDecl() {
		if(current.identifierStr != "var") {
			cout << "Invalid call to function varDecl! current token is not 'var'!" << endl;
			return NULL;
		}
		TreeNode* varNode = new TreeNode(current);

		step(); // consume 'var'
		if(current.token != tok_identifier) {
			delete varNode;
			return customError("invalid identifier for variable!");
		}
		TreeNode* varName = new TreeNode(current);

		step(); // consume var name
		if(current.op != '=') {
			delete varNode;
			delete varName;
			return unexpectedSymbolError("=");
		}
		step(); // consume =

		TreeNode* expr = expression();
		if(!expr || expr->isErrorNode) {
			return expr;
		}
		varName->addChild(expr);
		varNode->addChild(varName);
		return varNode;
	}

	//var (x = ((a) + (b)))    
	TreeNode* expression() {
		TreeNode* result = NULL;
		TreeNode* lhs = term();
		if(!lhs || lhs->isErrorNode) {
			return lhs;
		}

		TreeNode* rhs = expr2Half();
		if(rhs) {
			if(rhs->isErrorNode) {
				delete lhs;
				return rhs;
			}
		}
		LexerResult r;
		r.identifierStr = "Expression";
		result = new TreeNode(r);
		result->addChild(lhs);
		if(rhs) {
			result->addChild(rhs);
		}
		return result;
	}

	// may return null (epsylon rule)
	TreeNode* expr2Half() {
		TreeNode* result = NULL;
		if(current.op == '+' || current.op == '-') {
			result = new TreeNode(current);
			step(); // consume *
			TreeNode* lhs = term();
			if(!lhs || lhs->isErrorNode) {
				return lhs;
			}
			result->addChild(lhs);
			TreeNode* rhs = expr2Half();
			if(rhs) {
				if(rhs->isErrorNode) {
					delete result;
					return rhs;
				}
				result->addChild(rhs);
			}
		}
		return result;
	}

	TreeNode* term() {
		TreeNode* result = NULL;
		TreeNode* lhs = factor();
		if(!lhs || lhs->isErrorNode) {
			return lhs;
		}
		TreeNode* rhs = term2Half();
		if(rhs) {
			if(rhs->isErrorNode) {
				delete lhs;
				return rhs;
			}
		}
		LexerResult r;
		r.identifierStr = "Term";
		result = new TreeNode(r);
		result->addChild(lhs);
		if(rhs) {
			result->addChild(rhs);
		}
		return result;
	}

	// may return null (epsylon rule)
	TreeNode* term2Half() {
		TreeNode* result = NULL;
		if(current.op == '*' || current.op == '/') {
			result = new TreeNode(current);
			step(); // consume *
			TreeNode* lhs = factor();
			if(!lhs || lhs->isErrorNode) {
				return lhs;
			}
			result->addChild(lhs);
			TreeNode* rhs = term2Half();
			if(rhs) {
				if(rhs->isErrorNode) {
					delete result;
					return rhs;
				}
				result->addChild(rhs);
			}
		}
		return result;
	}

	TreeNode* factor() {
		TreeNode* result = NULL;
		LexerResult r;
		r.identifierStr = "Factor";
		result = new TreeNode(r);
		if(current.identifierStr == "(") {
			step(); // consume (
			TreeNode* expr = expression();
			if(!expr || expr->isErrorNode) {
				return expr;
			}
			result->addChild( expr );
			if(current.identifierStr == ")") {
				step(); // consume )
			} else {
				delete result;
				result = unexpectedSymbolError(")");
			}

		} else if(current.token == tok_number) {
			result->addChild( new TreeNode(current) );
			step(); // eat number
		} else {
			result = unexpectedSymbolError("( or Number");
		}
		return result;
	}

};



#endif /* PARSER_H_ */
