/*
 * Copyright 1993, 2000 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

# include "jam.h"

# include "lists.h"
# include "parse.h"
# include "compile.h"
# include "variable.h"
# include "expand.h"
# include "rules.h"
# include "newstr.h"
# include "make.h"
# include "search.h"
# include "hdrmacro.h"
# include "hash.h"

# include <time.h>
/*
 * compile.c - compile parsed jam statements
 *
 * External routines:
 *
 *	compile_append() - append list results of two statements
 *	compile_foreach() - compile the "for x in y" statement
 *	compile_if() - compile 'if' rule
 *	compile_while() - compile 'while' rule
 *	compile_include() - support for 'include' - call include() on file
 *	compile_list() - expand and return a list 
 *	compile_local() - declare (and set) local variables
 *	compile_null() - do nothing -- a stub for parsing
 *	compile_rule() - compile a single user defined rule
 *	compile_rules() - compile a chain of rules
 *	compile_set() - compile the "set variable" statement
 *	compile_setcomp() - support for `rule` - save parse tree 
 *	compile_setexec() - support for `actions` - save execution string 
 *	compile_settings() - compile the "on =" (set variable on exec) statement
 *	compile_switch() - compile 'switch' rule
 *
 * Internal routines:
 *
 *	debug_compile() - printf with indent to show rule expansion.
 *
 *	evaluate_if() - evaluate if to determine which leg to compile
 *	evaluate_rule() - execute a rule invocation
 *
 *	builtin_depends() - DEPENDS/INCLUDES rule
 *	builtin_echo() - ECHO rule
 *	builtin_exit() - EXIT rule
 *	builtin_flags() - NOCARE, NOTFILE, TEMPORARY rule
 *
 * 02/03/94 (seiwald) -	Changed trace output to read "setting" instead of 
 *			the awkward sounding "settings".
 * 04/12/94 (seiwald) - Combined build_depends() with build_includes().
 * 04/12/94 (seiwald) - actionlist() now just appends a single action.
 * 04/13/94 (seiwald) - added shorthand L0 for null list pointer
 * 05/13/94 (seiwald) - include files are now bound as targets, and thus
 *			can make use of $(SEARCH)
 * 06/01/94 (seiwald) - new 'actions existing' does existing sources
 * 08/23/94 (seiwald) - Support for '+=' (append to variable)
 * 12/20/94 (seiwald) - NOTIME renamed NOTFILE.
 * 01/22/95 (seiwald) - Exit rule.
 * 02/02/95 (seiwald) - Always rule; LEAVES rule.
 * 02/14/95 (seiwald) - NoUpdate rule.
 * 09/11/00 (seiwald) - new evaluate_rule() for headers().
 * 09/11/00 (seiwald) - compile_xxx() now return LIST *.
 *			New compile_append() and compile_list() in
 *			support of building lists here, rather than
 *			in jamgram.yy.
 */

static void debug_compile( int which, char *s );

static int evaluate_if( PARSE *parse, LOL *args );

static LIST *builtin_depends( PARSE *parse, LOL *args );
static LIST *builtin_echo( PARSE *parse, LOL *args );
static LIST *builtin_exit( PARSE *parse, LOL *args );
static LIST *builtin_flags( PARSE *parse, LOL *args );
static LIST *builtin_hdrmacro( PARSE *parse, LOL *args );
LIST *builtin_subst( PARSE  *parse, LOL  *args );

int glob( char *s, char *c );



/*
 * compile_builtin() - define builtin rules
 */

# define P0 (PARSE *)0
# define C0 (char *)0

static void lol_build( LOL* lol, char** elements )
{
    LIST* l = L0;
    lol_init( lol );
    
    while ( elements && *elements )
    {
        if ( !strcmp( *elements, ":" ) )
        {
            lol_add( lol, l );
            l = L0 ;
        }
        else
        {
            l = list_new( l, newstr( *elements ) );
        }
        ++elements;
    }
    
    if ( l != L0 )
        lol_add( lol, l );
}

