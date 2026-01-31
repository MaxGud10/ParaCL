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





#include "parser.hpp"


// Unqualified %code blocks.
#line 27 "/home/max/WORK/ParaCL/grammar/parser.y"

	#include <memory>

	#include "log.h"
	#include "driver.hpp"

#line 53 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"


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
#line 145 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"

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
      case symbol_kind::S_NUMBER: // "number"
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ID: // "identifier"
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Assign: // Assign
        value.YY_MOVE_OR_COPY< std::unique_ptr<AST::AssignNode> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Expr: // Expr
        value.YY_MOVE_OR_COPY< std::unique_ptr<AST::ExpressionNode> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_For_Stm: // For_Stm
        value.YY_MOVE_OR_COPY< std::unique_ptr<AST::ForNode> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_If_Stm: // If_Stm
        value.YY_MOVE_OR_COPY< std::unique_ptr<AST::IfNode> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Print: // Print
        value.YY_MOVE_OR_COPY< std::unique_ptr<AST::PrintNode> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Scope: // Scope
        value.YY_MOVE_OR_COPY< std::unique_ptr<AST::ScopeNode> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Statement: // Statement
        value.YY_MOVE_OR_COPY< std::unique_ptr<AST::StatementNode> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Variable: // Variable
        value.YY_MOVE_OR_COPY< std::unique_ptr<AST::VariableNode> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_While_Stm: // While_Stm
        value.YY_MOVE_OR_COPY< std::unique_ptr<AST::WhileNode> > (YY_MOVE (that.value));
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
      case symbol_kind::S_NUMBER: // "number"
        value.move< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ID: // "identifier"
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Assign: // Assign
        value.move< std::unique_ptr<AST::AssignNode> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Expr: // Expr
        value.move< std::unique_ptr<AST::ExpressionNode> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_For_Stm: // For_Stm
        value.move< std::unique_ptr<AST::ForNode> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_If_Stm: // If_Stm
        value.move< std::unique_ptr<AST::IfNode> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Print: // Print
        value.move< std::unique_ptr<AST::PrintNode> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Scope: // Scope
        value.move< std::unique_ptr<AST::ScopeNode> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Statement: // Statement
        value.move< std::unique_ptr<AST::StatementNode> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Variable: // Variable
        value.move< std::unique_ptr<AST::VariableNode> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_While_Stm: // While_Stm
        value.move< std::unique_ptr<AST::WhileNode> > (YY_MOVE (that.value));
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
      case symbol_kind::S_NUMBER: // "number"
        value.copy< int > (that.value);
        break;

      case symbol_kind::S_ID: // "identifier"
        value.copy< std::string > (that.value);
        break;

      case symbol_kind::S_Assign: // Assign
        value.copy< std::unique_ptr<AST::AssignNode> > (that.value);
        break;

      case symbol_kind::S_Expr: // Expr
        value.copy< std::unique_ptr<AST::ExpressionNode> > (that.value);
        break;

      case symbol_kind::S_For_Stm: // For_Stm
        value.copy< std::unique_ptr<AST::ForNode> > (that.value);
        break;

      case symbol_kind::S_If_Stm: // If_Stm
        value.copy< std::unique_ptr<AST::IfNode> > (that.value);
        break;

      case symbol_kind::S_Print: // Print
        value.copy< std::unique_ptr<AST::PrintNode> > (that.value);
        break;

      case symbol_kind::S_Scope: // Scope
        value.copy< std::unique_ptr<AST::ScopeNode> > (that.value);
        break;

      case symbol_kind::S_Statement: // Statement
        value.copy< std::unique_ptr<AST::StatementNode> > (that.value);
        break;

      case symbol_kind::S_Variable: // Variable
        value.copy< std::unique_ptr<AST::VariableNode> > (that.value);
        break;

      case symbol_kind::S_While_Stm: // While_Stm
        value.copy< std::unique_ptr<AST::WhileNode> > (that.value);
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
      case symbol_kind::S_NUMBER: // "number"
        value.move< int > (that.value);
        break;

      case symbol_kind::S_ID: // "identifier"
        value.move< std::string > (that.value);
        break;

      case symbol_kind::S_Assign: // Assign
        value.move< std::unique_ptr<AST::AssignNode> > (that.value);
        break;

      case symbol_kind::S_Expr: // Expr
        value.move< std::unique_ptr<AST::ExpressionNode> > (that.value);
        break;

      case symbol_kind::S_For_Stm: // For_Stm
        value.move< std::unique_ptr<AST::ForNode> > (that.value);
        break;

      case symbol_kind::S_If_Stm: // If_Stm
        value.move< std::unique_ptr<AST::IfNode> > (that.value);
        break;

      case symbol_kind::S_Print: // Print
        value.move< std::unique_ptr<AST::PrintNode> > (that.value);
        break;

      case symbol_kind::S_Scope: // Scope
        value.move< std::unique_ptr<AST::ScopeNode> > (that.value);
        break;

      case symbol_kind::S_Statement: // Statement
        value.move< std::unique_ptr<AST::StatementNode> > (that.value);
        break;

      case symbol_kind::S_Variable: // Variable
        value.move< std::unique_ptr<AST::VariableNode> > (that.value);
        break;

      case symbol_kind::S_While_Stm: // While_Stm
        value.move< std::unique_ptr<AST::WhileNode> > (that.value);
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
#line 88 "/home/max/WORK/ParaCL/grammar/parser.y"
                 { yyo << yysym.value.template as < std::string > (); }
#line 473 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
        break;

      case symbol_kind::S_NUMBER: // "number"
