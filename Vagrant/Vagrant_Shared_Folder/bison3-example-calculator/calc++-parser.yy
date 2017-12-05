/***********************
 * Example of C++ in Bison (yacc)
 * Compare Bison Manual, Section 10.1.6 A Complete C++ Example
 * https://www.gnu.org/software/bison/manual/html_node/A-Complete-C_002b_002b-Example.html
 * The Makefile has been simplified radically, but otherwise
 * everything here comes from the Bison source code (see also).
 * (This comment added by Prof. R. C. Moore, fbi.h-da.de)
 *
 * This is the yacc (bison) file, i.e. grammar file.
 * See also calc++-scanner.ll for the lexical scanner 
 * (flex input).
 *
 ***********************/

%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0.2"
%defines
%define parser_class_name {calcxx_parser}

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires
{
# include <string>
class calcxx_driver;
}

// The parsing context.
%param { calcxx_driver& driver }

%locations
%initial-action
{
  // Initialize the initial location.
  @$.begin.filename = @$.end.filename = &driver.file;
};

%define parse.trace
%define parse.error verbose

%code
{
# include "calc++-driver.hh"
}

%define api.token.prefix {TOK_}
%token
  END  0  "end of file"
  ASSIGN  "="
  MINUS   "-"
  PLUS    "+"
  STAR    "*"
  SLASH   "/"
  LPAREN  "("
  RPAREN  ")"
  LBREACK "{"
  RBREACK "}"
  ENDLN		";"
  
  PACKAGE 	"package"
  IMPORT 	"import"
  FUNC 		"func"
  VAR 		"var"
  
  QUOT 		"\""
;

%token <std::string> IDENTIFIER "identifier"
%token <int> NUMBER "number"
%type  <int> exp
%type <std::string> assignable

%printer { yyoutput << $$; } <*>;

%%
%start s;

s: 
PACKAGE IDENTIFIER ";" imports { driver.result = 123;};

imports: 
	  funcdefs				{}
	| import imports		{};
	
import :
	IMPORT QUOT IDENTIFIER QUOT ";" {};
	
	
funcdefs:
	funcdef funcdefs	{};
	| %empty			{};
	
funcdef:
	 FUNC IDENTIFIER "(" ")" "{" funcbody "}" {};
	 
	 
funcbody: 
	assignment funcbody				{}
	| declaration funcbody			{}
	| %empty						{};

	
declaration:
	VAR IDENTIFIER ";"						{driver.variables[$2] = "unassigned";};
	
	
assignment:
  IDENTIFIER "=" assignable ";" 			{driver.variables[$1] = $3;}
  | VAR IDENTIFIER "=" assignable ";" 		{driver.variables[$2] = $4;};
  
assignable:
	exp	{$$ = std::to_string($1);}
	| QUOT IDENTIFIER QUOT {$$ = $2;};
	

%left "+" "-";
%left "*" "/";
exp:
  exp "+" exp   { $$ = $1 + $3; }
| exp "-" exp   { $$ = $1 - $3; }
| exp "*" exp   { $$ = $1 * $3; }
| exp "/" exp   { $$ = $1 / $3; }
| "(" exp ")"   { std::swap ($$, $2); }
| "identifier"  { $$ = std::stoi(driver.variables[$1]); }
| "number"      { std::swap ($$, $1); };
%%

void
yy::calcxx_parser::error (const location_type& l,
                          const std::string& m)
{
  driver.error (l, m);
}
