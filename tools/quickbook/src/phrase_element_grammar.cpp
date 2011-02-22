/*=============================================================================
    Copyright (c) 2002 2004 2006 Joel de Guzman
    Copyright (c) 2004 Eric Niebler
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "grammar_impl.hpp"
#include "actions_class.hpp"
#include "utils.hpp"
#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_assign_actor.hpp>
#include <boost/spirit/include/classic_clear_actor.hpp>
#include <boost/spirit/include/classic_if.hpp>

namespace quickbook
{
    namespace cl = boost::spirit::classic;

    struct phrase_element_grammar_local
    {
        cl::rule<scanner>
                        image,
                        bold, italic, underline, teletype,
                        strikethrough, url, funcref, classref,
                        memberref, enumref, macroref, headerref, conceptref, globalref,
                        anchor, link,
                        source_mode_cpp, source_mode_python, source_mode_teletype,
                        quote, footnote, replaceable,
                        cond_phrase, inner_phrase
                        ;
    };

    void quickbook_grammar::impl::init_phrase_elements()
    {
        using detail::var;

        phrase_element_grammar_local& local = store_.create();

        elements.add
            ("?", element_info(element_info::phrase, &local.cond_phrase))
            ;

        local.cond_phrase =
                blank
            >>  macro_identifier                [actions.values.entry]
            >>  actions.scoped_cond_phrase[extended_phrase]
            ;

        elements.add
            ("$", element_info(element_info::phrase, &local.image))
            ;

        local.image =
                blank
            >>  cl::if_p(qbk_since(105u)) [
                        (+(
                            *cl::space_p
                        >>  +(cl::anychar_p - (cl::space_p | phrase_end | '['))
                        ))                      [actions.values.entry]
                    >>  hard_space
                    >>  *actions.values.scoped[
                            '['
                        >>  (*(cl::alnum_p | '_')) 
                                                [actions.values.entry]
                        >>  space
                        >>  (*(cl::anychar_p - (phrase_end | '[')))
                                                [actions.values.entry]
                        >>  ']'
                        >>  space
                        ]
                ].else_p [
                        (*(cl::anychar_p - phrase_end))
                                                [actions.values.entry]
                ]
            >>  cl::eps_p(']')                  [actions.image]
            ;
            
        elements.add
            ("@", element_info(element_info::phrase, &local.url))
            ;

        local.url =
                (*(cl::anychar_p -
                    (']' | hard_space)))  [actions.url_pre]
            >>  hard_space
            >>  phrase                          [actions.url_post]
            ;

        elements.add
            ("link", element_info(element_info::phrase, &local.link))
            ;

        local.link =
                space
            >>  (*(cl::anychar_p - (']' | hard_space)))
                                                [actions.link_pre]
            >>  hard_space
            >>  phrase                          [actions.link_post]
            ;

        elements.add
            ("#", element_info(element_info::phrase, &local.anchor))
            ;

        local.anchor =
                blank
            >>  (*(cl::anychar_p - phrase_end)) [actions.anchor]
            ;

        elements.add
            ("funcref", element_info(element_info::phrase, &local.funcref))
            ("classref", element_info(element_info::phrase, &local.classref))
            ("memberref", element_info(element_info::phrase, &local.memberref))
            ("enumref", element_info(element_info::phrase, &local.enumref))
            ("macroref", element_info(element_info::phrase, &local.macroref))
            ("headerref", element_info(element_info::phrase, &local.headerref))
            ("conceptref", element_info(element_info::phrase, &local.conceptref))
            ("globalref", element_info(element_info::phrase, &local.globalref))
            ;

        local.funcref =
                space
            >>  (*(cl::anychar_p -
                    (']' | hard_space)))        [actions.funcref_pre]
            >>  hard_space
            >>  phrase                          [actions.funcref_post]
            ;

        local.classref =
                space
            >>  (*(cl::anychar_p -
                    (']' | hard_space)))        [actions.classref_pre]
            >>  hard_space
            >>  phrase                          [actions.classref_post]
            ;

        local.memberref =
                space
            >>  (*(cl::anychar_p -
                    (']' | hard_space)))        [actions.memberref_pre]
            >>  hard_space
            >>  phrase                          [actions.memberref_post]
            ;

        local.enumref =
                space
            >>  (*(cl::anychar_p -
                    (']' | hard_space)))        [actions.enumref_pre]
            >>  hard_space
            >>  phrase                          [actions.enumref_post]
            ;

        local.macroref =
                space
            >>  (*(cl::anychar_p -
                    (']' | hard_space)))        [actions.macroref_pre]
            >>  hard_space
            >>  phrase                          [actions.macroref_post]
            ;

        local.headerref =
                space
            >>  (*(cl::anychar_p -
                    (']' | hard_space)))        [actions.headerref_pre]
            >>  hard_space
            >>  phrase                          [actions.headerref_post]
            ;

        local.conceptref =
                space
            >>  (*(cl::anychar_p -
                    (']' | hard_space)))        [actions.conceptref_pre]
            >>  hard_space
            >>  phrase                          [actions.conceptref_post]
            ;

        local.globalref =
                space
            >>  (*(cl::anychar_p -
                    (']' | hard_space)))        [actions.globalref_pre]
            >>  hard_space
            >>  phrase                          [actions.globalref_post]
            ;

        elements.add
            ("*", element_info(element_info::phrase, &local.bold))
            ("'", element_info(element_info::phrase, &local.italic))
            ("_", element_info(element_info::phrase, &local.underline))
            ("^", element_info(element_info::phrase, &local.teletype))
            ("-", element_info(element_info::phrase, &local.strikethrough))
            ("\"", element_info(element_info::phrase, &local.quote))
            ("~", element_info(element_info::phrase, &local.replaceable))
            ;

        local.bold =
                blank
            >>  local.inner_phrase              [actions.bold]
            ;

        local.italic =
                blank
            >>  local.inner_phrase              [actions.italic]
            ;

        local.underline =
                blank
            >>  local.inner_phrase              [actions.underline]
            ;

        local.teletype =
                blank
            >>  local.inner_phrase              [actions.teletype]
            ;

        local.strikethrough =
                blank
            >>  local.inner_phrase              [actions.strikethrough]
            ;

        local.quote =
                blank
            >>  local.inner_phrase              [actions.quote]
            ;

        local.replaceable =
                blank
            >>  local.inner_phrase              [actions.replaceable]
            ;

        elements.add
            ("c++", element_info(element_info::phrase, &local.source_mode_cpp))
            ("python", element_info(element_info::phrase, &local.source_mode_python))
            ("teletype", element_info(element_info::phrase, &local.source_mode_teletype))
            ;
        
        local.source_mode_cpp = cl::eps_p [cl::assign_a(actions.source_mode, "c++")];
        local.source_mode_python = cl::eps_p [cl::assign_a(actions.source_mode, "python")];
        local.source_mode_teletype = cl::eps_p [cl::assign_a(actions.source_mode, "teletype")];

        elements.add
            ("footnote", element_info(element_info::phrase, &local.footnote))
            ;

        local.footnote =
                blank
            >>  local.inner_phrase              [actions.footnote]
            ;

        local.inner_phrase =
            actions.scoped_phrase[
            actions.values.save
            [   cl::eps_p                       [actions.inner_phrase_pre]
            >>  phrase
            >>  cl::eps_p                       [actions.inner_phrase_post]
            ]                                   [actions.phrase_value]
            ]
            ;
    }
}
