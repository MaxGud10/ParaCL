// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.





#include "parser.tab.h"


// Unqualified %code blocks.
#line 27 "parser.y"

	#include <memory>

	#include "log.h"
	#include "driver.hpp"

#line 53 "parser.tab.c"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 145 "parser.tab.c"

  /// Build a parser object.
  parser::parser (Driver& drv_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      yy_lac_established_ (false),
      drv (drv_yyarg)
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/



  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_kind_type
  parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_Assign: // Assign
        value.YY_MOVE_OR_COPY< AST::AssignNode* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_BinaryOp: // BinaryOp
        value.YY_MOVE_OR_COPY< AST::BinaryOpNode* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Expr: // Expr
      case symbol_kind::S_Primary: // Primary
      case symbol_kind::S_Postfix: // Postfix
        value.YY_MOVE_OR_COPY< AST::ExpressionNode* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_For_Stm: // For_Stm
        value.YY_MOVE_OR_COPY< AST::ForNode* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FunctionLit: // FunctionLit
        value.YY_MOVE_OR_COPY< AST::FunctionNode* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_If_Stm: // If_Stm
        value.YY_MOVE_OR_COPY< AST::IfNode* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Print: // Print
        value.YY_MOVE_OR_COPY< AST::PrintNode* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Scope: // Scope
        value.YY_MOVE_OR_COPY< AST::ScopeNode* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Statement: // Statement
        value.YY_MOVE_OR_COPY< AST::StatementNode* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_UnaryOp: // UnaryOp
        value.YY_MOVE_OR_COPY< AST::UnaryOpNode* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Variable: // Variable
        value.YY_MOVE_OR_COPY< AST::VariableNode* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_While_Stm: // While_Stm
        value.YY_MOVE_OR_COPY< AST::WhileNode* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NUMBER: // "number"
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ID: // "identifier"
      case symbol_kind::S_NameOpt: // NameOpt
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ArgListOpt: // ArgListOpt
      case symbol_kind::S_ArgList: // ArgList
        value.YY_MOVE_OR_COPY< std::vector<AST::ExpressionNode*> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ParamListOpt: // ParamListOpt
      case symbol_kind::S_ParamList: // ParamList
        value.YY_MOVE_OR_COPY< std::vector<std::string> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_Assign: // Assign
        value.move< AST::AssignNode* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_BinaryOp: // BinaryOp
        value.move< AST::BinaryOpNode* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Expr: // Expr
      case symbol_kind::S_Primary: // Primary
      case symbol_kind::S_Postfix: // Postfix
        value.move< AST::ExpressionNode* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_For_Stm: // For_Stm
        value.move< AST::ForNode* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FunctionLit: // FunctionLit
        value.move< AST::FunctionNode* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_If_Stm: // If_Stm
        value.move< AST::IfNode* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Print: // Print
        value.move< AST::PrintNode* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Scope: // Scope
        value.move< AST::ScopeNode* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Statement: // Statement
        value.move< AST::StatementNode* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_UnaryOp: // UnaryOp
        value.move< AST::UnaryOpNode* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Variable: // Variable
        value.move< AST::VariableNode* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_While_Stm: // While_Stm
        value.move< AST::WhileNode* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NUMBER: // "number"
        value.move< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ID: // "identifier"
      case symbol_kind::S_NameOpt: // NameOpt
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ArgListOpt: // ArgListOpt
      case symbol_kind::S_ArgList: // ArgList
        value.move< std::vector<AST::ExpressionNode*> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ParamListOpt: // ParamListOpt
      case symbol_kind::S_ParamList: // ParamList
        value.move< std::vector<std::string> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_Assign: // Assign
        value.copy< AST::AssignNode* > (that.value);
        break;

      case symbol_kind::S_BinaryOp: // BinaryOp
        value.copy< AST::BinaryOpNode* > (that.value);
        break;

      case symbol_kind::S_Expr: // Expr
      case symbol_kind::S_Primary: // Primary
      case symbol_kind::S_Postfix: // Postfix
        value.copy< AST::ExpressionNode* > (that.value);
        break;

      case symbol_kind::S_For_Stm: // For_Stm
        value.copy< AST::ForNode* > (that.value);
        break;

      case symbol_kind::S_FunctionLit: // FunctionLit
        value.copy< AST::FunctionNode* > (that.value);
        break;

      case symbol_kind::S_If_Stm: // If_Stm
        value.copy< AST::IfNode* > (that.value);
        break;

      case symbol_kind::S_Print: // Print
        value.copy< AST::PrintNode* > (that.value);
        break;

      case symbol_kind::S_Scope: // Scope
        value.copy< AST::ScopeNode* > (that.value);
        break;

      case symbol_kind::S_Statement: // Statement
        value.copy< AST::StatementNode* > (that.value);
        break;

      case symbol_kind::S_UnaryOp: // UnaryOp
        value.copy< AST::UnaryOpNode* > (that.value);
        break;

      case symbol_kind::S_Variable: // Variable
        value.copy< AST::VariableNode* > (that.value);
        break;

      case symbol_kind::S_While_Stm: // While_Stm
        value.copy< AST::WhileNode* > (that.value);
        break;

      case symbol_kind::S_NUMBER: // "number"
        value.copy< int > (that.value);
        break;

      case symbol_kind::S_ID: // "identifier"
      case symbol_kind::S_NameOpt: // NameOpt
        value.copy< std::string > (that.value);
        break;

      case symbol_kind::S_ArgListOpt: // ArgListOpt
      case symbol_kind::S_ArgList: // ArgList
        value.copy< std::vector<AST::ExpressionNode*> > (that.value);
        break;

      case symbol_kind::S_ParamListOpt: // ParamListOpt
      case symbol_kind::S_ParamList: // ParamList
        value.copy< std::vector<std::string> > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_Assign: // Assign
        value.move< AST::AssignNode* > (that.value);
        break;

      case symbol_kind::S_BinaryOp: // BinaryOp
        value.move< AST::BinaryOpNode* > (that.value);
        break;

      case symbol_kind::S_Expr: // Expr
      case symbol_kind::S_Primary: // Primary
      case symbol_kind::S_Postfix: // Postfix
        value.move< AST::ExpressionNode* > (that.value);
        break;

      case symbol_kind::S_For_Stm: // For_Stm
        value.move< AST::ForNode* > (that.value);
        break;

      case symbol_kind::S_FunctionLit: // FunctionLit
        value.move< AST::FunctionNode* > (that.value);
        break;

      case symbol_kind::S_If_Stm: // If_Stm
        value.move< AST::IfNode* > (that.value);
        break;

      case symbol_kind::S_Print: // Print
        value.move< AST::PrintNode* > (that.value);
        break;

      case symbol_kind::S_Scope: // Scope
        value.move< AST::ScopeNode* > (that.value);
        break;

      case symbol_kind::S_Statement: // Statement
        value.move< AST::StatementNode* > (that.value);
        break;

      case symbol_kind::S_UnaryOp: // UnaryOp
        value.move< AST::UnaryOpNode* > (that.value);
        break;

      case symbol_kind::S_Variable: // Variable
        value.move< AST::VariableNode* > (that.value);
        break;

      case symbol_kind::S_While_Stm: // While_Stm
        value.move< AST::WhileNode* > (that.value);
        break;

      case symbol_kind::S_NUMBER: // "number"
        value.move< int > (that.value);
        break;

      case symbol_kind::S_ID: // "identifier"
      case symbol_kind::S_NameOpt: // NameOpt
        value.move< std::string > (that.value);
        break;

      case symbol_kind::S_ArgListOpt: // ArgListOpt
      case symbol_kind::S_ArgList: // ArgList
        value.move< std::vector<AST::ExpressionNode*> > (that.value);
        break;

      case symbol_kind::S_ParamListOpt: // ParamListOpt
      case symbol_kind::S_ParamList: // ParamList
        value.move< std::vector<std::string> > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        switch (yykind)
    {
      case symbol_kind::S_ID: // "identifier"
#line 109 "parser.y"
                 { yyo << yysym.value.template as < std::string > (); }
#line 573 "parser.tab.c"
        break;

      case symbol_kind::S_NUMBER: // "number"
#line 109 "parser.y"
                 { yyo << yysym.value.template as < int > (); }
#line 579 "parser.tab.c"
        break;

      case symbol_kind::S_Statement: // Statement
#line 109 "parser.y"
                 { yyo << yysym.value.template as < AST::StatementNode* > (); }
#line 585 "parser.tab.c"
        break;

      case symbol_kind::S_Scope: // Scope
#line 109 "parser.y"
                 { yyo << yysym.value.template as < AST::ScopeNode* > (); }
#line 591 "parser.tab.c"
        break;

      case symbol_kind::S_If_Stm: // If_Stm
#line 109 "parser.y"
                 { yyo << yysym.value.template as < AST::IfNode* > (); }
#line 597 "parser.tab.c"
        break;

      case symbol_kind::S_For_Stm: // For_Stm
#line 109 "parser.y"
                 { yyo << yysym.value.template as < AST::ForNode* > (); }
#line 603 "parser.tab.c"
        break;

      case symbol_kind::S_While_Stm: // While_Stm
#line 109 "parser.y"
                 { yyo << yysym.value.template as < AST::WhileNode* > (); }
#line 609 "parser.tab.c"
        break;

      case symbol_kind::S_Assign: // Assign
#line 109 "parser.y"
                 { yyo << yysym.value.template as < AST::AssignNode* > (); }
#line 615 "parser.tab.c"
        break;

      case symbol_kind::S_Print: // Print
#line 109 "parser.y"
                 { yyo << yysym.value.template as < AST::PrintNode* > (); }
#line 621 "parser.tab.c"
        break;

      case symbol_kind::S_Expr: // Expr
#line 109 "parser.y"
                 { yyo << yysym.value.template as < AST::ExpressionNode* > (); }
#line 627 "parser.tab.c"
        break;

      case symbol_kind::S_BinaryOp: // BinaryOp
#line 109 "parser.y"
                 { yyo << yysym.value.template as < AST::BinaryOpNode* > (); }
#line 633 "parser.tab.c"
        break;

      case symbol_kind::S_UnaryOp: // UnaryOp
#line 109 "parser.y"
                 { yyo << yysym.value.template as < AST::UnaryOpNode* > (); }
#line 639 "parser.tab.c"
        break;

      case symbol_kind::S_Variable: // Variable
#line 109 "parser.y"
                 { yyo << yysym.value.template as < AST::VariableNode* > (); }
#line 645 "parser.tab.c"
        break;

      case symbol_kind::S_ArgListOpt: // ArgListOpt
#line 109 "parser.y"
                 { yyo << yysym.value.template as < std::vector<AST::ExpressionNode*> > (); }
#line 651 "parser.tab.c"
        break;

      case symbol_kind::S_ArgList: // ArgList
#line 109 "parser.y"
                 { yyo << yysym.value.template as < std::vector<AST::ExpressionNode*> > (); }
#line 657 "parser.tab.c"
        break;

      case symbol_kind::S_ParamListOpt: // ParamListOpt
#line 109 "parser.y"
                 { yyo << yysym.value.template as < std::vector<std::string> > (); }
#line 663 "parser.tab.c"
        break;

      case symbol_kind::S_ParamList: // ParamList
#line 109 "parser.y"
                 { yyo << yysym.value.template as < std::vector<std::string> > (); }
#line 669 "parser.tab.c"
        break;

      case symbol_kind::S_NameOpt: // NameOpt
#line 109 "parser.y"
                 { yyo << yysym.value.template as < std::string > (); }
#line 675 "parser.tab.c"
        break;

      case symbol_kind::S_FunctionLit: // FunctionLit
#line 109 "parser.y"
                 { yyo << yysym.value.template as < AST::FunctionNode* > (); }
#line 681 "parser.tab.c"
        break;

      case symbol_kind::S_Primary: // Primary
#line 109 "parser.y"
                 { yyo << yysym.value.template as < AST::ExpressionNode* > (); }
#line 687 "parser.tab.c"
        break;

      case symbol_kind::S_Postfix: // Postfix
#line 109 "parser.y"
                 { yyo << yysym.value.template as < AST::ExpressionNode* > (); }
#line 693 "parser.tab.c"
        break;

      default:
        break;
    }
        yyo << ')';
      }
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

    // Discard the LAC context in case there still is one left from a
    // previous invocation.
    yy_lac_discard_ ("init");

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex (drv));
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        if (!yy_lac_establish_ (yyla.kind ()))
          goto yyerrlab;
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        if (!yy_lac_establish_ (yyla.kind ()))
          goto yyerrlab;

        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    yy_lac_discard_ ("shift");
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_Assign: // Assign
        yylhs.value.emplace< AST::AssignNode* > ();
        break;

      case symbol_kind::S_BinaryOp: // BinaryOp
        yylhs.value.emplace< AST::BinaryOpNode* > ();
        break;

      case symbol_kind::S_Expr: // Expr
      case symbol_kind::S_Primary: // Primary
      case symbol_kind::S_Postfix: // Postfix
        yylhs.value.emplace< AST::ExpressionNode* > ();
        break;

      case symbol_kind::S_For_Stm: // For_Stm
        yylhs.value.emplace< AST::ForNode* > ();
        break;

      case symbol_kind::S_FunctionLit: // FunctionLit
        yylhs.value.emplace< AST::FunctionNode* > ();
        break;

      case symbol_kind::S_If_Stm: // If_Stm
        yylhs.value.emplace< AST::IfNode* > ();
        break;

      case symbol_kind::S_Print: // Print
        yylhs.value.emplace< AST::PrintNode* > ();
        break;

      case symbol_kind::S_Scope: // Scope
        yylhs.value.emplace< AST::ScopeNode* > ();
        break;

      case symbol_kind::S_Statement: // Statement
        yylhs.value.emplace< AST::StatementNode* > ();
        break;

      case symbol_kind::S_UnaryOp: // UnaryOp
        yylhs.value.emplace< AST::UnaryOpNode* > ();
        break;

      case symbol_kind::S_Variable: // Variable
        yylhs.value.emplace< AST::VariableNode* > ();
        break;

      case symbol_kind::S_While_Stm: // While_Stm
        yylhs.value.emplace< AST::WhileNode* > ();
        break;

      case symbol_kind::S_NUMBER: // "number"
        yylhs.value.emplace< int > ();
        break;

      case symbol_kind::S_ID: // "identifier"
      case symbol_kind::S_NameOpt: // NameOpt
        yylhs.value.emplace< std::string > ();
        break;

      case symbol_kind::S_ArgListOpt: // ArgListOpt
      case symbol_kind::S_ArgList: // ArgList
        yylhs.value.emplace< std::vector<AST::ExpressionNode*> > ();
        break;

      case symbol_kind::S_ParamListOpt: // ParamListOpt
      case symbol_kind::S_ParamList: // ParamList
        yylhs.value.emplace< std::vector<std::string> > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // Program: $end
