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
#   include <sys/wait.h> 
#elif defined(BOOST_WINDOWS_API) 
#   include <windows.h> 
#else 
#   error "Unsupported platform." 
#endif 

#define BOOST_TEST_MAIN 
#include "util/boost.hpp" 
#include "util/use_helpers.hpp" 
#include <string> 
#include <vector> 
#include <cstdlib> 

BOOST_AUTO_TEST_CASE(test_child_wait) 
{ 
    check_helpers(); 

    std::vector<std::string> args; 
    args.push_back("exit-success"); 

    bp::child c = bp::create_child(get_helpers_path(), args); 

    int exit_code = c.wait(); 
#if defined(BOOST_POSIX_API) 
    BOOST_REQUIRE(WIFEXITED(exit_code)); 
    BOOST_CHECK_EQUAL(WEXITSTATUS(exit_code), EXIT_SUCCESS); 
#elif defined(BOOST_WINDOWS_API) 
    BOOST_CHECK_EQUAL(exit_code, EXIT_SUCCESS); 
#endif 
} 

BOOST_AUTO_TEST_CASE(test_status_wait) 
{ 
    check_helpers(); 

    std::vector<std::string> args; 
    args.push_back("exit-success"); 

    bp::child c = bp::create_child(get_helpers_path(), args); 

    ba::io_service ioservice; 
    bp::status s(ioservice); 
    int exit_code = s.wait(c.get_id()); 
#if defined(BOOST_POSIX_API) 
    BOOST_REQUIRE(WIFEXITED(exit_code)); 
    BOOST_CHECK_EQUAL(WEXITSTATUS(exit_code), EXIT_SUCCESS); 
#elif defined(BOOST_WINDOWS_API) 
    BOOST_CHECK_EQUAL(exit_code, EXIT_SUCCESS); 
#endif 
} 

void handler(boost::system::error_code ec, int exit_code) 
{ 
    BOOST_REQUIRE_EQUAL(ec, boost::system::error_code()); 
#if defined(BOOST_POSIX_API) 
    BOOST_REQUIRE(WIFEXITED(exit_code)); 
    BOOST_CHECK_EQUAL(WEXITSTATUS(exit_code), EXIT_SUCCESS); 
#elif defined(BOOST_WINDOWS_API) 
    BOOST_CHECK_EQUAL(exit_code, EXIT_SUCCESS); 
#endif 
} 

BOOST_AUTO_TEST_CASE(test_status_async_wait) 
{ 
    check_helpers(); 

    std::vector<std::string> args; 
    args.push_back("exit-success"); 

    bp::child c = bp::create_child(get_helpers_path(), args); 

    ba::io_service ioservice; 
    bp::status s(ioservice); 
    s.async_wait(c.get_id(), handler); 
    ioservice.run(); 
} 

BOOST_AUTO_TEST_CASE(test_status_async_wait_for_two_child_processes) 
{ 
    check_helpers(); 

    std::vector<std::string> args; 
    args.push_back("exit-success"); 

    bp::child c1 = bp::create_child(get_helpers_path(), args); 
    bp::child c2 = bp::create_child(get_helpers_path(), args); 

    ba::io_service ioservice; 
    bp::status s(ioservice); 
    s.async_wait(c1.get_id(), handler); 
    s.async_wait(c2.get_id(), handler); 
    ioservice.run(); 
} 

BOOST_AUTO_TEST_CASE(test_status_async_wait_twice) 
{ 
    check_helpers(); 

    std::vector<std::string> args; 
    args.push_back("exit-success"); 

    ba::io_service ioservice; 
    bp::status s(ioservice); 

    bp::child c1 = bp::create_child(get_helpers_path(), args); 
    s.async_wait(c1.get_id(), handler); 
    ioservice.run(); 

    ioservice.reset(); 

    bp::child c2 = bp::create_child(get_helpers_path(), args); 
    s.async_wait(c2.get_id(), handler); 
    ioservice.run(); 
} 

BOOST_AUTO_TEST_CASE(test_status_async_wait_shutdown) 
{ 
    check_helpers(); 

    std::vector<std::string> args; 
    args.push_back("loop"); 

    bp::child c = bp::create_child(get_helpers_path(), args); 

    { 
        ba::io_service ioservice; 
        bp::status s(ioservice); 
        s.async_wait(c.get_id(), handler); 
    } 

    c.terminate(); 
} 
