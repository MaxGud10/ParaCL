%{ /* -*- C++ -*- */

# include <cerrno>
# include <climits>
# include <cstdlib>
# include <cstring>
# include <string>

# include "driver.hpp"
# include "parser.hpp"

%}

%option noyywrap nounput noinput batch debug

%{
	yy::parser::symbol_type
	make_NUMBER (const std::string &s, const yy::parser::location_type& loc);
%}

ID		[a-zA-Z][a-zA-Z_0-9]*
INT		[0-9]+
BLANK 	[ \t\r]

%x COMMENT

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


"-="        return yy::parser::make_MINUS_ASSIGN(loc);

"//".*           {}

"/*"             { BEGIN(COMMENT); }

<COMMENT>"*/"    { BEGIN(INITIAL); }

<COMMENT>\n      {
                    loc.lines(1);
                    loc.step();
                 }

<COMMENT>.       {}

<COMMENT><<EOF>> {
                    throw yy::parser::syntax_error(
                        loc, "unterminated comment"
                    );
                 }

"-"			return yy::parser::make_MINUS		    (loc);
"+="    return yy::parser::make_PLUS_ASSIGN (loc);
"+"			return yy::parser::make_PLUS		    (loc);
"*="    return yy::parser::make_STAR_ASSIGN (loc);
"*"			return yy::parser::make_STAR		    (loc);
"/="    return yy::parser::make_SLASH_ASSIGN(loc);
"/"			return yy::parser::make_SLASH		    (loc);
"("			return yy::parser::make_LPAREN		  (loc);
")"			return yy::parser::make_RPAREN		  (loc);
"="			return yy::parser::make_ASSIGN		  (loc);
"?"			return yy::parser::make_READ		    (loc);
"print"	return yy::parser::make_PRINT		    (loc);
"{"			return yy::parser::make_LCPAREN		  (loc);
"}"			return yy::parser::make_RCPAREN		  (loc);
";"			return yy::parser::make_SEMIC       (loc);
"if"		return yy::parser::make_IF			    (loc);
"else"  return yy::parser::make_ELSE        (loc);
"while"	return yy::parser::make_WHILE		    (loc);
"for"   return yy::parser::make_FOR         (loc);
">"			return yy::parser::make_GREATER		  (loc);
"<"			return yy::parser::make_LESS		    (loc);
">="		return yy::parser::make_GREATER_E	  (loc);
"<="		return yy::parser::make_LESS_E	  	(loc);
"=="		return yy::parser::make_EQUAL		    (loc);
"!="		return yy::parser::make_NOT_EQUAL	  (loc);
"!"			return yy::parser::make_NOT			    (loc);
"%"			return yy::parser::make_MOD			    (loc);
"&&"    return yy::parser::make_AND_OP      (loc);
"||"    return yy::parser::make_OR_OP       (loc);
"&"     return yy::parser::make_BIT_AND     (loc);
"|"     return yy::parser::make_BIT_OR      (loc);
"+="		return yy::parser::make_ASSIGN_PLUS (loc);
"-="		return yy::parser::make_ASSIGN_MINUS(loc);
"*="		return yy::parser::make_ASSIGN_MUL  (loc);
"/="		return yy::parser::make_ASSIGN_DIV  (loc);
"%="		return yy::parser::make_ASSIGN_MOD  (loc);


{INT}		return make_NUMBER         (yytext,  loc);
{ID}		return yy::parser::make_ID (yytext,  loc);

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
