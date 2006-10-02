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
#   include <boost/process/detail/posix_ops.hpp>
#elif defined(BOOST_PROCESS_WIN32_API)
#   include <tchar.h>
#   include <windows.h>
#   include <boost/process/detail/win32_ops.hpp>
#else
#   error "Unsupported platform."
#endif

#include <string>
#include <vector>

#include <boost/assert.hpp>
#include <boost/process/child.hpp>
#include <boost/process/detail/file_handle.hpp>
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
    if (exe.length() > 4 &&
        (exe.substr(exe.length() - 4) == ".exe" ||
         exe.substr(exe.length() - 4) == ".com" ||
         exe.substr(exe.length() - 4) == ".bat"))
        end = exe.length() - 4;
#endif

    return exe.substr(begin, end);
}

// ------------------------------------------------------------------------

//!
//! \brief Starts a new child process.
//!
//! Launches a new process based on the binary image specified by the
//! executable, the set of arguments to be passed to it and several
//! parameters that describe the execution context.
//!
//! \remark <b>Blocking remarks</b>: This function may block if the device
//! holding the executable blocks when loading the image.  This might
//! happen if, e.g., the binary is being loaded from a network share.
//!
//! \return A handle to the new child process.
//!
template< class Executable, class Arguments, class Context >
child
launch(const Executable& exe, const Arguments& args, const Context& ctx)
{
    using detail::stream_info;

    child::handle_type ph;
    detail::file_handle fhstdin, fhstdout, fhstderr;

    BOOST_ASSERT(!args.empty());

    // Validate execution context.
    // XXX Should this be a 'validate()' method in it?
    BOOST_ASSERT(!ctx.m_work_directory.empty());

#if defined(BOOST_PROCESS_POSIX_API)
    detail::info_map infoin, infoout;

    if (ctx.m_stdin_behavior.get_type() != stream_behavior::close) {
        stream_info si = stream_info(ctx.m_stdin_behavior, false);
        infoin.insert(detail::info_map::value_type(STDIN_FILENO, si));
    }

    if (ctx.m_stdout_behavior.get_type() != stream_behavior::close) {
        stream_info si = stream_info(ctx.m_stdout_behavior, true);
        infoout.insert(detail::info_map::value_type(STDOUT_FILENO, si));
    }

    if (ctx.m_stderr_behavior.get_type() != stream_behavior::close) {
        stream_info si = stream_info(ctx.m_stderr_behavior, true);
        infoout.insert(detail::info_map::value_type(STDERR_FILENO, si));
    }

    detail::posix_setup s;
    s.m_work_directory = ctx.m_work_directory;

    ph = detail::posix_start(exe, args, ctx.m_environment, infoin, infoout, s);

    if (ctx.m_stdin_behavior.get_type() == stream_behavior::capture)
        fhstdin = posix_info_locate_pipe(infoin, STDIN_FILENO, false);

    if (ctx.m_stdout_behavior.get_type() == stream_behavior::capture)
        fhstdout = posix_info_locate_pipe(infoout, STDOUT_FILENO, true);

    if (ctx.m_stderr_behavior.get_type() == stream_behavior::capture)
        fhstderr = posix_info_locate_pipe(infoout, STDERR_FILENO, true);
#elif defined(BOOST_PROCESS_WIN32_API)
    stream_info behin = stream_info(ctx.m_stdin_behavior, false);
    if (behin.m_type == stream_info::use_pipe)
        fhstdin = behin.m_pipe->wend();
    stream_info behout = stream_info(ctx.m_stdout_behavior, true);
    if (behout.m_type == stream_info::use_pipe)
        fhstdout = behout.m_pipe->rend();
    stream_info beherr = stream_info(ctx.m_stderr_behavior, true);
    if (beherr.m_type == stream_info::use_pipe)
        fhstderr = beherr.m_pipe->rend();

    STARTUPINFO si;
    ::ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    detail::win32_setup s;
    s.m_work_directory = ctx.m_work_directory;
    s.m_startupinfo = &si;

    PROCESS_INFORMATION pi =
        detail::win32_start(exe, args, ctx.m_environment,
                            behin, behout, beherr, s);

    ph = pi.hProcess;
#endif

    return child(ph, fhstdin, fhstdout, fhstderr);
}

// ------------------------------------------------------------------------

//!
//! \brief Launches a shell-based command.
//!
//! Executes the given command through the default system shell.  The
//! command is subject to pattern expansion, redirection and pipelining.
//! The shell is launched as described by the parameters in the execution
//! context.
//!
//! This function behaves similarly to the system(3) system call.  In a
//! POSIX system, the command is fed to /bin/sh whereas under a Win32
//! system, it is fed to cmd.exe.  It is difficult to write portable
//! commands as the first parameter, but this function comes in handy in
//! multiple situations.
//!
template< class Context >
inline
child
launch_shell(const std::string& command, const Context& ctx)
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

    return launch(exe, args, ctx);
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_OPERATIONS_HPP)
