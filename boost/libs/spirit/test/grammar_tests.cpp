/*=============================================================================
    Spirit v1.7.0
    Copyright (c) 2001-2003 Joel de Guzman
    Copyright (c) 2003 Hartmut Kaiser
    http://spirit.sourceforge.net/

    Permission to copy, use, modify, sell and distribute this software is
    granted provided this copyright notice appears in all copies. This
    software is provided "as is" without express or implied warranty, and
    with no claim as to its suitability for any purpose.
=============================================================================*/
#include <iostream>
#include <cassert>

using namespace std;

//#define BOOST_SPIRIT_DEBUG
#include <boost/spirit/core.hpp>
using namespace boost::spirit;

///////////////////////////////////////////////////////////////////////////////
//  The grammar_def stuff is available only to compilers that support
//  partial template specialization
#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
#define BOOST_SPIRIT_USE_GRAMMARDEF
#endif

//////////////////////////////////////////////////////////////////////////////
//
//  Grammar tests
//
///////////////////////////////////////////////////////////////////////////////
struct num_list : public grammar<num_list>
{
    enum {
        default_rule = 0,
        num_rule = 1,
    };

    template <typename ScannerT>
    struct definition 
#if defined(BOOST_SPIRIT_USE_GRAMMARDEF)
    :   public grammar_def<rule<ScannerT>, same>
#endif
    {
        definition(num_list const& self)
        {
            num = int_p;
            r = num >> *(',' >> num);

#if defined(BOOST_SPIRIT_USE_GRAMMARDEF)
            this->start_parsers(r, num);
#endif
            BOOST_SPIRIT_DEBUG_RULE(num);
            BOOST_SPIRIT_DEBUG_RULE(r);
        }

        rule<ScannerT> r, num;

#if !defined(BOOST_SPIRIT_USE_GRAMMARDEF)
        rule<ScannerT> const& start() const { return r; }
#endif
    };
};

struct num_list_ex : public grammar<num_list_ex>
{
    enum {
        default_rule = 0,
        num_rule = 1,
        integer_rule = 2
    };
    
    template <typename ScannerT>
    struct definition 
#if defined(BOOST_SPIRIT_USE_GRAMMARDEF)
    :   public grammar_def<rule<ScannerT>, same, int_parser<int, 10, 1, -1> >
#endif
    {
        definition(num_list_ex const& self)
        {
            num = integer;
            r = num >> *(',' >> num);

#if defined(BOOST_SPIRIT_USE_GRAMMARDEF)
            this->start_parsers(r, num, integer);
#endif
            BOOST_SPIRIT_DEBUG_RULE(num);
            BOOST_SPIRIT_DEBUG_RULE(r);
        }

        rule<ScannerT> r, num;
        int_parser<int, 10, 1, -1> integer;
        
#if !defined(BOOST_SPIRIT_USE_GRAMMARDEF)
        rule<ScannerT> const& start() const { return r; }
#endif
    };
};

void
grammar_tests()
{
    num_list nlist;
    BOOST_SPIRIT_DEBUG_GRAMMAR(nlist);

    parse_info<char const*> pi;
    pi = parse("123, 456, 789", nlist, space_p);
    assert(pi.hit);
    assert(pi.full);

#if defined(BOOST_SPIRIT_USE_GRAMMARDEF)
    num_list_ex nlistex;
    BOOST_SPIRIT_DEBUG_GRAMMAR(nlistex);
    
    pi = parse("123, 456, 789", nlist.use_parser<num_list::default_rule>(), 
        space_p);
    assert(pi.hit);
    assert(pi.full);

    pi = parse("123", nlist.use_parser<num_list::num_rule>(), space_p);
    assert(pi.hit);
    assert(pi.full);

    pi = parse("123, 456, 789", nlistex, space_p);
    assert(pi.hit);
    assert(pi.full);

    pi = parse("123, 456, 789", 
        nlistex.use_parser<num_list_ex::default_rule>(), space_p);
    assert(pi.hit);
    assert(pi.full);

    pi = parse("123", nlistex.use_parser<num_list_ex::num_rule>(), space_p);
    assert(pi.hit);
    assert(pi.full);

    pi = parse("123", nlistex.use_parser<num_list_ex::integer_rule>(), 
        space_p);
    assert(pi.hit);
    assert(pi.full);
#endif // defined(BOOST_SPIRIT_USE_GRAMMARDEF)
}

///////////////////////////////////////////////////////////////////////////////
//
//  Main
//
///////////////////////////////////////////////////////////////////////////////
int
main()
{
    grammar_tests();
    cout << "Tests concluded successfully\n";
    return 0;
}

