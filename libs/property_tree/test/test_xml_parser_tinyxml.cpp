// ----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Marcin Kalicinski
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org
// ----------------------------------------------------------------------------

// Do not deprecate insecure CRT calls
#if (defined(BOOST_MSVC) && (BOOST_MSVC >= 1400))
#   define _CRT_SECURE_NO_DEPRECATE
#endif

// Enable ptree debugging
#define BOOST_PROPERTY_TREE_DEBUG           

#include <boost/test/minimal.hpp>
#include <boost/property_tree/ptree.hpp>
#define BOOST_PROPERTY_TREE_XML_PARSER_TINYXML
#include <boost/property_tree/xml_parser.hpp>
#include "generic_parser_test.hpp"
#include "xml_parser_test_data.hpp"

struct ReadFunc
{
    template<class Ptree>
    void operator()(const std::string &filename, Ptree &pt) const
    {
        boost::property_tree::read_xml(filename, pt);
    }
};

struct WriteFunc
{
    template<class Ptree>
    void operator()(const std::string &filename, const Ptree &pt) const
    {
        boost::property_tree::write_xml(filename, pt);
    }
};

template<class Ptree>
void test_xml_parser()
{

    using namespace boost::property_tree;

    generic_parser_test_ok<Ptree, ReadFunc, WriteFunc>
    (
        ReadFunc(), WriteFunc(), ok_data_1, NULL, 
        "testok1.xml", NULL, "testok1out.xml", 2, 0, 5
    );
    
    generic_parser_test_ok<Ptree, ReadFunc, WriteFunc>
    (
        ReadFunc(), WriteFunc(), ok_data_2, NULL, 
        "testok2.xml", NULL, "testok2out.xml", 669, 31638, 4104
    );

    generic_parser_test_ok<Ptree, ReadFunc, WriteFunc>
    (
        ReadFunc(), WriteFunc(), ok_data_3, NULL, 
        "testok3.xml", NULL, "testok3out.xml", 107, 3652, 1244
    );

    generic_parser_test_error<Ptree, ReadFunc, WriteFunc, xml_parser_error>
    (
        ReadFunc(), WriteFunc(), error_data_1, NULL,
        "testerr1.xml", NULL, "testerr1out.xml", 1
    );

    generic_parser_test_error<Ptree, ReadFunc, WriteFunc, xml_parser_error>
    (
        ReadFunc(), WriteFunc(), error_data_2, NULL,
        "testerr2.xml", NULL, "testerr2out.xml", 2
    );

}

int test_main(int argc, char *argv[])
{
    using namespace boost::property_tree;
    test_xml_parser<ptree>();
#ifndef BOOST_NO_CWCHAR
    test_xml_parser<wptree>();
#endif
    return 0;
}
