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
	class Driver;

	#include <string>
	#include "node.hpp"
	#include "ast.hpp"
}

%param { Driver& drv }

%code
{
	#include <memory>

	#include "log.h"
	#include "driver.hpp"
}

%define api.token.prefix {TOK_}
%token
	ASSIGN		"="
    PLUS_ASSIGN "+="
    MINUS_ASSIGN "-="
    STAR_ASSIGN "*="
    SLASH_ASSIGN "/="
	MINUS		"-"
	PLUS		"+"
	STAR		"*"
	SLASH		"/"
	LPAREN		"("
	RPAREN		")"
	READ		"?"
	PRINT		"print"
	LCPAREN		"{"
	RCPAREN		"}"
	SEMIC		";"
	IF			"if"
	ELSE        "else"
	WHILE		"while"
	FOR         "for"
	GREATER		">"
	LESS		"<"
	GREATER_E	">="
	LESS_E		"<="
	EQUAL		"=="
	NOT_EQUAL	"!="
	NOT			"!"
	MOD			"%"
	AND_OP      "&&"
    OR_OP       "||"
	BIT_AND     "&"
    BIT_OR      "|"
;

%token <std::string>	ID		"identifier"
%token <int> 			NUMBER 	"number"

// ----- Statement derived -----
%nterm <AST::ExpressionNode*> 	Expr
%nterm <AST::UnaryOpNode*> 		UnaryOp
%nterm <AST::BinaryOpNode*> 	BinaryOp
%nterm <AST::AssignNode*> 		Assign
%nterm <AST::ScopeNode*> 		Scope
%nterm <AST::PrintNode*> 		Print
%nterm <AST::IfNode*> 			If_Stm
%nterm <AST::ForNode*>          For_Stm
%nterm <AST::WhileNode*> 		While_Stm
%nterm <AST::VariableNode*> 	Variable

%nterm <AST::StatementNode*>	Statement

%printer { yyo << $$; } <*>;

%precedence IFX
%right ELSE

%right "if"
%right "print"
%right "while"
%right "for"

%left "="

%left "||"
%left "&&"
%left "|"
%left "&"

%left "==" "!="
%left "<" "<=" ">" ">="

%left "+" "-"
%left "*" "/"

%right UMINUS NOT

%%

Program: /* nothing */
			| Statements YYEOF
	   		{
				MSG("Initialising global scope with vector of statements:\n");
				for ([[maybe_unused]] const auto& stm : drv.stm_table[drv.cur_scope_id])
				{
					LOG("{}\n", static_cast<const void*>(stm));
				}

				drv.ast.globalScope =
                    drv.bld.create<AST::ScopeNode>(std::move(drv.stm_table[drv.cur_scope_id]));
			};

Statements: Statement
			{
				LOG("Pushing statement : {}\n",
					static_cast<const void*>($1));

				drv.stm_table[drv.cur_scope_id].push_back($1);
			}
		|	Statements Statement
		  	{
				LOG("Pushing statement : {}\n",
					static_cast<const void*>($2));

				drv.stm_table[drv.cur_scope_id].push_back($2);
			};

Statement:
			";"
			{
				MSG("Void statement\n");
				$$ = drv.bld.create<AST::VoidNode>();
			}

		|	Expr ";"
			{
				LOG("It's Expr. Moving from concrete rule: {}\n",
					static_cast<const void*>($$));

				$$ = $1;
			}
		| 	Scope
		 	{
				LOG("It's Scope. Moving from concrete rule: {}\n",
					static_cast<const void*>($$));

				$$ = $1;
			}
		| 	Assign ";"
		 	{
				LOG("It's Assign. Moving from concrete rule: {}\n",
					static_cast<const void*>($$));

				$$ = $1;
			}
		| 	If_Stm
		 	{
				LOG("It's If_Stm. Moving from concrete rule: {}\n",
					static_cast<const void*>($$));

				$$ = $1;
			}
		|   While_Stm
			{
				LOG("It's While_Stm. Moving from concrete rule: {}\n",
					static_cast<const void*>($$));

				$$ = $1;
			}
		|   For_Stm
            {
                LOG("It's For_Stm. Moving from concrete rule: {}\n",
                    static_cast<const void*>($$));

                $$ = $1;
            }
		| 	Print ";"
		 	{
				LOG("It's Print. Moving from concrete rule: {}\n",
					static_cast<const void*>($$));

				$$ = $1;
			};

