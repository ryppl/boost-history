%token _BANG
%token _BANG_EQUALS
%token _AMPERAMPER
%token _LPAREN
%token _RPAREN
%token _PLUS_EQUALS
%token _COLON
%token _SEMIC
%token _LANGLE
%token _LANGLE_EQUALS
%token _EQUALS
%token _RANGLE
%token _RANGLE_EQUALS
%token _QUESTION_EQUALS
%token _LBRACKET
%token _RBRACKET
%token ACTIONS
%token BIND
%token CASE
%token DEFAULT
%token ELSE
%token EXISTING
%token FOR
%token IF
%token IGNORE
%token IN
%token INCLUDE
%token LOCAL
%token ON
%token PIECEMEAL
%token QUIETLY
%token RETURN
%token RULE
%token SWITCH
%token TOGETHER
%token UPDATED
%token _LBRACE
%token _BARBAR
%token _RBRACE
/*
 * Copyright 1993, 2000 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/*
 * jamgram.yy - jam grammar
 *
 * 04/13/94 (seiwald) - added shorthand L0 for null list pointer
 * 06/01/94 (seiwald) - new 'actions existing' does existing sources
 * 08/23/94 (seiwald) - Support for '+=' (append to variable)
 * 08/31/94 (seiwald) - Allow ?= as alias for "default =".
 * 09/15/94 (seiwald) - if conditionals take only single arguments, so
 *			that 'if foo == bar' gives syntax error (use =).
 * 02/11/95 (seiwald) - when scanning arguments to rules, only treat
 *			punctuation keywords as keywords.  All arg lists
 *			are terminated with punctuation keywords.
 *
 * 09/11/00 (seiwald) - Support for function calls:
 *
 *		Rules now return lists (LIST *), rather than void.
 *
 *		New "[ rule ]" syntax evals rule into a LIST.
 *
 *		Lists are now generated by compile_list() and 
 *		compile_append(), and any other rule that indirectly
 *		makes a list, rather than being built directly here,
 *		so that lists values can contain rule evaluations.
 *
 *		New 'return' rule sets the return value, though
 *		other statements also may have return values.
 *
 *		'run' production split from 'block' production so 
 *		that empty blocks can be handled separately.
 */

%token ARG STRING

%left _BARBAR
%left _AMPERAMPER
%left _BANG

%{
#include "jam.h"

#include "lists.h"
#include "parse.h"
#include "scan.h"
#include "compile.h"
#include "newstr.h"

# define F0 (LIST *(*)(PARSE *, LOL *))0
# define P0 (PARSE *)0
# define S0 (char *)0

# define pappend( l,r )    	parse_make( compile_append,l,r,P0,S0,S0,0 )
# define pfor( s,l,r )    	parse_make( compile_foreach,l,r,P0,s,S0,0 )
# define pif( l,r,t )	  	parse_make( compile_if,l,r,t,S0,S0,0 )
# define pincl( l )       	parse_make( compile_include,l,P0,P0,S0,S0,0 )
# define plist( s )	  	parse_make( compile_list,P0,P0,P0,s,S0,0 )
# define plocal( l,r,t )  	parse_make( compile_local,l,r,t,S0,S0,0 )
# define pnull()	  	parse_make( compile_null,P0,P0,P0,S0,S0,0 )
# define prule( s,p )     	parse_make( compile_rule,p,P0,P0,s,S0,0 )
# define prules( l,r )	  	parse_make( compile_rules,l,r,P0,S0,S0,0 )
# define pset( l,r,a ) 	  	parse_make( compile_set,l,r,P0,S0,S0,a )
# define pset1( l,r,t,a )	parse_make( compile_settings,l,r,t,S0,S0,a )
# define psetc( s,p )     	parse_make( compile_setcomp,p,P0,P0,s,S0,0 )
# define psetc_args( s,p,a )    parse_make( compile_setcomp,p,a,P0,s,S0,0 )
# define psete( s,l,s1,f ) 	parse_make( compile_setexec,l,P0,P0,s,s1,f )
# define pswitch( l,r )   	parse_make( compile_switch,l,r,P0,S0,S0,0 )

# define pnode( l,r )    	parse_make( F0,l,r,P0,S0,S0,0 )
# define pcnode( c,l,r )	parse_make( F0,l,r,P0,S0,S0,c )
# define psnode( s,l )     	parse_make( F0,l,P0,P0,s,S0,0 )

%}

%%

run	: /* empty */
		/* do nothing */
	| rules
		{ parse_save( $1.parse ); }
	;

/*
 * block - zero or more rules
 * rules - one or more rules
 * rule - any one of jam's rules
 * right-recursive so rules execute in order.
 */

block	: /* empty */
		{ $$.parse = pnull(); }
	| rules
		{ $$.parse = $1.parse; }
	;

rules	: rule
		{ $$.parse = $1.parse; }
	| rule rules
		{ $$.parse = prules( $1.parse, $2.parse ); }
	| LOCAL list _SEMIC block
		{ $$.parse = plocal( $2.parse, pnull(), $4.parse ); }
	| LOCAL list _EQUALS list _SEMIC block
		{ $$.parse = plocal( $2.parse, $4.parse, $6.parse ); }
	;

