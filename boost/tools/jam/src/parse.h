/*
 * Copyright 1993, 2000 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

#ifndef PARSE_DWA20011020_H
# define PARSE_DWA20011020_H
/*
 * parse.h - make and destroy parse trees as driven by the parser
 */

/*
 * parse tree node
 */

typedef struct _PARSE PARSE;
typedef struct module module;
typedef struct frame FRAME;

struct _PARSE {
    LIST    *(*func)( PARSE *p, FRAME *frame );
    PARSE   *left;
    PARSE   *right;
    PARSE   *third;
    char    *string;
    char    *string1;
    int num;
    int refs;
    module* module;
    char* rulename;
} ;

void    parse_file( char *f, FRAME* frame );
void    parse_save( PARSE *p );

PARSE * parse_make( 
    LIST    *(*func)( PARSE *p, FRAME* frame ),
    PARSE   *left,
    PARSE   *right,
    PARSE   *third,
    char    *string,
    char    *string1,
    int num );

void    parse_refer( PARSE *p );
void    parse_free( PARSE *p );

#endif // PARSE_DWA20011020_H