#line 138 "parser.y"
                        {
				drv.ast.globalScope =
					drv.bld.create<AST::ScopeNode>(std::vector<AST::StatementNode*>{});
			}
#line 1028 "parser.tab.c"
    break;

  case 3: // Program: Statements $end
#line 143 "parser.y"
                        {
				drv.build_global_scope();
			}
#line 1036 "parser.tab.c"
    break;

  case 4: // Statements: Statement
#line 148 "parser.y"
                        {
				LOG("Pushing statement : {}\n",
					static_cast<const void*>(yystack_[0].value.as < AST::StatementNode* > ()));

				drv.push_statement(yystack_[0].value.as < AST::StatementNode* > ());
			}
#line 1047 "parser.tab.c"
    break;

  case 5: // Statements: Statements Statement
#line 155 "parser.y"
                        {
				LOG("Pushing statement : {}\n",
					static_cast<const void*>(yystack_[0].value.as < AST::StatementNode* > ()));

				drv.push_statement(yystack_[0].value.as < AST::StatementNode* > ());
			}
#line 1058 "parser.tab.c"
    break;

  case 6: // Statement: ";"
#line 164 "parser.y"
                        {
				MSG("Void statement\n");
				yylhs.value.as < AST::StatementNode* > () = drv.bld.create<AST::VoidNode>();
			}
