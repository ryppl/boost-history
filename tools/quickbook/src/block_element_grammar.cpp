/*=============================================================================
    Copyright (c) 2002 2004  2006Joel de Guzman
    Copyright (c) 2004 Eric Niebler
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "utils.hpp"
#include "actions_class.hpp"
#include "grammar_impl.hpp"
#include "block_tags.hpp"
#include "template_tags.hpp"
#include <boost/spirit/include/classic_assign_actor.hpp>
#include <boost/spirit/include/classic_if.hpp>
#include <boost/spirit/include/classic_clear_actor.hpp>
#include <boost/spirit/include/phoenix1_primitives.hpp>

namespace quickbook
{
    namespace cl = boost::spirit::classic;
    namespace ph = phoenix;

    struct block_element_grammar_local
    {
        cl::rule<scanner>
                        heading,
                        blurb, blockquote,
                        warning, caution, important, note, tip,
                        inner_phrase, def_macro,
                        table, table_row, variablelist,
                        varlistentry, varlistterm, varlistitem, table_cell,
                        preformatted, begin_section, end_section,
                        xinclude, include,
                        template_, template_id, template_formal_arg,
                        template_body, identifier, import,
                        element_id, element_id_1_5, element_id_1_6;
    };

    void quickbook_grammar::impl::init_block_elements()
    {
        using detail::var;

        block_element_grammar_local& local = store_.create();

        local.element_id =
            !(  ':'
            >>  (   cl::if_p(qbk_since(105u)) [space]
                >>  (+(cl::alnum_p | '_'))      [actions.values.entry(ph::arg1, ph::arg2, general_tags::element_id)]
                |   cl::eps_p                   [actions.element_id_warning]
                )
            )
            ;
        
        local.element_id_1_5 =
                cl::if_p(qbk_since(105u)) [
                    local.element_id
                ]
                ;

        local.element_id_1_6 =
                cl::if_p(qbk_since(106u)) [
                    local.element_id
                ]
                ;

        elements.add
            ("section", element_info(element_info::block, &local.begin_section))
            ("endsect", element_info(element_info::block, &local.end_section))
            ;

        local.begin_section =
            actions.values.scoped(block_tags::begin_section)
            [   space
            >>  local.element_id
            >>  space
            >>  local.inner_phrase
            ]                                   [actions.begin_section]
            ;

        local.end_section =
                cl::eps_p                       [actions.values.entry(ph::arg1, ph::arg2, block_tags::end_section)]
                                                [actions.end_section]
            ;

        local.heading
            =   actions.values.scoped
                [   space
                >>  local.element_id_1_6
                >>  space
                >>  local.inner_phrase
                ]
            ;

        elements.add
            ("heading", element_info(element_info::block, &local.heading, block_tags::generic_heading))
            ("h1", element_info(element_info::block, &local.heading, block_tags::heading1))
            ("h2", element_info(element_info::block, &local.heading, block_tags::heading2))
            ("h3", element_info(element_info::block, &local.heading, block_tags::heading3))
            ("h4", element_info(element_info::block, &local.heading, block_tags::heading4))
            ("h5", element_info(element_info::block, &local.heading, block_tags::heading5))
            ("h6", element_info(element_info::block, &local.heading, block_tags::heading6))
            ;

        elements.add("blurb", element_info(element_info::block, &local.blurb));

        local.blurb =
            inside_paragraph                    [actions.blurb]
            ;

        elements.add
            (":", element_info(element_info::block, &local.blockquote))
            ;

        local.blockquote =
            blank >> inside_paragraph           [actions.blockquote]
            ;

        elements.add
            ("warning", element_info(element_info::block, &local.warning))
            ("caution", element_info(element_info::block, &local.caution))
            ("important", element_info(element_info::block, &local.important))
            ("note", element_info(element_info::block, &local.note))
            ("tip", element_info(element_info::block, &local.tip))
            ;

        local.warning =
            inside_paragraph                    [actions.warning]
            ;

        local.caution =
            inside_paragraph                    [actions.caution]
            ;

        local.important =
            inside_paragraph                    [actions.important]
            ;

        local.note =
            inside_paragraph                    [actions.note]
            ;

        local.tip =
            inside_paragraph                    [actions.tip]
            ;

        elements.add
            ("pre", element_info(element_info::block, &local.preformatted))
            ;

        local.preformatted =
                space
            >>  !eol
            >>  actions.set_no_eols[phrase]     [actions.preformatted]
            ;

        elements.add
            ("def", element_info(element_info::block, &local.def_macro))
            ;

        local.def_macro =
               space
            >> macro_identifier                 [actions.macro_identifier]
            >> blank >> phrase                  [actions.macro_definition]
            ;

        local.identifier =
            (cl::alpha_p | '_') >> *(cl::alnum_p | '_')
            ;

        local.template_id =
            local.identifier | (cl::punct_p - (cl::ch_p('[') | ']'))
            ;

        elements.add
            ("template", element_info(element_info::block, &local.template_))
            ;

        local.template_ =
               space
            >> local.template_id                [actions.values.reset()]
                                                [actions.values.entry(ph::arg1, ph::arg2)]
            >> actions.values.scoped[
            !(
                space >> '['
                >> *(
                        space
                    >>  local.template_id       [actions.values.entry(ph::arg1, ph::arg2)]
                    )
                >> space >> ']'
            )
            ]
            >>  (   cl::eps_p(*cl::blank_p >> cl::eol_p)
                >>  local.template_body         [actions.values.entry(ph::arg1, ph::arg2, template_tags::block)]
                |   local.template_body         [actions.values.entry(ph::arg1, ph::arg2, template_tags::phrase)]
                )                               [actions.template_body]
            ;

        local.template_body =
           *(('[' >> local.template_body >> ']') | (cl::anychar_p - ']'))
            >> cl::eps_p(space >> ']')
            >> space
            ;

        elements.add
            ("variablelist", element_info(element_info::block, &local.variablelist))
            ;

        local.variablelist =
                (cl::eps_p(*cl::blank_p >> cl::eol_p) | space)
            >>  (*(cl::anychar_p - eol))        [actions.values.entry(ph::arg1, ph::arg2, table_tags::title)]
            >>  (+eol)                          [actions.output_pre]
            >>  *local.varlistentry
            >>  cl::eps_p                       [actions.variablelist]
            ;

        local.varlistentry =
            space
            >>  cl::ch_p('[')
            >>  actions.values.scoped
            [
                (
                    local.varlistterm
                    >>  (   +local.varlistitem
                        |   cl::eps_p           [actions.error]
                        )
                    >>  cl::ch_p(']')
                    >>  space
                )
                | cl::eps_p                     [actions.error]
            ]
            ;

        local.varlistterm =
            space
            >>  cl::ch_p('[')
            >>  actions.values.save
                [   phrase
                >>  cl::ch_p(']')
                >>  space
                |   cl::eps_p                   [actions.error]
                ]                               [actions.phrase_value]                
            ;

        local.varlistitem =
            space
            >>  cl::ch_p('[')
            >>  (   inside_paragraph
                >>  cl::ch_p(']')
                >>  space
                |   cl::eps_p                   [actions.error]
                )
            ;

        elements.add
            ("table", element_info(element_info::block, &local.table))
            ;

        local.table =
                (cl::eps_p(*cl::blank_p >> cl::eol_p) | space)
            >>  local.element_id_1_5
            >>  (cl::eps_p(*cl::blank_p >> cl::eol_p) | space)
            >>  (*(cl::anychar_p - eol))        [actions.values.entry(ph::arg1, ph::arg2, table_tags::title)]
            >>  (+eol)                          [actions.output_pre]
            >>  *local.table_row
            >>  cl::eps_p                       [actions.table]
            ;

        local.table_row =
            space
            >>  cl::ch_p('[')
            >>
            (
                (
                    actions.values.scoped(table_tags::row)[*local.table_cell]
                    >>  cl::ch_p(']')
                    >>  space
                )
                | cl::eps_p                     [actions.error]
            )
            ;

        local.table_cell =
                space
            >>  cl::ch_p('[')
            >>  (   cl::eps_p                   [actions.values.tag(table_tags::cell)]
                >>  inside_paragraph
                >>  cl::ch_p(']')
                >>  space
                | cl::eps_p                     [actions.error]
                )
            ;

        elements.add
            ("xinclude", element_info(element_info::conditional_or_block, &local.xinclude))
            ("import", element_info(element_info::conditional_or_block, &local.import))
            ("include", element_info(element_info::conditional_or_block, &local.include))
            ;

        local.xinclude =
               space
            >> (*(cl::anychar_p - phrase_end))
                                                [actions.xinclude]
            ;

        local.import =
               space
            >> (*(cl::anychar_p - phrase_end))
                                                [actions.import]
            ;

        local.include =
               space
            >>
           !(
                ':'
                >> (*((cl::alnum_p | '_') - cl::space_p))
                                                [cl::assign_a(actions.include_doc_id)]
                >> space
            )
            >> (*(cl::anychar_p - phrase_end))
                                                [actions.include]
            ;

        local.inner_phrase =
            actions.values.save
            [   cl::eps_p                       [actions.inner_phrase_pre]
            >>  phrase
            >>  cl::eps_p                       [actions.inner_phrase_post]
            ]                                   [actions.docinfo_value(ph::arg1, ph::arg2)]
            ;
    }
}
