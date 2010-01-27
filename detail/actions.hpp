/*=============================================================================
    Copyright (c) 2002 2004 2006 Joel de Guzman
    Copyright (c) 2004 Eric Niebler
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_QUICKBOOK_ACTIONS_HPP)
#define BOOST_SPIRIT_QUICKBOOK_ACTIONS_HPP

#include <time.h>
#include <map>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
#include <boost/spirit/include/classic_iterator.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/foreach.hpp>
#include <boost/tuple/tuple.hpp>
#include "./collector.hpp"
#include "./template_stack.hpp"
#include "./utils.hpp"

#ifdef BOOST_MSVC
// disable copy/assignment could not be generated, unreferenced formal params
#pragma warning (push)
#pragma warning(disable : 4511 4512 4100)
#endif

namespace quickbook
{
    namespace qi = boost::spirit::qi;
    namespace fs = boost::filesystem;
    using boost::spirit::unused_type;

    struct macro {
        macro() {}
        explicit macro(char const* x) : raw_markup(x) {};
        explicit macro(std::string const& x) : raw_markup(x) {};

        std::string raw_markup;
    };

    typedef qi::symbols<char, macro> macro_symbols;    

    typedef boost::spirit::classic::position_iterator<
        std::string::const_iterator> iterator;
    typedef boost::spirit::classic::file_position file_position;
    typedef boost::iterator_range<iterator> iterator_range;
    typedef std::map<std::string, std::string> attribute_map;

    struct actions;
    extern tm* current_time; // the current time
    extern tm* current_gm_time; // the current UTC time
    extern bool debug_mode;
    extern std::vector<std::string> include_path;

    // forward declarations
    struct actions;
    int parse(char const* filein_, actions& actor, bool ignore_docinfo = false);

    struct error_action
    {
        // Prints an error message to std::cerr

        error_action(
            int& error_count)
        : error_count(error_count) {}

        void operator()(iterator_range, unused_type, unused_type) const;

        int& error_count;
    };

    struct span
    {
        // Decorates c++ code fragments

        span(char const* name, collector& out)
        : name(name), out(out) {}

        void operator()(iterator_range, unused_type, unused_type) const;

        char const* name;
        collector& out;
    };

    struct unexpected_char
    {
        // Handles unexpected chars in c++ syntax

        unexpected_char(collector& out)
        : out(out) {}

        void operator()(iterator_range, unused_type, unused_type) const;

        collector& out;
    };

    extern char const* quickbook_get_date;
    extern char const* quickbook_get_time;

    struct space
    {
        // Prints a space

        space(collector& out)
            : out(out) {}

        void operator()(iterator_range, unused_type, unused_type) const;
        void operator()(char ch, unused_type, unused_type) const;

        collector& out;
    };

    struct raw_char_action
    {
        // Prints a single raw (unprocessed) char.
        // Allows '<', '>'... etc.

        raw_char_action(collector& phrase)
        : phrase(phrase) {}

        void operator()(char ch, unused_type, unused_type) const;
        void operator()(iterator_range, unused_type, unused_type) const;

        collector& phrase;
    };

    struct plain_char_action
    {
        // Prints a single plain char.
        // Converts '<' to "&lt;"... etc See utils.hpp

        plain_char_action(collector& phrase)
        : phrase(phrase) {}

        void operator()(char ch, unused_type, unused_type) const;
        void operator()(iterator_range, unused_type, unused_type) const;

        collector& phrase;
    };
    
    struct markup_action
    {
        template <typename T = void> struct result { typedef void type; };

        // A generic markup action

        markup_action(collector& phrase, std::string const& str)
        : phrase(phrase), str(str) {}

        void operator()(unused_type, unused_type, unused_type) const
        {
            phrase << str;
        }

        collector& phrase;
        std::string str;
    };

    struct syntax_highlight
    {
        syntax_highlight(
            std::string const& source_mode
          , actions& escape_actions)
        : source_mode(source_mode)
        , escape_actions(escape_actions)
        {
        }

        std::string operator()(iterator begin, iterator end) const;

        std::string const& source_mode;
        actions& escape_actions;
    };

   struct element_id_warning_action
   {
       void operator()(iterator_range, unused_type, unused_type) const;
   };

    struct xml_author
    {
        // Handles xml author

        xml_author(collector& out)
        : out(out) {}

        void operator()(std::pair<std::string, std::string> const& author) const;

        collector& out;
    };

    struct xml_year
    {
        // Handles xml year

        xml_year(collector& out)
            : out(out) {}

        void operator()(std::string const &year) const;

        collector& out;
    };

    struct xml_copyright
    {
        // Handles xml copyright

        xml_copyright(collector& out)
            : out(out) {}

        void operator()(std::pair<std::vector<std::string>, std::string> const &copyright) const;

        collector& out;
    };

    void pre(collector& out, quickbook::actions& actions, bool ignore_docinfo = false);
    void post(collector& out, quickbook::actions& actions, bool ignore_docinfo = false);
    
    struct phrase_push_action
    {
        phrase_push_action(collector& phrase)
            : phrase(phrase) {}

        void operator()(unused_type, unused_type, unused_type) const;
        
        collector& phrase;
    };

    struct phrase_pop_action
    {
        phrase_pop_action(collector& phrase)
            : phrase(phrase) {}

        template <typename Context>
        void operator()(unused_type x1, Context& c, unused_type x2) const
        {
            boost::spirit::_val(x1, c, x2) = (*this)();
        }
        
        std::string operator()() const;
        
        collector& phrase;
    };

    struct phrase_to_string_action
    {
        phrase_to_string_action(std::string& out, collector& phrase)
            : out(out) , phrase(phrase) {}

        void operator()(unused_type, unused_type, unused_type) const;

        std::string& out;
        collector& phrase;
    };

    struct code_snippet_actions
    {
        code_snippet_actions(std::vector<template_symbol>& storage,
                                 std::string const& doc_id,
                                 char const* source_type)
            : storage(storage)
            , doc_id(doc_id)
            , source_type(source_type)
        {}

        void pass_thru(char);
        void escaped_comment(std::string const&);
        void compile(boost::iterator_range<iterator>);
        void callout(std::string const&, char const* role);
        void inline_callout(std::string const&);
        void line_callout(std::string const&);

        std::string code;
        std::string snippet;
        std::string id;
        std::vector<std::string> callouts;
        std::vector<template_symbol>& storage;
        std::string const doc_id;
        char const* const source_type;
    };
    
    struct process_action
    {
        process_action(quickbook::actions& actions)
            : actions(actions) {}
        
        typedef void result_type;

        template <typename Arg1, typename Arg2 = void, typename Arg3 = void>
        struct result { typedef void type; };

        template <typename Attrib, typename Context>
        void operator()(Attrib& a, Context& c, bool& pass) const {
            (*this)(a);
        }
        
        template <typename T>
        void operator()(boost::optional<T> const& x) const {
            if(x) (*this)(*x);
        }

        template <BOOST_VARIANT_ENUM_PARAMS(typename T)>
        void operator()(boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)> const& x) const {
            return boost::apply_visitor(*this, x);
        }

        void operator()(unused_type) const {
        }

        template <typename T>
        void operator()(T const& x) const {
            process(actions, x);
        }
        
        quickbook::actions& actions;
    };
}

#ifdef BOOST_MSVC
#pragma warning (pop)
#endif

#endif // BOOST_SPIRIT_QUICKBOOK_ACTIONS_HPP

