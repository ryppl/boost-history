// detail/grammars.hpp
// ~~~~~~~~~~~~~~~~~~~
// 
// Copyright (C) 2010 Denis Shevchenko (for @ dshevchenko.biz)
//
// Distributed under the Boost Software License, version 1.0
// (see http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONFIGURATOR_GRAMMARS_HPP
#define BOOST_CONFIGURATOR_GRAMMARS_HPP

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <string>

namespace boost {

/// \namespace cf
/// \brief Main namespace of library.
namespace cf {

/// \namespace cf::detail
/// \brief Details of realization.
namespace detail {

struct obtained_option {
    std::string name;
    std::string value;
};

} // namespace detail
} // namespace cf
} // namespace boost

BOOST_FUSION_ADAPT_STRUCT(
    boost::cf::detail::obtained_option,
    (std::string, name)
    (std::string, value)
)

namespace boost {

/// \namespace cf
/// \brief Main namespace of library.
namespace cf {

/// \namespace cf::detail
/// \brief Details of realization.
namespace detail {

template< typename Iterator >
struct obtained_option_parser_space : boost::spirit::qi::grammar< Iterator
                                                                  , obtained_option()
                                                                  , boost::spirit::ascii::space_type > {
    obtained_option_parser_space() :
            obtained_option_parser_space::base_type( start ) {
        using boost::spirit::qi::lexeme;
        using boost::spirit::qi::char_;
         
        name_extractor  %= lexeme[ +( char_ - ' ' ) ];
        value_extractor %= lexeme[ +( char_ - ' ' ) ];
        start %= name_extractor >> value_extractor;
    }
public: 
    boost::spirit::qi::rule
        < 
            Iterator
            , std::string()
            , boost::spirit::ascii::space_type
        >
    name_extractor;
    boost::spirit::qi::rule
        < 
            Iterator
            , std::string()
            , boost::spirit::ascii::space_type
        >
    value_extractor;
    boost::spirit::qi::rule
        < 
            Iterator
            , obtained_option()
            , boost::spirit::ascii::space_type
        >
    start;
};

template< typename Iterator >
struct obtained_option_parser : boost::spirit::qi::grammar< Iterator
                                                            , obtained_option()
                                                            , boost::spirit::ascii::space_type > {
    obtained_option_parser( char separator ) :
            obtained_option_parser::base_type( start ) {
        using boost::spirit::qi::lexeme;
        using boost::spirit::qi::char_;
        using boost::spirit::qi::skip;
        using boost::spirit::ascii::space;
         
        name_extractor  %= skip( space )[ +( char_ - separator ) ];
        value_extractor %= skip( space )[ +( char_ - separator ) ];
        start %= name_extractor >> separator >> value_extractor;
    }
public: 
    boost::spirit::qi::rule
        < 
            Iterator
            , std::string()
            , boost::spirit::ascii::space_type
        >
    name_extractor;
    boost::spirit::qi::rule
        < 
            Iterator
            , std::string()
            , boost::spirit::ascii::space_type
        >
    value_extractor;
    boost::spirit::qi::rule
        < 
            Iterator
            , obtained_option()
            , boost::spirit::ascii::space_type
        >
    start;
};

} // namespace detail
} // namespace cf
} // namespace boost

#endif // BOOST_CONFIGURATOR_GRAMMARS_HPP
