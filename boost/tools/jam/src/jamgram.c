
/*  A Bison parser, made from jamgram.y
    by GNU Bison version 1.28  */

#define YYBISON 1  /* Identify Bison output.  */

#define	_BANG	257
#define	_BANG_EQUALS	258
#define	_AMPERAMPER	259
#define	_LPAREN	260
#define	_RPAREN	261
#define	_PLUS_EQUALS	262
#define	_COLON	263
#define	_SEMIC	264
#define	_LANGLE	265
#define	_LANGLE_EQUALS	266
#define	_EQUALS	267
#define	_RANGLE	268
#define	_RANGLE_EQUALS	269
#define	_QUESTION_EQUALS	270
#define	_LBRACKET	271
#define	_RBRACKET	272
#define	ACTIONS	273
#define	BIND	274
#define	CASE	275
#define	DEFAULT	276
#define	ELSE	277
#define	EXISTING	278
#define	FOR	279
#define	IF	280
#define	IGNORE	281
#define	IN	282
#define	INCLUDE	283
#define	LOCAL	284
#define	ON	285
#define	PIECEMEAL	286
#define	QUIETLY	287
#define	RETURN	288
#define	RULE	289
#define	SWITCH	290
#define	TOGETHER	291
#define	UPDATED	292
#define	_LBRACE	293
#define	_BARBAR	294
#define	_RBRACE	295
#define	ARG	296
#define	STRING	297


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

#ifndef YYSTYPE
#define YYSTYPE int
#endif
#ifndef YYDEBUG
#define YYDEBUG 1
#endif

#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		125
#define	YYFLAG		-32768
#define	YYNTBASE	44

#define YYTRANSLATE(x) ((unsigned)(x) <= 297 ? yytranslate[x] : 61)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
    17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
    27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
    37,    38,    39,    40,    41,    42,    43
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     1,     3,     4,     6,     8,    11,    16,    23,    27,
    31,    35,    40,    47,    51,    59,    65,    71,    79,    86,
    90,    91,    92,   102,   104,   106,   108,   111,   113,   117,
   121,   125,   129,   133,   137,   141,   144,   148,   152,   156,
   157,   160,   165,   167,   171,   173,   174,   177,   179,   184,
   185,   188,   190,   192,   194,   196,   198,   200,   201
};

static const short yyrhs[] = {    -1,
    46,     0,     0,    46,     0,    47,     0,    47,    46,     0,
    30,    55,    10,    45,     0,    30,    55,    13,    55,    10,
    45,     0,    39,    45,    41,     0,    29,    55,    10,     0,
    42,    54,    10,     0,    57,    50,    55,    10,     0,    57,
    31,    55,    50,    55,    10,     0,    34,    55,    10,     0,
    25,    42,    28,    55,    39,    45,    41,     0,    36,    55,
    39,    52,    41,     0,    26,    51,    39,    45,    41,     0,
    26,    51,    39,    45,    41,    23,    47,     0,    35,    42,
     6,    54,     7,    47,     0,    35,    42,    47,     0,     0,
     0,    19,    58,    42,    60,    39,    48,    43,    49,    41,
     0,    13,     0,     8,     0,    16,     0,    22,    13,     0,
    57,     0,    57,    13,    57,     0,    57,     4,    57,     0,
    57,    11,    57,     0,    57,    12,    57,     0,    57,    14,
    57,     0,    57,    15,    57,     0,    57,    28,    55,     0,
     3,    51,     0,    51,     5,    51,     0,    51,    40,    51,
     0,     6,    51,     7,     0,     0,    53,    52,     0,    21,
    42,     9,    45,     0,    55,     0,    55,     9,    54,     0,
    56,     0,     0,    56,    57,     0,    42,     0,    17,    42,
    54,    18,     0,     0,    58,    59,     0,    38,     0,    37,
     0,    27,     0,    33,     0,    32,     0,    24,     0,     0,
    20,    55,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   120,   122,   133,   135,   139,   141,   143,   145,   149,   151,
   153,   155,   157,   159,   161,   163,   165,   167,   169,   171,
   173,   176,   178,   185,   187,   189,   191,   199,   201,   203,
   205,   207,   209,   211,   213,   215,   217,   219,   221,   231,
   233,   237,   246,   248,   258,   262,   264,   268,   270,   280,
   282,   286,   288,   290,   292,   294,   296,   305,   307
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","_BANG",
"_BANG_EQUALS","_AMPERAMPER","_LPAREN","_RPAREN","_PLUS_EQUALS","_COLON","_SEMIC",
"_LANGLE","_LANGLE_EQUALS","_EQUALS","_RANGLE","_RANGLE_EQUALS","_QUESTION_EQUALS",
"_LBRACKET","_RBRACKET","ACTIONS","BIND","CASE","DEFAULT","ELSE","EXISTING",
"FOR","IF","IGNORE","IN","INCLUDE","LOCAL","ON","PIECEMEAL","QUIETLY","RETURN",
"RULE","SWITCH","TOGETHER","UPDATED","_LBRACE","_BARBAR","_RBRACE","ARG","STRING",
"run","block","rules","rule","@1","@2","assign","cond","cases","case","lol",
"list","listp","arg","eflags","eflag","bindlist", NULL
};
#endif