#line 88 "/home/max/WORK/ParaCL/grammar/parser.y"
                 { yyo << yysym.value.template as < int > (); }
#line 479 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
        break;

      case symbol_kind::S_Statement: // Statement
#line 88 "/home/max/WORK/ParaCL/grammar/parser.y"
                 { yyo << yysym.value.template as < std::unique_ptr<AST::StatementNode> > (); }
#line 485 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
        break;

      case symbol_kind::S_Scope: // Scope
#line 88 "/home/max/WORK/ParaCL/grammar/parser.y"
                 { yyo << yysym.value.template as < std::unique_ptr<AST::ScopeNode> > (); }
#line 491 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
        break;

      case symbol_kind::S_If_Stm: // If_Stm
#line 88 "/home/max/WORK/ParaCL/grammar/parser.y"
                 { yyo << yysym.value.template as < std::unique_ptr<AST::IfNode> > (); }
#line 497 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
        break;

      case symbol_kind::S_For_Stm: // For_Stm
#line 88 "/home/max/WORK/ParaCL/grammar/parser.y"
                 { yyo << yysym.value.template as < std::unique_ptr<AST::ForNode> > (); }
#line 503 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
        break;

      case symbol_kind::S_While_Stm: // While_Stm
#line 88 "/home/max/WORK/ParaCL/grammar/parser.y"
                 { yyo << yysym.value.template as < std::unique_ptr<AST::WhileNode> > (); }
#line 509 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
        break;

      case symbol_kind::S_Assign: // Assign
#line 88 "/home/max/WORK/ParaCL/grammar/parser.y"
                 { yyo << yysym.value.template as < std::unique_ptr<AST::AssignNode> > (); }
#line 515 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
        break;

      case symbol_kind::S_Print: // Print
#line 88 "/home/max/WORK/ParaCL/grammar/parser.y"
                 { yyo << yysym.value.template as < std::unique_ptr<AST::PrintNode> > (); }
#line 521 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
        break;

      case symbol_kind::S_Expr: // Expr
#line 88 "/home/max/WORK/ParaCL/grammar/parser.y"
                 { yyo << yysym.value.template as < std::unique_ptr<AST::ExpressionNode> > (); }
#line 527 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
        break;

      case symbol_kind::S_Variable: // Variable
#line 88 "/home/max/WORK/ParaCL/grammar/parser.y"
                 { yyo << yysym.value.template as < std::unique_ptr<AST::VariableNode> > (); }
#line 533 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
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
      case symbol_kind::S_NUMBER: // "number"
        yylhs.value.emplace< int > ();
        break;

      case symbol_kind::S_ID: // "identifier"
        yylhs.value.emplace< std::string > ();
        break;

      case symbol_kind::S_Assign: // Assign
        yylhs.value.emplace< std::unique_ptr<AST::AssignNode> > ();
        break;

      case symbol_kind::S_Expr: // Expr
        yylhs.value.emplace< std::unique_ptr<AST::ExpressionNode> > ();
        break;

      case symbol_kind::S_For_Stm: // For_Stm
        yylhs.value.emplace< std::unique_ptr<AST::ForNode> > ();
        break;

      case symbol_kind::S_If_Stm: // If_Stm
        yylhs.value.emplace< std::unique_ptr<AST::IfNode> > ();
        break;

      case symbol_kind::S_Print: // Print
        yylhs.value.emplace< std::unique_ptr<AST::PrintNode> > ();
        break;

      case symbol_kind::S_Scope: // Scope
        yylhs.value.emplace< std::unique_ptr<AST::ScopeNode> > ();
        break;

      case symbol_kind::S_Statement: // Statement
        yylhs.value.emplace< std::unique_ptr<AST::StatementNode> > ();
        break;

      case symbol_kind::S_Variable: // Variable
        yylhs.value.emplace< std::unique_ptr<AST::VariableNode> > ();
        break;

      case symbol_kind::S_While_Stm: // While_Stm
        yylhs.value.emplace< std::unique_ptr<AST::WhileNode> > ();
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
  case 3: // Program: Statements $end
#line 120 "/home/max/WORK/ParaCL/grammar/parser.y"
                        {
				MSG("Initialising global scope with vector of statements:\n");
				for ([[maybe_unused]] const auto& stm : drv.stm_table[drv.cur_scope_id])
				{
					LOG("{}\n", static_cast<const void*>(stm.get()));
				}

				drv.ast.globalScope =
					std::make_unique<AST::ScopeNode>(std::move(drv.stm_table[drv.cur_scope_id]));
			}
#line 849 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 4: // Statements: Statement
#line 132 "/home/max/WORK/ParaCL/grammar/parser.y"
                        {
				LOG("Pushing statement : {}\n",
					static_cast<const void*>(yystack_[0].value.as < std::unique_ptr<AST::StatementNode> > ().get()));

				drv.stm_table[drv.cur_scope_id].push_back(std::move(yystack_[0].value.as < std::unique_ptr<AST::StatementNode> > ()));
			}
#line 860 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 5: // Statements: Statements Statement
#line 139 "/home/max/WORK/ParaCL/grammar/parser.y"
                        {
				LOG("Pushing statement : {}\n",
					static_cast<const void*>(yystack_[0].value.as < std::unique_ptr<AST::StatementNode> > ().get()));

				drv.stm_table[drv.cur_scope_id].push_back(std::move(yystack_[0].value.as < std::unique_ptr<AST::StatementNode> > ()));
			}
