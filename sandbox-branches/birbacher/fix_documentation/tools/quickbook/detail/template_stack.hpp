/*=============================================================================
    Copyright (c) 2002 2004 2006 Joel de Guzman
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_QUICKBOOK_TEMPLATE_STACK_HPP)
#define BOOST_SPIRIT_QUICKBOOK_TEMPLATE_STACK_HPP

#include <string>
#include <deque>
#include <vector>
#include <boost/tuple/tuple.hpp>
#include <boost/assert.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>
#include <boost/spirit/include/classic_functor_parser.hpp>
#include <boost/spirit/include/classic_symbols.hpp>
#include <boost/next_prior.hpp>

namespace quickbook
{
    struct template_scope;

    //  template symbols with N arguments are stored as follows:
    //
    //  vector<std::string>
    //        0: template name
    //        1: template param name[0]
    //        2: template param name[1]
    //      ...
    //        N: template param name[N-1]
    //      N+1: template body
    //  file position
    //  template scope (only used for 1.5+, 1.4- uses the dynamic scope)

    typedef boost::tuple<
            std::vector<std::string>
          , boost::spirit::classic::file_position
          , template_scope const*>
    template_symbol;

    typedef boost::spirit::classic::symbols<template_symbol> template_symbols;
    
    // template scope
    //
    // 1.4-: parent_scope is the previous scope on the stack
    //       (the template's dynamic parent).
    // 1.5+: parent_scope is the template's lexical parent.
    //
    // This means that a search along the parent_scope chain will follow the
    // correct lookup chain for that version of quickboook.
    //
    // symbols contains the templates defined in this scope.
    
    struct template_scope
    {
        template_scope() : parent_scope() {}
        template_scope const* parent_scope;
        template_symbols symbols;
    };

    struct template_stack
    {
        typedef std::deque<template_scope> deque;

        struct parser
        {
            typedef boost::spirit::classic::nil_t result_t;

            parser(template_stack& ts)
                : ts(ts) {}

            template <typename Scanner>
            std::ptrdiff_t
            operator()(Scanner const& scan, result_t) const
            {
                // search all scopes for the longest matching symbol.
                typename Scanner::iterator_t f = scan.first;
                std::ptrdiff_t len = -1;
                for (template_scope const* i = &*ts.scopes.begin(); i; i = i->parent_scope)
                {
                    boost::spirit::classic::match<> m = i->symbols.parse(scan);
                    if (m.length() > len)
                        len = m.length();
                    scan.first = f;
                }
                if (len >= 0)
                    scan.first = boost::next(f, len);
                return len;
            }

            template_stack& ts;
        };

        template_stack();
        template_symbol* find(std::string const& symbol) const;
        template_symbol* find_top_scope(std::string const& symbol) const;
        template_symbols const& top() const;
        template_scope const& top_scope() const;
        // Add the given template symbol to the current scope.
        // If it doesn't have a scope, sets the symbol's scope to the current scope.
        void add(std::string const& symbol, template_symbol const& ts);
        void push();
        void pop();

        // Set the current scope's parent.
        void set_parent_scope(template_scope const&);

        boost::spirit::classic::functor_parser<parser> scope;

    private:

        friend struct parser;
        deque scopes;
    };
}

#endif // BOOST_SPIRIT_QUICKBOOK_TEMPLATE_STACK_HPP