static const short yyr1[] = {     0,
    44,    44,    45,    45,    46,    46,    46,    46,    47,    47,
    47,    47,    47,    47,    47,    47,    47,    47,    47,    47,
    48,    49,    47,    50,    50,    50,    50,    51,    51,    51,
    51,    51,    51,    51,    51,    51,    51,    51,    51,    52,
    52,    53,    54,    54,    55,    56,    56,    57,    57,    58,
    58,    59,    59,    59,    59,    59,    59,    60,    60
};

static const short yyr2[] = {     0,
     0,     1,     0,     1,     1,     2,     4,     6,     3,     3,
     3,     4,     6,     3,     7,     5,     5,     7,     6,     3,
     0,     0,     9,     1,     1,     1,     2,     1,     3,     3,
     3,     3,     3,     3,     3,     2,     3,     3,     3,     0,
     2,     4,     1,     3,     1,     0,     2,     1,     4,     0,
     2,     1,     1,     1,     1,     1,     1,     0,     2
};

static const short yydefact[] = {     1,
     0,    50,     0,     0,    46,    46,    46,     0,    46,     3,
    48,     2,     5,     0,    46,     0,     0,     0,     0,    48,
     0,    28,     0,    45,     0,     0,     0,     0,     0,     4,
     0,    43,     6,    25,    24,    26,     0,    46,    46,     0,
    57,    54,    56,    55,    53,    52,    58,    51,    46,    36,
     0,     0,     3,     0,     0,     0,     0,     0,     0,     0,
    46,    10,    47,     3,    46,    14,    46,    20,    40,     9,
    11,    46,    27,     0,     0,    49,    46,     0,     0,    39,
    37,     0,    38,    30,    31,    32,    29,    33,    34,    35,
     7,     0,     0,     0,     0,    40,    44,    46,    12,    59,
    21,     3,    17,     3,     0,     0,    16,    41,     0,     0,
     0,     0,     8,    19,     3,    13,    22,    15,    18,    42,
     0,    23,     0,     0,     0
};

static const short yydefgoto[] = {   123,
    29,    30,    13,   110,   121,    39,    21,    95,    96,    31,
    32,    24,    14,    16,    48,    78
};

static const short yypact[] = {    98,
   -33,-32768,   -27,     5,-32768,-32768,-32768,   -18,-32768,    98,
    10,-32768,    98,    76,-32768,   132,   -10,     5,     5,-32768,
     0,    17,    15,    -7,    52,    16,    77,    -3,     1,-32768,
    40,    42,-32768,-32768,-32768,-32768,    50,-32768,-32768,    48,
-32768,-32768,-32768,-32768,-32768,-32768,    49,-32768,-32768,-32768,
     9,     5,    98,     5,    -7,    -7,    -7,    -7,    -7,    -7,
-32768,-32768,-32768,    98,-32768,-32768,-32768,-32768,    59,-32768,
-32768,-32768,-32768,    45,    60,-32768,-32768,    43,    46,-32768,
-32768,    54,    81,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,    80,    84,    55,    58,    59,-32768,-32768,-32768,-32768,
-32768,    98,    64,    98,   119,    91,-32768,-32768,    94,    62,
    67,   119,-32768,-32768,    98,-32768,-32768,-32768,-32768,-32768,
    68,-32768,   101,   110,-32768
};

static const short yypgoto[] = {-32768,
   -47,    41,   -24,-32768,-32768,    44,    -6,    18,-32768,    -8,
    -5,-32768,    19,-32768,-32768,-32768
};


#define	YYLAST		174