#line 871 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 6: // Statement: ";"
#line 148 "/home/max/WORK/ParaCL/grammar/parser.y"
                        {
				MSG("Void statement\n");
				yylhs.value.as < std::unique_ptr<AST::StatementNode> > () = std::make_unique<AST::VoidNode>();
			}
#line 880 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 7: // Statement: Expr ";"
#line 154 "/home/max/WORK/ParaCL/grammar/parser.y"
                        {
				LOG("It's Expr. Moving from concrete rule: {}\n",
					static_cast<const void*>(yylhs.value.as < std::unique_ptr<AST::StatementNode> > ().get()));

				yylhs.value.as < std::unique_ptr<AST::StatementNode> > () = std::move(yystack_[1].value.as < std::unique_ptr<AST::ExpressionNode> > ());
			}
#line 891 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 8: // Statement: Scope
#line 161 "/home/max/WORK/ParaCL/grammar/parser.y"
                        {
				LOG("It's Scope. Moving from concrete rule: {}\n",
					static_cast<const void*>(yylhs.value.as < std::unique_ptr<AST::StatementNode> > ().get()));

				yylhs.value.as < std::unique_ptr<AST::StatementNode> > () = std::move(yystack_[0].value.as < std::unique_ptr<AST::ScopeNode> > ());
			}
#line 902 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 9: // Statement: If_Stm
#line 175 "/home/max/WORK/ParaCL/grammar/parser.y"
                        {
				LOG("It's If_Stm. Moving from concrete rule: {}\n",
					static_cast<const void*>(yylhs.value.as < std::unique_ptr<AST::StatementNode> > ().get()));

				yylhs.value.as < std::unique_ptr<AST::StatementNode> > () = std::move(yystack_[0].value.as < std::unique_ptr<AST::IfNode> > ());
			}
#line 913 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 10: // Statement: While_Stm
#line 182 "/home/max/WORK/ParaCL/grammar/parser.y"
                        {
				LOG("It's While_Stm. Moving from concrete rule: {}\n",
					static_cast<const void*>(yylhs.value.as < std::unique_ptr<AST::StatementNode> > ().get()));

				yylhs.value.as < std::unique_ptr<AST::StatementNode> > () = std::move(yystack_[0].value.as < std::unique_ptr<AST::WhileNode> > ());
			}
#line 924 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 11: // Statement: For_Stm
#line 189 "/home/max/WORK/ParaCL/grammar/parser.y"
            {
                LOG("It's For_Stm. Moving from concrete rule: {}\n",
                    static_cast<const void*>(yylhs.value.as < std::unique_ptr<AST::StatementNode> > ().get()));

                yylhs.value.as < std::unique_ptr<AST::StatementNode> > () = std::move(yystack_[0].value.as < std::unique_ptr<AST::ForNode> > ());
            }
#line 935 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 12: // Statement: Print ";"
#line 196 "/home/max/WORK/ParaCL/grammar/parser.y"
                        {
				LOG("It's Print. Moving from concrete rule: {}\n",
					static_cast<const void*>(yylhs.value.as < std::unique_ptr<AST::StatementNode> > ().get()));

				yylhs.value.as < std::unique_ptr<AST::StatementNode> > () = std::move(yystack_[1].value.as < std::unique_ptr<AST::PrintNode> > ());
			}
#line 946 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 13: // Scope: StartScope Statements EndScope
#line 204 "/home/max/WORK/ParaCL/grammar/parser.y"
                {
			MSG("Initialising scope with vector of statements:\n");
			for ([[maybe_unused]] const auto& stm : drv.stm_table[drv.cur_scope_id])
			{
				LOG("{}\n", static_cast<const void*>(stm.get()));
			}

			yylhs.value.as < std::unique_ptr<AST::ScopeNode> > () = std::make_unique<AST::ScopeNode>(std::move(drv.stm_table[drv.cur_scope_id]));

			MSG("Scope end.\n");

			--drv.cur_scope_id;

			LOG("drv.cur_scope_id is now {}\n", drv.cur_scope_id);

			drv.stm_table.pop_back();
		}
#line 968 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 14: // Scope: StartScope EndScope
#line 222 "/home/max/WORK/ParaCL/grammar/parser.y"
                {
              MSG("Initialising empty scope\n");
              yylhs.value.as < std::unique_ptr<AST::ScopeNode> > () = std::make_unique<AST::ScopeNode>(std::vector<std::unique_ptr<AST::StatementNode>>());
              --drv.cur_scope_id;
              LOG("drv.cur_scope_id is now {}\n", drv.cur_scope_id);
              drv.stm_table.pop_back();
		}
#line 980 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 15: // StartScope: "{"
#line 231 "/home/max/WORK/ParaCL/grammar/parser.y"
                        {
				MSG("Scope start.\n");

				++drv.cur_scope_id;

				LOG("drv.cur_scope_id is now {}\n", drv.cur_scope_id);

				drv.stm_table.resize(drv.stm_table.size() + 1);
			}
#line 994 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 16: // EndScope: "}"
#line 242 "/home/max/WORK/ParaCL/grammar/parser.y"
                        {

			}
#line 1002 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 17: // If_Stm: "if" "(" Expr ")" Statement
#line 247 "/home/max/WORK/ParaCL/grammar/parser.y"
                        {
				MSG("Initialising if statement\n");
				yylhs.value.as < std::unique_ptr<AST::IfNode> > () = std::make_unique<AST::IfNode>(std::move(yystack_[2].value.as < std::unique_ptr<AST::ExpressionNode> > ()), std::move(yystack_[0].value.as < std::unique_ptr<AST::StatementNode> > ()));
			}
