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
#elif defined(BOOST_WINDOWS_API) 
#else 
#  error "Unsupported platform." 
#endif 

#define BOOST_TEST_MAIN 
#include "util/boost.hpp" 
#include "util/use_helpers.hpp" 
#include <string> 
#include <vector> 

bp::child launch_helper(const std::string &command) 
{ 
    std::vector<std::string> args; 
    args.push_back(command); 
    bp::context ctx; 
    return bp::create_child(get_helpers_path(), args, ctx); 
} 

BOOST_AUTO_TEST_CASE(test_exit_failure) 
{ 
    int status = launch_helper("exit-failure").wait(); 
#if defined(BOOST_POSIX_API) 
    BOOST_REQUIRE(WIFEXITED(status)); 
    BOOST_CHECK_EQUAL(WEXITSTATUS(status), EXIT_FAILURE); 
#elif defined(BOOST_WINDOWS_API) 
    BOOST_CHECK_EQUAL(status, EXIT_FAILURE); 
#endif 
} 

BOOST_AUTO_TEST_CASE(test_exit_success) 
{ 
    int status = launch_helper("exit-success").wait(); 
#if defined(BOOST_POSIX_API) 
    BOOST_REQUIRE(WIFEXITED(status)); 
    BOOST_CHECK_EQUAL(WEXITSTATUS(status), EXIT_SUCCESS); 
#elif defined(BOOST_WINDOWS_API) 
    BOOST_CHECK_EQUAL(status, EXIT_SUCCESS); 
#endif 
} 
