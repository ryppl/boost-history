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
#include <vector>
#include <string>

namespace
{
    
    // Test arguments
    char *argv[] = 
    {
        "c:\\program.exe",
        "-Isrc/include1",
        "   file2.cc   ",
        "/L    src/lib1",
        "-Lsrc/lib2",
        "/ooutput",
        "file1.cc",
        "-g",
        "-",
        "/",
        " /Isrc/include2 ",
        "   file3.cc   ",
        "-I  src/include3   "
    };

    // Test arguments count
    const int argc = sizeof(argv) / sizeof(*argv);

}

template<class Ptree>
void test_cmdline_parser()
{

    using namespace boost::property_tree;
    typedef typename Ptree::char_type Ch;
    typedef std::basic_string<Ch> Str;

    // Prepare arguments of proper char type
    std::vector<Ch *> p;
    std::vector<Str> strings;
    strings.reserve(argc);
    for (int i = 0; i < argc; ++i)
    {
        strings.push_back(detail::widen<Ch>(argv[i]));
        p.push_back(const_cast<Ch *>(strings.back().c_str()));
    }

    Ptree pt1;
    read_cmdline<Ptree>(argc, &p.front(), detail::widen<Ch>("-/"), pt1);

    // Check total sizes
    //std::cerr << total_size(pt1) << " " << total_data_size(pt1) << " " << total_keys_size(pt1) << "\n";
    BOOST_CHECK(total_size(pt1) == 21);
    BOOST_CHECK(total_data_size(pt1) == 138);
    BOOST_CHECK(total_keys_size(pt1) == 6);
        
    Ptree pt2;
    read_cmdline<Ptree>(argc, &p.front(), detail::widen<Ch>("-"), pt2);

    // Check total sizes
    //std::cerr << total_size(pt2) << " " << total_data_size(pt2) << " " << total_keys_size(pt2) << "\n";
    BOOST_CHECK(total_size(pt2) == 19);
    BOOST_CHECK(total_data_size(pt2) == 139);
    BOOST_CHECK(total_keys_size(pt2) == 4);
        
    Ptree pt3;
    read_cmdline<Ptree>(argc, &p.front(), detail::widen<Ch>("/"), pt3);

    // Check total sizes
    //std::cerr << total_size(pt3) << " " << total_data_size(pt3) << " " << total_keys_size(pt3) << "\n";
    BOOST_CHECK(total_size(pt3) == 19);
    BOOST_CHECK(total_data_size(pt3) == 157);
    BOOST_CHECK(total_keys_size(pt3) == 4);

}

int test_main(int argc, char *argv[])
{
    using namespace boost::property_tree;
    test_cmdline_parser<ptree>();
#ifndef BOOST_NO_CWCHAR    
    test_cmdline_parser<wptree>();
#endif
    return 0;
}