Scope: 	StartScope Statements EndScope
		{
			MSG("Initialising scope with vector of statements:\n");
			for ([[maybe_unused]] const auto& stm : drv.stm_table[drv.cur_scope_id])
			{
				LOG("{}\n", static_cast<const void*>(stm));
			}

			$$ = drv.bld.create<AST::ScopeNode>(std::move(drv.stm_table[drv.cur_scope_id]));

			MSG("Scope end.\n");

			--drv.cur_scope_id;

			LOG("drv.cur_scope_id is now {}\n", drv.cur_scope_id);

			drv.stm_table.pop_back();
		}
		| StartScope EndScope
		{
              MSG("Initialising empty scope\n");
              $$ = drv.bld.create<AST::ScopeNode>(std::vector<AST::StatementNode*>{});
              --drv.cur_scope_id;
              LOG("drv.cur_scope_id is now {}\n", drv.cur_scope_id);
              drv.stm_table.pop_back();
		};

StartScope: "{"
			{
				MSG("Scope start.\n");

				++drv.cur_scope_id;

				LOG("drv.cur_scope_id is now {}\n", drv.cur_scope_id);

				drv.stm_table.resize(drv.stm_table.size() + 1);
			};

EndScope: 	"}"
			{

			};

If_Stm: 	IF "(" Expr ")" Statement %prec IFX
			{
				MSG("Initialising if statement\n");
				$$ = drv.bld.create<AST::IfNode>($3, $5);
			}
		|   IF "(" Expr ")" Statement ELSE Statement
			{
				$$ = drv.bld.create<AST::IfNode>(
					$3,
					$5,
					$7
				);
			};

For_Stm:    FOR "(" Assign ";" Expr ";" Assign ")" Statement
            {
                MSG("Initialising for statement\n");
                $$ = drv.bld.create<AST::ForNode>(
                        $3,  // init  (AssignNode)
                        $5,  // cond  (Expr)
                        $7,  // iter  (AssignNode)
                        $9   // body  (Statement)
                );
            };

While_Stm:	WHILE "(" Expr ")" Statement
			{
				MSG("Initialising while statement\n");
				$$ = drv.bld.create<AST::WhileNode>($3, $5);
			};

Assign: Variable "=" Expr
		{
			$$ = drv.bld.create<AST::AssignNode>($1, $3);
			LOG("Initialising assignment: {}\n", static_cast<const void*>($$));
		};
    |   Variable "+=" Expr
        {
            auto oldX = drv.bld.create<AST::VariableNode>(drv.bld.intern($1->get_name()));
            auto val  = drv.bld.create<AST::BinaryOpNode>(oldX, AST::BinaryOp::ADD, $3);
            $$ = drv.bld.create<AST::AssignNode>($1, val);
        }
    |   Variable "-=" Expr
        {
            auto oldX = drv.bld.create<AST::VariableNode>(drv.bld.intern($1->get_name()));
            auto val  = drv.bld.create<AST::BinaryOpNode>(oldX, AST::BinaryOp::SUB, $3);
            $$ = drv.bld.create<AST::AssignNode>($1, val);
        }
    |   Variable "*=" Expr
        {
            auto oldX = drv.bld.create<AST::VariableNode>(drv.bld.intern($1->get_name()));
            auto val  = drv.bld.create<AST::BinaryOpNode>(oldX, AST::BinaryOp::MUL, $3);
            $$ = drv.bld.create<AST::AssignNode>($1, val);
        }
    |   Variable "/=" Expr
        {
            auto oldX = drv.bld.create<AST::VariableNode>(drv.bld.intern($1->get_name()));
            auto val  = drv.bld.create<AST::BinaryOpNode>(oldX, AST::BinaryOp::DIV, $3);
            $$ = drv.bld.create<AST::AssignNode>($1, val);
        };

Print: 	"print" Expr
		{
			MSG("Initialising print\n");
			$$ = drv.bld.create<AST::PrintNode>($2);
		}


Expr:	BinaryOp
		{
			MSG("Moving BinaryOp\n");
			$$ = $1;
		}
	|	UnaryOp
		{
			MSG("Moving UnaryOp\n");
			$$ = $1;
		}
  	| 	"(" Expr ")"
		{
			MSG("Moving Expression in parenthesis\n");
			$$ = $2;
		}
  	| 	NUMBER
		{
			MSG("Initialising ConstantNode\n");
			$$ = drv.bld.create<AST::ConstantNode>($1);
		}
	| 	"?"
		{
			MSG("Initialising InNode\n");
			$$ = drv.bld.create<AST::InNode>();
		}
  	| 	Variable
		{
			MSG("Moving VarialeNode\n");
			$$ = $1;
		};

