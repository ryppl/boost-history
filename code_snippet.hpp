/*=============================================================================
    Copyright (c) 2006 Joel de Guzman
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_QUICKBOOK_CODE_SNIPPET_HPP)
#define BOOST_SPIRIT_QUICKBOOK_CODE_SNIPPET_HPP

#include <boost/spirit/include/qi_core.hpp>
#include <boost/spirit/include/qi_eol.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include "./grammars.hpp"
#include "./parse_utils.hpp"
#include "./detail/actions.hpp"

namespace quickbook
{
    namespace ph = boost::phoenix;

    template <typename Iterator>
    python_code_snippet_grammar<Iterator>::python_code_snippet_grammar(actions_type & actions)
        : python_code_snippet_grammar::base_type(start_)
        , actions(actions)
    {
        start_ =
            +(
                    qi::raw[snippet]            [ph::bind(&actions_type::compile, &actions, qi::_1)]
                |   qi::char_
            )
            ;

        identifier =
            (qi::alpha | '_') >> *(qi::alnum | '_')
            ;

        snippet =
            "#[" >> *qi::space
            >> identifier                       [ph::ref(actions.id) = qi::_1]
            >> (*(code_elements - "#]"))
            >> "#]"
            ;

        code_elements =
                escaped_comment
            |   ignore
            |   (qi::char_ - "#]")              [ph::bind(&actions_type::pass_thru, &actions, qi::_1)]
            ;

        ignore =
                *qi::blank >> "#<-"
                >> (*(qi::char_ - "#->"))
                >> "#->" >> *qi::blank >> qi::eol
            |   "\"\"\"<-\"\"\""
                >> (*(qi::char_ - "\"\"\"->\"\"\""))
                >> "\"\"\"->\"\"\""
            |   "\"\"\"<-"
                >> (*(qi::char_ - "->\"\"\""))
                >> "->\"\"\""
            ;

        escaped_comment =
                *qi::space >> "#`"
                >> ((*(qi::char_ - qi::eol))
                    >> qi::eol)                 [ph::bind(&actions_type::escaped_comment, &actions, as_string(qi::_1))]
            |   *qi::space >> "\"\"\"`"
                >> (*(qi::char_ - "\"\"\""))
                                                [ph::bind(&actions_type::escaped_comment, &actions, as_string(qi::_1))]
                >> "\"\"\""
            ;
    }

    template <typename Iterator>
    cpp_code_snippet_grammar<Iterator>::cpp_code_snippet_grammar(actions_type & actions)
        : cpp_code_snippet_grammar::base_type(start_)
        , actions(actions)
    {
        start_ =
            +(
                    qi::raw[snippet]            [ph::bind(&actions_type::compile, &actions, qi::_1)]
                |   qi::char_
            )
            ;

        identifier =
            (qi::alpha | '_') >> *(qi::alnum | '_')
            ;

        snippet =
                "//[" >> *qi::space
                >> identifier                   [ph::ref(actions.id) = qi::_1]
                >> (*(code_elements - "//]"))
                >> "//]"
            |
                "/*[" >> *qi::space
                >> identifier                   [ph::ref(actions.id) = qi::_1]
                >> *qi::space >> "*/"
                >> (*(code_elements - "/*]*"))
                >> "/*]*/"
            ;

        code_elements =
                escaped_comment
            |   ignore
            |   line_callout
            |   inline_callout
            |   (qi::char_ - "//]" - "/*]*/")   [ph::bind(&actions_type::pass_thru, &actions, qi::_1)]
            ;

        inline_callout =
            "/*<"
            >> (*(qi::char_ - ">*/"))           [ph::bind(&actions_type::inline_callout, &actions, as_string(qi::_1))]
            >> ">*/"
            ;

        line_callout =
            "/*<<"
            >> (*(qi::char_ - ">>*/"))          [ph::bind(&actions_type::line_callout, &actions, as_string(qi::_1))]
            >> ">>*/"
            >> *qi::space
            ;

        ignore =
                *qi::blank >> "//<-"
                >> (*(qi::char_ - "//->"))
                >> "//->" >> *qi::blank >> qi::eol
            |   "/*<-*/"
                >> (*(qi::char_ - "/*->*/"))
                >> "/*->*/"
            |   "/*<-"
                >> (*(qi::char_ - "->*/"))
                >> "->*/"
            ;

        escaped_comment =
                *qi::space >> "//`"
                >> ((*(qi::char_ - qi::eol))
                    >> qi::eol)                 [ph::bind(&actions_type::escaped_comment, &actions, as_string(qi::_1))]
            |   *qi::space >> "/*`"
                >> (*(qi::char_ - "*/"))        [ph::bind(&actions_type::escaped_comment, &actions, as_string(qi::_1))]
                >> "*/"
            ;
    }
}

#endif // BOOST_SPIRIT_QUICKBOOK_CODE_SNIPPET_HPP