#line 1067 "parser.tab.c"
    break;

  case 7: // Statement: Expr ";"
#line 169 "parser.y"
                        {
				LOG("It's Expr. Moving from concrete rule: {}\n",
					static_cast<const void*>(yylhs.value.as < AST::StatementNode* > ()));

				yylhs.value.as < AST::StatementNode* > () = yystack_[1].value.as < AST::ExpressionNode* > ();
			}
#line 1078 "parser.tab.c"
    break;

  case 8: // Statement: "return" Expr ";"
#line 176 "parser.y"
            {
                MSG("Return with value\n");
                yylhs.value.as < AST::StatementNode* > () = drv.bld.create<AST::ReturnNode>(yystack_[1].value.as < AST::ExpressionNode* > ());
            }
#line 1087 "parser.tab.c"
    break;

  case 9: // Statement: "return" ";"
#line 181 "parser.y"
            {
                MSG("Return without value\n");
                yylhs.value.as < AST::StatementNode* > () = drv.bld.create<AST::ReturnNode>(nullptr);
            }
#line 1096 "parser.tab.c"
    break;

  case 10: // Statement: Scope
#line 186 "parser.y"
                        {
				LOG("It's Scope. Moving from concrete rule: {}\n",
					static_cast<const void*>(yylhs.value.as < AST::StatementNode* > ()));

				yylhs.value.as < AST::StatementNode* > () = yystack_[0].value.as < AST::ScopeNode* > ();
			}
#line 1107 "parser.tab.c"
    break;

  case 11: // Statement: If_Stm
#line 193 "parser.y"
                        {
				LOG("It's If_Stm. Moving from concrete rule: {}\n",
					static_cast<const void*>(yylhs.value.as < AST::StatementNode* > ()));

				yylhs.value.as < AST::StatementNode* > () = yystack_[0].value.as < AST::IfNode* > ();
			}
#line 1118 "parser.tab.c"
    break;

  case 12: // Statement: While_Stm
#line 200 "parser.y"
                        {
				LOG("It's While_Stm. Moving from concrete rule: {}\n",
					static_cast<const void*>(yylhs.value.as < AST::StatementNode* > ()));

				yylhs.value.as < AST::StatementNode* > () = yystack_[0].value.as < AST::WhileNode* > ();
			}
#line 1129 "parser.tab.c"
    break;

  case 13: // Statement: For_Stm
#line 207 "parser.y"
            {
                LOG("It's For_Stm. Moving from concrete rule: {}\n",
                    static_cast<const void*>(yylhs.value.as < AST::StatementNode* > ()));

                yylhs.value.as < AST::StatementNode* > () = yystack_[0].value.as < AST::ForNode* > ();
            }
#line 1140 "parser.tab.c"
    break;

  case 14: // Statement: Print ";"
#line 214 "parser.y"
                        {
				LOG("It's Print. Moving from concrete rule: {}\n",
					static_cast<const void*>(yylhs.value.as < AST::StatementNode* > ()));

				yylhs.value.as < AST::StatementNode* > () = yystack_[1].value.as < AST::PrintNode* > ();
			}
#line 1151 "parser.tab.c"
    break;

  case 15: // Scope: StartScope Statements EndScope
#line 222 "parser.y"
                {
			MSG("Initialising scope with vector of statements:\n");
			yylhs.value.as < AST::ScopeNode* > () = drv.exit_scope_node();
		}
#line 1160 "parser.tab.c"
    break;

  case 16: // Scope: StartScope EndScope
#line 227 "parser.y"
                {
            MSG("Initialising empty scope\n");
			yylhs.value.as < AST::ScopeNode* > () = drv.exit_scope_node();
		}
#line 1169 "parser.tab.c"
    break;

  case 17: // StartScope: "{"
#line 233 "parser.y"
                        {
				MSG("Scope start.\n");
				drv.enter_scope();
			}
#line 1178 "parser.tab.c"
    break;

  case 18: // EndScope: "}"
#line 239 "parser.y"
                        {

			}
#line 1186 "parser.tab.c"
    break;

  case 19: // If_Stm: "if" "(" Expr ")" Statement
#line 244 "parser.y"
                        {
				MSG("Initialising if statement\n");
				yylhs.value.as < AST::IfNode* > () = drv.bld.create<AST::IfNode>(yystack_[2].value.as < AST::ExpressionNode* > (), yystack_[0].value.as < AST::StatementNode* > ());
			}
#line 1195 "parser.tab.c"
    break;

  case 20: // If_Stm: "if" "(" Expr ")" Statement "else" Statement
#line 249 "parser.y"
                        {
				yylhs.value.as < AST::IfNode* > () = drv.bld.create<AST::IfNode>(
					yystack_[4].value.as < AST::ExpressionNode* > (),
					yystack_[2].value.as < AST::StatementNode* > (),
					yystack_[0].value.as < AST::StatementNode* > ()
				);
			}
#line 1207 "parser.tab.c"
    break;

  case 21: // For_Stm: "for" "(" Assign ";" Expr ";" Assign ")" Statement
#line 258 "parser.y"
            {
                MSG("Initialising for statement\n");
                yylhs.value.as < AST::ForNode* > () = drv.bld.create<AST::ForNode>(
                        yystack_[6].value.as < AST::AssignNode* > (),  // init  (AssignNode)
                        yystack_[4].value.as < AST::ExpressionNode* > (),  // cond  (Expr)
                        yystack_[2].value.as < AST::AssignNode* > (),  // iter  (AssignNode)
                        yystack_[0].value.as < AST::StatementNode* > ()   // body  (Statement)
                );
            }
#line 1221 "parser.tab.c"
    break;

  case 22: // While_Stm: "while" "(" Expr ")" Statement
#line 269 "parser.y"
                        {
				MSG("Initialising while statement\n");
				yylhs.value.as < AST::WhileNode* > () = drv.bld.create<AST::WhileNode>(yystack_[2].value.as < AST::ExpressionNode* > (), yystack_[0].value.as < AST::StatementNode* > ());
			}
#line 1230 "parser.tab.c"
    break;

  case 23: // Assign: Variable "=" Expr
#line 275 "parser.y"
                {
			yylhs.value.as < AST::AssignNode* > () = drv.bld.create<AST::AssignNode>(yystack_[2].value.as < AST::VariableNode* > (), yystack_[0].value.as < AST::ExpressionNode* > ());
			LOG("Initialising assignment: {}\n", static_cast<const void*>(yylhs.value.as < AST::AssignNode* > ()));
		}