#line 1011 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 18: // If_Stm: "if" "(" Expr ")" Statement "else" Statement
#line 252 "/home/max/WORK/ParaCL/grammar/parser.y"
                        {
				yylhs.value.as < std::unique_ptr<AST::IfNode> > () = std::make_unique<AST::IfNode>(
					std::move(yystack_[4].value.as < std::unique_ptr<AST::ExpressionNode> > ()),
					std::move(yystack_[2].value.as < std::unique_ptr<AST::StatementNode> > ()),
					std::move(yystack_[0].value.as < std::unique_ptr<AST::StatementNode> > ())
				);
			}
#line 1023 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 19: // For_Stm: "for" "(" Assign ";" Expr ";" Assign ")" Statement
#line 261 "/home/max/WORK/ParaCL/grammar/parser.y"
            {
                MSG("Initialising for statement\n");
                yylhs.value.as < std::unique_ptr<AST::ForNode> > () = std::make_unique<AST::ForNode>(
                        std::move(yystack_[6].value.as < std::unique_ptr<AST::AssignNode> > ()),
                        std::move(yystack_[4].value.as < std::unique_ptr<AST::ExpressionNode> > ()),
                        std::move(yystack_[2].value.as < std::unique_ptr<AST::AssignNode> > ()),
                        std::move(yystack_[0].value.as < std::unique_ptr<AST::StatementNode> > ())
                );
            }
#line 1037 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 20: // While_Stm: "while" "(" Expr ")" Statement
#line 272 "/home/max/WORK/ParaCL/grammar/parser.y"
                        {
				MSG("Initialising while statement\n");
				yylhs.value.as < std::unique_ptr<AST::WhileNode> > () = std::make_unique<AST::WhileNode>(std::move(yystack_[2].value.as < std::unique_ptr<AST::ExpressionNode> > ()), std::move(yystack_[0].value.as < std::unique_ptr<AST::StatementNode> > ()));
			}
#line 1046 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 21: // Assign: Variable "=" Expr
#line 278 "/home/max/WORK/ParaCL/grammar/parser.y"
                {
			yylhs.value.as < std::unique_ptr<AST::AssignNode> > () = std::make_unique<AST::AssignNode>(	std::move(yystack_[2].value.as < std::unique_ptr<AST::VariableNode> > ()),
													AST::AssignType::ASSIGN_DEFAULT,
													std::move(yystack_[0].value.as < std::unique_ptr<AST::ExpressionNode> > ()));
			LOG("Initialising assignment: {}\n", static_cast<const void*>(yylhs.value.as < std::unique_ptr<AST::AssignNode> > ().get()));
		}
#line 1057 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 22: // Assign: Variable "+=" Expr
#line 286 "/home/max/WORK/ParaCL/grammar/parser.y"
                        {
				MSG("Intializing ASSIGN_PLUS operation\n");
				yylhs.value.as < std::unique_ptr<AST::AssignNode> > () = std::make_unique<AST::AssignNode>( std::move(yystack_[2].value.as < std::unique_ptr<AST::VariableNode> > ()),
														  AST::AssignType::ASSIGN_PLUS,
														  std::move(yystack_[0].value.as < std::unique_ptr<AST::ExpressionNode> > ()));

			}
#line 1069 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 23: // Assign: Variable "*=" Expr
#line 294 "/home/max/WORK/ParaCL/grammar/parser.y"
                        {
				MSG("Intializing ASSIGN_MUL operation\n");
				yylhs.value.as < std::unique_ptr<AST::AssignNode> > () = std::make_unique<AST::AssignNode>( std::move(yystack_[2].value.as < std::unique_ptr<AST::VariableNode> > ()),
														  AST::AssignType::ASSIGN_MUL,
														  std::move(yystack_[0].value.as < std::unique_ptr<AST::ExpressionNode> > ()));

			}
#line 1081 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 24: // Assign: Variable "-=" Expr
#line 302 "/home/max/WORK/ParaCL/grammar/parser.y"
                        {
				MSG("Intializing ASSIGN_MINUS operation\n");
				yylhs.value.as < std::unique_ptr<AST::AssignNode> > () = std::make_unique<AST::AssignNode>( std::move(yystack_[2].value.as < std::unique_ptr<AST::VariableNode> > ()),
														  AST::AssignType::ASSIGN_MINUS,
														  std::move(yystack_[0].value.as < std::unique_ptr<AST::ExpressionNode> > ()));

			}
#line 1093 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 25: // Assign: Variable "/=" Expr
#line 310 "/home/max/WORK/ParaCL/grammar/parser.y"
                        {
				MSG("Intializing ASSIGN_DIV operation\n");
				yylhs.value.as < std::unique_ptr<AST::AssignNode> > () = std::make_unique<AST::AssignNode>( std::move(yystack_[2].value.as < std::unique_ptr<AST::VariableNode> > ()),
														  AST::AssignType::ASSIGN_DIV,
														  std::move(yystack_[0].value.as < std::unique_ptr<AST::ExpressionNode> > ()));

			}
#line 1105 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 26: // Assign: Variable "%=" Expr
#line 318 "/home/max/WORK/ParaCL/grammar/parser.y"
                        {
				MSG("Intializing ASSIGN_MOD operation\n");
				yylhs.value.as < std::unique_ptr<AST::AssignNode> > () = std::make_unique<AST::AssignNode>( std::move(yystack_[2].value.as < std::unique_ptr<AST::VariableNode> > ()),
														  AST::AssignType::ASSIGN_MOD,
														  std::move(yystack_[0].value.as < std::unique_ptr<AST::ExpressionNode> > ()));

			}
