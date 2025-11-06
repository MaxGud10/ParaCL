
%{ /* -*- C++ -*- */

# include <cerrno>
# include <climits>
# include <cstdlib>
# include <cstring>
# include <string>
# include "driver.hh"
# include "parser.hh"

%}

%option noyywrap nounput noinput batch debug

%{
	yy::parser::symbol_type
	make_NUMBER (const std::string &s, const yy::parser::location_type& loc);
%}

ID		[a-zA-Z][a-zA-Z_0-9]*
INT		[0-9]+
BLANK 	[ \t\r]

%{
	#define YY_USER_ACTION	loc.columns (yyleng);
%}

%%

%{
	yy::location& loc = drv.location;
	loc.step ();
%}

{BLANK}+	loc.step ();
\n+			loc.lines (yyleng); loc.step ();

"-"			return yy::parser::make_MINUS(loc);
"+"			return yy::parser::make_PLUS(loc);
"*"			return yy::parser::make_STAR(loc);
"/"			return yy::parser::make_SLASH(loc);
"("			return yy::parser::make_LPAREN(loc);
")"			return yy::parser::make_RPAREN(loc);
"="			return yy::parser::make_ASSIGN(loc);

{INT}		return make_NUMBER (yytext, loc);
{ID}		return yy::parser::make_IDENTIFIER (yytext, loc);

.			{
				throw yy::parser::syntax_error
				(loc, "invalid character: " + std::string(yytext));
			}

<<EOF>>		return yy::parser::make_YYEOF (loc);

%%

yy::parser::symbol_type
make_NUMBER (const std::string &s, const yy::parser::location_type& loc)
{
	long num = strtol (s.c_str(), NULL, 10);

	if (! (INT_MIN <= num && num <= INT_MAX && errno != ERANGE))
		throw yy::parser::syntax_error (loc, "integer is out of range: " + s);

	return yy::parser::make_NUMBER (static_cast<int>(num), loc);
}

void driver::scan_begin()
{
	yy_flex_debug = trace_scanning;

	if (file.empty () || file == "-") yyin = stdin;
	else if (!(yyin = fopen (file.c_str (), "r")))
	{
		std::cerr << "cannot open " << file << ": " << strerror (errno) << '\n';
		exit (EXIT_FAILURE);
	}
}

void driver::scan_end ()
{
	fclose (yyin);
}