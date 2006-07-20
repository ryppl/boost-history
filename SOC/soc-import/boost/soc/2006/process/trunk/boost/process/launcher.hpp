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
#   include <windows.h>
#else
#   error "Unsupported platform."
#endif

#include <boost/assert.hpp>
#include <boost/optional.hpp>
#include <boost/process/basic_child.hpp>
#include <boost/process/detail/environment.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

class launcher
{
    int m_flags;
    detail::environment m_environment;

#if defined(BOOST_PROCESS_POSIX_API)
    template< class Attributes >
    basic_child< Attributes > start_posix
        (const Attributes& attrs,
         boost::optional< detail::shared_pipe > pstdin,
         boost::optional< detail::shared_pipe > pstdout,
         boost::optional< detail::shared_pipe > pstderr);
#elif defined(BOOST_PROCESS_WIN32_API)
    template< class Attributes >
    basic_child< Attributes > start_win32
        (const Attributes& attrs,
         boost::optional< detail::shared_pipe > pstdin,
         boost::optional< detail::shared_pipe > pstdout,
         boost::optional< detail::shared_pipe > pstderr);
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

    void set_environment(const std::string& var, const std::string& value);
    void unset_environment(const std::string& var);

    template< class Attributes >
    basic_child< Attributes > start(const Attributes& attrs);
};

// ------------------------------------------------------------------------

inline
launcher::launcher(int flags) :
    m_flags(flags)
{
    BOOST_ASSERT
        ((m_flags & REDIR_STDERR) || (m_flags & REDIR_STDERR_TO_STDOUT) !=
         REDIR_STDERR || REDIR_STDERR_TO_STDOUT);
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

template< class Attributes >
inline
basic_child< Attributes >
launcher::start(const Attributes& attrs)
{
    boost::optional< detail::shared_pipe > pstdin;
    if (m_flags & REDIR_STDIN)
        pstdin = detail::shared_pipe();

    boost::optional< detail::shared_pipe > pstdout;
    if (m_flags & REDIR_STDOUT)
        pstdout = detail::shared_pipe();

    boost::optional< detail::shared_pipe > pstderr;
    if (m_flags & REDIR_STDERR)
        pstderr = detail::shared_pipe();

#if defined(BOOST_PROCESS_POSIX_API)
    return start_posix(attrs, pstdin, pstdout, pstderr);
#elif defined(BOOST_PROCESS_WIN32_API)
    return start_win32(attrs, pstdin, pstdout, pstderr);
#endif
}

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
template< class Attributes >
inline
basic_child< Attributes >
launcher::start_posix(const Attributes& attrs,
                      boost::optional< detail::shared_pipe > pstdin,
                      boost::optional< detail::shared_pipe > pstdout,
                      boost::optional< detail::shared_pipe > pstderr)
{
    pid_t pid = ::fork();
    if (pid == -1) {
        boost::throw_exception
            (system_error("boost::process::launcher::start",
                          "fork(2) failed", errno));
    } else if (pid == 0) {
        if (m_flags & REDIR_STDIN) {
            BOOST_ASSERT(pstdin);
            (*pstdin)->close_write_end();
            ::close(STDIN_FILENO);
            (*pstdin)->remap_read_end(STDIN_FILENO);
        }

        if (m_flags & REDIR_STDOUT) {
            BOOST_ASSERT(pstdout);
            (*pstdout)->close_read_end();
            ::close(STDOUT_FILENO);
            (*pstdout)->remap_write_end(STDOUT_FILENO);
        }

        if (m_flags & REDIR_STDERR) {
            BOOST_ASSERT(pstderr);
            (*pstderr)->close_read_end();
            ::close(STDERR_FILENO);
            (*pstderr)->remap_write_end(STDERR_FILENO);
        }

        if (m_flags & REDIR_STDERR_TO_STDOUT)
            ::dup2(STDOUT_FILENO, STDERR_FILENO);

        try {
            attrs.setup();
        } catch (const system_error& e) {
            ::write(STDERR_FILENO, e.what(), std::strlen(e.what()));
            ::write(STDERR_FILENO, "\n", 1);
            ::exit(EXIT_FAILURE);
        }

        size_t nargs = attrs.get_command_line().get_arguments().size();
        char* args[nargs + 1];
        const std::string& executable =
            attrs.get_command_line().get_executable();

        for (size_t i = 0; i < nargs; i++)
            args[i] = ::strdup
                (attrs.get_command_line().get_arguments()[i].c_str());
        args[nargs] = NULL;

        char** envp = m_environment.envp();

        ::execve(executable.c_str(), args, envp);
        system_error e("boost::process::launcher::start",
                       "execvp(2) failed", errno);

        for (size_t i = 0; i <= nargs; i++)
            delete [] args[i];

        for (size_t i = 0; i < m_environment.size(); i++)
            delete [] envp[i];
        delete [] envp;

        ::write(STDERR_FILENO, e.what(), std::strlen(e.what()));
        ::write(STDERR_FILENO, "\n", 1);
        ::exit(EXIT_FAILURE);
     } else {
        if (m_flags & REDIR_STDIN) {
            BOOST_ASSERT(pstdin);
            (*pstdin)->close_read_end();
        }

        if (m_flags & REDIR_STDOUT) {
            BOOST_ASSERT(pstdout);
            (*pstdout)->close_write_end();
        }

        if (m_flags & REDIR_STDERR) {
            BOOST_ASSERT(pstderr);
            (*pstderr)->close_write_end();
        }
    }

    return basic_child< Attributes >(pid, attrs, pstdin, pstdout, pstderr);
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_WIN32_API)
template< class Attributes >
inline
basic_child< Attributes >
launcher::start_win32(const Attributes& attrs,
                      boost::optional< detail::shared_pipe > pstdin,
                      boost::optional< detail::shared_pipe > pstdout,
                      boost::optional< detail::shared_pipe > pstderr)
{
    // TODO
}
#endif

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_LAUNCHER_HPP)