#line 1117 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 27: // Print: "print" Expr
#line 328 "/home/max/WORK/ParaCL/grammar/parser.y"
                {
			MSG("Initialising print\n");
			yylhs.value.as < std::unique_ptr<AST::PrintNode> > () = std::make_unique<AST::PrintNode>(std::move(yystack_[0].value.as < std::unique_ptr<AST::ExpressionNode> > ()));
		}
#line 1126 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 28: // Expr: Assign
#line 336 "/home/max/WORK/ParaCL/grammar/parser.y"
        {
            MSG("Moving Assign as expression\n");
            yylhs.value.as < std::unique_ptr<AST::ExpressionNode> > () = std::move(yystack_[0].value.as < std::unique_ptr<AST::AssignNode> > ());
        }
#line 1135 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 29: // Expr: Expr "||" Expr
#line 342 "/home/max/WORK/ParaCL/grammar/parser.y"
        {
            MSG("Initialising OR operation\n");
            yylhs.value.as < std::unique_ptr<AST::ExpressionNode> > () = std::make_unique<AST::BinaryOpNode>(
                std::move(yystack_[2].value.as < std::unique_ptr<AST::ExpressionNode> > ()),
                AST::BinaryOp::OR,
                std::move(yystack_[0].value.as < std::unique_ptr<AST::ExpressionNode> > ())
            );
        }
#line 1148 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 30: // Expr: Expr "&&" Expr
#line 352 "/home/max/WORK/ParaCL/grammar/parser.y"
        {
            MSG("Initialising AND operation\n");
            yylhs.value.as < std::unique_ptr<AST::ExpressionNode> > () = std::make_unique<AST::BinaryOpNode>(
                std::move(yystack_[2].value.as < std::unique_ptr<AST::ExpressionNode> > ()),
                AST::BinaryOp::AND,
                std::move(yystack_[0].value.as < std::unique_ptr<AST::ExpressionNode> > ())
            );
        }
#line 1161 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 31: // Expr: Expr "|" Expr
#line 362 "/home/max/WORK/ParaCL/grammar/parser.y"
        {
            MSG("Initialising BIT_OR operation\n");
            yylhs.value.as < std::unique_ptr<AST::ExpressionNode> > () = std::make_unique<AST::BinaryOpNode>(
                std::move(yystack_[2].value.as < std::unique_ptr<AST::ExpressionNode> > ()),
                AST::BinaryOp::BIT_OR,
                std::move(yystack_[0].value.as < std::unique_ptr<AST::ExpressionNode> > ())
            );
        }
#line 1174 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 32: // Expr: Expr "&" Expr
#line 372 "/home/max/WORK/ParaCL/grammar/parser.y"
        {
            MSG("Initialising BIT_AND operation\n");
            yylhs.value.as < std::unique_ptr<AST::ExpressionNode> > () = std::make_unique<AST::BinaryOpNode>(
                std::move(yystack_[2].value.as < std::unique_ptr<AST::ExpressionNode> > ()),
                AST::BinaryOp::BIT_AND,
                std::move(yystack_[0].value.as < std::unique_ptr<AST::ExpressionNode> > ())
            );
        }
#line 1187 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 33: // Expr: Expr "==" Expr
#line 382 "/home/max/WORK/ParaCL/grammar/parser.y"
        {
            MSG("Initialising EQ operation\n");
            yylhs.value.as < std::unique_ptr<AST::ExpressionNode> > () = std::make_unique<AST::BinaryOpNode>(
                std::move(yystack_[2].value.as < std::unique_ptr<AST::ExpressionNode> > ()),
                AST::BinaryOp::EQ,
                std::move(yystack_[0].value.as < std::unique_ptr<AST::ExpressionNode> > ())
            );
        }
#line 1200 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 34: // Expr: Expr "!=" Expr
#line 391 "/home/max/WORK/ParaCL/grammar/parser.y"
        {
            MSG("Initialising NOT_EQ operation\n");
            yylhs.value.as < std::unique_ptr<AST::ExpressionNode> > () = std::make_unique<AST::BinaryOpNode>(
                std::move(yystack_[2].value.as < std::unique_ptr<AST::ExpressionNode> > ()),
                AST::BinaryOp::NOT_EQ,
                std::move(yystack_[0].value.as < std::unique_ptr<AST::ExpressionNode> > ())
            );
        }
#line 1213 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 35: // Expr: Expr "<" Expr
#line 401 "/home/max/WORK/ParaCL/grammar/parser.y"
        {
            MSG("Initialising LS operation\n");
            yylhs.value.as < std::unique_ptr<AST::ExpressionNode> > () = std::make_unique<AST::BinaryOpNode>(
                std::move(yystack_[2].value.as < std::unique_ptr<AST::ExpressionNode> > ()),
                AST::BinaryOp::LS,
                std::move(yystack_[0].value.as < std::unique_ptr<AST::ExpressionNode> > ())
            );
        }
#line 1226 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 36: // Expr: Expr "<=" Expr
#line 410 "/home/max/WORK/ParaCL/grammar/parser.y"
        {
            MSG("Initialising LS_EQ operation\n");
            yylhs.value.as < std::unique_ptr<AST::ExpressionNode> > () = std::make_unique<AST::BinaryOpNode>(
                std::move(yystack_[2].value.as < std::unique_ptr<AST::ExpressionNode> > ()),
                AST::BinaryOp::LS_EQ,
                std::move(yystack_[0].value.as < std::unique_ptr<AST::ExpressionNode> > ())
            );
        }