BinaryOp: 	Expr "+" Expr
			{
				MSG("Initialising ADD operation\n");
				$$ = drv.bld.create<AST::BinaryOpNode>(	$1,AST::BinaryOp::ADD, $3);
			}
		| 	Expr "-" Expr
			{
				MSG("Initialising SUB operation\n");
				$$ = drv.bld.create<AST::BinaryOpNode>(	$1,
														AST::BinaryOp::SUB,
														$3);
			}
		| 	Expr "*" Expr
			{
				MSG("Initialising MUL operation\n");
				$$ = drv.bld.create<AST::BinaryOpNode>(	$1,
														AST::BinaryOp::MUL,
														$3);
			}
		| 	Expr "/" Expr
			{
				MSG("Initialising DIV operation\n");
				$$ = drv.bld.create<AST::BinaryOpNode>(	$1,
														AST::BinaryOp::DIV,
													    $3);
			}
		|	Expr ">" Expr
			{
				MSG("Initialising GR operation\n");
				$$ = drv.bld.create<AST::BinaryOpNode>( $1,
														AST::BinaryOp::GR,
														$3);
			}
		|	Expr "<" Expr
			{
				MSG("Initialising LS operation\n");
				$$ = drv.bld.create<AST::BinaryOpNode>(	$1,
														AST::BinaryOp::LS,
														$3);
			}
		|	Expr ">=" Expr
			{
				MSG("Initialising RG_EQ operation\n");
				$$ = drv.bld.create<AST::BinaryOpNode>(	$1,
														AST::BinaryOp::GR_EQ,
														$3);
			}
		|	Expr "<=" Expr
			{
				MSG("Initialising LS_EQ operation\n");
				$$ = drv.bld.create<AST::BinaryOpNode>(	$1,
														AST::BinaryOp::LS_EQ,
													    $3);
			}
		|	Expr "==" Expr
			{
				MSG("Initialising EQ operation\n");
				$$ = drv.bld.create<AST::BinaryOpNode>(	$1,
														AST::BinaryOp::EQ,
														$3);
			}
		|	Expr "!=" Expr
			{
				MSG("Initialising NOT_EQ operation\n");
				$$ = drv.bld.create<AST::BinaryOpNode>(	$1,
														AST::BinaryOp::NOT_EQ,
														$3);
			}
		|	Expr "&&" Expr
			{
				MSG("Initialising AND operation\n");
				$$ = drv.bld.create<AST::BinaryOpNode>(	$1,
														AST::BinaryOp::AND,
													    $3);
			}
		|	Expr "||" Expr
			{
				MSG("Initialising OR operation\n");
				$$ = drv.bld.create<AST::BinaryOpNode>(	$1,
														AST::BinaryOp::OR,
														$3);
			}
        |   Expr "&" Expr
            {
                MSG("Initialising BIT_AND operation\n");
                $$ = drv.bld.create<AST::BinaryOpNode>( $1,
                                                        AST::BinaryOp::BIT_AND,
                                                        $3);
            }
        |   Expr "|" Expr
            {
                MSG("Initialising BIT_OR operation\n");
                $$ = drv.bld.create<AST::BinaryOpNode>( $1,
                                                        AST::BinaryOp::BIT_OR,
                                                        $3);
            }
		|	Expr "%" Expr
			{
				MSG("Initialising MOD operation\n");
				$$ = drv.bld.create<AST::BinaryOpNode>( $1,
														AST::BinaryOp::MOD,
														$3);
			};


UnaryOp	: 	"-" Expr %prec UMINUS
			{
				MSG("Initialising NEG operation\n");
				$$ = drv.bld.create<AST::UnaryOpNode>($2, AST::UnaryOp::NEG);
			}
	 	| 	"!" Expr %prec NOT
			{
				MSG("Initialising NOT operation\n");
				$$ = drv.bld.create<AST::UnaryOpNode>($2, AST::UnaryOp::NOT);
			};

Variable: 	ID
			{
				MSG("Initialising VariableNode\n");
				$$ = drv.bld.create<AST::VariableNode>(drv.bld.intern($1));
			};

%%

void yy::parser::error (const location_type& loc, const std::string& msg)
{
	std::cerr << loc << ": " << msg << '\n';
}