#line 1239 "parser.tab.c"
    break;

  case 24: // Assign: Variable "+=" Expr
#line 280 "parser.y"
        {
            auto oldX = drv.bld.create<AST::VariableNode>(drv.bld.intern(yystack_[2].value.as < AST::VariableNode* > ()->get_name()));
            auto val  = drv.bld.create<AST::BinaryOpNode>(oldX, AST::BinaryOp::ADD, yystack_[0].value.as < AST::ExpressionNode* > ());
            yylhs.value.as < AST::AssignNode* > () = drv.bld.create<AST::AssignNode>(yystack_[2].value.as < AST::VariableNode* > (), val);
        }
#line 1249 "parser.tab.c"
    break;

  case 25: // Assign: Variable "-=" Expr
#line 286 "parser.y"
        {
            auto oldX = drv.bld.create<AST::VariableNode>(drv.bld.intern(yystack_[2].value.as < AST::VariableNode* > ()->get_name()));
            auto val  = drv.bld.create<AST::BinaryOpNode>(oldX, AST::BinaryOp::SUB, yystack_[0].value.as < AST::ExpressionNode* > ());
            yylhs.value.as < AST::AssignNode* > () = drv.bld.create<AST::AssignNode>(yystack_[2].value.as < AST::VariableNode* > (), val);
        }
#line 1259 "parser.tab.c"
    break;

  case 26: // Assign: Variable "*=" Expr
#line 292 "parser.y"
        {
            auto oldX = drv.bld.create<AST::VariableNode>(drv.bld.intern(yystack_[2].value.as < AST::VariableNode* > ()->get_name()));
            auto val  = drv.bld.create<AST::BinaryOpNode>(oldX, AST::BinaryOp::MUL, yystack_[0].value.as < AST::ExpressionNode* > ());
            yylhs.value.as < AST::AssignNode* > () = drv.bld.create<AST::AssignNode>(yystack_[2].value.as < AST::VariableNode* > (), val);
        }
#line 1269 "parser.tab.c"
    break;

  case 27: // Assign: Variable "/=" Expr
#line 298 "parser.y"
        {
            auto oldX = drv.bld.create<AST::VariableNode>(drv.bld.intern(yystack_[2].value.as < AST::VariableNode* > ()->get_name()));
            auto val  = drv.bld.create<AST::BinaryOpNode>(oldX, AST::BinaryOp::DIV, yystack_[0].value.as < AST::ExpressionNode* > ());
            yylhs.value.as < AST::AssignNode* > () = drv.bld.create<AST::AssignNode>(yystack_[2].value.as < AST::VariableNode* > (), val);
        }
#line 1279 "parser.tab.c"
    break;

  case 28: // Print: "print" Expr
#line 305 "parser.y"
                {
			MSG("Initialising print\n");
			yylhs.value.as < AST::PrintNode* > () = drv.bld.create<AST::PrintNode>(yystack_[0].value.as < AST::ExpressionNode* > ());
		}
#line 1288 "parser.tab.c"
    break;

  case 29: // Expr: Postfix
#line 312 "parser.y"
        {
			yylhs.value.as < AST::ExpressionNode* > () = yystack_[0].value.as < AST::ExpressionNode* > ();
		}
#line 1296 "parser.tab.c"
    break;

  case 30: // Expr: BinaryOp
#line 316 "parser.y"
                {
			MSG("Moving BinaryOp\n");
			yylhs.value.as < AST::ExpressionNode* > () = yystack_[0].value.as < AST::BinaryOpNode* > ();
		}
#line 1305 "parser.tab.c"
    break;

  case 31: // Expr: UnaryOp
#line 321 "parser.y"
                {
			MSG("Moving UnaryOp\n");
			yylhs.value.as < AST::ExpressionNode* > () = yystack_[0].value.as < AST::UnaryOpNode* > ();
		}
#line 1314 "parser.tab.c"
    break;

  case 32: // Expr: Assign
#line 325 "parser.y"
                 { yylhs.value.as < AST::ExpressionNode* > () = yystack_[0].value.as < AST::AssignNode* > (); }
#line 1320 "parser.tab.c"
    break;

  case 33: // BinaryOp: Expr "+" Expr
#line 328 "parser.y"
                        {
				MSG("Initialising ADD operation\n");
				yylhs.value.as < AST::BinaryOpNode* > () = drv.bld.create<AST::BinaryOpNode>(	yystack_[2].value.as < AST::ExpressionNode* > (),AST::BinaryOp::ADD, yystack_[0].value.as < AST::ExpressionNode* > ());
			}
#line 1329 "parser.tab.c"
    break;

  case 34: // BinaryOp: Expr "-" Expr
#line 333 "parser.y"
                        {
				MSG("Initialising SUB operation\n");
				yylhs.value.as < AST::BinaryOpNode* > () = drv.bld.create<AST::BinaryOpNode>(	yystack_[2].value.as < AST::ExpressionNode* > (),
														AST::BinaryOp::SUB,
														yystack_[0].value.as < AST::ExpressionNode* > ());
			}
#line 1340 "parser.tab.c"
    break;

  case 35: // BinaryOp: Expr "*" Expr
#line 340 "parser.y"
                        {
				MSG("Initialising MUL operation\n");
				yylhs.value.as < AST::BinaryOpNode* > () = drv.bld.create<AST::BinaryOpNode>(	yystack_[2].value.as < AST::ExpressionNode* > (),
														AST::BinaryOp::MUL,
														yystack_[0].value.as < AST::ExpressionNode* > ());
			}
#line 1351 "parser.tab.c"
    break;

  case 36: // BinaryOp: Expr "/" Expr
#line 347 "parser.y"
                        {
				MSG("Initialising DIV operation\n");
				yylhs.value.as < AST::BinaryOpNode* > () = drv.bld.create<AST::BinaryOpNode>(	yystack_[2].value.as < AST::ExpressionNode* > (),
														AST::BinaryOp::DIV,
													    yystack_[0].value.as < AST::ExpressionNode* > ());
			}
#line 1362 "parser.tab.c"
    break;

  case 37: // BinaryOp: Expr ">" Expr
#line 354 "parser.y"
                        {
				MSG("Initialising GR operation\n");
				yylhs.value.as < AST::BinaryOpNode* > () = drv.bld.create<AST::BinaryOpNode>( yystack_[2].value.as < AST::ExpressionNode* > (),
														AST::BinaryOp::GR,
														yystack_[0].value.as < AST::ExpressionNode* > ());
			}
#line 1373 "parser.tab.c"
    break;

  case 38: // BinaryOp: Expr "<" Expr
#line 361 "parser.y"
                        {
				MSG("Initialising LS operation\n");
				yylhs.value.as < AST::BinaryOpNode* > () = drv.bld.create<AST::BinaryOpNode>(	yystack_[2].value.as < AST::ExpressionNode* > (),
														AST::BinaryOp::LS,
														yystack_[0].value.as < AST::ExpressionNode* > ());
			}
#line 1384 "parser.tab.c"
    break;

  case 39: // BinaryOp: Expr ">=" Expr
#line 368 "parser.y"
                        {
				MSG("Initialising RG_EQ operation\n");
				yylhs.value.as < AST::BinaryOpNode* > () = drv.bld.create<AST::BinaryOpNode>(	yystack_[2].value.as < AST::ExpressionNode* > (),
														AST::BinaryOp::GR_EQ,
														yystack_[0].value.as < AST::ExpressionNode* > ());
			}
#line 1395 "parser.tab.c"
    break;

  case 40: // BinaryOp: Expr "<=" Expr
