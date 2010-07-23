// 
// Boost.Process 
// ~~~~~~~~~~~~~ 
// 
// Copyright (c) 2006, 2007 Julio M. Merino Vidal 
// Copyright (c) 2008, 2009 Boris Schaeling
// Copyright (c) 2010 Felipe Tanus, Boris Schaeling
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying 
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt) 
// 

#include <boost/process/config.hpp> 

#if defined(BOOST_POSIX_API) 
#   include <sys/wait.h> 
#elif defined(BOOST_WINDOWS_API) 
#   include <cstdlib> 
#else 
#  error "Unsupported platform." 
#endif 

#define BOOST_TEST_MAIN 
#include "util/boost.hpp" 
#include "util/use_helpers.hpp" 
#include <string> 
#include <vector> 

namespace process_test { 

// 
// Overview 
// -------- 
// 
// The functions below implement tests for the basic Process implementation. 
// In order to ensure appropriate behavior, all implementations must 
// have the same behavior in common public methods; keeping this set of 
// tests generic makes it easy to check this restriction because the tests 
// can easily be applied to any specific Process implementation. 
// 
// Factory concept 
// --------------- 
// 
// The functions in this file all rely on a Factory concept. This concept 
// provides a class whose () operator constructs a new Process instance 
// based on a process's identifier. Note that this is the most possible 
// generic construction, which should be conceptually supported by all 
// implementations. 
// 

template <class Process, class Factory> 
void test_getters() 
{ 
    bp::pid_type id = static_cast<bp::pid_type>(0); 
    Process c = Factory()(id); 

    BOOST_CHECK_EQUAL(c.get_id(), id); 
} 

template <class Process, class Factory> 
void test_terminate() 
{ 
    std::vector<std::string> args; 
    args.push_back("loop"); 

    bp::context ctx; 
    bp::child c = bp::create_child(get_helpers_path(), args, ctx); 

    Process p = Factory()(c.get_id()); 
    p.terminate(); 

    int status = c.wait(); 
#if defined(BOOST_POSIX_API) 
    BOOST_REQUIRE(!WIFEXITED(status)); 
    BOOST_REQUIRE(WIFSIGNALED(status)); 
#elif defined(BOOST_WINDOWS_API) 
    BOOST_CHECK_EQUAL(status, EXIT_FAILURE); 
#endif 
} 

} 