static RULE* bind_builtin( char* name, LIST*(*f)(PARSE*, LOL*), int num, char** args )
{
    RULE* r = bindrule( name );
    r->procedure = parse_make( f, P0, P0, P0, C0, C0, num );
    if ( args )
    {
        lol_free( &r->arguments );
        lol_build( &r->arguments, args );
    }
    return r;
}

static RULE* duplicate_rule( char* name, RULE* other )
{
    int i;
    
    RULE* r = bindrule( name );
    r->procedure = other->procedure;
    parse_refer( r->procedure );
    
    for (i = 0; i < other->arguments.count; ++i)
    {
        lol_add( &r->arguments,
                 list_copy( L0, other->arguments.list[i] ) );
    }
    return r;
}

void
compile_builtins()
{
    duplicate_rule( "Always", bind_builtin( "ALWAYS", builtin_flags, T_FLAG_TOUCHED, 0 ) );
    duplicate_rule( "Depends", bind_builtin( "DEPENDS", builtin_depends, T_DEPS_DEPENDS, 0 ) );
    duplicate_rule( "Echo", bind_builtin( "ECHO", builtin_echo, 0, 0 ) );
    duplicate_rule( "Exit", bind_builtin( "EXIT", builtin_exit, 0, 0 ) );
    duplicate_rule( "Includes", bind_builtin( "INCLUDES", builtin_depends, T_DEPS_INCLUDES, 0 ) );
    duplicate_rule( "HdrMacro", bind_builtin( "HDRMACRO", builtin_hdrmacro, 0, 0 ) );
    duplicate_rule( "Leaves", bind_builtin( "LEAVES", builtin_flags, T_FLAG_LEAVES, 0 ) );
    duplicate_rule( "NoCare", bind_builtin( "NOCARE", builtin_flags, T_FLAG_NOCARE, 0 ) );
    duplicate_rule( "NOTIME",
        duplicate_rule( "NotFile",
            bind_builtin( "NOTFILE", builtin_flags, T_FLAG_NOTFILE, 0 ) ) );

    duplicate_rule( "NoUpdate", bind_builtin( "NOUPDATE", builtin_flags, T_FLAG_NOUPDATE, 0 ) );
    duplicate_rule( "Temporary", bind_builtin( "TEMPORARY", builtin_flags, T_FLAG_TEMP, 0 ) );

  /* FAIL_EXPECTED is an experimental built-in that is used to indicate        */
  /* that the result of a target build action should be inverted (ok <=> fail) */
  /* this can be useful when performing test runs from Jamfiles..              */
  /*                                                                           */
  /* Beware that this rule might disappear or be renamed in the future..       */
  /* contact david.turner@freetype.org for more details..                      */
    bind_builtin( "FAIL_EXPECTED", builtin_flags, T_FLAG_FAIL_EXPECTED, 0 );

    {
        char* args[] = { "string", "pattern", "replacements", "+", 0 };
        duplicate_rule( "subst", bind_builtin( "SUBST", builtin_subst, 0, args ) );
    }
}

/*
 * compile_append() - append list results of two statements
 *
 *	parse->left	more compile_append() by left-recursion
 *	parse->right	single rule
 */

LIST *
compile_append(
	PARSE	*parse,
	LOL	*args )
{
	/* Append right to left. */

	return list_append( 
		(*parse->left->func)( parse->left, args ),
		(*parse->right->func)( parse->right, args ) );
}

/*
 * compile_foreach() - compile the "for x in y" statement
 *
 * Compile_foreach() resets the given variable name to each specified
 * value, executing the commands enclosed in braces for each iteration.
 *
 *	parse->string	index variable
 *	parse->left	variable values
 *	parse->right	rule to compile
 */

