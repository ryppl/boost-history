// test_format_write_ts.cpp

// Boost Logging library
//
// Author: John Torjo, www.torjo.com
//
// Copyright (C) 2007 John Torjo (see www.torjo.com for email)
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org for updates, documentation, and revision history.
// See http://www.torjo.com/log2/ for more details


#include <boost/logging/logging.hpp>
#include <boost/logging/format/optimize.hpp>
#include <boost/logging/process_msg/ostream_like.hpp>
#include <boost/logging/format.hpp>
#include <sstream>
#include <boost/shared_ptr.hpp>
#include <time.h>
#include <iostream>
#include <fstream>
#include <boost/logging/writer/on_dedicated_thread.hpp>

using namespace boost::logging;

typedef optimize::cache_string_one_str<> cache_string;
typedef destination::base<const std::string &, op_equal::same_type> destination_base;


void test_format_write_ts() {
}

#ifdef SINGLE_TEST

int main() {
    test_format_write_ts();
}

#endif

// End of file

