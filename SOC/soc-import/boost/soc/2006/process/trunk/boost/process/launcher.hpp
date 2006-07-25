//
// Boost.Process
// Implementation of the Launcher concept.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#if !defined(BOOST_PROCESS_LAUNCHER_HPP)
#define BOOST_PROCESS_LAUNCHER_HPP

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_POSIX_API)
#   include <cstring>
#elif defined(BOOST_PROCESS_WIN32_API)
#   include <tchar.h>
#   include <windows.h>
#else
#   error "Unsupported platform."
#endif

#include <boost/assert.hpp>
#include <boost/optional.hpp>
#include <boost/process/basic_child.hpp>
#include <boost/process/detail/environment.hpp>
#include <boost/process/detail/file_handle.hpp>
#include <boost/process/exceptions.hpp>
#include <boost/throw_exception.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

class launcher
{
    int m_flags;
    detail::environment m_environment;

#if defined(BOOST_PROCESS_POSIX_API)
    template< class Command_Line, class Attributes >
    basic_child< Command_Line, Attributes > start_posix
        (const Command_Line& cl,
         const Attributes& attrs,
         boost::optional< detail::pipe > pstdin,
         boost::optional< detail::pipe > pstdout,
         boost::optional< detail::pipe > pstderr);
#elif defined(BOOST_PROCESS_WIN32_API)
    template< class Command_Line, class Attributes >
    basic_child< Command_Line, Attributes > start_win32
        (const Command_Line& cl,
         const Attributes& attrs,
         boost::optional< detail::pipe > pstdin,
         boost::optional< detail::pipe > pstdout,
         boost::optional< detail::pipe > pstderr);
#endif

protected:
#if defined(BOOST_PROCESS_POSIX_API)
    template< class Command_Line, class Attributes >
    void posix_child_entry(const Command_Line& cl, const Attributes& attrs);
#endif

public:
    static const int REDIR_NONE = 0;
    static const int REDIR_STDIN = (1 << 0);
    static const int REDIR_STDOUT = (1 << 1);
    static const int REDIR_STDERR = (1 << 2);
    static const int REDIR_STDERR_TO_STDOUT = (1 << 3);
    static const int REDIR_ALL =
        REDIR_STDIN | REDIR_STDOUT | REDIR_STDERR;
    static const int REDIR_ALL_MERGE =
        REDIR_STDIN | REDIR_STDOUT | REDIR_STDERR_TO_STDOUT;

    launcher(int flags = REDIR_ALL);

    int get_flags(void) const;
    void set_flags(int flags);

    void set_environment(const std::string& var, const std::string& value);
    void unset_environment(const std::string& var);

    template< class Command_Line, class Attributes >
    basic_child< Command_Line, Attributes > start(const Command_Line& cl,
                                                  const Attributes& attrs);
};

// ------------------------------------------------------------------------

inline
launcher::launcher(int flags) :
    m_flags(REDIR_NONE)
{
    set_flags(flags);
}

// ------------------------------------------------------------------------

inline
void
launcher::set_environment(const std::string& var, const std::string& value)
{
    BOOST_ASSERT(!var.empty());
    m_environment.set(var, value);
}

// ------------------------------------------------------------------------

inline
void
launcher::unset_environment(const std::string& var)
{
    BOOST_ASSERT(!var.empty());
    m_environment.unset(var);
}

// ------------------------------------------------------------------------

inline
int
launcher::get_flags(void)
    const
{
    return m_flags;
}

// ------------------------------------------------------------------------

inline
void
launcher::set_flags(int flags)
{
    BOOST_ASSERT
        ((flags & REDIR_STDERR) || (flags & REDIR_STDERR_TO_STDOUT) !=
         REDIR_STDERR || REDIR_STDERR_TO_STDOUT);
    m_flags = flags;
}

// ------------------------------------------------------------------------