LIST *
compile_foreach(
	PARSE	*parse,
	LOL	*args )
{
	LIST	*nv = (*parse->left->func)( parse->left, args );
	LIST	*l;
	SETTINGS *s = 0;
        
        if ( parse->num )
        {
            s = addsettings( s, 0, parse->string, L0 );
            pushsettings( s );
        }

	/* Call var_set to reset $(parse->string) for each val. */

	for( l = nv; l; l = list_next( l ) )
	{
	    LIST *val = list_new( L0, copystr( l->string ) );

	    var_set( parse->string, val, VAR_SET );

	    list_free( (*parse->right->func)( parse->right, args ) );
	}

        if ( parse->num )
            popsettings( s );

	list_free( nv );

	return L0;
}

/*
 * compile_if() - compile 'if' rule
 *
 *	parse->left		condition tree
 *	parse->right		then tree
 *	parse->third		else tree
 */

LIST *
compile_if(
	PARSE	*p,
	LOL	*args )
{
	if( evaluate_if( p->left, args ) )
	{
	    return (*p->right->func)( p->right, args );
	}
	else
	{
	    return (*p->third->func)( p->third, args );
	}
}

LIST *
compile_while(
	PARSE	*p,
	LOL	*args )
{
    while ( evaluate_if( p->left, args ) )
    {
        list_free( (*p->right->func)( p->right, args ) );
    }
    return L0;
}

/*
 * evaluate_if() - evaluate if to determine which leg to compile
 *
 * Returns:
 *	!0	if expression true - compile 'then' clause
 *	0	if expression false - compile 'else' clause
 */

static int
evaluate_if(
	PARSE	*parse,
	LOL	*args )
{
	int	status;

	if( parse->num <= COND_OR )
	{
	    /* Handle one of the logical operators */

	    switch( parse->num )
	    {
	    case COND_NOT:
		status = !evaluate_if( parse->left, args );
		break;

	    case COND_AND:
		status = evaluate_if( parse->left, args ) &&
			 evaluate_if( parse->right, args );
		break;

	    case COND_OR:
		status = evaluate_if( parse->left, args ) ||
			 evaluate_if( parse->right, args );
		break;

	    default:
		status = 0;	/* can't happen */
	    }
	}
	else
	{
	    /* Handle one of the comparison operators */
	    /* Expand targets and sources */

	    LIST *nt = (*parse->left->func)( parse->left, args );
	    LIST *ns = (*parse->right->func)( parse->right, args );

	    /* "a in b" make sure each of a is equal to something in b. */
	    /* Otherwise, step through pairwise comparison. */

	    if( parse->num == COND_IN )
	    {
		LIST *s, *t;

		/* Try each t until failure. */

		for( status = 1, t = nt; status && t; t = list_next( t ) )
		{
		    int stat1;

		    /* Try each s until success */

		    for( stat1 = 0, s = ns; !stat1 && s; s = list_next( s ) )
			stat1 = !strcmp( t->string, s->string );

		    status = stat1;
		}
	    }
	    else
	    {
		LIST *s = ns, *t = nt;

		status = 0;

		while( !status && ( t || s ) )
		{
		    char *st = t ? t->string : "";
		    char *ss = s ? s->string : "";

		    status = strcmp( st, ss );

		    t = t ? list_next( t ) : t;
		    s = s ? list_next( s ) : s;
		}
	    }

	    switch( parse->num )
	    {
	    case COND_EXISTS:	status = status > 0 ; break;
	    case COND_EQUALS:	status = !status; break;
	    case COND_NOTEQ:	status = status != 0; break;
	    case COND_LESS:	status = status < 0; break;
	    case COND_LESSEQ:	status = status <= 0; break;
	    case COND_MORE:	status = status > 0; break;
	    case COND_MOREEQ:	status = status >= 0; break;
	    case COND_IN:	/* status = status */ break;
	    }

	    if( DEBUG_IF )
	    {
		debug_compile( 0, "if" );
		list_print( nt );
		printf( "(%d)", status );
		list_print( ns );
		printf( "\n" );
	    }

	    list_free( nt );
	    list_free( ns );

	}

	return status;
}

