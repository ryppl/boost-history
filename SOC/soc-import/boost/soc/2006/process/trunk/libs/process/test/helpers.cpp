//
// Boost.Process
// Helper utilities for tests that launch child processes.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_WIN32_API)
#   include <windows.h>
#endif

#include <cstdlib>
#include <iostream>
#include <string>

#include <boost/filesystem/operations.hpp>
#include <boost/process/detail/systembuf.hpp>

namespace bfs = ::boost::filesystem;
namespace bpd = ::boost::process::detail;

int h_echo_stdout(int, char*[]);
int h_echo_stderr(int, char*[]);
int h_echo_stdout_stderr(int, char*[]);
int h_exit_failure(int, char*[]);
int h_exit_success(int, char*[]);
int h_pwd(int, char*[]);
int h_query_env(int, char*[]);
int h_stdin_to_stdout(int, char*[]);

#if defined(BOOST_PROCESS_POSIX_API)
int h_posix_echo_one(int, char*[]);
int h_posix_echo_two(int, char*[]);
#endif

struct helper {
    const char* m_name;
    int (*m_entry)(int, char*[]);
    int m_min_argc;
    const char* m_syntax;
} helpers[] = {
    { "echo-stdout", h_echo_stdout, 2, "message" },
    { "echo-stderr", h_echo_stderr, 2, "message" },
    { "echo-stdout-stderr", h_echo_stdout_stderr, 2, "message" },
    { "exit-failure", h_exit_failure, 1, "" },
    { "exit-success", h_exit_success, 1, "" },
    { "pwd", h_pwd, 1, "" },
    { "query-env", h_query_env, 2, "variable" },
    { "stdin-to-stdout", h_stdin_to_stdout, 1, "" },

#if defined(BOOST_PROCESS_POSIX_API)
    { "posix-echo-one", h_posix_echo_one, 3, "desc message" },
    { "posix-echo-two", h_posix_echo_two, 4, "desc1 desc2 message" },
#endif

    { NULL, NULL }
};

// ------------------------------------------------------------------------

int
h_echo_stdout(int argc, char* argv[])
{
    std::cout << argv[1] << std::endl;
    return EXIT_SUCCESS;
}

// ------------------------------------------------------------------------

int
h_echo_stderr(int argc, char* argv[])
{
    std::cerr << argv[1] << std::endl;
    return EXIT_SUCCESS;
}

// ------------------------------------------------------------------------

int
h_echo_stdout_stderr(int argc, char* argv[])
{
    std::cout << "stdout " << argv[1] << std::endl;
    std::cout.flush();
    std::cerr << "stderr " << argv[1] << std::endl;
    std::cerr.flush();
    return EXIT_SUCCESS;
}

// ------------------------------------------------------------------------

int
h_exit_failure(int argc, char* argv[])
{
    return EXIT_FAILURE;
}

// ------------------------------------------------------------------------

int
h_exit_success(int argc, char* argv[])
{
    return EXIT_SUCCESS;
}

// ------------------------------------------------------------------------

int
h_pwd(int argc, char* argv[])
{
    std::cout << bfs::current_path().string() << std::endl;
    return EXIT_SUCCESS;
}

// ------------------------------------------------------------------------

int
h_query_env(int argc, char* argv[])
{
#if defined(BOOST_PROCESS_WIN32_API)
    TCHAR buf[1024];
    DWORD res = GetEnvironmentVariable(TEXT(argv[1]), (LPTSTR) &buf, sizeof(buf));
    if (res == 0)
        std::cout << "undefined" << std::endl;
    else {
        std::cout << "defined" << std::endl;
        std::cout << "'" << buf << "'" << std::endl;
    }
#else
    const char* value = ::getenv(argv[1]);
    if (value == NULL)
        std::cout << "undefined" << std::endl;
    else {
        std::cout << "defined" << std::endl;
        std::cout << "'" << value << "'" << std::endl;
    }
#endif

    return EXIT_SUCCESS;
}

// ------------------------------------------------------------------------

int
h_stdin_to_stdout(int argc, char* argv[])
{
    char ch;
    while (std::cin >> ch)
        std::cout << ch;

    return EXIT_SUCCESS;
}

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
int
h_posix_echo_one(int argc, char* argv[])
{
    int desc = std::atoi(argv[1]);

    bpd::systembuf buf(desc);
    std::ostream os(&buf);
    os << argv[2] << std::endl;

    return EXIT_SUCCESS;
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
int
h_posix_echo_two(int argc, char* argv[])
{
    int desc1 = std::atoi(argv[1]);
    int desc2 = std::atoi(argv[2]);

    bpd::systembuf buf1(desc1);
    std::ostream os1(&buf1);
    os1 << argv[1] << " " << argv[3] << std::endl;
    os1.flush();

    bpd::systembuf buf2(desc2);
    std::ostream os2(&buf2);
    os2 << argv[2] << " " << argv[3] << std::endl;
    os2.flush();

    return EXIT_SUCCESS;
}
#endif

// ------------------------------------------------------------------------

int
main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "helpers: Missing command" << std::endl;
        return 128;
    }

    std::string command(argv[1]);
    argc--; argv++;

    struct helper* h = helpers;
    while (h->m_name != NULL) {
        if (command == h->m_name) {
            int res;
            if (argc < h->m_min_argc) {
                std::cerr << "helpers: Command syntax: `" << command << " "
                          << h->m_syntax << "'" << std::endl;
                res = 128;
            } else
                res = (*h->m_entry)(argc, argv);
            return res;
        }
        h++;
    }

    std::cerr << "helpers: Invalid command `" << command << "'"
              << std::endl;
    return 128;
}
