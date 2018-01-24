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
class NewTreeNode;
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
# include "NewTreeNode.h"
# include <vector>
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
  COMMA		","
  
  PACKAGE 	"package"
  IMPORT 	"import"
  FUNC 		"func"
  VAR 		"var"
  
  QUOT 		"\""
;

%token <std::string> IDENTIFIER "identifier"
%token <int> NUMBER "number"

%type <NewTreeNode*> s
%type <NewTreeNode*> package
%type <std::vector<NewTreeNode*>*> imports
%type <NewTreeNode*> import
%type <std::vector<NewTreeNode*>*> funcdefs
%type <NewTreeNode*> funcdef
%type <std::vector<NewTreeNode*>*> funcbody
%type <NewTreeNode*> declaration
%type <NewTreeNode*> assignment
%type <NewTreeNode*> assignable
%type <NewTreeNode*> exp
%type <std::vector<NewTreeNode*>*> funcargs
%type <NewTreeNode*> funccall


%printer { yyoutput << $$; } <*>;

%%
%start s;

s: 
	package imports funcdefs	{ 
									$$ = new ContainerNode("root");
									$$->addChild($1);
									NewTreeNode* tmp = new ContainerNode("imports");
									for(int i = $2->size()-1; i >= 0; i--) {
										tmp->addChild((*$2)[i]);
									}
									$$->addChild(tmp);
									tmp = new ContainerNode("functions");
									for(int i = $3->size()-1; i >= 0; i--) {
										tmp->addChild((*$3)[i]);
									}									
									$$->addChild(tmp);
									
									driver.tree = $$;
								}
	
package:
	PACKAGE IDENTIFIER ";"	{ 
								$$ = new ContainerNode("package"); 
								$$->addChild( new IdentifierNode($2) ); 
							};

imports: 
	import imports		{
							$$ = $2;
							$$->push_back($1);
						}
	| %empty			{ $$ = new std::vector<NewTreeNode*>; };
	
import :
	IMPORT QUOT IDENTIFIER QUOT ";" { $$ = new IdentifierNode($3); };
	
	
funcdefs:
	funcdef funcdefs	{
							$$ = $2;
							$$->push_back($1);
						}
	| %empty			{ $$ = new std::vector<NewTreeNode*>; };
	
funcdef:
	 FUNC IDENTIFIER "(" funcargs ")" "{" funcbody "}" 	{
															NewTreeNode* tmp = new ContainerNode("funcargs");
															$$ = new StatementNode(new IdentifierNode($2), tmp, "function", "func");
															for(int i = 0; i < $4->size(); i++) {
																tmp->addChild((*$4)[i]);
															}
															for(int i = $7->size()-1; i >= 0; i--) {
																$$->addChild((*$7)[i]);
															}
															driver.variables[$2] = "func";
														};
	 
	 
funcargs:
	funcargs IDENTIFIER				{
										$$ = $1;
										$$->push_back( new IdentifierNode($2) );
									}
	| IDENTIFIER	","				{
										$$ = new std::vector<NewTreeNode*>;
										$$->push_back( new IdentifierNode($1) );
									}									
	| %empty						{ $$ = new std::vector<NewTreeNode*>; };
										
	 
funcbody: 
	assignment funcbody				{
										$$ = $2;
										$$->push_back($1);
									}
	| declaration funcbody			{
										$$ = $2;
										$$->push_back($1);
									}
									
	| funccall funcbody				{
										$$ = $2;
										$$->push_back($1);
									}
	| %empty						{ $$ = new std::vector<NewTreeNode*>; };
	
	
funccall:
	IDENTIFIER "(" funcargs ")" ";" {
										$$ = new ContainerNode("call to func: " + $1);
										if(!(driver.variables[$1] == "func")) {
											std::cerr << "invalid call to func: " << $1 << ". The called function was not declared\n";
											exit(42);
										}
										for(int i = 0; i < $3->size(); i++) {
											$$->addChild((*$3)[i]);
										}
									};

	
declaration:
	VAR IDENTIFIER ";"					{
											driver.variables[$2] = "unassigned";
											$$ = new StatementNode(new IdentifierNode($2), NULL, "declaration", "");
										};
	
	
assignment:
  IDENTIFIER "=" assignable ";" 			{
												driver.variables[$1] = $3->flatToString();
												$$ = new StatementNode(new IdentifierNode($1), $3, "assigment", "=");
											}
  | VAR IDENTIFIER "=" assignable ";" 		{
												driver.variables[$2] = $4->flatToString();
												$$ = new StatementNode(new IdentifierNode($2), $4, "assigment", "=");
											};
  
assignable:
	exp								{ $$ = new ContainerNode("mathExp"); $$->addChild($1); }
	| QUOT IDENTIFIER QUOT 			{ $$ = new ValueNode("string", $2);	};
	

%left "+" "-";
%left "*" "/";
exp:
  exp "+" exp   { $$ = new ExpressionNode($1, $3, "+"); }
| exp "-" exp   { $$ = new ExpressionNode($1, $3, "-"); }
| exp "*" exp   { $$ = new ExpressionNode($1, $3, "*"); }
| exp "/" exp   { $$ = new ExpressionNode($1, $3, "/"); }
| "(" exp ")"   { $$ = $2; }
| "identifier"  { $$ = new IdentifierNode($1); }
| "number"      { $$ = new ValueNode( "number", std::to_string($1) ); };


%%
void
yy::calcxx_parser::error (const location_type& l,
                          const std::string& m)
{
  driver.error (l, m);
}