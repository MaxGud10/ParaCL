%skeleton "lalr1.cc" // -*- C++ -*-
%require "3.8.2"
%header

%define api.token.raw
%define api.token.constructor
%define api.value.type variant
%define parse.assert
%define parse.trace
%define parse.error detailed
%define parse.lac full

%locations

%code requires
{
	#include <string>
	class Driver;
}

%param { Driver& drv }

%code
{
	#include "driver.hpp"
}

%define api.token.prefix {TOK_}
%token
	ASSIGN	"="
	MINUS	"-"
	PLUS	"+"
	STAR	"*"
	SLASH	"/"
	LPAREN	"("
	RPAREN	")"
;

%token <std::string> IDENTIFIER "identifier"
%token <int> NUMBER "number"

%nterm <int> exp

%printer { yyo << $$; } <*>;

%left "+" "-";
%left "*" "/";

%%

unit: assignments exp	{ drv.result = $2; };

assignments:
	%empty {}
| assignments assignment {};

assignment:
	IDENTIFIER "=" exp { drv.variables[$1] = $3; };

exp:
	NUMBER
| IDENTIFIER	{ $$ = drv.variables[$1]; }
| exp "+" exp	 { $$ = $1 + $3; }
| exp "-" exp	 { $$ = $1 - $3; }
| exp "*" exp	 { $$ = $1 * $3; }
| exp "/" exp	 { $$ = $1 / $3; }
| "(" exp ")"	 { $$ = $2; }

%%

void yy::parser::error (const location_type& loc, const std::string& msg)
{
	std::cerr << loc << ": " << msg << '\n';
}