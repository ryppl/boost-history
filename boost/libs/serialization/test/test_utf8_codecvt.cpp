/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_utf8_codecvt.cpp

// (C) Copyright 2002-4 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <locale>
#include <vector>
#include <string>

#include <boost/test/test_tools.hpp>
#include <boost/archive/iterators/istream_iterator.hpp>
#include <boost/archive/iterators/ostream_iterator.hpp>

#include <boost/archive/add_facet.hpp>
#include <boost/utf8_codecvt_facet.hpp>

template<size_t s>
struct test_data
{
    static unsigned char utf8_encoding[];
    static wchar_t wchar_encoding[];
};

template<>
unsigned char test_data<2>::utf8_encoding[] = {
    0x01,
    0x7f,
    0xc2, 0x80,
    0xdf, 0xbf,
    0xe0, 0xa0, 0x80,
    0xe7, 0xbf, 0xbf
};

template<>
wchar_t test_data<2>::wchar_encoding[] = {
    0x0001,
    0x007f,
    0x0080,
    0x07ff,
    0x0800,
    0x7fff
};

template<>
unsigned char test_data<4>::utf8_encoding[] = {
    0x01,
    0x7f,
    0xc2, 0x80,
    0xdf, 0xbf,
    0xe0, 0xa0, 0x80,
    0xef, 0xbf, 0xbf,
    0xf0, 0x90, 0x80, 0x80,
    0xf4, 0x8f, 0xbf, 0xbf,
    0xf7, 0xbf, 0xbf, 0xbf,
    0xf8, 0x88, 0x80, 0x80, 0x80,
    0xfb, 0xbf, 0xbf, 0xbf, 0xbf,
    0xfc, 0x84, 0x80, 0x80, 0x80, 0x80,
    0xfd, 0xbf, 0xbf, 0xbf, 0xbf, 0xbf
};

template<>
wchar_t test_data<4>::wchar_encoding[] = {
    0x00000001,
    0x0000007f,
    0x00000080,
    0x000007ff,
    0x00000800,
    0x0000ffff,
    0x00010000,
    0x0010ffff,
    0x001fffff,
    0x00200000,
    0x03ffffff,
    0x04000000,
    0x7fffffff
};

int
test_main(int /* argc */, char * /* argv */[]) {
    std::locale old_loc;
    std::locale * utf8_locale
        = boost::archive::add_facet(old_loc, new utf8_codecvt_facet<wchar_t, char>);

    typedef char utf8_t;
    typedef test_data<sizeof(wchar_t)> td;

    // Send our test UTF-8 data to file
    {
        std::ofstream ofs;
        ofs.open("test.dat", std::ios::binary);
        std::copy(
            td::utf8_encoding,
            td::utf8_encoding + sizeof(td::utf8_encoding),
            boost::archive::iterators::ostream_iterator<utf8_t>(ofs)
        );
    }

    // Read the test data back in, converting to UCS-4 on the way in
    std::vector<wchar_t> from_file;
    {
        std::wifstream ifs;
        ifs.imbue(*utf8_locale);
        ifs.open("test.dat");

        wchar_t item = 0;
        // note can't use normal vector from iterator constructor because
        // dinkumware doesn't have it.
        for(;;){
            item = ifs.get();
            if(item == WEOF)
                break;
            //ifs >> item;
            //if(ifs.eof())
            //    break;
            from_file.push_back(item);
        }
    }

    // compare the data read back in with the orginal 
    BOOST_TEST(from_file.size() == sizeof(td::wchar_encoding) / sizeof(wchar_t));

    BOOST_TEST(std::equal(
            from_file.begin(),
            from_file.end(),
            td::wchar_encoding
    ));
  
    // Send the UCS4_data back out, converting to UTF-8
    {
        std::wofstream ofs;
        ofs.imbue(*utf8_locale);
        ofs.open("test2.dat");
        std::copy(
            from_file.begin(),
            from_file.end(),
            boost::archive::iterators::ostream_iterator<wchar_t>(ofs)
        );
    }

    // Make sure that both files are the same
    {
        typedef boost::archive::iterators::istream_iterator<utf8_t> is_iter;
        is_iter end_iter;

        std::ifstream ifs1("test.dat");
        is_iter it1(ifs1);
        std::vector<utf8_t> data1;
        std::copy(it1, end_iter, std::back_inserter(data1));

        std::ifstream ifs2("test2.dat");
        is_iter it2(ifs2);
        std::vector<utf8_t> data2;
        std::copy(it2, end_iter, std::back_inserter(data2));

        BOOST_TEST(data1 == data2);
    }
  
    delete utf8_locale;
    return boost::exit_success;
}