#line 375 "parser.y"
                        {
				MSG("Initialising LS_EQ operation\n");
				yylhs.value.as < AST::BinaryOpNode* > () = drv.bld.create<AST::BinaryOpNode>(	yystack_[2].value.as < AST::ExpressionNode* > (),
														AST::BinaryOp::LS_EQ,
													    yystack_[0].value.as < AST::ExpressionNode* > ());
			}
#line 1406 "parser.tab.c"
    break;

  case 41: // BinaryOp: Expr "==" Expr
#line 382 "parser.y"
                        {
				MSG("Initialising EQ operation\n");
				yylhs.value.as < AST::BinaryOpNode* > () = drv.bld.create<AST::BinaryOpNode>(	yystack_[2].value.as < AST::ExpressionNode* > (),
														AST::BinaryOp::EQ,
														yystack_[0].value.as < AST::ExpressionNode* > ());
			}
#line 1417 "parser.tab.c"
    break;

  case 42: // BinaryOp: Expr "!=" Expr
#line 389 "parser.y"
                        {
				MSG("Initialising NOT_EQ operation\n");
				yylhs.value.as < AST::BinaryOpNode* > () = drv.bld.create<AST::BinaryOpNode>(	yystack_[2].value.as < AST::ExpressionNode* > (),
														AST::BinaryOp::NOT_EQ,
														yystack_[0].value.as < AST::ExpressionNode* > ());
			}
#line 1428 "parser.tab.c"
    break;

  case 43: // BinaryOp: Expr "&&" Expr
#line 396 "parser.y"
                        {
				MSG("Initialising AND operation\n");
				yylhs.value.as < AST::BinaryOpNode* > () = drv.bld.create<AST::BinaryOpNode>(	yystack_[2].value.as < AST::ExpressionNode* > (),
														AST::BinaryOp::AND,
													    yystack_[0].value.as < AST::ExpressionNode* > ());
			}
#line 1439 "parser.tab.c"
    break;

  case 44: // BinaryOp: Expr "||" Expr
#line 403 "parser.y"
                        {
				MSG("Initialising OR operation\n");
				yylhs.value.as < AST::BinaryOpNode* > () = drv.bld.create<AST::BinaryOpNode>(	yystack_[2].value.as < AST::ExpressionNode* > (),
														AST::BinaryOp::OR,
														yystack_[0].value.as < AST::ExpressionNode* > ());
			}
#line 1450 "parser.tab.c"
    break;

  case 45: // BinaryOp: Expr "&" Expr
#line 410 "parser.y"
            {
                MSG("Initialising BIT_AND operation\n");
                yylhs.value.as < AST::BinaryOpNode* > () = drv.bld.create<AST::BinaryOpNode>( yystack_[2].value.as < AST::ExpressionNode* > (),
                                                        AST::BinaryOp::BIT_AND,
                                                        yystack_[0].value.as < AST::ExpressionNode* > ());
            }
#line 1461 "parser.tab.c"
    break;

  case 46: // BinaryOp: Expr "|" Expr
#line 417 "parser.y"
            {
                MSG("Initialising BIT_OR operation\n");
                yylhs.value.as < AST::BinaryOpNode* > () = drv.bld.create<AST::BinaryOpNode>( yystack_[2].value.as < AST::ExpressionNode* > (),
                                                        AST::BinaryOp::BIT_OR,
                                                        yystack_[0].value.as < AST::ExpressionNode* > ());
            }
#line 1472 "parser.tab.c"
    break;

  case 47: // BinaryOp: Expr "%" Expr
#line 424 "parser.y"
                        {
				MSG("Initialising MOD operation\n");
				yylhs.value.as < AST::BinaryOpNode* > () = drv.bld.create<AST::BinaryOpNode>( yystack_[2].value.as < AST::ExpressionNode* > (),
														AST::BinaryOp::MOD,
														yystack_[0].value.as < AST::ExpressionNode* > ());
			}
#line 1483 "parser.tab.c"
    break;

  case 48: // UnaryOp: "-" Expr
#line 433 "parser.y"
                        {
				MSG("Initialising NEG operation\n");
				yylhs.value.as < AST::UnaryOpNode* > () = drv.bld.create<AST::UnaryOpNode>(yystack_[0].value.as < AST::ExpressionNode* > (), AST::UnaryOp::NEG);
			}
#line 1492 "parser.tab.c"
    break;

  case 49: // UnaryOp: "!" Expr
#line 438 "parser.y"
                        {
				MSG("Initialising NOT operation\n");
				yylhs.value.as < AST::UnaryOpNode* > () = drv.bld.create<AST::UnaryOpNode>(yystack_[0].value.as < AST::ExpressionNode* > (), AST::UnaryOp::NOT);
			}
#line 1501 "parser.tab.c"
    break;

  case 50: // Variable: "identifier"
#line 444 "parser.y"
                        {
				MSG("Initialising VariableNode\n");
				yylhs.value.as < AST::VariableNode* > () = drv.bld.create<AST::VariableNode>(drv.bld.intern(yystack_[0].value.as < std::string > ()));
			}
#line 1510 "parser.tab.c"
    break;

  case 51: // ArgListOpt: %empty
#line 451 "parser.y"
                        {
				yylhs.value.as < std::vector<AST::ExpressionNode*> > () = {};
			}
#line 1518 "parser.tab.c"
    break;

  case 52: // ArgListOpt: ArgList
#line 455 "parser.y"
                        {
				yylhs.value.as < std::vector<AST::ExpressionNode*> > () = std::move(yystack_[0].value.as < std::vector<AST::ExpressionNode*> > ());
			}
#line 1526 "parser.tab.c"
    break;

  case 53: // ArgList: Expr
#line 460 "parser.y"
                        {
				yylhs.value.as < std::vector<AST::ExpressionNode*> > () = std::vector<AST::ExpressionNode*>{ yystack_[0].value.as < AST::ExpressionNode* > () };
			}
#line 1534 "parser.tab.c"
    break;

  case 54: // ArgList: ArgList "," Expr
#line 464 "parser.y"
                        {
				yystack_[2].value.as < std::vector<AST::ExpressionNode*> > ().push_back(yystack_[0].value.as < AST::ExpressionNode* > ());
				yylhs.value.as < std::vector<AST::ExpressionNode*> > () = std::move(yystack_[2].value.as < std::vector<AST::ExpressionNode*> > ());
			}
#line 1543 "parser.tab.c"
    break;

  case 55: // ParamListOpt: %empty
#line 471 "parser.y"
                        {
				yylhs.value.as < std::vector<std::string> > () = {};
			}
#line 1551 "parser.tab.c"
    break;

  case 56: // ParamListOpt: ParamList
#line 475 "parser.y"
                        {
				yylhs.value.as < std::vector<std::string> > () = std::move(yystack_[0].value.as < std::vector<std::string> > ());
			}
#line 1559 "parser.tab.c"
    break;

  case 57: // ParamList: "identifier"
#line 480 "parser.y"
                        {
				yylhs.value.as < std::vector<std::string> > () = std::vector<std::string>{ yystack_[0].value.as < std::string > () };
			}
#line 1567 "parser.tab.c"
    break;

  case 58: // ParamList: ParamList "," "identifier"
#line 484 "parser.y"
                        {
				yystack_[2].value.as < std::vector<std::string> > ().push_back(yystack_[0].value.as < std::string > ());
				yylhs.value.as < std::vector<std::string> > () = std::move(yystack_[2].value.as < std::vector<std::string> > ());
			}
#line 1576 "parser.tab.c"
    break;

  case 59: // NameOpt: %empty