#line 1239 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 37: // Expr: Expr ">" Expr
#line 419 "/home/max/WORK/ParaCL/grammar/parser.y"
        {
            MSG("Initialising GR operation\n");
            yylhs.value.as < std::unique_ptr<AST::ExpressionNode> > () = std::make_unique<AST::BinaryOpNode>(
                std::move(yystack_[2].value.as < std::unique_ptr<AST::ExpressionNode> > ()),
                AST::BinaryOp::GR,
                std::move(yystack_[0].value.as < std::unique_ptr<AST::ExpressionNode> > ())
            );
        }
#line 1252 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 38: // Expr: Expr ">=" Expr
#line 428 "/home/max/WORK/ParaCL/grammar/parser.y"
        {
            MSG("Initialising GR_EQ operation\n");
            yylhs.value.as < std::unique_ptr<AST::ExpressionNode> > () = std::make_unique<AST::BinaryOpNode>(
                std::move(yystack_[2].value.as < std::unique_ptr<AST::ExpressionNode> > ()),
                AST::BinaryOp::GR_EQ,
                std::move(yystack_[0].value.as < std::unique_ptr<AST::ExpressionNode> > ())
            );
        }
#line 1265 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 39: // Expr: Expr "+" Expr
#line 438 "/home/max/WORK/ParaCL/grammar/parser.y"
        {
            MSG("Initialising ADD operation\n");
            yylhs.value.as < std::unique_ptr<AST::ExpressionNode> > () = std::make_unique<AST::BinaryOpNode>(
                std::move(yystack_[2].value.as < std::unique_ptr<AST::ExpressionNode> > ()),
                AST::BinaryOp::ADD,
                std::move(yystack_[0].value.as < std::unique_ptr<AST::ExpressionNode> > ())
            );
        }
#line 1278 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 40: // Expr: Expr "-" Expr
#line 447 "/home/max/WORK/ParaCL/grammar/parser.y"
        {
            MSG("Initialising SUB operation\n");
            yylhs.value.as < std::unique_ptr<AST::ExpressionNode> > () = std::make_unique<AST::BinaryOpNode>(
                std::move(yystack_[2].value.as < std::unique_ptr<AST::ExpressionNode> > ()),
                AST::BinaryOp::SUB,
                std::move(yystack_[0].value.as < std::unique_ptr<AST::ExpressionNode> > ())
            );
        }
#line 1291 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 41: // Expr: Expr "*" Expr
#line 457 "/home/max/WORK/ParaCL/grammar/parser.y"
        {
            MSG("Initialising MUL operation\n");
            yylhs.value.as < std::unique_ptr<AST::ExpressionNode> > () = std::make_unique<AST::BinaryOpNode>(
                std::move(yystack_[2].value.as < std::unique_ptr<AST::ExpressionNode> > ()),
                AST::BinaryOp::MUL,
                std::move(yystack_[0].value.as < std::unique_ptr<AST::ExpressionNode> > ())
            );
        }
#line 1304 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 42: // Expr: Expr "/" Expr
#line 466 "/home/max/WORK/ParaCL/grammar/parser.y"
        {
            MSG("Initialising DIV operation\n");
            yylhs.value.as < std::unique_ptr<AST::ExpressionNode> > () = std::make_unique<AST::BinaryOpNode>(
                std::move(yystack_[2].value.as < std::unique_ptr<AST::ExpressionNode> > ()),
                AST::BinaryOp::DIV,
                std::move(yystack_[0].value.as < std::unique_ptr<AST::ExpressionNode> > ())
            );
        }
#line 1317 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 43: // Expr: Expr "%" Expr
#line 475 "/home/max/WORK/ParaCL/grammar/parser.y"
        {
            MSG("Initialising MOD operation\n");
            yylhs.value.as < std::unique_ptr<AST::ExpressionNode> > () = std::make_unique<AST::BinaryOpNode>(
                std::move(yystack_[2].value.as < std::unique_ptr<AST::ExpressionNode> > ()),
                AST::BinaryOp::MOD,
                std::move(yystack_[0].value.as < std::unique_ptr<AST::ExpressionNode> > ())
            );
        }
#line 1330 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 44: // Expr: "-" Expr
#line 485 "/home/max/WORK/ParaCL/grammar/parser.y"
        {
            MSG("Initialising NEG operation\n");
            yylhs.value.as < std::unique_ptr<AST::ExpressionNode> > () = std::make_unique<AST::UnaryOpNode>(
                std::move(yystack_[0].value.as < std::unique_ptr<AST::ExpressionNode> > ()),
                AST::UnaryOp::NEG
            );
        }
#line 1342 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 45: // Expr: "!" Expr
#line 493 "/home/max/WORK/ParaCL/grammar/parser.y"
        {
            MSG("Initialising NOT operation\n");
            yylhs.value.as < std::unique_ptr<AST::ExpressionNode> > () = std::make_unique<AST::UnaryOpNode>(
                std::move(yystack_[0].value.as < std::unique_ptr<AST::ExpressionNode> > ()),
                AST::UnaryOp::NOT
            );
        }
#line 1354 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 46: // Expr: "(" Expr ")"
#line 502 "/home/max/WORK/ParaCL/grammar/parser.y"
        {
            MSG("Moving Expression in parenthesis\n");
            yylhs.value.as < std::unique_ptr<AST::ExpressionNode> > () = std::move(yystack_[1].value.as < std::unique_ptr<AST::ExpressionNode> > ());
        }
#line 1363 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 47: // Expr: "number"
#line 507 "/home/max/WORK/ParaCL/grammar/parser.y"
        {
            MSG("Initialising ConstantNode\n");
            yylhs.value.as < std::unique_ptr<AST::ExpressionNode> > () = std::make_unique<AST::ConstantNode>(yystack_[0].value.as < int > ());
        }