/*
 * compile_include() - support for 'include' - call include() on file
 *
 * 	parse->left	list of files to include (can only do 1)
 */

LIST *
compile_include(
	PARSE	*parse,
	LOL	*args )
{
	LIST	*nt = (*parse->left->func)( parse->left, args );

	if( DEBUG_COMPILE )
	{
	    debug_compile( 0, "include" );
	    list_print( nt );
	    printf( "\n" );
	}

	if( nt )
	{
	    TARGET *t = bindtarget( nt->string );

	    /* Bind the include file under the influence of */
	    /* "on-target" variables.  Though they are targets, */
	    /* include files are not built with make(). */

	    pushsettings( t->settings );
	    t->boundname = search( t->name, &t->time );
	    popsettings( t->settings );

	    parse_file( t->boundname );
	}

	list_free( nt );

	return L0;
}

/*
 * compile_list() - expand and return a list 
 *
 * 	parse->string - character string to expand
 */

LIST *
compile_list(
	PARSE	*parse,
	LOL	*args )
{
	/* voodoo 1 means: s is a copyable string */
	char *s = parse->string;
	return var_expand( L0, s, s + strlen( s ), args, 1 );
}

/*
 * compile_local() - declare (and set) local variables
 *
 *	parse->left	list of variables
 *	parse->right	list of values
 *	parse->third	rules to execute
 */

LIST *
compile_local(
	PARSE	*parse,
	LOL	*args )
{
	LIST *l;
	SETTINGS *s = 0;
	LIST	*nt = (*parse->left->func)( parse->left, args );
	LIST	*ns = (*parse->right->func)( parse->right, args );
	LIST	*result;

	if( DEBUG_COMPILE )
	{
	    debug_compile( 0, "local" );
	    list_print( nt );
	    printf( " = " );
	    list_print( ns );
	    printf( "\n" );
	}

	/* Initial value is ns */

	for( l = nt; l; l = list_next( l ) )
	    s = addsettings( s, 0, l->string, list_copy( (LIST*)0, ns ) );

	list_free( ns );
	list_free( nt );

	/* Note that callees of the current context get this "local" */
	/* variable, making it not so much local as layered. */

	pushsettings( s );
	result = (*parse->third->func)( parse->third, args );
	popsettings( s );

	freesettings( s );

	return result;
}

/*
 * compile_null() - do nothing -- a stub for parsing
 */

LIST *
compile_null(
	PARSE	*parse,
	LOL	*args )
{
	return L0;
}

/*
 * compile_rule() - compile a single user defined rule
 *
 *	parse->string	name of user defined rule
 *	parse->left	parameters (list of lists) to rule, recursing left
 *
 * Wrapped around evaluate_rule() so that headers() can share it.
 */

LIST *
compile_rule(
	PARSE	*parse,
	LOL	*args )
{
	LOL	nargs[1];
	LIST	*result;
	PARSE	*p;

	/* Build up the list of arg lists */

	lol_init( nargs );

	for( p = parse->left; p; p = p->left )
	    lol_add( nargs, (*p->right->func)( p->right, args ) );

	/* And invoke rule */

	result = evaluate_rule( parse->string, nargs );

	lol_free( nargs );

	return result;
}

static void argument_error( char* message, RULE* rule, LOL* actual, LIST* arg )
{
    printf( "### argument error\n# rule %s ( ", rule->name );
    lol_print( &rule->arguments );
    printf( ")\n# called with: ( " );
    lol_print( actual );
    printf( ")\n# %s %s", message, arg ? arg->string : "" );
    printf("\n");
    exit(1);
}


/*
 * collect_arguments() - local argument checking and collection
 */
