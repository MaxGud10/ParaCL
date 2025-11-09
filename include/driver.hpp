#pragma once

#include <string>
#include <unordered_map>
#include <ostream>

#include "parser.hpp"
#include "ast.hpp"

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
	std::vector<std::vector<AST::StmtPtr>> stm_table;
	size_t                                 cur_scope_id = 0;

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

	void scan_end ()
	{
		fclose (yyin);
	}
};
