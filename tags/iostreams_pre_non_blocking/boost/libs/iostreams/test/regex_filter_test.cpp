// (C) Copyright Jonathan Turkanis 2004
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

#include <fstream>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/regex_filter.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test.hpp>
#include "detail/temp_file.hpp"
#include "detail/verification.hpp"

using namespace std;
using namespace boost::iostreams;
using namespace boost::iostreams::test;
using boost::unit_test::test_suite;    

struct replace_lower {
    std::string operator() (const boost::match_results<const char*>&)
    { return "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; }
};

void regex_filter_test()
{
    // Note: Given the basic stream and filter tests, two regex tests 
    // are probably sufficient: reading with a filter based on a function,
    // and writing with a filter based on a format string.

    test_file       test; 
    uppercase_file  upper; 

    boost::regex match_lower("[a-z]+");
    std::string fmt = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    {
        // Note: the ifstream second is placed in a nested scope because 
        // closing and reopening a single ifstream failed for CW 9.4 on Windows.

        // Test reading from a regex filter based on a function in chars.
        filtering_istream
            first(boost::iostreams::regex_filter(match_lower, replace_lower()));
        first.push(file_source(test.name(), in_mode));
        {
            ifstream second(upper.name().c_str(), in_mode);  
            BOOST_CHECK_MESSAGE(
                compare_streams_in_chars(first, second),
                "failed reading from function-based regex_filter in chars"
            );
        }
        first.pop();

        // Test reading from a regex filter based on a function in chunks.
        // (Also tests reusing the regex filter.)
        first.push(file_source(test.name(), in_mode));
        {
            ifstream second(upper.name().c_str(), in_mode);
            BOOST_CHECK_MESSAGE(
                compare_streams_in_chunks(first, second),
                "failed reading from function-based regex_filter in chunks"
            );
        }
    }

    {
        // Note: the ifstream second is placed in a nested scope because 
        // closing and reopening a single ifstream failed for CW 9.4 on Windows.

        // Test reading from a regex filter based on a format string in chars.
        filtering_istream 
            first(boost::iostreams::regex_filter(match_lower, fmt));
        first.push(file_source(test.name(), in_mode));
        {
            ifstream second(upper.name().c_str(), in_mode);
            BOOST_CHECK_MESSAGE(
                compare_streams_in_chars(first, second),
                "failed reading from format-string-based regex_filter in chars"
            );
        }
        first.pop();

        // Test reading from a regex filter based on a format string in chunks.
        // (Also tests reusing the regex filter.)
        first.push(file_source(test.name(), in_mode));
        {
            ifstream second(upper.name().c_str(), in_mode);
            BOOST_CHECK_MESSAGE(
                compare_streams_in_chars(first, second),
                "failed reading from format-string-based regex_filter in chunks"
            );
        }
    }

    {
        test_file dest1;
        test_file dest2;

        // Test writing to a regex filter based on a function in chars.
        filtering_ostream 
            out(boost::iostreams::regex_filter(match_lower, replace_lower()));
        out.push(file_sink(dest1.name(), out_mode));
        write_data_in_chars(out);
        out.pop();
        BOOST_CHECK_MESSAGE(
            compare_files(dest1.name(), upper.name()),
            "failed writing to function-based regex_filter in chars"
        );

        // Test writing to a regex filter based on a function in chunks.
        // (Also tests reusing the regex filter.)
        out.push(file_sink(dest2.name(), out_mode));
        write_data_in_chunks(out);
        out.pop();
        BOOST_CHECK_MESSAGE(
            compare_files(dest2.name(), upper.name()),
            "failed writing to function-based regex_filter in chunks"
        );
    }

    {
        test_file dest1;
        test_file dest2;

        // Test writing to a regex filter based on a format string in chars.
        filtering_ostream  
            out(boost::iostreams::regex_filter(match_lower, fmt));
        out.push(file_sink(dest1.name(), out_mode));
        write_data_in_chars(out);
        out.pop();
        BOOST_CHECK_MESSAGE(
            compare_files(dest1.name(), upper.name()),
            "failed writing to format-string-based regex_filter in chars"
        );

        // Test writing to a regex filter based on a format string in chunks.
        // (Also tests reusing the regex filter.)
        out.push(file_sink(dest2.name(), out_mode));
        write_data_in_chunks(out);
        out.pop();
        BOOST_CHECK_MESSAGE(
            compare_files(dest2.name(), upper.name()),
            "failed writing to format-string-based regex_filter in chunks"
        );
    }
}

test_suite* init_unit_test_suite(int, char* []) 
{
    test_suite* test = BOOST_TEST_SUITE("regex_filter test");
    test->add(BOOST_TEST_CASE(&regex_filter_test));
    return test;
}