template< class Command_Line, class Attributes >
inline
basic_child< Command_Line, Attributes >
launcher::start(const Command_Line& cl, const Attributes& attrs)
{
    boost::optional< detail::pipe > pstdin;
    if (m_flags & REDIR_STDIN)
        pstdin = detail::pipe();

    boost::optional< detail::pipe > pstdout;
    if (m_flags & REDIR_STDOUT)
        pstdout = detail::pipe();

    boost::optional< detail::pipe > pstderr;
    if (m_flags & REDIR_STDERR)
        pstderr = detail::pipe();

#if defined(BOOST_PROCESS_POSIX_API)
    return start_posix(cl, attrs, pstdin, pstdout, pstderr);
#elif defined(BOOST_PROCESS_WIN32_API)
    return start_win32(cl, attrs, pstdin, pstdout, pstderr);
#endif
}

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
template< class Command_Line, class Attributes >
inline
basic_child< Command_Line, Attributes >
launcher::start_posix(const Command_Line& cl,
                      const Attributes& attrs,
                      boost::optional< detail::pipe > pstdin,
                      boost::optional< detail::pipe > pstdout,
                      boost::optional< detail::pipe > pstderr)
{
    pid_t pid = ::fork();
    if (pid == -1) {
        boost::throw_exception
            (system_error("boost::process::launcher::start",
                          "fork(2) failed", errno));
    } else if (pid == 0) {
        try {
            // File descriptors that remain open for the child process to
            // communicate with its parent are disowned from their file
            // handle objects to ensure that their destructor, if ever
            // executed, does not close them.
            if (m_flags & REDIR_STDIN) {
                pstdin->wend().close();
                pstdin->rend().posix_remap(STDIN_FILENO);
                pstdin->rend().disown();
            }

            if (m_flags & REDIR_STDOUT) {
                pstdout->rend().close();
                pstdout->wend().posix_remap(STDOUT_FILENO);
                pstdout->wend().disown();
            }

            if (m_flags & REDIR_STDERR) {
                pstderr->rend().close();
                pstderr->wend().posix_remap(STDERR_FILENO);
                pstderr->wend().disown();
            }

            if (m_flags & REDIR_STDERR_TO_STDOUT) {
                detail::file_handle errfh = detail::file_handle::posix_dup
                    (STDOUT_FILENO, STDERR_FILENO);
                errfh.disown();
            }
        } catch (const system_error& e) {
            ::write(STDERR_FILENO, e.what(), std::strlen(e.what()));
            ::write(STDERR_FILENO, "\n", 1);
            ::exit(EXIT_FAILURE);
        }

        posix_child_entry(cl, attrs);
        BOOST_ASSERT(false); // Not reached.
    } else {
        detail::file_handle fhstdin, fhstdout, fhstderr;

        if (m_flags & REDIR_STDIN) {
            pstdin->rend().close();
            fhstdin = pstdin->wend();
        }

        if (m_flags & REDIR_STDOUT) {
            pstdout->wend().close();
            fhstdout = pstdout->rend();
        }

        if (m_flags & REDIR_STDERR) {
            pstderr->wend().close();
            fhstderr = pstderr->rend();
        }

        BOOST_ASSERT(!(m_flags & REDIR_STDIN) || fhstdin.is_valid());
        BOOST_ASSERT(!(m_flags & REDIR_STDOUT) || fhstdout.is_valid());
        BOOST_ASSERT(!(m_flags & REDIR_STDERR) || fhstderr.is_valid());
        return basic_child< Command_Line, Attributes >
            (pid, cl, attrs, fhstdin, fhstdout, fhstderr);
    }

    BOOST_ASSERT(false); // Not reached.
    detail::file_handle fhstdin, fhstdout, fhstderr;
    return basic_child< Command_Line, Attributes >
        (pid, cl, attrs, fhstdin, fhstdout, fhstderr);
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
template< class Command_Line, class Attributes >
inline
void
launcher::posix_child_entry(const Command_Line& cl, const Attributes& attrs)
{
    try {
        attrs.setup();
    } catch (const system_error& e) {
        ::write(STDERR_FILENO, e.what(), std::strlen(e.what()));
        ::write(STDERR_FILENO, "\n", 1);
        ::exit(EXIT_FAILURE);
    }

    std::pair< size_t, char** > args = cl.posix_argv();
    char** envp = m_environment.envp();

    ::execve(cl.get_executable().c_str(), args.second, envp);
    system_error e("boost::process::launcher::start",
                   "execvp(2) failed", errno);

    for (size_t i = 0; i < args.first; i++)
        delete [] args.second[i];
    delete [] args.second;

    for (size_t i = 0; i < m_environment.size(); i++)
        delete [] envp[i];
    delete [] envp;

    ::write(STDERR_FILENO, e.what(), std::strlen(e.what()));
    ::write(STDERR_FILENO, "\n", 1);
    ::exit(EXIT_FAILURE);
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_WIN32_API)
template< class Command_Line, class Attributes >
inline
basic_child< Command_Line, Attributes >
launcher::start_win32(const Command_Line& cl, const Attributes& attrs,
                      boost::optional< detail::pipe > pstdin,
                      boost::optional< detail::pipe > pstdout,
                      boost::optional< detail::pipe > pstderr)
{
    detail::file_handle fhstdin, fhstdout, fhstderr;
    detail::file_handle chstdin, chstdout, chstderr;

    STARTUPINFO si;
    ::ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;

    if (m_flags & REDIR_STDIN) {
        pstdin->rend().win32_set_inheritable(true);
        chstdin = pstdin->rend();
        fhstdin = pstdin->wend();
    } else
        chstdin = detail::file_handle::win32_std(STD_INPUT_HANDLE, true);
    si.hStdInput = chstdin.get();

    if (m_flags & REDIR_STDOUT) {
        pstdout->wend().win32_set_inheritable(true);
        chstdout = pstdout->wend();
        fhstdout = pstdout->rend();
    } else
        chstdout = detail::file_handle::win32_std(STD_OUTPUT_HANDLE, true);
    si.hStdOutput = chstdout.get();

    if (m_flags & REDIR_STDERR) {
        pstderr->wend().win32_set_inheritable(true);
        chstderr = pstderr->wend();
        fhstderr = pstderr->rend();
    } else
        chstderr = detail::file_handle::win32_std(STD_ERROR_HANDLE, true);
    if (m_flags & REDIR_STDERR_TO_STDOUT)
        chstderr = detail::file_handle::win32_dup(si.hStdOutput, true);
    si.hStdError = chstderr.get();

    PROCESS_INFORMATION pi;
    ::ZeroMemory(&pi, sizeof(pi));

    boost::shared_array< TCHAR > cmdline = cl.win32_cmdline();
    boost::scoped_array< TCHAR > executable
        (::_tcsdup(TEXT(cl.get_executable().c_str())));
    boost::scoped_array< TCHAR > workdir
        (::_tcsdup(TEXT(attrs.get_work_directory().c_str())));
    m_environment.set("", attrs.get_work_directory());

    boost::shared_array< TCHAR > env = m_environment.strings();
    if (!::CreateProcess(executable.get(), cmdline.get(), NULL, NULL, TRUE,
                         0, env.get(), workdir.get(), &si, &pi)) {
        boost::throw_exception
            (system_error("boost::process::launcher::start",
                          "CreateProcess failed", ::GetLastError()));
    }

    BOOST_ASSERT(!(m_flags & REDIR_STDIN) || fhstdin.is_valid());
    BOOST_ASSERT(!(m_flags & REDIR_STDOUT) || fhstdout.is_valid());
    BOOST_ASSERT(!(m_flags & REDIR_STDERR) || fhstderr.is_valid());
    return basic_child< Command_Line, Attributes >
        (pi.hProcess, cl, attrs, fhstdin, fhstdout, fhstderr);
}
#endif

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_LAUNCHER_HPP)