static const short yytable[] = {    23,
    25,    26,    68,    28,    52,    82,    40,    18,    15,     1,
    19,    50,    51,    52,    17,    80,    91,    49,   -46,   -46,
    55,     1,    22,    27,    62,    66,   -46,    56,    57,    58,
    59,    60,    74,    75,    20,    69,    22,    22,    53,    54,
    12,    70,    63,    79,    61,    81,    20,    83,    54,    71,
    72,   -46,    34,    33,   111,    90,   113,    35,    93,    92,
    36,    64,    73,    97,    65,    76,    37,   120,    77,    99,
    22,   100,    22,    84,    85,    86,    87,    88,    89,    94,
   114,   101,    67,    34,   102,    52,   112,   119,    35,   104,
   105,    36,   109,     1,   103,     2,   106,    37,   107,   115,
   124,     3,     4,   116,   117,     5,    38,   118,   122,   125,
     7,     8,     9,   108,     1,    10,     2,    98,    11,     0,
     0,     0,     3,     4,     0,     0,     5,     6,     0,     0,
     0,     7,     8,     9,     0,     1,    10,     2,     0,    11,
     0,     0,     0,     3,     4,     0,     0,     5,     0,     0,
     0,     0,     7,     8,     9,    41,     0,    10,    42,     0,
    11,     0,     0,    43,    44,     0,     0,     0,    45,    46,
     0,     0,     0,    47
};

