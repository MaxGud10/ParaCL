#pragma once

# include <string>
# include <map>

# include "parser.hh"

extern int yy_flex_debug;
// extern int yydebug;
extern FILE* yyin;

# define YY_DECL \
	yy::parser::symbol_type yylex (driver& drv)

YY_DECL;


class driver
{
  public:
	std::map<std::string, int> variables;

	int result;

	std::string file;

	yy::location location;

  public:

	int parse(const std::string &f)
	{
		file = f;

		location.initialize(&file);

		scan_begin();

		// yydebug = YYDEBUG;

		yy::parser parse(*this);

		int res = parse();

		scan_end();

		return res;
	}


	void scan_begin()
	{
		yy_flex_debug = YY_FLEX_DEBUG;

		if (file.empty () || file == "-") yyin = stdin;
		else if (!(yyin = fopen (file.c_str (), "r")))
		{
			std::cerr << "cannot open " << file << ": " << strerror (errno) << '\n';
			exit (EXIT_FAILURE);
		}
	}

	void scan_end ()
	{
		fclose (yyin);
	}
};