#line 491 "parser.y"
                    {
				yylhs.value.as < std::string > () = "";
			}
#line 1584 "parser.tab.c"
    break;

  case 60: // NameOpt: ":" "identifier"
#line 495 "parser.y"
                        {
				yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > ();
			}
#line 1592 "parser.tab.c"
    break;

  case 61: // FunctionLit: "func" "(" ParamListOpt ")" NameOpt Scope
#line 501 "parser.y"
                        {
			 	std::vector<std::string_view> params;
				params.reserve(yystack_[3].value.as < std::vector<std::string> > ().size());
				for (auto& s : yystack_[3].value.as < std::vector<std::string> > ())
					params.push_back(drv.bld.intern(s));

				bool hasName = !yystack_[1].value.as < std::string > ().empty();
				std::string_view fname = hasName ? drv.bld.intern(yystack_[1].value.as < std::string > ()) : std::string_view{};

				yylhs.value.as < AST::FunctionNode* > () = drv.bld.create<AST::FunctionNode>(std::move(params), yystack_[0].value.as < AST::ScopeNode* > (), fname, hasName);
            }
#line 1608 "parser.tab.c"
    break;

  case 62: // Primary: "number"
#line 515 "parser.y"
            {
				MSG("Initialising ConstantNode\n");
				yylhs.value.as < AST::ExpressionNode* > () = drv.bld.create<AST::ConstantNode>(yystack_[0].value.as < int > ());
           }
#line 1617 "parser.tab.c"
    break;

  case 63: // Primary: "?"
#line 520 "parser.y"
            {
				MSG("Initialising InNode\n");
				yylhs.value.as < AST::ExpressionNode* > () = drv.bld.create<AST::InNode>();
            }
#line 1626 "parser.tab.c"
    break;

  case 64: // Primary: Variable
#line 525 "parser.y"
                {
            	MSG("Moving VariableNode\n");
            	yylhs.value.as < AST::ExpressionNode* > () = yystack_[0].value.as < AST::VariableNode* > ();
        	}
#line 1635 "parser.tab.c"
    break;

  case 65: // Primary: "(" Expr ")"
#line 530 "parser.y"
                        {
				MSG("Moving Expression in parenthesis\n");
				yylhs.value.as < AST::ExpressionNode* > () = yystack_[1].value.as < AST::ExpressionNode* > ();
			}
#line 1644 "parser.tab.c"
    break;

  case 66: // Primary: "(" Scope ")"
#line 535 "parser.y"
                        {
				MSG("Moving Scope as expression\n");
				yylhs.value.as < AST::ExpressionNode* > () = yystack_[1].value.as < AST::ScopeNode* > ();
			}
#line 1653 "parser.tab.c"
    break;

  case 67: // Primary: FunctionLit
#line 540 "parser.y"
                        {
				MSG("Moving Function literal\n");
				yylhs.value.as < AST::ExpressionNode* > () = yystack_[0].value.as < AST::FunctionNode* > ();
			}
#line 1662 "parser.tab.c"
    break;

  case 68: // Postfix: Primary
#line 547 "parser.y"
            {
				yylhs.value.as < AST::ExpressionNode* > () = yystack_[0].value.as < AST::ExpressionNode* > ();
			}
#line 1670 "parser.tab.c"
    break;

  case 69: // Postfix: Postfix "(" ArgListOpt ")"
#line 551 "parser.y"
                        {
				MSG("Initialising CallNode\n");
				yylhs.value.as < AST::ExpressionNode* > () = drv.bld.create<AST::CallNode>(yystack_[3].value.as < AST::ExpressionNode* > (), std::move(yystack_[1].value.as < std::vector<AST::ExpressionNode*> > ()));
			}
#line 1679 "parser.tab.c"
    break;