static SETTINGS *
collect_arguments( RULE* rule, LOL* all_actual )
{
    SETTINGS *locals = 0;
    
    LOL *all_formal = &rule->arguments;
    if ( all_formal->count >= 0 ) /* Nothing to set; nothing to check */
    {
        int max = all_formal->count > all_actual->count
            ? all_formal->count
            : all_actual->count;
        
        int n;
        for ( n = 0; n < max ; ++n )
        {
            LIST *formal = lol_get( all_formal, n );
            LIST *actual = lol_get( all_actual, n );
            while ( formal )
            {
                char* name = formal->string;
                char modifier = 0;
                LIST* value = 0;
                if ( formal->next )
                {
                    char *next = formal->next->string;
                    if ( next && next[0] != 0 && next[1] == 0 )
                        modifier = next[0];
                }
                
                if ( !actual && modifier != '?' && modifier != '*' )
                {
                    argument_error( "missing argument", rule, all_actual, formal );
                }

                switch ( modifier )
                {
                case '+':
                case '*':
                    value = list_copy( 0, actual );
                    actual = 0;
                    /* skip an extra element for the modifier */
                    formal = formal->next; 
                    break;
                case '?':
                    /* skip an extra element for the modifier */
                    formal = formal->next; 
                    /* fall through */
                default:
                    if ( actual ) /* in case actual is missing */
                    {
                        value = list_new( 0, actual->string );
                        actual = actual->next;
                    }
                }
                
                locals = addsettings( locals, 0, name, value );
                formal = formal->next;
            }
            
            if ( actual )
            {
                argument_error( "extra argument", rule, all_actual, actual );
            }
        }
    }
    return locals;
}

struct profile_info
{
    char* name;                 /* name of rule being called */
    clock_t cumulative;         /* cumulative time spent in rule */
    clock_t subrules;           /* time spent in subrules */
    unsigned long num_entries;  /* number of time rule was entered */
};
typedef struct profile_info profile_info;

struct profile_frame
{
    profile_info* info;               /* permanent storage where data accumulates */
    clock_t overhead;                 /* overhead for profiling in this call */
    clock_t entry_time;               /* time of last entry to rule */
    struct profile_frame* caller;     /* stack frame of caller */
};
typedef struct profile_frame profile_frame;

static profile_frame* profile_stack = 0;
static struct hash* profile_hash = 0;

static void profile_enter(char* rulename, profile_frame* frame)
{
    clock_t start = clock();
    profile_info info, *p = &info;
    
    if ( !profile_hash )
        profile_hash = hashinit(sizeof(profile_info), "profile");

    info.name = rulename;
    
    if ( hashenter( profile_hash, (HASHDATA **)&p ) )
        p->cumulative = p->subrules = p->num_entries = 0;
    
    ++(p->num_entries);
    
    frame->info = p;
    
    frame->caller = profile_stack;
    profile_stack = frame;

    frame->entry_time = clock();
    frame->overhead = 0;

    /* caller pays for the time it takes to play with the hash table */
    if ( frame->caller )
        frame->caller->overhead += frame->entry_time - start;
}
    
static void profile_exit(profile_frame* frame)
{
    /* cumulative time for this call */
    clock_t t = clock() - frame->entry_time - frame->overhead;
    frame->info->cumulative += t;
    
    if (frame->caller)
    {
        /* caller's cumulative time must account for this overhead */
        frame->caller->overhead += frame->overhead;
        frame->caller->info->subrules += t;
    }
    /* pop this stack frame */
    profile_stack = frame->caller;
}

static void dump_profile_entry(void* p_)
{
    profile_info* p = p_;
    clock_t total = p->cumulative;
    printf("%10d %10d %10d %s\n", total, total - p->subrules, p->num_entries, p->name);
}

void profile_dump()
{
    if ( profile_hash )
        hashenumerate( profile_hash, dump_profile_entry );
}

