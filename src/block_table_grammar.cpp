/*=============================================================================
    Copyright (c) 2002 2004  2006Joel de Guzman
    Copyright (c) 2004 Eric Niebler
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <boost/spirit/include/qi_core.hpp>
#include <boost/spirit/include/qi_eps.hpp>
#include <boost/spirit/include/qi_eol.hpp>
#include "grammar_impl.hpp"
#include "block.hpp"
#include "actions.hpp"
#include "misc_rules.hpp"
#include "parse_utils.hpp"

namespace quickbook
{
    namespace qi = boost::spirit::qi;
    namespace ph = boost::phoenix;
    
    struct block_table_grammar_local
    {
        qi::rule<iterator> table_block;
        qi::rule<iterator, quickbook::table()> table;
        qi::rule<iterator, quickbook::table_row()> table_row;
        qi::rule<iterator, quickbook::table_cell()> table_cell;
        qi::rule<iterator> variablelist_block;
        qi::rule<iterator, quickbook::variablelist()> variablelist;
        qi::rule<iterator, quickbook::varlistentry()> varlistentry;
        qi::rule<iterator, quickbook::block_formatted()> varlistterm;
        qi::rule<iterator, quickbook::block_formatted()> varlistitem;

        qi::rule<iterator> nested_cell_check;
    };

    void quickbook_grammar::impl::init_block_table()
    {
        block_table_grammar_local& local = store_.create();

        // Error checking
        
        local.nested_cell_check = &qi::char_("[]");
        local.nested_cell_check.name("nested cell");

        // Table
        
        block_keyword_rules.add("table", &local.table_block);

        local.table_block = local.table[actions.process];

        local.table =
                (&(*qi::blank >> qi::eol) | space)
            >   -(  qi::eps(qbk_since(105u))
                >>  element_id                  [member_assign(&quickbook::table::id)]
                )
            >   (&(*qi::blank >> qi::eol) | space)
            >   qi::raw[*(qi::char_ - eol)]     [member_assign(&quickbook::table::title)]
            >   eol
            >   space
            >   (*local.table_row)              [member_assign(&quickbook::table::rows)]
            ;

        local.table_row
            =   '['
            >   space
            >   local.nested_cell_check
            >   *local.table_cell
            >   ']'
            >   space
            ;

        local.table_cell
            =   '['
            >   inside_paragraph                    [member_assign(&quickbook::block_formatted::content)]
                                                    [member_assign(&quickbook::block_formatted::type, "cell")]
            >   ']'
            >   space
            ;

        local.table.name("table");
        local.table_row.name("table_row");
        local.table_cell.name("table_cell");

        block_keyword_rules.add("variablelist", &local.variablelist_block);
        
        local.variablelist_block = local.variablelist[actions.process];

        local.variablelist =
                (&(*qi::blank >> qi::eol) | space)
            >   qi::raw[*(qi::char_ - eol)]         [member_assign(&quickbook::variablelist::title)]
            >   eol
            >   space
            >   (*local.varlistentry)               [member_assign(&quickbook::variablelist::entries)]
            ;
            
        local.varlistentry
            =   '['
            >   space
            >   local.varlistterm
            >   +local.varlistitem
            >   ']'
            >   space
            ;

        local.varlistterm
            =   '['
            >   phrase                              [member_assign(&quickbook::block_formatted::content)]
                                                    [member_assign(&quickbook::block_formatted::type, "varlistterm")]
            >   ']'
            >   space
            ;

        local.varlistitem
            =   '['
            >   inside_paragraph                    [member_assign(&quickbook::block_formatted::content)]
                                                    [member_assign(&quickbook::block_formatted::type, "varlistitem")]
            >   ']'
            >   space
            ;
    }
}
