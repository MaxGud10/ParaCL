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
    FUNC        "func"
    RETURN      "return"
    COLON       ":"
    COMMA       ","
;

%token <std::string>	ID		"identifier"
%token <int> 			NUMBER 	"number"

// ----- Statement derived -----
%nterm <AST::ExpressionNode*> 	Expr
%nterm <AST::UnaryOpNode*> 		UnaryOp
%nterm <AST::BinaryOpNode*> 	BinaryOp
%nterm <AST::AssignNode*> 		Assign
%nterm <AST::AssignNode*>       FunctionAssign
%nterm <AST::ScopeNode*> 		Scope
%nterm <AST::PrintNode*> 		Print
%nterm <AST::IfNode*> 			If_Stm
%nterm <AST::ForNode*>          For_Stm
%nterm <AST::WhileNode*> 		While_Stm
%nterm <AST::VariableNode*> 	Variable
%nterm <AST::FunctionNode*>     FunctionLit
%nterm <AST::ExpressionNode*>   NonFunctionPrimary
%nterm <AST::ExpressionNode*>   NonFunctionPostfix
%nterm <AST::ExpressionNode*>   NonFunctionExpr
%nterm <AST::UnaryOpNode*>      NonFunctionUnaryOp
%nterm <AST::BinaryOpNode*>     NonFunctionBinaryOp
%nterm <AST::ExpressionNode*>   Primary
%nterm <AST::ExpressionNode*>   Postfix

%nterm <std::vector<AST::ExpressionNode*>> ArgList
%nterm <std::vector<AST::ExpressionNode*>> ArgListOpt
%nterm <std::vector<std::string>>          ParamList
%nterm <std::vector<std::string>>          ParamListOpt
%nterm <std::string>                       NameOpt

%nterm <AST::StatementNode*>	Statement

%printer { yyo << $$; } <*>;

%precedence IFX
%right ELSE

%right "if"
%right "print"
%right "while"
%right "for"

%precedence FUNC_ASSIGN_NO_SEMI
%precedence ";"

%right "=" "+=" "-=" "*=" "/=" "%="

%left "||"
%left "&&"
%left "|"
%left "&"

%nonassoc "==" "!="
%nonassoc "<" "<=" ">" ">="

%left "+" "-"
%left "*" "/" "%"

%right UMINUS NOT

%%

Program: /* nothing */
			YYEOF
			{
				drv.ast.globalScope =
					drv.attach_location(
						drv.bld.create<AST::ScopeNode>(std::vector<AST::StatementNode*>{}), @$);
			}
			| Statements YYEOF
	   		{
				drv.build_global_scope(@1);
			};

Statements: Statement
			{
				LOG("Pushing statement : {}\n",
					static_cast<const void*>($1));

				drv.push_statement($1);
			}
		|	Statements Statement
		  	{
				LOG("Pushing statement : {}\n",
					static_cast<const void*>($2));

				drv.push_statement($2);
			};