/*
 * evaluate_rule() - execute a rule invocation
 */

LIST *
evaluate_rule(
	char	*rulename,
	LOL	*args )
{
    LIST	  *result = L0;
    RULE          *rule;
    profile_frame frame;

    /* special case, if the rulename begins with "$(", we try */
    /* to expand it.. this is needed by Boost..               */
    if ( rulename[0] == '$' && rulename[1] == '(' )
    {
        LIST*  l;
        
        l = var_expand( L0, rulename, rulename+strlen(rulename), args, 0 );

        if ( !l )
        {
            printf( "warning: unknown rule %s\n", rulename );
            return result;
        }
            
        if ( DEBUG_COMPILE )
        {
            debug_compile( 1, l->string );
            lol_print( args );
            printf( "\n" );
        }
        rulename = l->string;
        rule = bindrule( l->string );
        
        list_free( l );
    }
    else
    {
        rule = bindrule( rulename );

        if( DEBUG_COMPILE )
        {
	    debug_compile( 1, rulename );
	    lol_print( args );
	    printf( "\n" );
        }
    }
    
    
    if ( DEBUG_PROFILE )
        profile_enter( rulename, &frame );

    /* Check traditional targets $(<) and sources $(>) */

    if( !rule->actions && !rule->procedure )
        printf( "warning: unknown rule %s\n", rule->name );

    /* If this rule will be executed for updating the targets */
    /* then construct the action for make(). */

    if( rule->actions )
    {
        TARGETS	*t;
        ACTION	*action;

        /* The action is associated with this instance of this rule */

        action = (ACTION *)malloc( sizeof( ACTION ) );
        memset( (char *)action, '\0', sizeof( *action ) );

        action->rule = rule;
        action->targets = targetlist( (TARGETS *)0, lol_get( args, 0 ) );
        action->sources = targetlist( (TARGETS *)0, lol_get( args, 1 ) );

        /* Append this action to the actions of each target */

        for( t = action->targets; t; t = t->next )
            t->target->actions = actionlist( t->target->actions, action );
    }

    /* Now recursively compile any parse tree associated with this rule */
    /* refer/free to ensure rule not freed during use */

    if( rule->procedure )
    {
        SETTINGS *local_args = collect_arguments( rule, args );
        PARSE *parse = rule->procedure;
        parse_refer( parse );
        
        pushsettings( local_args );
        result = (*parse->func)( parse, args );
        popsettings( local_args );
        
        parse_free( parse );
    }

    if ( DEBUG_PROFILE )
        profile_exit( &frame );

    if( DEBUG_COMPILE )
        debug_compile( -1, 0 );

    return result;
}

/*
 * compile_rules() - compile a chain of rules
 *
 *	parse->left	more compile_rules() by left-recursion
 *	parse->right	single rule
 */

LIST *
compile_rules(
	PARSE	*parse,
	LOL	*args )
{
	/* Ignore result from first statement; return the 2nd. */

	list_free( (*parse->left->func)( parse->left, args ) );
	return (*parse->right->func)( parse->right, args );
}

/*
 * compile_set() - compile the "set variable" statement
 *
 *	parse->left	variable names
 *	parse->right	variable values 
 *	parse->num	ASSIGN_SET/APPEND/DEFAULT
 */

LIST *
compile_set(
	PARSE	*parse,
	LOL	*args )
{
	LIST	*nt = (*parse->left->func)( parse->left, args );
	LIST	*ns = (*parse->right->func)( parse->right, args );
	LIST	*l;
	int	setflag;
	char	*trace;

	switch( parse->num )
	{
	case ASSIGN_SET:	setflag = VAR_SET; trace = "="; break;
	case ASSIGN_APPEND:	setflag = VAR_APPEND; trace = "+="; break;
	case ASSIGN_DEFAULT:	setflag = VAR_DEFAULT; trace = "?="; break;
	default:		setflag = VAR_SET; trace = ""; break;
	}

	if( DEBUG_COMPILE )
	{
	    debug_compile( 0, "set" );
	    list_print( nt );
	    printf( " %s ", trace );
	    list_print( ns );
	    printf( "\n" );
	}

	/* Call var_set to set variable */
	/* var_set keeps ns, so need to copy it */

	for( l = nt; l; l = list_next( l ) )
	    var_set( l->string, list_copy( L0, ns ), setflag );

	list_free( nt );

	return ns;
}

