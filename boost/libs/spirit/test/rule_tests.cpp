/*=============================================================================
    Spirit v1.7.0
    Copyright (c) 1998-2003 Joel de Guzman
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
#define BOOST_SPIRIT_RULE_SCANNERTYPE_LIMIT 3

#include <boost/spirit/core.hpp>
using namespace boost::spirit;

///////////////////////////////////////////////////////////////////////////////
//
//  Rule tests
//
///////////////////////////////////////////////////////////////////////////////
void
aliasing_tests()
{
    rule<>  a = ch_p('a');
    rule<>  b = ch_p('b');
    rule<>  c = ch_p('c');

    BOOST_SPIRIT_DEBUG_RULE(a);
    BOOST_SPIRIT_DEBUG_RULE(b);
    BOOST_SPIRIT_DEBUG_RULE(c);

    rule<>  start;
    BOOST_SPIRIT_DEBUG_RULE(start);

    rule<>  d;
    d = start;  // aliasing

    parse_info<char const*> pi;

    start = *(a | b | c);
    pi = parse("abcabcacb", d);
    assert(pi.hit);
    assert(pi.full);
    assert(pi.length == 9);
    assert(*pi.stop == 0);

    start   = (a | b) >> (start | b);
    pi = parse("aaaabababaaabbb", d);
    assert(pi.hit);
    assert(pi.full);
    assert(pi.length == 15);
    assert(*pi.stop == 0);
}

void
rule_template_param_tests()
{
    // test that rules can be issued its template params in any order:
    
    rule<> rx1;
    rule<scanner<> > rx2;
    rule<scanner<>, parser_context> rx3;
    rule<scanner<>, parser_context, parser_address_tag> rx4;
   
    rule<parser_context> rx5;
    rule<parser_context, parser_address_tag> rx6;
    rule<parser_context, parser_address_tag, scanner<> > rx7;
 
    rule<parser_address_tag> rx8;
    rule<parser_address_tag, scanner<> > rx9;
    rule<parser_address_tag, scanner<>, parser_context> rx10;

    rule<parser_address_tag, parser_context> rx11;
    rule<parser_address_tag, parser_context, scanner<> > rx12;

    rule<parser_context, scanner<> > rx13;
    rule<parser_context, scanner<>, parser_address_tag> rx14;
}

//  More than one simultaneous scanner types for rules are supported for 
//  compilers only, which support partial template specialization.
#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
struct my_grammar : public grammar<my_grammar>
{
    template <typename ScannerT>
    struct definition 
    {
        definition(my_grammar const& self)
        {
            r = lower_p;
            rr = +(lexeme_d[r] >> as_lower_d[r] >> r);
        }

        typedef scanner_list<
            ScannerT
          , typename lexeme_scanner<ScannerT>::type
          , typename as_lower_scanner<ScannerT>::type
        > scanners;

        rule<scanners> r;
        rule<ScannerT> rr;
        rule<ScannerT> const& start() const { return rr; }
    };
};
#endif // !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

void
rule_2_or_more_scanners_tests()
{
#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
    { // 2 scanners
        typedef scanner_list<scanner<>, phrase_scanner_t> scanners;
        
        rule<scanners>  r = +anychar_p;
        assert(parse("abcdefghijk", r).full);
        assert(parse("a b c d e f g h i j k", r, space_p).full);
    }
    
    { // 3 scanners
        my_grammar g;
        assert(parse("abcdef aBc d e f aBc d E f", g, space_p).full);
    }
#endif // !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
}

void
rule_tests()
{
    rule<>  a = ch_p('a');
    rule<>  b = ch_p('b');
    rule<>  c = ch_p('c');

    BOOST_SPIRIT_DEBUG_RULE(a);
    BOOST_SPIRIT_DEBUG_RULE(b);
    BOOST_SPIRIT_DEBUG_RULE(c);

    parse_info<char const*> pi;

    rule<>  start = *(a | b | c);

    BOOST_SPIRIT_DEBUG_RULE(start);

    pi = parse("abcabcacb", start);
    assert(pi.hit);
    assert(pi.full);
    assert(pi.length == 9);
    assert(*pi.stop == 0);

    start   = (a | b) >> (start | b);
    pi = parse("aaaabababaaabbb", start);
    assert(pi.hit);
    assert(pi.full);
    assert(pi.length == 15);
    assert(*pi.stop == 0);

    pi = parse("aaaabababaaabba", start);
    assert(pi.hit);
    assert(!pi.full);
    assert(pi.length == 14);

    aliasing_tests();
    rule_template_param_tests();
    rule_2_or_more_scanners_tests();
}

///////////////////////////////////////////////////////////////////////////////
//
//  Main
//
///////////////////////////////////////////////////////////////////////////////
int
main()
{
    rule_tests();
    cout << "Tests concluded successfully\n";
    return 0;
}

