// 
// Boost.Process 
// ~~~~~~~~~~~~~ 
// 
// Copyright (c) 2006, 2007 Julio M. Merino Vidal 
// Copyright (c) 2008 Ilya Sokolov, Boris Schaeling 
// Copyright (c) 2009 Boris Schaeling 
// Copyright (c) 2010 Felipe Tanus, Boris Schaeling 
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying 
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt) 
// 

#include <boost/process/config.hpp> 

#if defined(BOOST_POSIX_API) 
#   include <sys/types.h> 
#   include <unistd.h> 
#elif defined(BOOST_WINDOWS_API) 
#   include <windows.h> 
#else 
#   error "Unsupported platform." 
#endif 

#define BOOST_TEST_MAIN 
#include "util/boost.hpp" 

BOOST_AUTO_TEST_CASE(test_handle) 
{ 
    bpb::pipe p(bpb::pipe::input_stream); 

    bp::handle read_end = p.get_child_end(); 
    bp::handle write_end = p.get_parent_end(); 

    bp::handle read_end2 = read_end; 
    bp::handle write_end2 = write_end; 

    BOOST_CHECK(read_end.valid()); 
    BOOST_CHECK(write_end.valid()); 
    BOOST_CHECK(read_end2.valid()); 
    BOOST_CHECK(write_end2.valid()); 

#if defined(BOOST_POSIX_API) 
    ssize_t written = write(write_end.native(), "test", 4); 
    BOOST_CHECK_EQUAL(written, 4); 
    char buf[4]; 
    ssize_t r = read(read_end.native(), buf, 4); 
    BOOST_CHECK_EQUAL(r, 4); 
#elif defined(BOOST_WINDOWS_API) 
    DWORD written; 
    BOOST_REQUIRE(WriteFile(write_end.native(), "test", 4, &written, NULL)); 
    BOOST_CHECK_EQUAL(written, 4); 
    char buf[4]; 
    DWORD read; 
    BOOST_REQUIRE(ReadFile(read_end.native(), buf, sizeof(buf), &read, NULL)); 
    BOOST_CHECK_EQUAL(read, 4); 
#endif 

    read_end.close(); 
    write_end2.close(); 

    BOOST_CHECK(!read_end.valid()); 
    BOOST_CHECK(!write_end.valid()); 
    BOOST_CHECK(!read_end2.valid()); 
    BOOST_CHECK(!write_end2.valid()); 

#if defined(BOOST_POSIX_API) 
    written = write(write_end.native(), "test", 4); 
    BOOST_CHECK_EQUAL(written, -1); 
#elif defined(BOOST_WINDOWS_API) 
    BOOST_REQUIRE(!WriteFile(write_end.native(), "test", 4, &written, NULL)); 
#endif 
} 