rule	: _LBRACE block _RBRACE
		{ $$.parse = $2.parse; }
	| INCLUDE list _SEMIC
		{ $$.parse = pincl( $2.parse ); }
	| ARG lol _SEMIC
		{ $$.parse = prule( $1.string, $2.parse ); }
	| arg assign list _SEMIC
		{ $$.parse = pset( $1.parse, $3.parse, $2.number ); }
	| arg ON list assign list _SEMIC
		{ $$.parse = pset1( $1.parse, $3.parse, $5.parse, $4.number ); }
	| RETURN list _SEMIC
		{ $$.parse = $2.parse; }
	| FOR ARG IN list _LBRACE block _RBRACE
		{ $$.parse = pfor( $2.string, $4.parse, $6.parse ); }
	| SWITCH list _LBRACE cases _RBRACE
		{ $$.parse = pswitch( $2.parse, $4.parse ); }
	| IF cond _LBRACE block _RBRACE 
		{ $$.parse = pif( $2.parse, $4.parse, pnull() ); }
	| IF cond _LBRACE block _RBRACE ELSE rule
		{ $$.parse = pif( $2.parse, $4.parse, $7.parse ); }
        | RULE ARG _LPAREN lol _RPAREN rule
		{ $$.parse = psetc_args( $2.string, $6.parse, $4.parse ); }
	| RULE ARG rule
		{ $$.parse = psetc( $2.string, $3.parse ); }
	| ACTIONS eflags ARG bindlist _LBRACE
		{ yymode( SCAN_STRING ); }
	  STRING 
		{ yymode( SCAN_NORMAL ); }
	  _RBRACE
		{ $$.parse = psete( $3.string,$4.parse,$7.string,$2.number ); }
	;

/*
 * assign - = or +=
 */

assign	: _EQUALS
		{ $$.number = ASSIGN_SET; }
	| _PLUS_EQUALS
		{ $$.number = ASSIGN_APPEND; }
	| _QUESTION_EQUALS
		{ $$.number = ASSIGN_DEFAULT; }
	| DEFAULT _EQUALS
		{ $$.number = ASSIGN_DEFAULT; }
	;

/*
 * cond - a conditional for 'if'
 */

cond	: arg 
		{ $$.parse = pcnode( COND_EXISTS, $1.parse, pnull() ); }
	| arg _EQUALS arg 
		{ $$.parse = pcnode( COND_EQUALS, $1.parse, $3.parse ); }
	| arg _BANG_EQUALS arg
		{ $$.parse = pcnode( COND_NOTEQ, $1.parse, $3.parse ); }
	| arg _LANGLE arg
		{ $$.parse = pcnode( COND_LESS, $1.parse, $3.parse ); }
	| arg _LANGLE_EQUALS arg 
		{ $$.parse = pcnode( COND_LESSEQ, $1.parse, $3.parse ); }
	| arg _RANGLE arg 
		{ $$.parse = pcnode( COND_MORE, $1.parse, $3.parse ); }
	| arg _RANGLE_EQUALS arg 
		{ $$.parse = pcnode( COND_MOREEQ, $1.parse, $3.parse ); }
	| arg IN list
		{ $$.parse = pcnode( COND_IN, $1.parse, $3.parse ); }
	| _BANG cond
		{ $$.parse = pcnode( COND_NOT, $2.parse, P0 ); }
	| cond _AMPERAMPER cond 
		{ $$.parse = pcnode( COND_AND, $1.parse, $3.parse ); }
	| cond _BARBAR cond
		{ $$.parse = pcnode( COND_OR, $1.parse, $3.parse ); }
	| _LPAREN cond _RPAREN
		{ $$.parse = $2.parse; }
	;

/*
 * cases - action elements inside a 'switch'
 * case - a single action element inside a 'switch'
 * right-recursive rule so cases can be examined in order.
 */

cases	: /* empty */
		{ $$.parse = P0; }
	| case cases
		{ $$.parse = pnode( $1.parse, $2.parse ); }
	;

case	: CASE ARG _COLON block
		{ $$.parse = psnode( $2.string, $4.parse ); }
	;

/*
 * lol - list of lists
 * right-recursive rule so that lists can be added in order.
 */

lol	: list
		{ $$.parse = pnode( P0, $1.parse ); }
	| list _COLON lol
		{ $$.parse = pnode( $3.parse, $1.parse ); }
	;

/*
 * list - zero or more args in a LIST
 * listp - list (in puncutation only mode)
 * arg - one ARG or function call
 */

list	: listp
		{ $$.parse = $1.parse; yymode( SCAN_NORMAL ); }
	;

listp	: /* empty */
		{ $$.parse = pnull(); yymode( SCAN_PUNCT ); }
	| listp arg
		{ $$.parse = pappend( $1.parse, $2.parse ); }
	;

arg	: ARG 
		{ $$.parse = plist( $1.string ); }
	| _LBRACKET ARG lol _RBRACKET
		{ $$.parse = prule( $2.string, $3.parse ); }
	;


/*
 * eflags - zero or more modifiers to 'executes'
 * eflag - a single modifier to 'executes'
 */

eflags	: /* empty */
		{ $$.number = 0; }
	| eflags eflag
		{ $$.number = $1.number | $2.number; }
	;

eflag	: UPDATED
		{ $$.number = EXEC_UPDATED; }
	| TOGETHER
		{ $$.number = EXEC_TOGETHER; }
	| IGNORE
		{ $$.number = EXEC_IGNORE; }
	| QUIETLY
		{ $$.number = EXEC_QUIETLY; }
	| PIECEMEAL
		{ $$.number = EXEC_PIECEMEAL; }
	| EXISTING
		{ $$.number = EXEC_EXISTING; }
	;


/*
 * bindlist - list of variable to bind for an action
 */

bindlist : /* empty */
		{ $$.parse = pnull(); }
	| BIND list
		{ $$.parse = $2.parse; }
	;