/*
 * compile_setcomp() - support for `rule` - save parse tree 
 *
 *	parse->string	rule name
 *	parse->left	rules for rule
 *  parse->right optional list-of-lists describing arguments
 */

LIST *
compile_setcomp(
	PARSE	*parse,
	LOL	*args )
{
	RULE	*rule = bindrule( parse->string );

	/* Free old one, if present */

	if( rule->procedure )
	    parse_free( rule->procedure );

    if( rule->arguments.count >= 0 )
    {
        lol_free( &rule->arguments );
        rule->arguments.count = -1;
    }

    /* Create new LOL describing argument requirements if supplied */
    if (parse->right)
    {
        PARSE *p;
        lol_init( &rule->arguments );
        for( p = parse->right; p; p = p->left )
            lol_add( &rule->arguments, (*p->right->func)( p->right, args ) );
        
    }

	rule->procedure = parse->left;

	/* we now own this parse tree */
	/* don't let parse_free() release it */

	parse->left = 0;	

	return L0;
}

/*
 * compile_setexec() - support for `actions` - save execution string 
 *
 *	parse->string	rule name
 *	parse->string1	OS command string
 *	parse->num	flags
 *	parse->left	`bind` variables
 *
 * Note that the parse flags (as defined in compile.h) are transfered
 * directly to the rule flags (as defined in rules.h).
 */

LIST *
compile_setexec(
	PARSE	*parse,
	LOL	*args )
{
	RULE	*rule = bindrule( parse->string );
	LIST	*bindlist = (*parse->left->func)( parse->left, args );
	
	/* Free old one, if present */

	if( rule->actions )
	{
	    freestr( rule->actions );
	    list_free( rule->bindlist );
	}

	rule->actions = copystr( parse->string1 );
	rule->bindlist = bindlist;
	rule->flags = parse->num; /* XXX translate this properly */

	return L0;
}

/*
 * compile_settings() - compile the "on =" (set variable on exec) statement
 *
 *	parse->left	variable names
 *	parse->right	target name 
 *	parse->third	variable value 
 *	parse->num	ASSIGN_SET/APPEND	
 */

LIST *
compile_settings(
	PARSE	*parse,
	LOL	*args )
{
	LIST	*nt = (*parse->left->func)( parse->left, args );
	LIST	*ns = (*parse->third->func)( parse->third, args );
	LIST	*targets = (*parse->right->func)( parse->right, args );
	LIST	*ts;
	int	append = parse->num == ASSIGN_APPEND;

	if( DEBUG_COMPILE )
	{
	    debug_compile( 0, "set" );
	    list_print( nt );
	    printf( "on " );
	    list_print( targets );
	    printf( " %s ", append ? "+=" : "=" );
	    list_print( ns );
	    printf( "\n" );
	}

	/* Call addsettings to save variable setting */
	/* addsettings keeps ns, so need to copy it */
	/* Pass append flag to addsettings() */

	for( ts = targets; ts; ts = list_next( ts ) )
	{
	    TARGET 	*t = bindtarget( ts->string );
	    LIST	*l;

	    for( l = nt; l; l = list_next( l ) )
		t->settings = addsettings( t->settings, append, 
				l->string, list_copy( (LIST*)0, ns ) );
	}

	list_free( nt );
	list_free( targets );

	return ns;
}