static const short yycheck[] = {     5,
     6,     7,    27,     9,     5,    53,    15,     3,    42,    17,
     6,    18,    19,     5,    42,     7,    64,    28,     9,    10,
     4,    17,     4,    42,    10,    10,    17,    11,    12,    13,
    14,    15,    38,    39,    42,    39,    18,    19,    39,    40,
     0,    41,    24,    49,    28,    52,    42,    54,    40,    10,
     9,    42,     8,    13,   102,    61,   104,    13,    67,    65,
    16,    10,    13,    72,    13,    18,    22,   115,    20,    10,
    52,    77,    54,    55,    56,    57,    58,    59,    60,    21,
   105,    39,     6,     8,    39,     5,    23,   112,    13,    10,
     7,    16,    98,    17,    41,    19,    42,    22,    41,     9,
     0,    25,    26,    10,    43,    29,    31,    41,    41,     0,
    34,    35,    36,    96,    17,    39,    19,    74,    42,    -1,
    -1,    -1,    25,    26,    -1,    -1,    29,    30,    -1,    -1,
    -1,    34,    35,    36,    -1,    17,    39,    19,    -1,    42,
    -1,    -1,    -1,    25,    26,    -1,    -1,    29,    -1,    -1,
    -1,    -1,    34,    35,    36,    24,    -1,    39,    27,    -1,
    42,    -1,    -1,    32,    33,    -1,    -1,    -1,    37,    38,
    -1,    -1,    -1,    42
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */


/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif



/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *yyssp++ = yystate;

  if (yyssp >= yyss + yystacksize)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 2:
{ parse_save( yyvsp[0].parse ); ;
    break;}
case 3:
{ yyval.parse = pnull(); ;
    break;}
case 4:
{ yyval.parse = yyvsp[0].parse; ;
    break;}
case 5:
{ yyval.parse = yyvsp[0].parse; ;
    break;}
case 6:
{ yyval.parse = prules( yyvsp[-1].parse, yyvsp[0].parse ); ;
    break;}
case 7:
{ yyval.parse = plocal( yyvsp[-2].parse, pnull(), yyvsp[0].parse ); ;
    break;}
case 8:
{ yyval.parse = plocal( yyvsp[-4].parse, yyvsp[-2].parse, yyvsp[0].parse ); ;
    break;}
case 9:
{ yyval.parse = yyvsp[-1].parse; ;
    break;}
case 10:
{ yyval.parse = pincl( yyvsp[-1].parse ); ;
    break;}
case 11:
{ yyval.parse = prule( yyvsp[-2].string, yyvsp[-1].parse ); ;
    break;}
case 12:
{ yyval.parse = pset( yyvsp[-3].parse, yyvsp[-1].parse, yyvsp[-2].number ); ;
    break;}
case 13:
{ yyval.parse = pset1( yyvsp[-5].parse, yyvsp[-3].parse, yyvsp[-1].parse, yyvsp[-2].number ); ;
    break;}
case 14:
{ yyval.parse = yyvsp[-1].parse; ;
    break;}
case 15:
{ yyval.parse = pfor( yyvsp[-5].string, yyvsp[-3].parse, yyvsp[-1].parse ); ;
    break;}
case 16:
{ yyval.parse = pswitch( yyvsp[-3].parse, yyvsp[-1].parse ); ;
    break;}
case 17:
{ yyval.parse = pif( yyvsp[-3].parse, yyvsp[-1].parse, pnull() ); ;
    break;}
case 18:
{ yyval.parse = pif( yyvsp[-5].parse, yyvsp[-3].parse, yyvsp[0].parse ); ;
    break;}
case 19:
{ yyval.parse = psetc_args( yyvsp[-4].string, yyvsp[0].parse, yyvsp[-2].parse ); ;
    break;}
case 20:
{ yyval.parse = psetc( yyvsp[-1].string, yyvsp[0].parse ); ;
    break;}
case 21:
{ yymode( SCAN_STRING ); ;
    break;}
case 22:
{ yymode( SCAN_NORMAL ); ;
    break;}
case 23:
{ yyval.parse = psete( yyvsp[-6].string,yyvsp[-5].parse,yyvsp[-2].string,yyvsp[-7].number ); ;
    break;}
case 24:
{ yyval.number = ASSIGN_SET; ;
    break;}
case 25:
{ yyval.number = ASSIGN_APPEND; ;
    break;}
case 26:
{ yyval.number = ASSIGN_DEFAULT; ;
    break;}
case 27:
{ yyval.number = ASSIGN_DEFAULT; ;
    break;}
case 28:
{ yyval.parse = pcnode( COND_EXISTS, yyvsp[0].parse, pnull() ); ;
    break;}
case 29:
{ yyval.parse = pcnode( COND_EQUALS, yyvsp[-2].parse, yyvsp[0].parse ); ;
    break;}
case 30:
{ yyval.parse = pcnode( COND_NOTEQ, yyvsp[-2].parse, yyvsp[0].parse ); ;
    break;}
case 31:
{ yyval.parse = pcnode( COND_LESS, yyvsp[-2].parse, yyvsp[0].parse ); ;
    break;}
case 32:
{ yyval.parse = pcnode( COND_LESSEQ, yyvsp[-2].parse, yyvsp[0].parse ); ;
    break;}
case 33:
{ yyval.parse = pcnode( COND_MORE, yyvsp[-2].parse, yyvsp[0].parse ); ;
    break;}
case 34:
{ yyval.parse = pcnode( COND_MOREEQ, yyvsp[-2].parse, yyvsp[0].parse ); ;
    break;}
case 35:
{ yyval.parse = pcnode( COND_IN, yyvsp[-2].parse, yyvsp[0].parse ); ;
    break;}
case 36:
{ yyval.parse = pcnode( COND_NOT, yyvsp[0].parse, P0 ); ;
    break;}
case 37:
{ yyval.parse = pcnode( COND_AND, yyvsp[-2].parse, yyvsp[0].parse ); ;
    break;}
case 38:
{ yyval.parse = pcnode( COND_OR, yyvsp[-2].parse, yyvsp[0].parse ); ;
    break;}
case 39:
{ yyval.parse = yyvsp[-1].parse; ;
    break;}
case 40:
{ yyval.parse = P0; ;
    break;}
case 41:
{ yyval.parse = pnode( yyvsp[-1].parse, yyvsp[0].parse ); ;
    break;}
case 42:
{ yyval.parse = psnode( yyvsp[-2].string, yyvsp[0].parse ); ;
    break;}
case 43:
{ yyval.parse = pnode( P0, yyvsp[0].parse ); ;
    break;}
case 44:
{ yyval.parse = pnode( yyvsp[0].parse, yyvsp[-2].parse ); ;
    break;}
case 45:
{ yyval.parse = yyvsp[0].parse; yymode( SCAN_NORMAL ); ;
    break;}
case 46:
{ yyval.parse = pnull(); yymode( SCAN_PUNCT ); ;
    break;}
case 47:
{ yyval.parse = pappend( yyvsp[-1].parse, yyvsp[0].parse ); ;
    break;}
case 48:
{ yyval.parse = plist( yyvsp[0].string ); ;
    break;}
case 49:
{ yyval.parse = prule( yyvsp[-2].string, yyvsp[-1].parse ); ;
    break;}
case 50:
{ yyval.number = 0; ;
    break;}
case 51:
{ yyval.number = yyvsp[-1].number | yyvsp[0].number; ;
    break;}
case 52:
{ yyval.number = EXEC_UPDATED; ;
    break;}
case 53:
{ yyval.number = EXEC_TOGETHER; ;
    break;}
case 54:
{ yyval.number = EXEC_IGNORE; ;
    break;}
case 55:
{ yyval.number = EXEC_QUIETLY; ;
    break;}
case 56:
{ yyval.number = EXEC_PIECEMEAL; ;
    break;}
case 57:
{ yyval.number = EXEC_EXISTING; ;
    break;}
case 58:
{ yyval.parse = pnull(); ;
    break;}
case 59:
{ yyval.parse = yyvsp[0].parse; ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */


  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *ssp1++);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *(yyssp - 1);
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *(yyssp - 1))
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp - 1 == yyss) YYABORT;
  yyvsp--;
  yyssp--;
  yystate = *(yyssp - 1);
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *ssp1++);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}


