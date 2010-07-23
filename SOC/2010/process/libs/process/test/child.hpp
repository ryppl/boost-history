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
#   include <stdlib.h> 
#   include <sys/wait.h> 
#   if defined(__CYGWIN__) 
#       undef BOOST_POSIX_API 
#       define BOOST_CYGWIN_PATH 
#   endif 
#elif defined(BOOST_WINDOWS_API) 
#   include <windows.h> 
#else 
#  error "Unsupported platform." 
#endif 

#define BOOST_TEST_MAIN 
#include "util/boost.hpp" 
#include "util/use_helpers.hpp" 
#include <string> 
#include <vector> 
#include <utility> 
#include <cstdlib> 

namespace child_test { 

// 
// Overview 
// -------- 
// 
// The functions below implement tests for common launcher functionality. 
// These are used to test different implementations without duplicating 
// much code. 
// 
// Launcher concept 
// ---------------- 
// 
// The functions in this file all rely on a Launcher concept. This concept 
// provides a class whose () operator starts a new process given an 
// executable, its arguments, an execution context and the redirections for 
// the standard streams, and returns a new Child object. The operator also 
// receives a boolean, defaulting to false, that indicates if the child 
// process focuses on testing stdin input. This is needed when testing 
// pipelines to properly place a dummy process in the flow. 
// 

template <class Launcher, class Context, class Child> 
void test_close_stdin() 
{ 
    std::vector<std::string> args; 
    args.push_back("is-closed-stdin"); 

    int s = Launcher()(args, Context(), bpb::close::def(), 
                        bpb::close::def(), bpb::close::def()).wait(); 
#if defined(BOOST_POSIX_API) 
    BOOST_REQUIRE(WIFEXITED(s)); 
    BOOST_CHECK_EQUAL(WEXITSTATUS(s), EXIT_SUCCESS); 
#elif defined(BOOST_WINDOWS_API) 
    BOOST_CHECK_EQUAL(s, EXIT_SUCCESS); 
#endif 
} 

template <class Launcher, class Context, class Child> 
void test_close_stdout() 
{ 
    std::vector<std::string> args; 
    args.push_back("is-closed-stdout"); 

    int s1 = Launcher()(args, Context()).wait(); 
#if defined(BOOST_POSIX_API) 
    BOOST_REQUIRE(WIFEXITED(s1)); 
    BOOST_CHECK_EQUAL(WEXITSTATUS(s1), EXIT_SUCCESS); 
#elif defined(BOOST_WINDOWS_API) 
    BOOST_CHECK_EQUAL(s1, EXIT_SUCCESS); 
#endif 

    int s2 = Launcher()(args, Context(), bpb::close::def(), 
                        bpb::pipe::def(bpb::pipe::output_stream)).wait(); 
#if defined(BOOST_POSIX_API) 
    BOOST_REQUIRE(WIFEXITED(s2)); 
    BOOST_CHECK_EQUAL(WEXITSTATUS(s2), EXIT_FAILURE); 
#elif defined(BOOST_WINDOWS_API) 
    BOOST_CHECK_EQUAL(s2, EXIT_FAILURE); 
#endif 
} 

template <class Launcher, class Context, class Child> 
void test_close_stderr() 
{ 
    std::vector<std::string> args; 
    args.push_back("is-closed-stderr"); 

    int s1 = Launcher()(args, Context()).wait(); 
#if defined(BOOST_POSIX_API) 
    BOOST_REQUIRE(WIFEXITED(s1)); 
    BOOST_CHECK_EQUAL(WEXITSTATUS(s1), EXIT_SUCCESS); 
#elif defined(BOOST_WINDOWS_API) 
    BOOST_CHECK_EQUAL(s1, EXIT_SUCCESS); 
#endif 

    int s2 = Launcher()(args, Context(), bpb::close::def(), bpb::close::def(), 
                        bpb::pipe::def(bpb::pipe::output_stream)).wait(); 
#if defined(BOOST_POSIX_API) 
    BOOST_REQUIRE(WIFEXITED(s2)); 
    BOOST_CHECK_EQUAL(WEXITSTATUS(s2), EXIT_FAILURE); 
#elif defined(BOOST_WINDOWS_API) 
    BOOST_CHECK_EQUAL(s2, EXIT_FAILURE); 
#endif 
} 

template <class Launcher, class Context, class Child> 
void test_input() 
{ 
    std::vector<std::string> args; 
    args.push_back("stdin-to-stdout"); 

    Child c = Launcher()(args, Context(), 
                        bpb::pipe::def(bpb::pipe::input_stream), 
                        bpb::pipe::def(bpb::pipe::output_stream)); 

    bp::postream &os = c.get_stdin(); 
    bp::pistream &is = c.get_stdout(); 

    os << "message-to-process" << std::endl; 
    os.close(); 

    std::string word; 
    is >> word; 
    BOOST_CHECK_EQUAL(word, "message-to-process"); 

    int s = c.wait(); 
#if defined(BOOST_POSIX_API) 
    BOOST_REQUIRE(WIFEXITED(s)); 
    BOOST_CHECK_EQUAL(WEXITSTATUS(s), EXIT_SUCCESS); 
#elif defined(BOOST_WINDOWS_API) 
    BOOST_CHECK_EQUAL(s, EXIT_SUCCESS); 
#endif 
} 

template <class Launcher, class Context, class Child> 
void test_output(bool out, const std::string &msg) 
{ 
    std::vector<std::string> args; 
    args.push_back(out ? "echo-stdout" : "echo-stderr"); 
    args.push_back(msg); 

    Child c = Launcher()(args, Context(), bpb::close::def(), 
                        out ? bpb::pipe::def(bpb::pipe::output_stream) : 
                        bpb::close::def(), out ? bpb::close::def() : 
                        bpb::pipe::def(bpb::pipe::output_stream)); 

    std::string word; 
    if (out) 
    { 
        bp::pistream &is = c.get_stdout(); 
        is >> word; 
    } 
    else 
    { 
        bp::pistream &is = c.get_stderr(); 
        is >> word; 
    } 
    BOOST_CHECK_EQUAL(word, msg); 

    int s = c.wait(); 
#if defined(BOOST_POSIX_API) 
    BOOST_REQUIRE(WIFEXITED(s)); 
    BOOST_CHECK_EQUAL(WEXITSTATUS(s), EXIT_SUCCESS); 
#elif defined(BOOST_WINDOWS_API) 
    BOOST_CHECK_EQUAL(s, EXIT_SUCCESS); 
#endif 
} 

template <class Launcher, class Context, class Child> 
void test_stderr() 
{ 
    test_output<Launcher, Context, Child>(false, "message1-stderr"); 
    test_output<Launcher, Context, Child>(false, "message2-stderr"); 
} 

template <class Launcher, class Context, class Child> 
void test_stdout() 
{ 
    test_output<Launcher, Context, Child>(true, "message1-stdout"); 
    test_output<Launcher, Context, Child>(true, "message2-stdout"); 
} 

/* 
template <class Launcher, class Context, class Child> 
void test_redirect_err_to_out() 
{ 
    std::vector<std::string> args; 
    args.push_back("echo-stdout-stderr"); 
    args.push_back("message-to-two-streams"); 

    Child c = Launcher()(args, Context(), bpb::close::def(), 
                         bpb::pipe::def(bpb::pipe::output_stream), 
                         bp::redirect_stream_to_stdout()); 

    bp::pistream &is = c.get_stdout(); 
    std::string word; 
    is >> word; 
    BOOST_CHECK_EQUAL(word, "stdout"); 
    is >> word; 
    BOOST_CHECK_EQUAL(word, "message-to-two-streams"); 
    is >> word; 
    BOOST_CHECK_EQUAL(word, "stderr"); 
    is >> word; 
    BOOST_CHECK_EQUAL(word, "message-to-two-streams"); 

    int s = c.wait(); 
#if defined(BOOST_POSIX_API) 
    BOOST_REQUIRE(WIFEXITED(s)); 
    BOOST_CHECK_EQUAL(WEXITSTATUS(s), EXIT_SUCCESS); 
#elif defined(BOOST_WINDOWS_API) 
    BOOST_CHECK_EQUAL(s, EXIT_SUCCESS); 
#endif 
} 
*/ 

template <class Launcher, class Context, class Child> 
void check_work_directory(const std::string &wdir) 
{ 
    std::vector<std::string> args; 
    args.push_back("pwd"); 

    Context ctx; 
    if (wdir.empty()) 
        BOOST_CHECK(bfs::equivalent(ctx.work_dir, 
                                    bfs::current_path().string())); 
    else 
        ctx.work_dir = wdir; 
    Child c = Launcher()(args, ctx, bpb::close::def(), 
                        bpb::pipe::def(bpb::pipe::output_stream)); 

    bp::pistream &is = c.get_stdout(); 
    std::string dir; 
    std::getline(is, dir); 
    std::string::size_type pos = dir.rfind('\r'); 
    if (pos != std::string::npos) 
        dir.erase(pos); 

    int s = c.wait(); 
#if defined(BOOST_POSIX_API) 
    BOOST_REQUIRE(WIFEXITED(s)); 
    BOOST_CHECK_EQUAL(WEXITSTATUS(s), EXIT_SUCCESS); 
#elif defined(BOOST_WINDOWS_API) 
    BOOST_CHECK_EQUAL(s, EXIT_SUCCESS); 
#endif 

    BOOST_CHECK_EQUAL(bfs::path(dir), bfs::path(ctx.work_dir)); 
} 

template <class Launcher, class Context, class Child> 
void test_work_directory() 
{ 
    check_work_directory<Launcher, Context, Child>(""); 

    bfs::path wdir = bfs::current_path() / "test.dir"; 
    BOOST_REQUIRE_NO_THROW(bfs::create_directory(wdir)); 
    try 
    { 
        check_work_directory<Launcher, Context, Child>(wdir.string()); 
        BOOST_CHECK_NO_THROW(bfs::remove_all(wdir)); 
    } 
    catch (...) 
    { 
        BOOST_CHECK_NO_THROW(bfs::remove_all(wdir)); 
        throw; 
    } 
} 

template <class Launcher, class Context, class Child> 
std::pair<bool, std::string> get_var_value(Context &ctx, const std::string &var) 
{ 
    std::vector<std::string> args; 
    args.push_back("query-env"); 
    args.push_back(var); 

    Child c = Launcher()(args, ctx, bpb::close::def(), 
                        bpb::pipe::def(bpb::pipe::output_stream)); 

    bp::pistream &is = c.get_stdout(); 
    std::string status; 
    is >> status; 
    std::string gotval; 
    if (status == "defined") 
        is >> gotval; 

    int s = c.wait(); 
#if defined(BOOST_POSIX_API) 
    BOOST_REQUIRE(WIFEXITED(s)); 
    BOOST_CHECK_EQUAL(WEXITSTATUS(s), EXIT_SUCCESS); 
#elif defined(BOOST_WINDOWS_API) 
    BOOST_CHECK_EQUAL(s, EXIT_SUCCESS); 
#endif 

    return std::pair<bool, std::string>(status == "defined", gotval); 
} 

template <class Launcher, class Context, class Child> 
void test_clear_environment() 
{ 
    Context ctx; 
    ctx.environment.erase("TO_BE_QUERIED"); 

#if defined(BOOST_POSIX_API) 
    BOOST_REQUIRE(setenv("TO_BE_QUERIED", "test", 1) != -1); 
    BOOST_REQUIRE(getenv("TO_BE_QUERIED") != 0); 
#elif defined(BOOST_WINDOWS_API) 
    BOOST_REQUIRE(SetEnvironmentVariableA("TO_BE_QUERIED", "test") != 0); 
    char buf[5]; 
    BOOST_REQUIRE(GetEnvironmentVariableA("TO_BE_QUERIED", buf, 5) == 4); 
#endif 

    std::pair<bool, std::string> p = 
        get_var_value<Launcher, Context, Child>(ctx, "TO_BE_QUERIED"); 
    BOOST_REQUIRE(!p.first); 
} 

template <class Launcher, class Context, class Child> 
void test_unset_environment() 
{ 
    std::vector<std::string> args; 
    args.push_back("query-env"); 
    args.push_back("TO_BE_UNSET"); 

#if defined(BOOST_POSIX_API) 
    BOOST_REQUIRE(setenv("TO_BE_UNSET", "test", 1) != -1); 
    BOOST_REQUIRE(getenv("TO_BE_UNSET") != 0); 
#elif defined(BOOST_WINDOWS_API) 
    BOOST_REQUIRE(SetEnvironmentVariableA("TO_BE_UNSET", "test") != 0); 
    char buf[5]; 
    BOOST_REQUIRE(GetEnvironmentVariableA("TO_BE_UNSET", buf, 5) == 4); 
#endif 

    Context ctx; 
    ctx.environment.erase("TO_BE_UNSET"); 
    std::pair<bool, std::string> p = 
        get_var_value<Launcher, Context, Child>(ctx, "TO_BE_UNSET"); 
    BOOST_CHECK(!p.first); 
} 

template <class Launcher, class Context, class Child> 
void test_set_environment_var(const std::string &value) 
{ 
    std::vector<std::string> args; 
    args.push_back("query-env"); 
    args.push_back("TO_BE_SET"); 

#if defined(BOOST_POSIX_API) 
    unsetenv("TO_BE_SET"); 
    BOOST_REQUIRE(getenv("TO_BE_SET") == 0); 
#elif defined(BOOST_WINDOWS_API) 
    char buf[5]; 
    BOOST_REQUIRE(GetEnvironmentVariableA("TO_BE_SET", buf, 5) == 0 || 
                    SetEnvironmentVariableA("TO_BE_SET", NULL) != 0); 
    BOOST_REQUIRE(GetEnvironmentVariableA("TO_BE_SET", buf, 5) == 0); 
#endif 

    Context ctx; 
    ctx.environment.insert(bp::environment_t::value_type("TO_BE_SET", value)); 
    std::pair<bool, std::string> p = 
        get_var_value<Launcher, Context, Child>(ctx, "TO_BE_SET"); 
    BOOST_CHECK(p.first); 
    BOOST_CHECK_EQUAL(p.second, "'" + value + "'"); 
} 

template <class Launcher, class Context, class Child> 
void test_set_environment() 
{ 
#if defined(BOOST_POSIX_API) 
    test_set_environment_var<Launcher, Context, Child>(""); 
#endif 
    test_set_environment_var<Launcher, Context, Child>("some-value-1"); 
    test_set_environment_var<Launcher, Context, Child>("some-value-2"); 
} 

} 
