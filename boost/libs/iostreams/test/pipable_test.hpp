// (C) Copyright Jonathan Turkanis 2004
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

#ifndef BOOST_IOSTREAMS_TEST_READ_INPUT_FILTER_HPP_INCLUDED
#define BOOST_IOSTREAMS_TEST_READ_INPUT_FILTER_HPP_INCLUDED

#include <fstream>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/test/test_tools.hpp>
#include "detail/filters.hpp"
#include "detail/sequence.hpp"
#include "detail/temp_file.hpp"
#include "detail/verification.hpp"

void pipable_test()
{
    using namespace std;
    using namespace boost;
    using namespace boost::iostreams;
    using namespace boost::iostreams::test;

    {
        test_file src;
        filtering_istream 
            in1( toupper_filter() | 
                 file_source(src.name()) );
        filtering_istream 
            in2( toupper_filter() | 
                 toupper_filter() | 
                 file_source(src.name()) );
        filtering_istream 
            in3( toupper_filter() | 
                 toupper_filter() | 
                 toupper_filter() | 
                 file_source(src.name()) );
        filtering_istream 
            in4( toupper_filter() | 
                 toupper_filter() | 
                 toupper_filter() | 
                 toupper_filter() | 
                 file_source(src.name()) );
        BOOST_CHECK(in1.size() == 2);
        BOOST_CHECK(in2.size() == 3);
        BOOST_CHECK(in3.size() == 4);
        BOOST_CHECK(in4.size() == 5);
    }  

    {
        test_file       src;
        uppercase_file  upper;
        filtering_istream 
            first( toupper_filter() | 
                   toupper_multichar_filter() | 
                   file_source(src.name()) );
        ifstream second(upper.name().c_str());
        BOOST_CHECK_MESSAGE(
            compare_streams_in_chunks(first, second),
            "failed reading from a filtering_istream in chunks with a "
            "multichar input filter"
        );
    }    
    
    {
        temp_file       dest;
        lowercase_file  lower;
        filtering_ostream  
            out( tolower_filter() |
                 tolower_multichar_filter() |
                 file_sink(dest.name()) );
        write_data_in_chunks(out);
        out.reset();
        BOOST_CHECK_MESSAGE(
            compare_files(dest.name(), lower.name()),
            "failed writing to a filtering_ostream in chunks with a "
            "multichar output filter with no buffer"
        );
    }
}

#endif // #ifndef BOOST_IOSTREAMS_TEST_READ_INPUT_FILTER_HPP_INCLUDED