/*
 * compile_switch() - compile 'switch' rule
 *
 *	parse->left	switch value (only 1st used)
 *	parse->right	cases
 *
 *	cases->left	1st case
 *	cases->right	next cases
 *
 *	case->string	argument to match
 *	case->left	parse tree to execute
 */

LIST *
compile_switch(
	PARSE	*parse,
	LOL	*args )
{
	LIST	*nt = (*parse->left->func)( parse->left, args );
	LIST	*result = 0;

	if( DEBUG_COMPILE )
	{
	    debug_compile( 0, "switch" );
	    list_print( nt );
	    printf( "\n" );
	}

	/* Step through cases */

	for( parse = parse->right; parse; parse = parse->right )
	{
	    if( !glob( parse->left->string, nt ? nt->string : "" ) )
	    {
		/* Get & exec parse tree for this case */
		parse = parse->left->left;
		result = (*parse->func)( parse, args );
		break;
	    }
	}

	list_free( nt );

	return result;
}



/*
 * builtin_depends() - DEPENDS/INCLUDES rule
 *
 * The DEPENDS builtin rule appends each of the listed sources on the 
 * dependency list of each of the listed targets.  It binds both the 
 * targets and sources as TARGETs.
 */

static LIST *
builtin_depends(
	PARSE	*parse,
	LOL	*args )
{
	LIST *targets = lol_get( args, 0 );
	LIST *sources = lol_get( args, 1 );
	int which = parse->num;
	LIST *l;

	for( l = targets; l; l = list_next( l ) )
	{
	    TARGET *t = bindtarget( l->string );
	    t->deps[ which ] = targetlist( t->deps[ which ], sources );
	}

	return L0;
}

/*
 * builtin_echo() - ECHO rule
 *
 * The ECHO builtin rule echoes the targets to the user.  No other 
 * actions are taken.
 */

static LIST *
builtin_echo(
	PARSE	*parse,
	LOL	*args )
{
	list_print( lol_get( args, 0 ) );
	printf( "\n" );
	return L0;
}

/*
 * builtin_exit() - EXIT rule
 *
 * The EXIT builtin rule echoes the targets to the user and exits
 * the program with a failure status.
 */

static LIST *
builtin_exit(
	PARSE	*parse,
	LOL	*args )
{
	list_print( lol_get( args, 0 ) );
	printf( "\n" );
	exit( EXITBAD ); /* yeech */
	return L0;
}

/*
 * builtin_flags() - NOCARE, NOTFILE, TEMPORARY rule
 *
 * Builtin_flags() marks the target with the appropriate flag, for use
 * by make0().  It binds each target as a TARGET.
 */

static LIST *
builtin_flags(
	PARSE	*parse,
	LOL	*args )
{
	LIST *l = lol_get( args, 0 );

	for( ; l; l = list_next( l ) )
	    bindtarget( l->string )->flags |= parse->num;

	return L0;
}


static LIST *
builtin_hdrmacro(
    PARSE    *parse,
    LOL      *args )
{
  LIST*  l = lol_get( args, 0 );
  
  for ( ; l; l = list_next(l) )
  {
    TARGET*  t = bindtarget( l->string );

    /* scan file for header filename macro definitions */    
    if ( DEBUG_HEADER )
      printf( "scanning '%s' for header file macro definitions\n",
              l->string );

    macro_headers( t );
  }
  
  return L0;
}

/*
 * debug_compile() - printf with indent to show rule expansion.
 */

static void
debug_compile( int which, char *s )
{
	static int level = 0;
	static char indent[36] = ">>>>|>>>>|>>>>|>>>>|>>>>|>>>>|>>>>|";
	
    if ( which >= 0 )
    {
      int i;
      
      i = (level+1)*2;
      while ( i > 35 )
      {
        printf( indent );
        i -= 35;
      }
      printf( "%*.*s ", i, i, indent );
    }

	if( s )
	    printf( "%s ", s );

	level += which;
}
