/*
 * DataStructures.h
 *
 *  Created on: 10.11.2017
 *      Author: Lars
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <iostream>

// The lexer returns tokens [0-255] if it is an unknown character, otherwise one
// of these for known things.
enum Token {
  tok_eof = -1,

  // commands
  tok_def = -2,
  tok_extern = -3,

  // primary
  tok_identifier = -4,
  tok_number = -5,
  tok_literal = -21,

  tok_operator = -6,

  tok_eqauls = -7,

  tok_quot_mark = -10,
  tok_l_curved_paren = -13,
  tok_r_curved_paren = -14,
  tok_lparen = -18,
  tok_rparen = -19,

  tok_breacked = -15,

  tok_package = -8,
  tok_import = -9,

  tok_func = -11,
  tok_func_body = -20,
  tok_main = -12,

  tok_var = -17,

  tok_endl = -16
};


struct LexerResult {
	int token;
	int lineNumber = -1;
	std::string identifierStr = "nix"; // Filled in if tok_identifier
	double numValue = -1; // Filled in if tok_number
	char op = '#';
	int id;
};



#endif /* UTIL_H_ */
