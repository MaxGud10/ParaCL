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
	READ	"?"
	PRINT	"print"
	LCPAREN	"{"
	RCPAREN	"}"
	SEMIC	";"
;

%token <std::string> ID "identifier"
%token <int> NUMBER "number"

%nterm <int> Expr

%printer { yyo << $$; } <*>;

%nonassoc "print"
%left "="
%left "+" "-";
%left "*" "/";
%nonassoc UMINUS

%%

Program: /* nothing */
	   | Statements YYEOF
	   ;

Statements: Statement
		  | Statements Statement
		  ;

Statement: /* nothing */
		 | Expr ";"
		 | Scope
		 ;

Scope: StartScope Statements EndScope
     | StartScope EndScope
     ;

StartScope: "{"
{
	++drv.cur_scope_id;
	drv.var_table.push_back(Driver::Variables{});
};

EndScope: "}"
{
	--drv.cur_scope_id;
	drv.var_table.push_back(Driver::Variables{});
};


Expr: Expr "+" Expr			{ $$ = $1 + $3; }
   	| Expr "-" Expr			{ $$ = $1 - $3; }
   	| Expr "*" Expr			{ $$ = $1 * $3; }
  	| Expr "/" Expr			{ $$ = $1 / $3; }
  	| "(" Expr ")"			{ $$ = $2;      }
  	| "-" Expr %prec UMINUS { $$ = - $2;    }
  	| NUMBER				{ $$ = $1;      }
  	| 	ID
		{
			for (int scope_id = static_cast<int>(drv.cur_scope_id); scope_id >= 0; --scope_id)
			{
				auto iter = drv.var_table[scope_id].find($1);

				if (iter == drv.var_table[scope_id].end())
					if (scope_id == 0)
						throw yy::parser::syntax_error(drv.location, "Unknown identifier\n");
					else continue;
				else
				{
					$$ = iter->second;
					break;
				}

			}
		}
	| "?"					{ std::cin >> $$; }
   	| ID "=" Expr			{ drv.var_table[drv.cur_scope_id][$1] = $3; }
	| "print" Expr			{ drv.out << $2; }
   	;

%%

void yy::parser::error (const location_type& loc, const std::string& msg)
{
	std::cerr << loc << ": " << msg << '\n';
}