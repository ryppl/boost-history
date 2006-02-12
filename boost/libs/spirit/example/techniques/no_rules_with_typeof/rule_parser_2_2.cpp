/*==============================================================================
    Copyright (c) 2002-2003 Joel de Guzman
    Copyright (c) 2006 Tobias Schwinger
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
//------------------------------------------------------------------------------
// This example demonstrates how to make recursive grammars scale with typeof.
// It uses a rule parser with member variables and the parser_reference utility.
// See boost/spirit/utility/rule_parser.hpp for details.
// This example is based on subrule_calc.cpp.
//------------------------------------------------------------------------------
#include <string>
#include <iostream>

// Note that typeof is applied on a per-rule basis, so the number of rules has
// no effect on the complexity of the types used with typeof.
// This is why we can set this macro to a comparatively low value (see the 
// documentation of Boost.Typeof for details).
// The previous example (rule_parser_2_1.cpp) requires more than twice of the 
// complexity...
#define BOOST_TYPEOF_LIMIT_SIZE 40
#include <boost/typeof/typeof.hpp>

#include <boost/spirit/core.hpp>
#include <boost/spirit/core/typeof.hpp>

#include <boost/spirit/utility/rule_parser.hpp>


// Don't forget to
#include BOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()

using namespace boost::spirit;

// Semantic actions
namespace 
{
  void do_int(int v)       { std::cout << "PUSH(" << v << ')' << std::endl; }
  void do_add(char const*, char const*)  { std::cout << "ADD" << std::endl; }
  void do_sub(char const*, char const*)  { std::cout << "SUB" << std::endl; }
  void do_mul(char const*, char const*)  { std::cout << "MUL" << std::endl; }
  void do_div(char const*, char const*)  { std::cout << "DIV" << std::endl; }
  void do_neg(char const*, char const*)  { std::cout << "NEG" << std::endl; }
}

// Operating at root namespace...
#define BOOST_SPIRIT__NAMESPACE -

// Our calculator grammar using paramterized rule parsers. 
// Subrules are passed to the rule parsers as arguments to allow recursion.

BOOST_SPIRIT_RULE_PARSER(expression,
  (1,(term)),
  -,
  -,

    term 
    >> *( ('+' >> term)[ &do_add ]
        | ('-' >> term)[ &do_sub ]
        )
)

BOOST_SPIRIT_RULE_PARSER(term,
  (1,(factor)),
  -,
  -,

    factor
    >> *( ('*' >> factor)[ &do_mul ]  
        | ('/' >> factor)[ &do_div ]  
        )
)

// This rule parser uses the 'parser_reference' utility to refer to itself.
// Note that here is another recursive loop, however, the self-reference, unlike
// a subrule, cannot be passed to contained parsers because we would end up with
// an endless loop at compile time finding the type.
BOOST_SPIRIT_RULE_PARSER(factor,
  (1,(expression)),
  -,
  (1,( ((parser_reference<factor_t>),factor,(*this)) )),

    (   int_p[&do_int]
    |   ('(' >> expression >> ')')
    |   ('-' >> factor)[&do_neg]
    |   ('+' >> factor)
    )
) 


// This rule parser implements recursion between the other ones.
BOOST_SPIRIT_RULE_PARSER( calc,
  -,
  -,
  (3,( ((subrule<0>),sr_expression,()),
       ((subrule<1>),sr_term,()),
       ((subrule<2>),sr_factor,() )) ),

  (
    sr_expression = expression(sr_term),
    sr_term       = term(sr_factor),
    sr_factor     = factor(sr_expression)
  )
)

// Main program
int main()
{
  std::cout 
  << "/////////////////////////////////////////////////////////////\n"
  << "\t\tA ruleless calculator using rule parsers and subrules...\n"
  << "/////////////////////////////////////////////////////////////\n"
  << "Type an expression...or an empty line to quit\n" 
  << std::endl;

  std::string str;
  while (std::getline(std::cin, str))
  {
    if (str.empty()) break;

    parse_info<> info = parse(str.c_str(), calc, space_p);

    if (info.full)
      std::cout 
      << "OK." 
      << std::endl;
    else
      std::cout 
      << "ERROR.\n"
      << "Stopped at: \": " << info.stop << "\".\n"
      << std::endl;
  }
  return 0;
}

