// ----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Marcin Kalicinski
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org
// ----------------------------------------------------------------------------

#include "test_utils.hpp"
#include <boost/property_tree/cmdline_parser.hpp>

void test_cmdline_parser()
{

    using namespace boost::property_tree;

    int argc = 5;
    char *argv[5] = {
        "c:\\someexe.exe",
        "-Isrc/include",
        " -L    src/lib  ",
        "file1.cc",
        "   file2.cc   "
    };

    ptree pt1;
    read_cmdline<ptree>(argc, argv, "-", pt1);

    // Check total sizes
    BOOST_CHECK(total_size(pt1) == 6);
    BOOST_CHECK(total_data_size(pt1) == 48);
    BOOST_CHECK(total_keys_size(pt1) == 2);
        
    ptree pt2;
    read_cmdline<ptree>(argc, argv, "/", pt2);

    // Check total sizes
    BOOST_CHECK(total_size(pt2) == 6);
    BOOST_CHECK(total_data_size(pt2) == 56);
    BOOST_CHECK(total_keys_size(pt2) == 0);
        
    ptree pt3;
    read_cmdline<ptree>(argc, argv, "-/", pt3);

    // Check total sizes
    BOOST_CHECK(total_size(pt3) == 6);
    BOOST_CHECK(total_data_size(pt3) == 48);
    BOOST_CHECK(total_keys_size(pt3) == 2);

}

#ifndef BOOST_NO_CWCHAR    

void test_wcmdline_parser()
{

    using namespace boost::property_tree;

    int argc = 5;
    wchar_t *argv[5] = {
        L"c:\\someexe.exe",
        L"-Isrc/include",
        L" -L    src/lib  ",
        L"file1.cc",
        L"   file2.cc   "
    };

    wptree pt1;
    read_cmdline<wptree>(argc, argv, L"-", pt1);

    // Check total sizes
    BOOST_CHECK(total_size(pt1) == 6);
    BOOST_CHECK(total_data_size(pt1) == 48);
    BOOST_CHECK(total_keys_size(pt1) == 2);
        
    wptree pt2;
    read_cmdline<wptree>(argc, argv, L"/", pt2);

    // Check total sizes
    BOOST_CHECK(total_size(pt2) == 6);
    BOOST_CHECK(total_data_size(pt2) == 56);
    BOOST_CHECK(total_keys_size(pt2) == 0);

    wptree pt3;
    read_cmdline<wptree>(argc, argv, L"-/", pt3);

    // Check total sizes
    BOOST_CHECK(total_size(pt3) == 6);
    BOOST_CHECK(total_data_size(pt3) == 48);
    BOOST_CHECK(total_keys_size(pt3) == 2);

}

#endif

int test_main(int argc, char *argv[])
{
    test_cmdline_parser();
#ifndef BOOST_NO_CWCHAR    
    test_wcmdline_parser();
#endif
    return 0;
}
