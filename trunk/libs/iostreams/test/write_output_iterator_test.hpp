// (C) Copyright Jonathan Turkanis 2004
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

#ifndef BOOST_IOSTREAMS_TEST_WRITE_OUTPUT_ITERATOR_HPP_INCLUDED
#define BOOST_IOSTREAMS_TEST_WRITE_OUTPUT_ITERATOR_HPP_INCLUDED

#include <fstream>
#include <iterator> // Back inserter.
#include <vector>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/test/test_tools.hpp>
#include "detail/sequence.hpp"
#include "detail/temp_file.hpp"
#include "detail/verification.hpp"

// Note: adding raw inserter iterators to chains is not supported
// on VC6.

void write_output_iterator_test()
{
    using namespace std;
    using namespace boost;
    using namespace boost::iostreams;
    using namespace boost::iostreams::test;

    test_file test;

    {
        vector<char> first;
        filtering_ostream out;
        out.push(std::back_inserter(first), 0);
        write_data_in_chars(out);
        ifstream second(test.name().c_str());
        BOOST_CHECK_MESSAGE(
            compare_container_and_stream(first, second),
            "failed writing to filtering_ostream based on an "
            "output iterator in chars with no buffer"
        );
    }

    {
        vector<char> first;
        filtering_ostream out;
        out.push(std::back_inserter(first), 0);
        write_data_in_chunks(out);
        ifstream second(test.name().c_str());
        BOOST_CHECK_MESSAGE(
            compare_container_and_stream(first, second),
            "failed writing to filtering_ostream based on an "
            "output iterator in chunks with no buffer"
        );
    }

    {
        vector<char> first;
        filtering_ostream out;
        out.push(std::back_inserter(first));
        write_data_in_chars(out);
        ifstream second(test.name().c_str());
        BOOST_CHECK_MESSAGE(
            compare_container_and_stream(first, second),
            "failed writing to filtering_ostream based on an "
            "output iterator in chars with large buffer"
        );
    }

    {
        vector<char> first;
        filtering_ostream out;
        out.push(std::back_inserter(first));
        write_data_in_chunks(out);
        ifstream second(test.name().c_str());
        BOOST_CHECK_MESSAGE(
            compare_container_and_stream(first, second),
            "failed writing to filtering_ostream based on an "
            "output iterator in chunks with large buffer"
        );
    }
}

#endif // #ifndef BOOST_IOSTREAMS_TEST_WRITE_OUTPUT_ITERATOR_HPP_INCLUDED
