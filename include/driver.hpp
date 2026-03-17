#pragma once

#include <string>
#include <unordered_map>
#include <ostream>

#include "parser.hpp"
#include "ast.hpp"
#include "builder.hpp"
#include "source_location.hpp"

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

    std::unordered_map<const AST::INode*, AST::SourceLocation> node_locations;

public:
  	Driver(std::ostream &out = std::cout) :	ast(out)
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

	AST::ScopeNode* exit_scope_node(const yy::location& loc)
	{
		if (stm_table.size() <= 1)
			throw std::runtime_error("exit_scope_node() called at global scope");

		auto stms = std::move(stm_table.back());
		stm_table.pop_back();

		return attach_location(
			bld.create<AST::ScopeNode>(std::move(stms)), loc);
	}

	void build_global_scope(const yy::location& loc)
	{
		ast.globalScope = attach_location(
			bld.create<AST::ScopeNode>(std::move(stm_table.front())), loc);
	}

	void scan_end ()
	{
		if (yyin && yyin != stdin)
			fclose(yyin);
	}

	AST::SourceLocation make_source_location(const yy::location& loc) const
	{
		AST::SourceLocation out;

		if (loc.begin.filename)
			out.file = *loc.begin.filename;

		out.line   = static_cast<int>(loc.begin.line);
		out.column = static_cast<int>(loc.begin.column);

		return out;
	}

	template <class T>
	T* attach_location(T* node, const yy::location& loc)
	{
		static_assert(std::is_base_of_v<AST::INode, T>,
					"attach_location(): T must derive from AST::INode");

		node_locations[node] = make_source_location(loc);
		return node;
	}

	const AST::SourceLocation* get_location(const AST::INode* node) const
	{
		auto it = node_locations.find(node);
		if (it == node_locations.end())
			return nullptr;

		return &it->second;
	}
};
