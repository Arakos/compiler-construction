/***********************
 * Example of C++ in Bison (yacc) 
 * Compare Bison Manual, Section 10.1.6 A Complete C++ Example
 * https://www.gnu.org/software/bison/manual/html_node/A-Complete-C_002b_002b-Example.html
 * The Makefile has been simplified radically, but otherwise
 * everything here comes from the Bison source code (see also).
 * (This comment added by Prof. R. C. Moore, fbi.h-da.de)
 *
 * This is the (f)lex file, i.e. token definitions
 * See also calc++-parser.yy for the parser (grammar)
 * (yacc/bison input).
 *
 ***********************/

%{ /* -*- C++ -*- */
# include <cerrno>
# include <climits>
# include <cstdlib>
# include <string>
# include "calc++-driver.hh"
# include "calc++-parser.hh"

// Work around an incompatibility in flex (at least versions
// 2.5.31 through 2.5.33): it generates code that does
// not conform to C89.  See Debian bug 333231
// <http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=333231>.
# undef yywrap
# define yywrap() 1

// The location of the current token.
static yy::location loc;
	
	/* definiton of constants
	PACKAGE */

%}


%option noyywrap nounput batch debug noinput
id    [a-zA-Z][a-zA-Z_0-9]*
int   [0-9]+
blank [ \t]


%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}

%%

%{
  // Code run each time yylex is called.
  loc.step ();
%}

{blank}+   loc.step ();
[\n]+      loc.lines (yyleng); loc.step ();


"package"	return yy::calcxx_parser::make_PACKAGE(loc);
"import" 	return yy::calcxx_parser::make_IMPORT(loc);
"func"		return yy::calcxx_parser::make_FUNC(loc);
"var"		return yy::calcxx_parser::make_VAR(loc);



"-"      	return yy::calcxx_parser::make_MINUS(loc);
"+"      	return yy::calcxx_parser::make_PLUS(loc);
"*"      	return yy::calcxx_parser::make_STAR(loc);
"/"      	return yy::calcxx_parser::make_SLASH(loc);
"("      	return yy::calcxx_parser::make_LPAREN(loc);
")"      	return yy::calcxx_parser::make_RPAREN(loc);
"="      	return yy::calcxx_parser::make_ASSIGN(loc);
";"			return yy::calcxx_parser::make_ENDLN(loc);
"\""		return yy::calcxx_parser::make_QUOT(loc);
"{"			return yy::calcxx_parser::make_LBREACK(loc);
"}"			return yy::calcxx_parser::make_RBREACK(loc);
","			return yy::calcxx_parser::make_COMMA(loc);
			


{int}      {
  errno = 0;
  long n = strtol (yytext, NULL, 10);
  if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE))
    driver.error (loc, "integer is out of range");
  return yy::calcxx_parser::make_NUMBER(n, loc);
}

{id}       return yy::calcxx_parser::make_IDENTIFIER(yytext, loc);
\r			;
.          driver.error (loc, "invalid character");
<<EOF>>    return yy::calcxx_parser::make_END(loc);
%%

void
calcxx_driver::scan_begin ()
{
  yy_flex_debug = trace_scanning;
  if (file.empty () || file == "-")
    yyin = stdin;
  else if (!(yyin = fopen (file.c_str (), "r")))
    {
      error ("cannot open " + file + ": " + strerror(errno));
      exit (EXIT_FAILURE);
    }
}



void
calcxx_driver::scan_end ()
{
  fclose (yyin);
}

