//
// Boost.Process
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

//!
//! \file boost/process/operations.hpp
//!
//! Provides miscellaneous free functions.
//!

#if !defined(BOOST_PROCESS_OPERATIONS_HPP)
/** \cond */
#define BOOST_PROCESS_OPERATIONS_HPP
/** \endcond */

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_POSIX_API)
#elif defined(BOOST_PROCESS_WIN32_API)
#   include <tchar.h>
#   include <windows.h>
#else
#   error "Unsupported platform."
#endif

#include <string>
#include <vector>

#include <boost/assert.hpp>
#include <boost/process/child.hpp>
#include <boost/process/exceptions.hpp>
#include <boost/throw_exception.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

//!
//! \brief Locates a program in the path.
//!
//! Locates the executable program \a file in all the directory components
//! specified in \a path.  If \a path is empty, the value of the PATH
//! environment variable is used.
//!
//! The path variable is interpreted following the same conventions used
//! to parse the PATH environment variable in the underlying platform.
//!
//! \throw not_found_error&lt;std::string&gt; If the file cannot be found
//! in the path.
//!
inline
std::string
find_executable_in_path(const std::string& file, std::string path = "")
{
#if defined(BOOST_PROCESS_POSIX_API)
    BOOST_ASSERT(file.find('/') == std::string::npos);
#elif defined(BOOST_PROCESS_WIN32_API)
    BOOST_ASSERT(file.find('\\') == std::string::npos);
#endif

    std::string result;

#if defined(BOOST_PROCESS_POSIX_API)
    if (path.empty()) {
        const char* envpath = ::getenv("PATH");
        if (envpath == NULL)
            boost::throw_exception(not_found_error< std::string >
                ("Cannot locate " + file + " in path; "
                 "error retrieving PATH's value", file));
        path = envpath;
    }
    BOOST_ASSERT(!path.empty());

    std::string::size_type pos1 = 0, pos2;
    do {
        pos2 = path.find(':', pos1);
        std::string dir = path.substr(pos1, pos2 - pos1);
        std::string f = dir + '/' + file;
        if (::access(f.c_str(), X_OK) == 0)
            result = f;
        pos1 = pos2 + 1;
    } while (pos2 != std::string::npos && result.empty());
#elif defined(BOOST_PROCESS_WIN32_API)
    const char* exts[] = { "", ".exe", ".com", ".bat", NULL };
    const char** ext = exts;
    while (*ext != NULL) {
        TCHAR buf[MAX_PATH];
        TCHAR* dummy;
        DWORD len = ::SearchPath(path.empty() ? NULL : TEXT(path.c_str()),
                                 TEXT(file.c_str()), TEXT(*ext), MAX_PATH,
                                 buf, &dummy);
        BOOST_ASSERT(len < MAX_PATH);
        if (len > 0) {
            result = buf;
            break;
        }
        ext++;
    }
#endif

    if (result.empty())
        boost::throw_exception(not_found_error< std::string >
            ("Cannot locate " + file + " in path", file));

    return result;
}

// ------------------------------------------------------------------------

inline
std::string
executable_to_progname(const std::string& exe)
{
    std::string::size_type tmp;
    std::string::size_type begin = 0;
    std::string::size_type end = std::string::npos;

#if defined(BOOST_PROCESS_POSIX_API)
    tmp = exe.rfind('/');
#elif defined(BOOST_PROCESS_WIN32_API)
    tmp = exe.rfind('\\');
    if (tmp == std::string::npos)
        tmp = exe.rfind('/');
#endif
    if (tmp != std::string::npos)
        begin = tmp + 1;

#if defined(BOOST_PROCESS_WIN32_API)
    if (exe.substr(exe.length() - 4) == ".exe" ||
        exe.substr(exe.length() - 4) == ".com" ||
        exe.substr(exe.length() - 4) == ".bat")
        end = exe.length() - 4;
#endif

    return exe.substr(begin, end);
}

// ------------------------------------------------------------------------

//!
//! \brief Launches a shell-based command.
//!
//! Executes the given command through the default system shell.  The
//! command is subject to pattern expansion, redirection and pipelining.
//!
//! In a POSIX system, the command is fed to /bin/sh; under a
//! Win32 system, it is fed to cmd.exe.  This makes it difficult to
//! write really portable commands using this launcher.
//!
template< class Launcher >
inline
child
launch_shell(Launcher& l, const std::string& command)
{
    std::string exe;
    std::vector< std::string > args;

#if defined(BOOST_PROCESS_POSIX_API)
    exe = "/bin/sh";
    args.push_back("sh");
    args.push_back("-c");
    args.push_back(command);
#elif defined(BOOST_PROCESS_WIN32_API)
    TCHAR buf[MAX_PATH];
    UINT res = ::GetSystemDirectory(buf, MAX_PATH);
    if (res == 0)
        boost::throw_exception
            (system_error("boost::process::launch_shell",
                          "GetWindowsDirectory failed", ::GetLastError()));
    BOOST_ASSERT(res < MAX_PATH);

    exe = std::string(buf) + "\\cmd.exe";
    args.push_back("cmd");
    args.push_back("/c");
    args.push_back(command);
#endif

    return l.start(exe, args);
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_OPERATIONS_HPP)