#line 1372 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 48: // Expr: "?"
#line 512 "/home/max/WORK/ParaCL/grammar/parser.y"
        {
            MSG("Initialising InNode\n");
            yylhs.value.as < std::unique_ptr<AST::ExpressionNode> > () = std::make_unique<AST::InNode>();
        }
#line 1381 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 49: // Expr: Variable
#line 517 "/home/max/WORK/ParaCL/grammar/parser.y"
        {
            MSG("Moving VariableNode\n");
            yylhs.value.as < std::unique_ptr<AST::ExpressionNode> > () = std::move(yystack_[0].value.as < std::unique_ptr<AST::VariableNode> > ());
        }
#line 1390 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;

  case 50: // Variable: "identifier"
#line 525 "/home/max/WORK/ParaCL/grammar/parser.y"
                        {
				MSG("Initialising VariableNode\n");
				yylhs.value.as < std::unique_ptr<AST::VariableNode> > () = std::make_unique<AST::VariableNode>(std::string(yystack_[0].value.as < std::string > ()));
			}
#line 1399 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"
    break;


#line 1403 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"

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
    "end of file", "error", "invalid token", "=", "-", "+", "*", "/", "(",
  ")", "?", "print", "{", "}", ";", "if", "else", "while", "for", ">", "<",
  ">=", "<=", "==", "!=", "!", "%", "&&", "||", "&", "|", "+=", "-=", "*=",
  "/=", "%=", "identifier", "number", "IFX", "UMINUS", "$accept",
  "Program", "Statements", "Statement", "Scope", "StartScope", "EndScope",
  "If_Stm", "For_Stm", "While_Stm", "Assign", "Print", "Expr", "Variable", YY_NULLPTR
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


  const signed char parser::yypact_ninf_ = -27;

  const signed char parser::yytable_ninf_ = -1;

  const short
  parser::yypact_[] =
  {
     126,   121,   121,   -27,   121,   -27,   -27,     2,     8,     9,
     121,   -27,   -27,    15,    80,   -27,   -27,    96,   -27,   -27,
     -27,   -27,    -6,   160,    29,   -27,   187,   295,   121,   121,
     -18,   -27,   -27,   -27,   -27,   -27,    96,   -27,   -27,   121,
     121,   121,   121,   -27,   121,   121,   121,   121,   121,   121,
     121,   121,   121,   121,   121,   121,   121,   121,   121,   121,
     121,   -27,   214,   241,     5,    29,   -27,     0,     0,   -27,
     -27,     7,     7,     7,     7,   384,   384,   -27,   349,   322,
     376,   149,   295,   -27,   -27,   -27,   -27,   295,   126,   126,
     121,     4,   -27,   268,   126,   -18,   -27,    12,   126,   -27
  };

  const signed char
  parser::yydefact_[] =
  {
       2,     0,     0,    48,     0,    15,     6,     0,     0,     0,
       0,    50,    47,     0,     0,     4,     8,     0,     9,    11,
      10,    28,     0,     0,    49,    44,     0,    27,     0,     0,
       0,    45,     1,     3,     5,    16,     0,    14,    12,     0,
       0,     0,     0,     7,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    46,     0,     0,     0,     0,    13,    40,    39,    41,
      42,    37,    35,    38,    36,    33,    34,    43,    30,    29,
      32,    31,    21,    22,    24,    23,    25,    26,     0,     0,
       0,    17,    20,     0,     0,     0,    18,     0,     0,    19
  };

  const signed char
  parser::yypgoto_[] =
  {
     -27,   -27,     6,   -12,   -27,   -27,   -14,   -27,   -27,   -27,
     -26,   -27,    -1,   -25
  };

  const signed char
  parser::yydefgoto_[] =
  {
       0,    13,    14,    15,    16,    17,    37,    18,    19,    20,
      21,    22,    23,    24
  };

  const signed char
  parser::yytable_[] =
  {
      25,    26,    34,    27,    64,    65,    41,    42,    38,    31,
      28,    39,    40,    41,    42,    32,    29,    30,    11,    90,
      94,    98,    66,    36,    34,     0,    50,    62,    63,     0,
       0,     0,    55,    50,     0,     0,     0,     0,    67,    68,
      69,    70,     0,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      56,    57,    58,    59,    60,     0,     0,     0,     0,    97,
      65,     0,     0,     0,     0,     0,    91,    92,     0,     0,
      33,     0,    96,     0,     1,     0,    99,     0,     2,    93,
       3,     4,     5,     0,     6,     7,     0,     8,     9,     0,
       1,     0,     0,     0,     2,    10,     3,     4,     5,    35,
       6,     7,     0,     8,     9,     0,    11,    12,     0,     0,
       0,    10,     0,     0,     0,     1,     0,     0,     0,     2,
       1,     3,    11,    12,     2,     0,     3,     4,     5,     0,
       6,     7,     0,     8,     9,     0,    10,     0,     0,     0,
       0,    10,     0,    39,    40,    41,    42,    11,    12,     0,
       0,     0,    11,    12,    39,    40,    41,    42,    44,    45,
      46,    47,    48,    49,    43,    50,     0,     0,    53,    44,
      45,    46,    47,    48,    49,     0,    50,    51,    52,    53,
      54,    39,    40,    41,    42,     0,    61,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    44,    45,    46,    47,
      48,    49,     0,    50,    51,    52,    53,    54,    39,    40,
      41,    42,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    44,    45,    46,    47,    48,    49,     0,
      50,    51,    52,    53,    54,    39,    40,    41,    42,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      44,    45,    46,    47,    48,    49,     0,    50,    51,    52,
      53,    54,    39,    40,    41,    42,     0,     0,     0,     0,
       0,     0,    95,     0,     0,     0,     0,    44,    45,    46,
      47,    48,    49,     0,    50,    51,    52,    53,    54,    39,
      40,    41,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    44,    45,    46,    47,    48,    49,
       0,    50,    51,    52,    53,    54,    39,    40,    41,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    44,    45,    46,    47,    48,    49,     0,    50,    51,
       0,    53,    54,    39,    40,    41,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    44,    45,
      46,    47,    48,    49,     0,    50,     0,     0,    53,    54,
      39,    40,    41,    42,     0,     0,     0,     0,    39,    40,
      41,    42,     0,     0,     0,    44,    45,    46,    47,    48,
      49,     0,    50,    44,    45,    46,    47,     0,     0,     0,
      50
  };

  const signed char
  parser::yycheck_[] =
  {
       1,     2,    14,     4,    30,    30,     6,     7,    14,    10,
       8,     4,     5,     6,     7,     0,     8,     8,    36,    14,
      16,     9,    36,    17,    36,    -1,    26,    28,    29,    -1,
      -1,    -1,     3,    26,    -1,    -1,    -1,    -1,    39,    40,
      41,    42,    -1,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,    95,
      95,    -1,    -1,    -1,    -1,    -1,    88,    89,    -1,    -1,
       0,    -1,    94,    -1,     4,    -1,    98,    -1,     8,    90,
      10,    11,    12,    -1,    14,    15,    -1,    17,    18,    -1,
       4,    -1,    -1,    -1,     8,    25,    10,    11,    12,    13,
      14,    15,    -1,    17,    18,    -1,    36,    37,    -1,    -1,
      -1,    25,    -1,    -1,    -1,     4,    -1,    -1,    -1,     8,
       4,    10,    36,    37,     8,    -1,    10,    11,    12,    -1,
      14,    15,    -1,    17,    18,    -1,    25,    -1,    -1,    -1,
      -1,    25,    -1,     4,     5,     6,     7,    36,    37,    -1,
      -1,    -1,    36,    37,     4,     5,     6,     7,    19,    20,
      21,    22,    23,    24,    14,    26,    -1,    -1,    29,    19,
      20,    21,    22,    23,    24,    -1,    26,    27,    28,    29,
      30,     4,     5,     6,     7,    -1,     9,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    20,    21,    22,
      23,    24,    -1,    26,    27,    28,    29,    30,     4,     5,
       6,     7,    -1,     9,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    20,    21,    22,    23,    24,    -1,
      26,    27,    28,    29,    30,     4,     5,     6,     7,    -1,
       9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    20,    21,    22,    23,    24,    -1,    26,    27,    28,
      29,    30,     4,     5,     6,     7,    -1,    -1,    -1,    -1,
      -1,    -1,    14,    -1,    -1,    -1,    -1,    19,    20,    21,
      22,    23,    24,    -1,    26,    27,    28,    29,    30,     4,
       5,     6,     7,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    28,    29,    30,     4,     5,     6,     7,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    20,    21,    22,    23,    24,    -1,    26,    27,
      -1,    29,    30,     4,     5,     6,     7,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    20,
      21,    22,    23,    24,    -1,    26,    -1,    -1,    29,    30,
       4,     5,     6,     7,    -1,    -1,    -1,    -1,     4,     5,
       6,     7,    -1,    -1,    -1,    19,    20,    21,    22,    23,
      24,    -1,    26,    19,    20,    21,    22,    -1,    -1,    -1,
      26
  };

  const signed char
  parser::yystos_[] =
  {
       0,     4,     8,    10,    11,    12,    14,    15,    17,    18,
      25,    36,    37,    41,    42,    43,    44,    45,    47,    48,
      49,    50,    51,    52,    53,    52,    52,    52,     8,     8,
       8,    52,     0,     0,    43,    13,    42,    46,    14,     4,
       5,     6,     7,    14,    19,    20,    21,    22,    23,    24,
      26,    27,    28,    29,    30,     3,    31,    32,    33,    34,
      35,     9,    52,    52,    50,    53,    46,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,     9,     9,
      14,    43,    43,    52,    16,    14,    43,    50,     9,    43
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    40,    41,    41,    42,    42,    43,    43,    43,    43,
      43,    43,    43,    44,    44,    45,    46,    47,    47,    48,
      49,    50,    50,    50,    50,    50,    50,    51,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      53
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     0,     2,     1,     2,     1,     2,     1,     1,
       1,     1,     2,     3,     2,     1,     1,     5,     7,     9,
       5,     3,     3,     3,     3,     3,     3,     2,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     3,     1,     1,     1,
       1
  };




#if YYDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   118,   118,   119,   131,   138,   147,   153,   160,   174,
     181,   188,   195,   203,   221,   230,   241,   246,   251,   260,
     271,   277,   285,   293,   301,   309,   317,   327,   335,   341,
     351,   361,   371,   381,   390,   400,   409,   418,   427,   437,
     446,   456,   465,   474,   484,   492,   501,   506,   511,   516,
     524
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
#line 2084 "/home/max/WORK/ParaCL/build-bring/parser/parser.cpp"

#line 530 "/home/max/WORK/ParaCL/grammar/parser.y"


void yy::parser::error (const location_type& loc, const std::string& msg)
{
	std::cerr << loc << ": " << msg << '\n';
}
