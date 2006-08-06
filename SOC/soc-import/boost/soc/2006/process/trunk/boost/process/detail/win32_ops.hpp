//
// Boost.Process
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

//!
//! \file boost/process/detail/win32_ops.hpp
//!
//! Provides some convenience functions to start processes under Win32
//! operating systems.
//!

#if !defined(BOOST_PROCESS_DETAIL_WIN32_OPS_HPP)
/** \cond */
#define BOOST_PROCESS_DETAIL_WIN32_OPS_HPP
/** \endcond */

#include <boost/process/config.hpp>

#if !defined(BOOST_PROCESS_WIN32_API)
#   error "Unsupported platform."
#endif

#include <tchar.h>
#include <windows.h>

#include <boost/optional.hpp>
#include <boost/process/detail/command_line_ops.hpp>
#include <boost/process/detail/environment.hpp>
#include <boost/process/detail/file_handle.hpp>
#include <boost/process/detail/pipe.hpp>
#include <boost/process/exceptions.hpp>
#include <boost/throw_exception.hpp>

namespace boost {
namespace process {
namespace detail {

// ------------------------------------------------------------------------

//!
//! \brief Helper class to configure a Win32 %child.
//!
//! This helper class is used to hold all the attributes that configure a
//! new Win32 %child process .
//!
//! All its fields are public for simplicity.  It is only intended for
//! internal use and it is heavily coupled with the Launcher
//! implementations.
//!
struct win32_setup
{
    std::string& m_work_directory;
    STARTUPINFO* m_startupinfo;
};

// ------------------------------------------------------------------------

//!
//! \brief Starts a new child process in a Win32 operating system.
//!
//! This helper functions is provided to simplify the Launcher's task when
//! it comes to starting up a new process in a Win32 operating system.
//!
//! \param cl The command line used to execute the child process.
//! \param env The environment variables that the new child process
//!            receives.
//! \param pstdin If the child should redirect its stdin, the pipe used
//!               to set up this communication channel.
//! \param pstdout If the child should redirect its stdout, the pipe used
//!                to set up this communication channel.
//! \param pstderr If the child should redirect its stderr, the pipe used
//!                to set up this communication channel.
//! \param setup A helper object holding extra child information.  The
//!              STARTUPINFO object in it is modified to set up the
//!              required redirections.
//! \return The new process' information as returned by the ::CreateProcess
//!         system call.  The caller is responsible of creating an
//!         appropriate Child representation for it.
//!
template< class Command_Line >
inline
PROCESSINFO
win32_start(const Command_Line& cl,
            const environment& env,
            boost::optional< pipe > pstdin,
            boost::optional< pipe > pstdout,
            boost::optional< pipe > pstderr,
            win32_setup& setup)
{
    file_handle fhstdin, fhstdout, fhstderr;
    file_handle chstdin, chstdout, chstderr;

    setup.si->dwFlags = STARTF_USESTDHANDLES;

    if (m_flags & REDIR_STDIN) {
        pstdin->rend().win32_set_inheritable(true);
        chstdin = pstdin->rend();
        fhstdin = pstdin->wend();
    } else
        chstdin = file_handle::win32_std(STD_INPUT_HANDLE, true);
    setup.si->hStdInput = chstdin.get();

    if (m_flags & REDIR_STDOUT) {
        pstdout->wend().win32_set_inheritable(true);
        chstdout = pstdout->wend();
        fhstdout = pstdout->rend();
    } else
        chstdout = file_handle::win32_std(STD_OUTPUT_HANDLE, true);
    setup.si->hStdOutput = chstdout.get();

    if (m_flags & REDIR_STDERR) {
        pstderr->wend().win32_set_inheritable(true);
        chstderr = pstderr->wend();
        fhstderr = pstderr->rend();
    } else
        chstderr = file_handle::win32_std(STD_ERROR_HANDLE, true);
    if (m_flags & REDIR_STDERR_TO_STDOUT)
        chstderr = file_handle::win32_dup(setup.si->hStdOutput, true);
    setup.si->hStdError = chstderr.get();

    PROCESSINFO pi;
    ::ZeroMemory(&pi, sizeof(pi));

    boost::shared_array< TCHAR > cmdline =
        command_line_to_win32_cmdline(cl);
    boost::scoped_array< TCHAR > executable
        (::_tcsdup(TEXT(cl.get_executable().c_str())));
    boost::scoped_array< TCHAR > workdir
        (::_tcsdup(TEXT(setup.m_work_directory.c_str())));

    boost::shared_array< TCHAR > env = env.win32_strings();
    if (!::CreateProcess(executable.get(), cmdline.get(), NULL, NULL, TRUE,
                         0, env.get(), workdir.get(), setup.si, &pi)) {
        boost::throw_exception
            (system_error("boost::process::detail::win32_start",
                          "CreateProcess failed", ::GetLastError()));
    }

    return pi;
}

// ------------------------------------------------------------------------

} // namespace detail
} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_DETAIL_WIN32_OPS_HPP)
