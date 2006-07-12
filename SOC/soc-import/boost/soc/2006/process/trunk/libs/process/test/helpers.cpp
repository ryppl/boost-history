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

#include <iostream>
#include <string>

#include <boost/filesystem/operations.hpp>

namespace bfs = ::boost::filesystem;

int h_echo_stderr(int, char*[]);
int h_echo_stdout(int, char*[]);
int h_exit_failure(int, char*[]);
int h_exit_success(int, char*[]);
int h_pwd(int, char*[]);
int h_stdin_to_stdout(int, char*[]);

struct helper {
    const char* m_name;
    int (*m_entry)(int, char*[]);
    int m_min_argc;
    const char* m_syntax;
} helpers[] = {
    { "echo-stderr", h_echo_stderr, 2, "message" },
    { "echo-stdout", h_echo_stdout, 2, "message" },
    { "exit-failure", h_exit_failure, 1, "" },
    { "exit-success", h_exit_success, 1, "" },
    { "pwd", h_pwd, 1, "" },
    { "stdin-to-stdout", h_stdin_to_stdout, 1, "" },
    { NULL, NULL }
};

// ------------------------------------------------------------------------

int
h_echo_stderr(int argc, char* argv[])
{
    std::cerr << argv[1] << std::endl;
    return EXIT_SUCCESS;
}

// ------------------------------------------------------------------------

int
h_echo_stdout(int argc, char* argv[])
{
    std::cout << argv[1] << std::endl;
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
h_stdin_to_stdout(int argc, char* argv[])
{
    char ch;
    while (std::cin >> ch)
        std::cout << ch;

    return EXIT_SUCCESS;
}

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
