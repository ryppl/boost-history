/*  (C) Copyright David Abrahams 2001. Permission to copy, use, modify, sell and
 *  distribute this software is granted provided this copyright notice appears
 *  in all copies. This software is provided "as is" without express or implied
 *  warranty, and with no claim as to its suitability for any purpose.
 */
#include "modules.h"
#include "jam.h"
#include "string.h"
#include "hash.h"
#include "newstr.h"
#include "lists.h"
#include "parse.h"
#include "rules.h"
#include "variable.h"

static struct hash* module_hash = 0;

static char* new_module_str( module* m, char* suffix )
{
    char buffer[4096] = "";
    strncat(buffer, m->name, sizeof(buffer) - 1);
    strncat(buffer, "variables", sizeof(buffer) - 1);
    return newstr( buffer );
}

module* bindmodule( char* name )
{
    char buffer[4096] = "";
    module m_, *m = &m_;
    
    if( !module_hash )
        module_hash = hashinit( sizeof( module ), "modules" );

    if (name)
    {
        strncat( buffer, name, sizeof(buffer) - 1 );
        strncat( buffer, ".", sizeof(buffer) - 1 );
    }
    else
    {
        *buffer = 0;
    }
        
    m->name = buffer;
    
    if ( hashenter( module_hash, (HASHDATA **)&m ) )
    {
        m->name = newstr( m->name );
        m->local_names = 0;
        m->locals = 0;
        m->rules = hashinit( sizeof( RULE ), new_module_str( m, "rules" ) );
    }
    return m;
}

module* root_module()
{
    static module* root = 0;
    if ( !root )
        root = bindmodule(0);
    return root;
}

/*
 * bind_module_var --
 *
 * Add the symbol to the module's list of symbols if it is not already in the
 * module. m is assumed to be the current module and if the symbol is new, any
 * current value is replaced by an empty list until the module is exited.
 *
 */
void bind_module_var( module* m, char* symbol )
{
    char** name = &symbol;
    
    if ( !m->local_names )
        m->local_names = hashinit( sizeof( char* ), new_module_str( m, "variables" ) );
    
    if ( hashenter( m->local_names, (HASHDATA **)&name ) )
    {
        m->locals = addsettings( m->locals, 0, symbol, var_swap( symbol, 0 ) );
    }
}

void enter_module( module* m )
{
    pushsettings( m->locals );
}

void exit_module( module* m )
{
    popsettings( m->locals );
}
