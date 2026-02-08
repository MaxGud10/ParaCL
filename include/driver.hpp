#pragma once

#include <string>
#include <unordered_map>
#include <ostream>

#include "parser.hpp"
#include "ast.hpp"
#include "builder.hpp"

extern int yy_flex_debug;
extern int yydebug;
extern FILE* yyin;

#define YY_DECL \
	yy::parser::symbol_type yylex (Driver& drv)

YY_DECL;


class Driver final
{
public: /* usings */
	using Variables = std::unordered_map<std::string, int>;

public: /* members */
	std::string 	                       file;
	yy::location 	                       location;
	AST::AST		                       ast;
	AST::NodeBuilder                       bld;
	std::vector<std::vector<AST::StmtPtr>> stm_table;
	// size_t                                 cur_scope_id = 0;

public:
  	Driver(std::ostream& out = std::cout) :	ast(out)
	{
		stm_table.push_back(std::vector<AST::StmtPtr>());
	}

	int parse(const std::string &f)
	{
		file = f;

		location.initialize(&file);

		scan_begin();

		yy::parser parse(*this);

		#if YYDEBUG
    	parse.set_debug_level(YYDEBUG);
		#endif

		int status = parse();

		scan_end();

		return status;
	}

	void scan_begin()
	{
		yy_flex_debug = YY_FLEX_DEBUG;

		if (file.empty () || file == "-") yyin = stdin;
		else if (!(yyin = fopen (file.c_str (), "r")))
		{
			std::cerr << "cannot open " << file << '\n';
			exit (EXIT_FAILURE);
		}
	}

	std::vector<AST::StmtPtr>& cur_stmts()
	{
		return stm_table.back();
	}

	void push_statement(AST::StmtPtr stm)
	{
		cur_stmts().push_back(stm);
	}

	void enter_scope()
	{
		stm_table.emplace_back();
	}

	AST::ScopeNode* exit_scope_node()
	{
		if (stm_table.size() <= 1)
			throw std::runtime_error("exit_scope_node() called at global scope");

		auto stms = std::move(stm_table.back());
		stm_table.pop_back();

		return bld.create<AST::ScopeNode>(std::move(stms));
	}

	void build_global_scope()
	{
		ast.globalScope = bld.create<AST::ScopeNode>(std::move(stm_table.front()));
	}

	void scan_end ()
	{
		fclose (yyin);
	}
};
