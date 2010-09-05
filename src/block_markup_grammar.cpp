/*=============================================================================
    Copyright (c) 2002 2004  2006Joel de Guzman
    Copyright (c) 2004 Eric Niebler
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <boost/spirit/include/qi_core.hpp>
#include <boost/spirit/include/qi_attr.hpp>
#include <boost/spirit/include/qi_eps.hpp>
#include <boost/spirit/include/qi_eol.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include "grammar_impl.hpp"
#include "block.hpp"
#include "actions.hpp"
#include "misc_rules.hpp"
#include "parse_utils.hpp"

namespace quickbook
{
    namespace qi = boost::spirit::qi;
    namespace ph = boost::phoenix;
    
    // Workaround for clang:
    namespace {
        struct dummmy {
            qi::rule<iterator, raw_string()> a1;
        };
    }

    struct block_markup_grammar_local
    {
        qi::rule<iterator, quickbook::block_formatted(formatted_type)> paragraph_block;
        qi::rule<iterator, quickbook::block_formatted()> preformatted_impl;
        qi::rule<iterator, quickbook::def_macro()> def_macro;
        qi::rule<iterator, quickbook::xinclude()> xinclude;
        qi::rule<iterator, quickbook::include()> include;
        qi::rule<iterator, raw_string()> include_id;
        qi::rule<iterator, quickbook::import()> import;
        
        qi::rule<iterator> blurb, warning, caution, important, note, tip;
        qi::rule<iterator> blockquote;
        qi::rule<iterator> preformatted, def, xinclude_block, include_block, import_block;
    };

    void quickbook_grammar::impl::init_block_markup()
    {
        block_markup_grammar_local& local = store_.create();

        // Paragraph Blocks

        block_keyword_rules.add
            ("blurb", &local.blurb)
            ("warning", &local.warning)
            ("caution", &local.caution)
            ("important", &local.important)
            ("note", &local.note)
            ("tip", &local.tip)
            ;

        local.blurb = local.paragraph_block(formatted_type("blurb")) [actions.process];
        local.warning = local.paragraph_block(formatted_type("warning")) [actions.process];
        local.caution = local.paragraph_block(formatted_type("caution")) [actions.process];
        local.important = local.paragraph_block(formatted_type("important")) [actions.process];
        local.note = local.paragraph_block(formatted_type("note")) [actions.process];
        local.tip = local.paragraph_block(formatted_type("tip")) [actions.process];

        block_symbol_rules.add(":", &local.blockquote);
        local.blockquote = local.paragraph_block(formatted_type("blockquote")) [actions.process];
        local.paragraph_block =
                qi::attr(qi::_r1)                   [member_assign(&quickbook::block_formatted::type)]
            >>  space
            >>  inside_paragraph                    [member_assign(&quickbook::block_formatted::content)]
            ;

        // Preformatted

        block_keyword_rules.add("pre", &local.preformatted);
        
        local.preformatted = local.preformatted_impl [actions.process];
        
        local.preformatted_impl =
                space                           [ph::ref(no_eols) = false]
                                                [member_assign(&quickbook::block_formatted::type, "preformatted")]
            >>  -eol
            >>  phrase                          [member_assign(&quickbook::block_formatted::content)]
            >>  qi::eps                         [ph::ref(no_eols) = true]
            ;

        // Define Macro

        block_keyword_rules.add("def", &local.def);
        
        local.def = local.def_macro[actions.process];
        
        local.def_macro =
                space
            >>  macro_identifier                [member_assign(&quickbook::def_macro::macro_identifier)]
            >>  blank
            >>  phrase                          [member_assign(&quickbook::def_macro::content)]
            ;

        // xinclude

        block_keyword_rules.add("xinclude", &local.xinclude_block);

        local.xinclude_block = local.xinclude[actions.process];

        // TODO: Why do these use phrase_end? It doesn't make any sense.
        local.xinclude =
                space
            >>  (*(qi::char_ - phrase_end))         [member_assign(&quickbook::xinclude::path)]
            ;
        
        // Include

        block_keyword_rules.add("include", &local.include_block);
        
        local.include_block = local.include[actions.process];

        local.include =
                space
            >>  -(
                    ':'
                >>  local.include_id
                >>  space
                )                                   [member_assign(&quickbook::include::id)]
            >>  (*(qi::char_ - phrase_end))         [member_assign(&quickbook::include::path)]
            ;

        local.include_id = qi::raw[*(qi::alnum | '_')]    [qi::_val = qi::_1];

        // Import

        block_keyword_rules.add("import", &local.import_block);
        
        local.import_block = local.import[actions.process];
        
        local.import =
                space
            >>  (*(qi::char_ - phrase_end))         [member_assign(&quickbook::import::path)]
            ;
    }
}