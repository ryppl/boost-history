// stylesheet.hpp 
// Copyright (C) Jacob Voytko 2007
// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org
// ----------------------------------------------------------------- 

#ifndef _BOOST_SVG_STYLESHEET_HPP
#define _BOOST_SVG_STYLESHEET_HPP


#include <boost/spirit/core.hpp>
#include <boost/spirit/utility/distinct.hpp>

#include <exception>
#include <string>
#include <fstream>
#include <ostream>
#include <iostream>

namespace boost{
namespace svg{

// Note: After GSoC, this will use the suggested CSS parser
// from the W3C CSS 2.1 Standard
struct css_parse : public ::boost::spirit::grammar<css_parse>
{
    template <typename ScannerT>
    struct definition
    {
        definition(css_parse const& /*self*/)
        {
            expression
                = *(class_expression | id_expression | normal_expression)
                ;

            class_expression
                =   wspace >> ::boost::spirit::ch_p('.') 
                    >> *(::boost::spirit::alnum_p) >> wspace >> ::boost::spirit::ch_p('{')
                    >> *statement >> wspace >> ::boost::spirit::ch_p('}')
                ;

            id_expression
                =   wspace >> ::boost::spirit::ch_p('#') 
                    >> *(::boost::spirit::alnum_p) >> wspace >> ::boost::spirit::ch_p('{')
                    >> wspace >> *statement >> wspace >> ::boost::spirit::ch_p('}')
                ;

            statement
                =   wspace >> identifier >> wspace >>
                    ::boost::spirit::ch_p(':') >> wspace >>
                    identifier >> wspace >>
                    ::boost::spirit::ch_p(';') >> wspace
                ;

            normal_expression
                =   wspace >> *(::boost::spirit::alnum_p) >> wspace 
                    >> ::boost::spirit::ch_p('{') >> wspace 
                    >> *statement >> wspace >> ::boost::spirit::ch_p('}') >> wspace
                ;

            identifier
                =   *(::boost::spirit::chset<>("a-zA-Z0-9#-")); 
                ;

            // whitespace
            wspace
                =   *(::boost::spirit::chset<>(" \t\n\r"));
                ;
        }

        ::boost::spirit::rule<ScannerT> expression, statement, class_expression, wspace,
            id_expression, normal_expression, identifier;

        ::boost::spirit::rule<ScannerT> const&
        start() const { return expression; }
    };
};

bool validate_stylesheet(std::ifstream& file)
{
    css_parse my_css_parser;
    std::string str, tmp;

    while(getline(file, tmp))
    {
        str += tmp;
    }

    ::boost::spirit::parse_info<> info = ::boost::spirit::
            parse(str.c_str(), my_css_parser, 
                ::boost::spirit::space_p);
        
    if (!info.full)
    {
        std::cout<<info.stop;
        return false;
    }
    

    return true;
}

bool validate_stylesheet(const std::string& file)
{
    std::ifstream f_str(file.c_str());

    if(f_str.fail())
    {
        throw std::runtime_error("Failed to open " + file);
    }

    return validate_stylesheet(f_str);
}

}
}

#endif