#line 1683 "parser.tab.c"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      yy_lac_discard_ ("error recovery");
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  const char *
  parser::symbol_name (symbol_kind_type yysymbol)
  {
    static const char *const yy_sname[] =
    {
    "end of file", "error", "invalid token", "=", "+=", "-=", "*=", "/=",
  "-", "+", "*", "/", "(", ")", "?", "print", "{", "}", ";", "if", "else",
  "while", "for", ">", "<", ">=", "<=", "==", "!=", "!", "%", "&&", "||",
  "&", "|", "func", "return", ":", ",", "identifier", "number", "IFX",
  "%=", "UMINUS", "$accept", "Program", "Statements", "Statement", "Scope",
  "StartScope", "EndScope", "If_Stm", "For_Stm", "While_Stm", "Assign",
  "Print", "Expr", "BinaryOp", "UnaryOp", "Variable", "ArgListOpt",
  "ArgList", "ParamListOpt", "ParamList", "NameOpt", "FunctionLit",
  "Primary", "Postfix", YY_NULLPTR
    };
    return yy_sname[yysymbol];
  }



  // parser::context.
  parser::context::context (const parser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  parser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

#if YYDEBUG
    // Execute LAC once. We don't care if it is successful, we
    // only do it for the sake of debugging output.
    if (!yyparser_.yy_lac_established_)
      yyparser_.yy_lac_check_ (yyla_.kind ());
#endif

    for (int yyx = 0; yyx < YYNTOKENS; ++yyx)
      {
        symbol_kind_type yysym = YY_CAST (symbol_kind_type, yyx);
        if (yysym != symbol_kind::S_YYerror
            && yysym != symbol_kind::S_YYUNDEF
            && yyparser_.yy_lac_check_ (yysym))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = yysym;
          }
      }
    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }




  bool
  parser::yy_lac_check_ (symbol_kind_type yytoken) const
  {
    // Logically, the yylac_stack's lifetime is confined to this function.
    // Clear it, to get rid of potential left-overs from previous call.
    yylac_stack_.clear ();
    // Reduce until we encounter a shift and thereby accept the token.
#if YYDEBUG
    YYCDEBUG << "LAC: checking lookahead " << symbol_name (yytoken) << ':';
#endif
    std::ptrdiff_t lac_top = 0;
    while (true)
      {
        state_type top_state = (yylac_stack_.empty ()
                                ? yystack_[lac_top].state
                                : yylac_stack_.back ());
        int yyrule = yypact_[+top_state];
        if (yy_pact_value_is_default_ (yyrule)
            || (yyrule += yytoken) < 0 || yylast_ < yyrule
            || yycheck_[yyrule] != yytoken)
          {
            // Use the default action.
            yyrule = yydefact_[+top_state];
            if (yyrule == 0)
              {
                YYCDEBUG << " Err\n";
                return false;
              }
          }
        else
          {
            // Use the action from yytable.
            yyrule = yytable_[yyrule];
            if (yy_table_value_is_error_ (yyrule))
              {
                YYCDEBUG << " Err\n";
                return false;
              }
            if (0 < yyrule)
              {
                YYCDEBUG << " S" << yyrule << '\n';
                return true;
              }
            yyrule = -yyrule;
          }
        // By now we know we have to simulate a reduce.
        YYCDEBUG << " R" << yyrule - 1;
        // Pop the corresponding number of values from the stack.
        {
          std::ptrdiff_t yylen = yyr2_[yyrule];
          // First pop from the LAC stack as many tokens as possible.
          std::ptrdiff_t lac_size = std::ptrdiff_t (yylac_stack_.size ());
          if (yylen < lac_size)
            {
              yylac_stack_.resize (std::size_t (lac_size - yylen));
              yylen = 0;
            }
          else if (lac_size)
            {
              yylac_stack_.clear ();
              yylen -= lac_size;
            }
          // Only afterwards look at the main stack.
          // We simulate popping elements by incrementing lac_top.
          lac_top += yylen;
        }
        // Keep top_state in sync with the updated stack.
        top_state = (yylac_stack_.empty ()
                     ? yystack_[lac_top].state
                     : yylac_stack_.back ());
        // Push the resulting state of the reduction.
        state_type state = yy_lr_goto_state_ (top_state, yyr1_[yyrule]);
        YYCDEBUG << " G" << int (state);
        yylac_stack_.push_back (state);
      }
  }

  // Establish the initial context if no initial context currently exists.
  bool
  parser::yy_lac_establish_ (symbol_kind_type yytoken)
  {
    /* Establish the initial context for the current lookahead if no initial
       context is currently established.

       We define a context as a snapshot of the parser stacks.  We define
       the initial context for a lookahead as the context in which the
       parser initially examines that lookahead in order to select a
       syntactic action.  Thus, if the lookahead eventually proves
       syntactically unacceptable (possibly in a later context reached via a
       series of reductions), the initial context can be used to determine
       the exact set of tokens that would be syntactically acceptable in the
       lookahead's place.  Moreover, it is the context after which any
       further semantic actions would be erroneous because they would be
       determined by a syntactically unacceptable token.

       yy_lac_establish_ should be invoked when a reduction is about to be
       performed in an inconsistent state (which, for the purposes of LAC,
       includes consistent states that don't know they're consistent because
       their default reductions have been disabled).

       For parse.lac=full, the implementation of yy_lac_establish_ is as
       follows.  If no initial context is currently established for the
       current lookahead, then check if that lookahead can eventually be
       shifted if syntactic actions continue from the current context.  */
    if (yy_lac_established_)
      return true;
    else
      {
#if YYDEBUG
        YYCDEBUG << "LAC: initial context established for "
                 << symbol_name (yytoken) << '\n';
#endif
        yy_lac_established_ = true;
        return yy_lac_check_ (yytoken);
      }
  }

  // Discard any previous initial lookahead context.
  void
  parser::yy_lac_discard_ (const char* event)
  {
   /* Discard any previous initial lookahead context because of Event,
      which may be a lookahead change or an invalidation of the currently
      established initial context for the current lookahead.

      The most common example of a lookahead change is a shift.  An example
      of both cases is syntax error recovery.  That is, a syntax error
      occurs when the lookahead is syntactically erroneous for the
      currently established initial context, so error recovery manipulates
      the parser stacks to try to find a new initial context in which the
      current lookahead is syntactically acceptable.  If it fails to find
      such a context, it discards the lookahead.  */
    if (yy_lac_established_)
      {
        YYCDEBUG << "LAC: initial context discarded due to "
                 << event << '\n';
        yy_lac_established_ = false;
      }
  }


  int
  parser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
         In the first two cases, it might appear that the current syntax
         error should have been detected in the previous state when
         yy_lac_check was invoked.  However, at that time, there might
         have been a different syntax error that discarded a different
         initial context during error recovery, leaving behind the
         current lookahead.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char parser::yypact_ninf_ = -35;

  const signed char parser::yytable_ninf_ = -1;

  const short
  parser::yypact_[] =
  {
       9,   -35,   179,    66,   -35,   179,   -35,   -35,    -5,    -4,
       4,   179,     6,   172,   -35,   -35,    11,   102,   -35,   -35,
     131,   -35,   -35,   -35,   -35,     1,   212,   -35,   -35,    36,
     -35,   -35,     8,   -35,     2,   239,   374,   179,   179,   -17,
     -35,   -13,   -35,   266,   -35,   -35,   -35,   -35,   131,   -35,
     -35,   179,   179,   179,   179,   -35,   179,   179,   179,   179,
     179,   179,   179,   179,   179,   179,   179,   179,   179,   179,
     179,   179,   179,   -35,   -35,   293,   320,    14,    36,   -35,
      16,    34,   -35,   -35,     3,     3,   -35,   -35,   497,   497,
     497,   497,   466,   466,   -35,   428,   401,   489,   455,   374,
     374,   374,   374,   374,   374,    22,    35,   143,   143,   179,
      10,     7,   -35,   179,    55,   -35,   347,    15,    60,   -35,
     374,   143,   -17,   -35,   -35,   -35,    64,   143,   -35
  };

  const signed char
  parser::yydefact_[] =
  {
       0,     2,     0,     0,    63,     0,    17,     6,     0,     0,
       0,     0,     0,     0,    50,    62,     0,     0,     4,    10,
       0,    11,    13,    12,    32,     0,     0,    30,    31,    64,
      67,    68,    29,    48,     0,     0,    28,     0,     0,     0,
      49,    55,     9,     0,     1,     3,     5,    18,     0,    16,
      14,     0,     0,     0,     0,     7,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    51,    66,    65,     0,     0,     0,     0,    57,
       0,    56,     8,    15,    34,    33,    35,    36,    37,    38,
      39,    40,    41,    42,    47,    43,    44,    45,    46,    23,
      24,    25,    26,    27,    53,     0,    52,     0,     0,     0,
      59,     0,    69,     0,    19,    22,     0,     0,     0,    58,
      54,     0,     0,    60,    61,    20,     0,     0,    21
  };

  const signed char
  parser::yypgoto_[] =
  {
     -35,   -35,    59,   -14,    -3,   -35,    33,   -35,   -35,   -35,
     -34,   -35,    -1,   -35,   -35,   -33,   -35,   -35,   -35,   -35,
     -35,   -35,   -35,   -35
  };

  const signed char
  parser::yydefgoto_[] =
  {
       0,    16,    17,    18,    19,    20,    49,    21,    22,    23,
      24,    25,    26,    27,    28,    29,   105,   106,    80,    81,
     118,    30,    31,    32
  };

  const short
  parser::yytable_[] =
  {
      34,    33,    35,    46,    36,    77,    78,    37,    38,     1,
      40,    44,    43,    53,    54,    73,    39,     2,    41,    50,
      72,     3,    14,     4,     5,     6,    79,     7,     8,   110,
       9,    10,   109,    62,    46,   112,    75,    76,    11,    67,
      68,    69,    70,    71,    12,    13,   119,   117,    14,    15,
      84,    85,    86,    87,   123,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   111,   113,     2,   121,     6,   127,     3,    48,
       4,    83,     6,     0,     0,     0,     0,     0,   126,    78,
       0,     0,     0,   114,   115,    11,     0,     0,     0,     0,
       0,    12,    45,     0,     0,    14,    15,   125,   116,     0,
       2,     0,   120,   128,     3,   124,     4,     5,     6,     0,
       7,     8,     0,     9,    10,     0,     0,     0,     0,     0,
       0,    11,     0,     0,     0,     0,     0,    12,    13,     2,
       0,    14,    15,     3,     0,     4,     5,     6,    47,     7,
       8,     2,     9,    10,     0,     3,     0,     4,     5,     6,
      11,     7,     8,     0,     9,    10,    12,    13,     0,     0,
      14,    15,    11,     0,     0,     0,     0,     0,    12,    13,
       2,     0,    14,    15,     3,     0,     4,     2,     0,     0,
      42,     3,     0,     4,     0,     0,     0,     0,     0,     0,
       0,    11,     0,     0,     0,     0,     0,    12,    11,     0,
       0,    14,    15,     0,    12,     0,     0,     0,    14,    15,
      51,    52,    53,    54,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,    56,    57,    58,    59,    60,
      61,     0,    62,    63,    64,    65,    66,    51,    52,    53,
      54,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    56,    57,    58,    59,    60,    61,     0,    62,
      63,    64,    65,    66,    51,    52,    53,    54,     0,     0,
       0,     0,     0,     0,    82,     0,     0,     0,     0,    56,
      57,    58,    59,    60,    61,     0,    62,    63,    64,    65,
      66,    51,    52,    53,    54,     0,   107,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    56,    57,    58,    59,
      60,    61,     0,    62,    63,    64,    65,    66,    51,    52,
      53,    54,     0,   108,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    56,    57,    58,    59,    60,    61,     0,
      62,    63,    64,    65,    66,    51,    52,    53,    54,     0,
       0,     0,     0,     0,     0,   122,     0,     0,     0,     0,
      56,    57,    58,    59,    60,    61,     0,    62,    63,    64,
      65,    66,    51,    52,    53,    54,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,    57,    58,
      59,    60,    61,     0,    62,    63,    64,    65,    66,    51,
      52,    53,    54,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,    57,    58,    59,    60,    61,
       0,    62,    63,     0,    65,    66,    51,    52,    53,    54,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    56,    57,    58,    59,    60,    61,     0,    62,     0,
       0,    65,    66,    51,    52,    53,    54,     0,     0,     0,
       0,     0,     0,     0,    51,    52,    53,    54,    56,    57,
      58,    59,    60,    61,     0,    62,     0,     0,    65,    56,
      57,    58,    59,    -1,    -1,     0,    62,    51,    52,    53,
      54,     0,     0,     0,     0,    51,    52,    53,    54,     0,
       0,     0,    56,    57,    58,    59,    60,    61,     0,    62,
      -1,    -1,    -1,    -1,     0,     0,     0,    62
  };

  const signed char
  parser::yycheck_[] =
  {
       3,     2,     3,    17,     5,    39,    39,    12,    12,     0,
      11,     0,    13,    10,    11,    13,    12,     8,    12,    18,
      12,    12,    39,    14,    15,    16,    39,    18,    19,    13,
      21,    22,    18,    30,    48,    13,    37,    38,    29,     3,
       4,     5,     6,     7,    35,    36,    39,    37,    39,    40,
      51,    52,    53,    54,    39,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    38,    38,     8,    20,    16,    13,    12,    20,
      14,    48,    16,    -1,    -1,    -1,    -1,    -1,   122,   122,
      -1,    -1,    -1,   107,   108,    29,    -1,    -1,    -1,    -1,
      -1,    35,     0,    -1,    -1,    39,    40,   121,   109,    -1,
       8,    -1,   113,   127,    12,   118,    14,    15,    16,    -1,
      18,    19,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    29,    -1,    -1,    -1,    -1,    -1,    35,    36,     8,
      -1,    39,    40,    12,    -1,    14,    15,    16,    17,    18,
      19,     8,    21,    22,    -1,    12,    -1,    14,    15,    16,
      29,    18,    19,    -1,    21,    22,    35,    36,    -1,    -1,
      39,    40,    29,    -1,    -1,    -1,    -1,    -1,    35,    36,
       8,    -1,    39,    40,    12,    -1,    14,     8,    -1,    -1,
      18,    12,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    29,    -1,    -1,    -1,    -1,    -1,    35,    29,    -1,
      -1,    39,    40,    -1,    35,    -1,    -1,    -1,    39,    40,
       8,     9,    10,    11,    -1,    -1,    -1,    -1,    -1,    -1,
      18,    -1,    -1,    -1,    -1,    23,    24,    25,    26,    27,
      28,    -1,    30,    31,    32,    33,    34,     8,     9,    10,
      11,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    24,    25,    26,    27,    28,    -1,    30,
      31,    32,    33,    34,     8,     9,    10,    11,    -1,    -1,
      -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,    23,
      24,    25,    26,    27,    28,    -1,    30,    31,    32,    33,
      34,     8,     9,    10,    11,    -1,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    24,    25,    26,
      27,    28,    -1,    30,    31,    32,    33,    34,     8,     9,
      10,    11,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    24,    25,    26,    27,    28,    -1,
      30,    31,    32,    33,    34,     8,     9,    10,    11,    -1,
      -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,
      23,    24,    25,    26,    27,    28,    -1,    30,    31,    32,
      33,    34,     8,     9,    10,    11,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    24,    25,
      26,    27,    28,    -1,    30,    31,    32,    33,    34,     8,
       9,    10,    11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    24,    25,    26,    27,    28,
      -1,    30,    31,    -1,    33,    34,     8,     9,    10,    11,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    24,    25,    26,    27,    28,    -1,    30,    -1,
      -1,    33,    34,     8,     9,    10,    11,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     8,     9,    10,    11,    23,    24,
      25,    26,    27,    28,    -1,    30,    -1,    -1,    33,    23,
      24,    25,    26,    27,    28,    -1,    30,     8,     9,    10,
      11,    -1,    -1,    -1,    -1,     8,     9,    10,    11,    -1,
      -1,    -1,    23,    24,    25,    26,    27,    28,    -1,    30,
      23,    24,    25,    26,    -1,    -1,    -1,    30
  };

  const signed char
  parser::yystos_[] =
  {
       0,     0,     8,    12,    14,    15,    16,    18,    19,    21,
      22,    29,    35,    36,    39,    40,    45,    46,    47,    48,
      49,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      65,    66,    67,    56,    48,    56,    56,    12,    12,    12,
      56,    12,    18,    56,     0,     0,    47,    17,    46,    50,
      18,     8,     9,    10,    11,    18,    23,    24,    25,    26,
      27,    28,    30,    31,    32,    33,    34,     3,     4,     5,
       6,     7,    12,    13,    13,    56,    56,    54,    59,    39,
      62,    63,    18,    50,    56,    56,    56,    56,    56,    56,
      56,    56,    56,    56,    56,    56,    56,    56,    56,    56,
      56,    56,    56,    56,    56,    60,    61,    13,    13,    18,
      13,    38,    13,    38,    47,    47,    56,    37,    64,    39,
      56,    20,    18,    39,    48,    47,    54,    13,    47
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    44,    45,    45,    46,    46,    47,    47,    47,    47,
      47,    47,    47,    47,    47,    48,    48,    49,    50,    51,
      51,    52,    53,    54,    54,    54,    54,    54,    55,    56,
      56,    56,    56,    57,    57,    57,    57,    57,    57,    57,
      57,    57,    57,    57,    57,    57,    57,    57,    58,    58,
      59,    60,    60,    61,    61,    62,    62,    63,    63,    64,
      64,    65,    66,    66,    66,    66,    66,    66,    67,    67
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     1,     2,     1,     2,     1,     2,     3,     2,
       1,     1,     1,     1,     2,     3,     2,     1,     1,     5,
       7,     9,     5,     3,     3,     3,     3,     3,     2,     1,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       1,     0,     1,     1,     3,     0,     1,     1,     3,     0,
       2,     6,     1,     1,     1,     3,     3,     1,     1,     4
  };




#if YYDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   137,   137,   142,   147,   154,   163,   168,   175,   180,
     185,   192,   199,   206,   213,   221,   226,   232,   238,   243,
     248,   257,   268,   274,   279,   285,   291,   297,   304,   311,
     315,   320,   325,   327,   332,   339,   346,   353,   360,   367,
     374,   381,   388,   395,   402,   409,   416,   423,   432,   437,
     443,   451,   454,   459,   463,   471,   474,   479,   483,   491,
     494,   500,   514,   519,   524,   529,   534,   539,   546,   550
  };

  void
  parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


} // yy
#line 2403 "parser.tab.c"

#line 555 "parser.y"


void yy::parser::error (const location_type& loc, const std::string& msg)
{
	std::cerr << loc << ": " << msg << '\n';
}