Statement:
			";"
			{
				MSG("Void statement\n");
				$$ = drv.attach_location(drv.bld.create<AST::VoidNode>(), @$);
			}
		|   FunctionAssign %prec FUNC_ASSIGN_NO_SEMI
			{
				LOG("It's FunctionAssign without semicolon. Moving from concrete rule: {}\n",
					static_cast<const void*>($1));

				$$ = $1;
			}
		|   FunctionAssign ";"
			{
				LOG("It's FunctionAssign with semicolon. Moving from concrete rule: {}\n",
					static_cast<const void*>($1));

				$$ = $1;
			}
		|	Expr ";"
			{
				LOG("It's Expr. Moving from concrete rule: {}\n",
					static_cast<const void*>($$));

				$$ = $1;
			}
        |   RETURN Expr ";"
            {
                MSG("Return with value\n");
                $$ = drv.attach_location(drv.bld.create<AST::ReturnNode>($2), @$);
            }
        |   RETURN ";"
            {
                MSG("Return without value\n");
				$$ = drv.attach_location(drv.bld.create<AST::ReturnNode>(nullptr), @$);
            }
		| 	Scope
		 	{
				LOG("It's Scope. Moving from concrete rule: {}\n",
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
			$$ = drv.exit_scope_node(@$);
		}
		| StartScope EndScope
		{
            MSG("Initialising empty scope\n");
			$$ = drv.exit_scope_node(@$);
		};

StartScope: "{"
			{
				MSG("Scope start.\n");
				drv.enter_scope();
			};

EndScope: 	"}"
			{

			};

If_Stm: 	IF "(" Expr ")" Statement %prec IFX
			{
				MSG("Initialising if statement\n");
				$$ = drv.attach_location(drv.bld.create<AST::IfNode>($3, $5), @$);
			}
		|   IF "(" Expr ")" Statement ELSE Statement
			{
				$$ = drv.attach_location(drv.bld.create<AST::IfNode>($3, $5, $7), @$);
			};

For_Stm:    FOR "(" Assign ";" Expr ";" Assign ")" Statement
            {
                MSG("Initialising for statement\n");
				$$ = drv.attach_location(drv.bld.create<AST::ForNode>($3, $5, $7, $9), @$);
            };

While_Stm:	WHILE "(" Expr ")" Statement
			{
				MSG("Initialising while statement\n");
				$$ = drv.attach_location(drv.bld.create<AST::WhileNode>($3, $5), @$);
			};

Assign: Variable "=" NonFunctionExpr
        {
            $$ = drv.attach_location(
                drv.bld.create<AST::AssignNode>($1, $3),
                @$
            );
            LOG("Initialising assignment: {}\n", static_cast<const void*>($$));
        }
    |   Variable "+=" Expr
        {
			auto oldX = drv.attach_location(drv.bld.create<AST::VariableNode>(drv.bld.intern($1->get_name())), @1);
			auto val  = drv.attach_location(drv.bld.create<AST::BinaryOpNode>(oldX, AST::BinaryOp::ADD, $3), @$);
			$$ = drv.attach_location(drv.bld.create<AST::AssignNode>($1, val), @$);
        }
    |   Variable "-=" Expr
        {
			auto oldX = drv.attach_location(drv.bld.create<AST::VariableNode>(drv.bld.intern($1->get_name())), @1);
			auto val  = drv.attach_location(drv.bld.create<AST::BinaryOpNode>(oldX, AST::BinaryOp::SUB, $3), @$);
			$$ = drv.attach_location(drv.bld.create<AST::AssignNode>($1, val), @$);
        }
    |   Variable "*=" Expr
        {
			auto oldX = drv.attach_location(drv.bld.create<AST::VariableNode>(drv.bld.intern($1->get_name())), @1);
			auto val  = drv.attach_location(drv.bld.create<AST::BinaryOpNode>(oldX, AST::BinaryOp::MUL, $3), @$);
			$$ = drv.attach_location(drv.bld.create<AST::AssignNode>($1, val), @$);
        }
    |   Variable "/=" Expr
        {
			auto oldX = drv.attach_location(drv.bld.create<AST::VariableNode>(drv.bld.intern($1->get_name())), @1);
			auto val  = drv.attach_location(drv.bld.create<AST::BinaryOpNode>(oldX, AST::BinaryOp::DIV, $3), @$);
			$$ = drv.attach_location(drv.bld.create<AST::AssignNode>($1, val), @$);
        };

FunctionAssign: Variable "=" FunctionLit
				{
					$$ = drv.attach_location(
						drv.bld.create<AST::AssignNode>($1, $3), @$);

					LOG("Initialising function assignment: {}\n",
						static_cast<const void*>($$));
				};

Print: 	"print" Expr
		{
			MSG("Initialising print\n");
			$$ = drv.attach_location(drv.bld.create<AST::PrintNode>($2), @$);
		}


Expr:   Postfix
        {
			$$ = $1;
		}
    |   BinaryOp
		{
			MSG("Moving BinaryOp\n");
			$$ = $1;
		}
	|	UnaryOp
		{
			MSG("Moving UnaryOp\n");
			$$ = $1;
		}
	| Assign { $$ = $1; };

NonFunctionExpr:
            NonFunctionPostfix
            {
                $$ = $1;
            }
        |   NonFunctionBinaryOp
            {
                $$ = $1;
            }
        |   NonFunctionUnaryOp
            {
                $$ = $1;
            }
        |   Assign
            {
                $$ = $1;
            };

NonFunctionBinaryOp:
            NonFunctionExpr "+" NonFunctionExpr
            {
                $$ = drv.attach_location(drv.bld.create<AST::BinaryOpNode>($1, AST::BinaryOp::ADD, $3), @$);
            }
        |   NonFunctionExpr "-" NonFunctionExpr
            {
                $$ = drv.attach_location(drv.bld.create<AST::BinaryOpNode>($1, AST::BinaryOp::SUB, $3), @$);
            }
        |   NonFunctionExpr "*" NonFunctionExpr
            {
                $$ = drv.attach_location(drv.bld.create<AST::BinaryOpNode>($1, AST::BinaryOp::MUL, $3), @$);
            }
        |   NonFunctionExpr "/" NonFunctionExpr
            {
                $$ = drv.attach_location(drv.bld.create<AST::BinaryOpNode>($1, AST::BinaryOp::DIV, $3), @$);
            }
        |   NonFunctionExpr ">" NonFunctionExpr
            {
                $$ = drv.attach_location(drv.bld.create<AST::BinaryOpNode>($1, AST::BinaryOp::GR, $3), @$);
            }
        |   NonFunctionExpr "<" NonFunctionExpr
            {
                $$ = drv.attach_location(drv.bld.create<AST::BinaryOpNode>($1, AST::BinaryOp::LS, $3), @$);
            }
        |   NonFunctionExpr ">=" NonFunctionExpr
            {
                $$ = drv.attach_location(drv.bld.create<AST::BinaryOpNode>($1, AST::BinaryOp::GR_EQ, $3), @$);
            }
        |   NonFunctionExpr "<=" NonFunctionExpr
            {
                $$ = drv.attach_location(drv.bld.create<AST::BinaryOpNode>($1, AST::BinaryOp::LS_EQ, $3), @$);
            }
        |   NonFunctionExpr "==" NonFunctionExpr
            {
                $$ = drv.attach_location(drv.bld.create<AST::BinaryOpNode>($1, AST::BinaryOp::EQ, $3), @$);
            }
        |   NonFunctionExpr "!=" NonFunctionExpr
            {
                $$ = drv.attach_location(drv.bld.create<AST::BinaryOpNode>($1, AST::BinaryOp::NOT_EQ, $3), @$);
            }
        |   NonFunctionExpr "&&" NonFunctionExpr
            {
                $$ = drv.attach_location(drv.bld.create<AST::BinaryOpNode>($1, AST::BinaryOp::AND, $3), @$);
            }
        |   NonFunctionExpr "||" NonFunctionExpr
            {
                $$ = drv.attach_location(drv.bld.create<AST::BinaryOpNode>($1, AST::BinaryOp::OR, $3), @$);
            }
        |   NonFunctionExpr "&" NonFunctionExpr
            {
                $$ = drv.attach_location(drv.bld.create<AST::BinaryOpNode>($1, AST::BinaryOp::BIT_AND, $3), @$);
            }
        |   NonFunctionExpr "|" NonFunctionExpr
            {
                $$ = drv.attach_location(drv.bld.create<AST::BinaryOpNode>($1, AST::BinaryOp::BIT_OR, $3), @$);
            }
        |   NonFunctionExpr "%" NonFunctionExpr
            {
                $$ = drv.attach_location(drv.bld.create<AST::BinaryOpNode>($1, AST::BinaryOp::MOD, $3), @$);
            }
        ;

BinaryOp: 	Expr "+" Expr
			{
				MSG("Initialising ADD operation\n");
				$$ = drv.attach_location(drv.bld.create<AST::BinaryOpNode>($1, AST::BinaryOp::ADD, $3), @$);
			}
		| 	Expr "-" Expr
			{
				MSG("Initialising SUB operation\n");
				$$ = drv.attach_location(drv.bld.create<AST::BinaryOpNode>($1, AST::BinaryOp::SUB, $3), @$);
			}
		| 	Expr "*" Expr
			{
				MSG("Initialising MUL operation\n");
				$$ = drv.attach_location(drv.bld.create<AST::BinaryOpNode>($1, AST::BinaryOp::MUL, $3), @$);
			}
		| 	Expr "/" Expr
			{
				MSG("Initialising DIV operation\n");
				$$ = drv.attach_location(drv.bld.create<AST::BinaryOpNode>($1, AST::BinaryOp::DIV, $3), @$);
			}
		|	Expr ">" Expr
			{
				MSG("Initialising GR operation\n");
				$$ = drv.attach_location(drv.bld.create<AST::BinaryOpNode>($1, AST::BinaryOp::GR, $3), @$);
			}
		|	Expr "<" Expr
			{
				MSG("Initialising LS operation\n");
				$$ = drv.attach_location(drv.bld.create<AST::BinaryOpNode>($1, AST::BinaryOp::LS, $3), @$);
			}
		|	Expr ">=" Expr
			{
				MSG("Initialising RG_EQ operation\n");
				$$ = drv.attach_location(drv.bld.create<AST::BinaryOpNode>($1, AST::BinaryOp::GR_EQ, $3), @$);
			}
		|	Expr "<=" Expr
			{
				MSG("Initialising LS_EQ operation\n");
				$$ = drv.attach_location(drv.bld.create<AST::BinaryOpNode>($1, AST::BinaryOp::LS_EQ, $3), @$);
			}
		|	Expr "==" Expr
			{
				MSG("Initialising EQ operation\n");
				$$ = drv.attach_location(drv.bld.create<AST::BinaryOpNode>($1, AST::BinaryOp::EQ, $3), @$);
			}
		|	Expr "!=" Expr
			{
				MSG("Initialising NOT_EQ operation\n");
				$$ = drv.attach_location(drv.bld.create<AST::BinaryOpNode>($1, AST::BinaryOp::NOT_EQ, $3), @$);
			}
		|	Expr "&&" Expr
			{
				MSG("Initialising AND operation\n");
				$$ = drv.attach_location(drv.bld.create<AST::BinaryOpNode>($1, AST::BinaryOp::AND, $3), @$);
			}
		|	Expr "||" Expr
			{
				MSG("Initialising OR operation\n");
				$$ = drv.attach_location(drv.bld.create<AST::BinaryOpNode>($1, AST::BinaryOp::OR, $3), @$);
			}
        |   Expr "&" Expr
            {
                MSG("Initialising BIT_AND operation\n");
				$$ = drv.attach_location(drv.bld.create<AST::BinaryOpNode>($1, AST::BinaryOp::BIT_AND, $3), @$);
            }
        |   Expr "|" Expr
            {
                MSG("Initialising BIT_OR operation\n");
				$$ = drv.attach_location(drv.bld.create<AST::BinaryOpNode>($1, AST::BinaryOp::BIT_OR, $3), @$);
            }
		|	Expr "%" Expr
			{
				MSG("Initialising MOD operation\n");
				$$ = drv.attach_location(drv.bld.create<AST::BinaryOpNode>($1, AST::BinaryOp::MOD, $3), @$);
			};

NonFunctionUnaryOp:
            "-" NonFunctionExpr %prec UMINUS
            {
                $$ = drv.attach_location(drv.bld.create<AST::UnaryOpNode>($2, AST::UnaryOp::NEG), @$);
            }
        |   "!" NonFunctionExpr %prec NOT
            {
                $$ = drv.attach_location(drv.bld.create<AST::UnaryOpNode>($2, AST::UnaryOp::NOT), @$);
            };

UnaryOp	: 	"-" Expr %prec UMINUS
			{
				MSG("Initialising NEG operation\n");
				$$ = drv.attach_location(drv.bld.create<AST::UnaryOpNode>($2, AST::UnaryOp::NEG), @$);
			}
	 	| 	"!" Expr %prec NOT
			{
				MSG("Initialising NOT operation\n");
				$$ = drv.attach_location(drv.bld.create<AST::UnaryOpNode>($2, AST::UnaryOp::NOT), @$);
			};

Variable: 	ID
			{
				MSG("Initialising VariableNode\n");
				$$ = drv.attach_location(drv.bld.create<AST::VariableNode>(drv.bld.intern($1)), @1);
			};

// списки аргументов
ArgListOpt: /* empty */
			{
				$$ = {};
			}
        |   ArgList
			{
				$$ = std::move($1);
			};

ArgList:    Expr
			{
				$$ = std::vector<AST::ExpressionNode*>{ $1 };
			}
        |   ArgList COMMA Expr
			{
				$1.push_back($3);
				$$ = std::move($1);
			};

// списки параметров
ParamListOpt: /* empty */
			{
				$$ = {};
			}
        |   ParamList
			{
				$$ = std::move($1);
			};

ParamList:  ID
			{
				$$ = std::vector<std::string>{ $1 };
			}
        |   ParamList COMMA ID
			{
				$1.push_back($3);
				$$ = std::move($1);
			};

// имя после двоеточия
NameOpt:    /* empty */
		    {
				$$ = "";
			}
        |   COLON ID
			{
				$$ = $2;
			};

// функция-литерал
FunctionLit: FUNC "(" ParamListOpt ")" NameOpt Scope
			{
			 	std::vector<std::string_view> params;
				params.reserve($3.size());
				for (auto& s : $3)
					params.push_back(drv.bld.intern(s));

				bool hasName = !$5.empty();
				std::string_view fname = hasName ? drv.bld.intern($5) : std::string_view{};

				$$ = drv.attach_location(drv.bld.create<AST::FunctionNode>(std::move(params), $6, fname, hasName), @$);
            };

NonFunctionPrimary:
            NUMBER
            {
                MSG("Initialising ConstantNode\n");
                $$ = drv.attach_location(drv.bld.create<AST::ConstantNode>($1), @1);
            }
        |   "?"
            {
                MSG("Initialising InNode\n");
                $$ = drv.attach_location(drv.bld.create<AST::InNode>(), @1);
            }
        |   Variable
            {
                MSG("Moving VariableNode\n");
                $$ = $1;
            }
        |   "(" Expr ")"
            {
                MSG("Moving Expression in parenthesis\n");
                $$ = $2;
            }
        |   "(" Scope ")"
            {
                MSG("Moving Scope as expression\n");
                $$ = $2;
            };

NonFunctionPostfix:
            NonFunctionPrimary
            {
                $$ = $1;
            }
        |   NonFunctionPostfix "(" ArgListOpt ")"
            {
                MSG("Initialising CallNode\n");
                $$ = drv.attach_location(
                    drv.bld.create<AST::CallNode>($1, std::move($3)),
                    @$
                );
            };

Primary:    NUMBER
            {
				MSG("Initialising ConstantNode\n");
				$$ = drv.attach_location(drv.bld.create<AST::ConstantNode>($1), @1);
           }
        |  "?"
            {
				MSG("Initialising InNode\n");
				$$ = drv.attach_location(drv.bld.create<AST::InNode>(), @1);
            }
        |   Variable
        	{
            	MSG("Moving VariableNode\n");
            	$$ = $1;
        	}
    	|  "(" Expr ")"
			{
				MSG("Moving Expression in parenthesis\n");
				$$ = $2;
			}
        |   "(" Scope ")"
			{
				MSG("Moving Scope as expression\n");
				$$ = $2;
			}
        |   FunctionLit
			{
				MSG("Moving Function literal\n");
				$$ = $1;
			};

// postfix вызовы
Postfix:    Primary
            {
				$$ = $1;
			}
    	|   Postfix "(" ArgListOpt ")"
			{
				MSG("Initialising CallNode\n");
				$$ = drv.attach_location(drv.bld.create<AST::CallNode>($1, std::move($3)), @$);
			};
%%

void yy::parser::error (const location_type& loc, const std::string& msg)
{
	std::cerr << loc << ": " << msg << '\n';
}
