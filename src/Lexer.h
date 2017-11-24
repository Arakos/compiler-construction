/*
 * Lexer.h
 *
 *  Created on: 08.11.2017
 *      Author: Lars
 */

#ifndef LEXER_H_
#define LEXER_H_

#include "TreeNode.h"
#include "Util.h"

static string BREAKER = "=================================================================================";

class Lexer {
private:

	string input;
	unsigned int idx;
	int currentLine;

	bool eof = false;

	int literalStringMode = 0;

	LexerResult handleAlphabetCharSeq() {
		LexerResult result;
		char lastChar = input[idx-1];
		result.identifierStr = lastChar;

		// identifier: [a-zA-Z][a-zA-Z0-9]*
		while (isalnum((int) ((lastChar = input[idx++])))) {
			result.identifierStr += lastChar;
		}

		// the char that broke the end of identifier might be needed again (eg. "test", the last " marked end of test id, but might be needed as next token as a " token!)
		idx--;

		if (result.identifierStr == "EOF")
			result.token = tok_eof;
		else if (result.identifierStr == "var")
			result.token = tok_var;
		else if (result.identifierStr == "package")
			result.token = tok_package;
		else if (result.identifierStr == "import")
			result.token = tok_import;
		else if (result.identifierStr == "func")
			result.token = tok_func;
		else
			result.token = tok_identifier;
		return result;
	}

	LexerResult handleNumberSeq() {
		LexerResult result;
		char lastChar = input[idx-1];

		// Number: [0-9.]+
		std::string NumStr;
		do {
			NumStr += lastChar;
			lastChar = input[idx++];
		} while (isdigit((int) (lastChar)) || lastChar == '.');
		idx--;
		result.identifierStr = NumStr;
		result.numValue = strtod(NumStr.c_str(), 0);
		result.token = tok_number;
		return result;
	}

	LexerResult handleComment() {
		char lastChar = '#';
		// Comment until end of line.
		do {
			lastChar = (int) input[idx++];
		} while (lastChar != EOF && lastChar != '\n' && lastChar != '\r');
		currentLine++;

		return gettok();
	}


	LexerResult handleLiteral() {
		string tmp = "";
		char lastChar = 'c';
		while(lastChar != EOF) {
			lastChar = input[idx++];
			if(lastChar == '\n') {
				currentLine++;
			}
			if(lastChar == '"') {
				break;
			}
			tmp += lastChar;
		}

		LexerResult lr;
		lr.token = tok_literal;
		lr.identifierStr = tmp;
		return lr;
	}

public :
	Lexer(string input) {
		setInput(input);
	}

	~Lexer() {

	}

	void setInput(string input) {
		this->input = input;
		idx = 0;
		currentLine = 1;
		eof = false;
		cout << "Lexer input file dump:" << endl << endl << input << endl << endl;
	}

	/// gettok - Return the next token from standard input.
	LexerResult gettok() {
		LexerResult result;

		if(eof) {
//			cout << "gettok called after eof was reached!" << endl;
			result.token = tok_eof;
			result.identifierStr = "EOF";
			result.lineNumber = currentLine;
			return result;
		}

		char lastChar = ' ';

		// Skip any whitespace.
		while (isspace(lastChar)) {
			lastChar = input[idx++];
			if(lastChar == '\n') {
				currentLine++;
			}
		}

		result.identifierStr = lastChar;

		if (isalpha(lastChar)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
			result = handleAlphabetCharSeq();

		} else if (isdigit((int)lastChar) || lastChar == '.') {   // Number: [0-9.]+
			result = handleNumberSeq();

		} else if (lastChar == '/' && input[idx] == '/') {
			result = handleComment();

		} else if(lastChar == '<'
				|| lastChar == '>'
				|| lastChar == '+'
				|| lastChar == '*'
				|| lastChar == '-'
				|| lastChar == '/') {
			result.token = tok_operator;
			result.op = (char) lastChar;

		} else if(lastChar == '='/*=*/) {
			result.token = tok_eqauls;
			result.op = '=';

		} else if(lastChar == '"' /* " */) {
			result = handleLiteral();

		} else if(lastChar == '{'|| lastChar == '}' || lastChar == '(' || lastChar == ')') {
			result.token = tok_breacked;
			result.identifierStr = lastChar;

		} else if(lastChar == ';') {
			result.token = tok_endl;

			// Check for end of file.  Don't eat the EOF.
		} else if (lastChar == EOF) {
			result.token = tok_eof;

			// Otherwise, just return the character as its ascii value.
		} else {
			result.token = (int) lastChar;
		}

		if(result.token == tok_eof || idx == input.length()) {
			eof = true;	// last token is carried out with this return value;
		}
		result.lineNumber = currentLine;

		return result;
	}

};



#endif /* LEXER_H_ */
