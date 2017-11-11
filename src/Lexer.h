/*
 * Lexer.h
 *
 *  Created on: 08.11.2017
 *      Author: Lars
 */

#ifndef LEXER_H_
#define LEXER_H_

#include "TreeNode.h"
#include "DataStructures.h"


class Lexer {
private:
	char* input;
	int idx;
	int currentLine;

	bool eof = false;

public :
	Lexer(char* input) {
		setInput(input);
	}

	~Lexer() {

	}

	void setInput(char* input) {
		this->input = input;
		idx = 0;
		currentLine = 1;
		eof = false;
		cout << "Lexer input:" << endl << input << endl << endl;
	}

	/// gettok - Return the next token from standard input.
	LexerResult gettok() {
		LexerResult result;

		if(eof) {
			cout << "gettok called after eof was reached!" << endl;
			result.token = tok_eof;
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
		result.lineNumber = currentLine;

		if (isalpha(lastChar)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
			while (isalnum((int)(lastChar = input[idx++])))
				result.identifierStr += lastChar;
			idx--;	// the char that broke the end of identifier might be needed again (eg. "test", the last " marked end of test id, but might be needed as next token as a " token!)
			if (result.identifierStr == "EOF")
				result.token = tok_eof;
			else if (result.identifierStr == "extern")
				result.token = tok_extern;
			else if(result.identifierStr == "package")
				result.token = tok_package;
			else if(result.identifierStr == "import")
				result.token = tok_import;
			else if(result.identifierStr == "func")
				result.token = tok_func;
			else
				result.token = tok_identifier;

		} else if (isdigit((int)lastChar) || lastChar == '.') {   // Number: [0-9.]+
			std::string NumStr;
			do {
				NumStr += lastChar;
				lastChar = input[idx++];
			} while (isdigit((int)lastChar) || lastChar == '.');
			idx--;
			result.identifierStr = NumStr;
			result.numValue = strtod(NumStr.c_str(), 0);
			result.token = tok_number;

		} else if (lastChar == '#') {
			// Comment until end of line.
			do
				lastChar = (int) input[idx++];
			while (lastChar != EOF && lastChar != '\n' && lastChar != '\r');

			if (lastChar != EOF)
				return gettok();

		} else if(lastChar == '<' /*<*/ || lastChar == '>' /*>*/) {
			result.token = tok_math_comp;
			result.identifierStr = (char) lastChar;

			lastChar = (int) input[idx++];

		} else if(lastChar == '='/*=*/) {
			result.token = tok_eqauls;
			result.identifierStr = "=";

			lastChar = (int) input[idx++];

		} else if(lastChar == '"' /* " */) {
			result.token = tok_quot_mark;
			result.identifierStr = "\"";

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
			lastChar = input[idx++];
		}

		if(result.token == tok_eof) {
			eof = true;
		}

		return result;
	}

};



#endif /* LEXER_H_ */
