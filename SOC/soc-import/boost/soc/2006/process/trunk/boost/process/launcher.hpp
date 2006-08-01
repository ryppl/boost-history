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
//! \file boost/process/launcher.hpp
//!
//! Includes the declaration of the launcher class.
//!

#if !defined(BOOST_PROCESS_LAUNCHER_HPP)
/** \cond */
#define BOOST_PROCESS_LAUNCHER_HPP
/** \endcond */

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_POSIX_API)
#   include <cstddef>
#   include <cstring>
#   include <boost/process/detail/posix_ops.hpp>
#elif defined(BOOST_PROCESS_WIN32_API)
#   include <tchar.h>
#   include <windows.h>
#   include <boost/process/detail/win32_ops.hpp>
#else
#   error "Unsupported platform."
#endif

#include <boost/assert.hpp>
#include <boost/process/basic_child.hpp>
#include <boost/process/detail/command_line_ops.hpp>
#include <boost/process/detail/environment.hpp>
#include <boost/process/detail/file_handle.hpp>
#include <boost/process/exceptions.hpp>
#include <boost/throw_exception.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

//!
//! \brief Generic implementation of the Launcher concept.
//!
//! The launcher class implements the Launcher concept in an operating
//! system agnostic way; it allows spawning new child process using a
//! single and common interface across different systems.
//!
class launcher
{
    //!
    //! \brief Bit field that specifies the desired stream redirections.
    //!
    int m_flags;

    //!
    //! \brief The process' environment.
    //!
    //! Contains the list of environment variables, alongside with their
    //! values, that will be passed to the spawned child process.
    //!
    detail::environment m_environment;

    //!
    //! \brief The process' initial work directory.
    //!
    //! The work directory is the directory in which the process starts
    //! execution.
    //!
    //! Ideally this could be of boost::filesystem::path type but it
    //! is a regular string to avoid depending on Boost.Filesystem.
    //!
    std::string m_work_directory;

protected:
    //!
    //! \brief Returns the child's environment.
    //!
    //! Returns a reference to the child's environment variables.
    //!
    const detail::environment& get_environment(void) const;

public:
    // See the constructor's description for information about these.
    static const int REDIR_NONE = 0;
    static const int REDIR_STDIN = (1 << 0);
    static const int REDIR_STDOUT = (1 << 1);
    static const int REDIR_STDERR = (1 << 2);
    static const int REDIR_STDERR_TO_STDOUT = (1 << 3);
    static const int REDIR_ALL =
        REDIR_STDIN | REDIR_STDOUT | REDIR_STDERR;
    static const int REDIR_ALL_MERGE =
        REDIR_STDIN | REDIR_STDOUT | REDIR_STDERR_TO_STDOUT;

    //!
    //! \brief Constructs a new launcher with redirections.
    //!
    //! Constructs a new launcher object ready to spawn a new child
    //! process.  The launcher is configured to redirect the data streams
    //! described by the \a flags bit field.
    //!
    //! If \a flags is REDIR_NONE, none of the standard data streams are
    //! redirected.  They are left untouched so they are generally shared
    //! with those of the caller.
    //!
    //! \a flags can also carry an OR of any of REDIR_STDIN, REDIR_STDOUT
    //! and REDIR_STDERR.  These tell the launcher to redirect their
    //! corresponding data streams so that they can later be fetched from
    //! the child object.  Additionally, the REDIR_STDERR_TO_STDOUT flag
    //! can be given, which configures the child process to use the same
    //! output channel for both the standard output and standard error
    //! flows; be aware that this conflicts with REDIR_STDERR.
    //!
    //! At last, two predefined bit sets are provided for simplicity:
    //! REDIR_ALL redirects all of the three standard flows and
    //! REDIR_ALL_MERGE does the same thing but redirects the standard
    //! error to the standard output.
    //!
    //! It is important to note that the initial work directory of the
    //! child processes is set to the currently working directory.  See
    //! set_work_directory() for more details.
    //!
    launcher(int flags = REDIR_ALL);

    //!
    //! \brief Gets the channel redirections.
    //!
    //! Returns a bit field indicating the channel redirections for the
    //! child process.  This function is possibly only useful in child
    //! classes.
    //!
    int get_flags(void) const;

    //!
    //! \brief Sets the channel redirections.
    //!
    //! Sets the channel redirections again to match those specified in
    //! \a flags.  See the constructor documentation for more details.
    //!
    void set_flags(int flags);

    //!
    //! \brief Sets the %environment variable \a var to \a value.
    //!
    //! Sets the new child's %environment variable \a var to \a value.
    //! The %environment of the current process is not touched.
    //!
    //! If the variable was already defined in the environment, its
    //! contents are replaced with the ones given in \a val.
    //!
    //! Be aware that \a value is allowed to be empty, although this may
    //! result in different behavior depending on the underlying operating
    //! system.  Win32 treats a variable with an empty value the same as
    //! it was undefined.  Contrarywise, POSIX systems consider a variable
    //! with an empty value to be defined.
    //!
    void set_environment(const std::string& var, const std::string& value);

    //!
    //! \brief Unsets the %environment variable \a var.
    //!
    //! Unsets the new child's %environment variable \a var.
    //! The %environment of the current process is not touched.
    //!
    void unset_environment(const std::string& var);

    //!
    //! \brief Gets the initial work directory for the new child.
    //!
    //! Returns the path to the directory in which the child process will
    //! start operation.
    //!
    const std::string& get_work_directory(void) const;

    //!
    //! \brief Sets the initial work directory for the new child.
    //!
    //! Sets the path to the directory in which the child process will
    //! start operation.
    //!
    void set_work_directory(const std::string& wd);

    //!
    //! \brief Starts a new child process.
    //!
    //! Given a command line \a cl, starts a new process with all the
    //! parameters configured in the launcher.  The launcher can be
    //! reused afterwards to launch other different command lines.
    //!
    //! \return A handle to the new child process.
    //!
    template< class Command_Line >
    basic_child< Command_Line > start(const Command_Line& cl);
};

// ------------------------------------------------------------------------

inline
launcher::launcher(int flags) :
    m_flags(REDIR_NONE)
{
    set_flags(flags);

#if defined(BOOST_PROCESS_POSIX_API)
    const char* buf = ::getcwd(NULL, 0);
    if (buf == NULL)
        boost::throw_exception
            (system_error
             ("boost::process::launcher::launcher",
              "getcwd(2) failed", errno));
    m_work_directory = buf;
#elif defined(BOOST_PROCESS_WIN32_API)
    DWORD length = ::GetCurrentDirectory(0, NULL);
    TCHAR* buf = new TCHAR[length * sizeof(TCHAR)];
    if (::GetCurrentDirectory(length, buf) == 0) {
        delete buf;
        boost::throw_exception
            (system_error
             ("boost::process::launcher::launcher",
              "GetCurrentDirectory failed", ::GetLastError()));
    }
    m_work_directory = buf;
    m_environment.set("", m_work_directory);
    delete buf;
#endif
    BOOST_ASSERT(!m_work_directory.empty());
}

// ------------------------------------------------------------------------

inline
const detail::environment&
launcher::get_environment(void)
    const
{
    return m_environment;
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
const std::string&
launcher::get_work_directory(void)
    const
{
    return m_work_directory;
}

// ------------------------------------------------------------------------

inline
void
launcher::set_work_directory(const std::string& wd)
{
    BOOST_ASSERT(wd.length() > 0);
    m_work_directory = wd;
#if defined(BOOST_PROCESS_WIN32_API)
    m_environment.set("", m_work_directory);
#endif
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

template< class Command_Line >
inline
basic_child< Command_Line >
launcher::start(const Command_Line& cl)
{
    typename basic_child< Command_Line >::handle_type ph;
    detail::file_handle fhstdin, fhstdout, fhstderr;

#if defined(BOOST_PROCESS_POSIX_API)
    detail::pipe_map inpipes, outpipes;
    detail::merge_set merges;

    if (m_flags & REDIR_STDIN)
        inpipes.insert(detail::pipe_map::value_type(STDIN_FILENO,
                                                    detail::pipe()));

    if (m_flags & REDIR_STDOUT)
        outpipes.insert(detail::pipe_map::value_type(STDOUT_FILENO,
                                                     detail::pipe()));

    if (m_flags & REDIR_STDERR)
        outpipes.insert(detail::pipe_map::value_type(STDERR_FILENO,
                                                     detail::pipe()));

    if (m_flags & REDIR_STDERR_TO_STDOUT)
        merges.insert(std::pair< int, int >(STDERR_FILENO, STDOUT_FILENO));

    detail::posix_setup s;
    s.m_work_directory = m_work_directory;

    ph = detail::posix_start(cl, m_environment, inpipes, outpipes, merges, s);

    if (m_flags & REDIR_STDIN) {
        detail::pipe_map::iterator iter = inpipes.find(STDIN_FILENO);
        BOOST_ASSERT(iter != inpipes.end());
        fhstdin = (*iter).second.wend().disown();
        BOOST_ASSERT(fhstdin.is_valid());
    }

    if (m_flags & REDIR_STDOUT) {
        detail::pipe_map::iterator iter = outpipes.find(STDOUT_FILENO);
        BOOST_ASSERT(iter != outpipes.end());
        fhstdout = (*iter).second.rend().disown();
        BOOST_ASSERT(fhstdout.is_valid());
    }

    if (m_flags & REDIR_STDERR) {
        detail::pipe_map::iterator iter = outpipes.find(STDERR_FILENO);
        BOOST_ASSERT(iter != outpipes.end());
        fhstderr = (*iter).second.rend().disown();
        BOOST_ASSERT(fhstderr.is_valid());
    }
#elif defined(BOOST_PROCESS_WIN32_API)
    boost::optional< detail::pipe > pstdin;
    if (m_flags & REDIR_STDIN)
        pstdin = detail::pipe();

    boost::optional< detail::pipe > pstdout;
    if (m_flags & REDIR_STDOUT)
        pstdout = detail::pipe();

    boost::optional< detail::pipe > pstderr;
    if (m_flags & REDIR_STDERR)
        pstderr = detail::pipe();

    STARTUPINFO si;
    ::ZeroMemory(si, sizeof(si));
    si.cb = sizeof(si);

    detail::win32_setup s;
    s.m_work_directory = m_work_directory;
    s.m_startupinfo = &si;

    PROCESSINFO pi = detail::win32_start(cl, m_environment, pstdin,
                                         pstdout, pstderr, setup);

    ph = pi.hProcess;
#endif

    return basic_child< Command_Line >(ph, cl, fhstdin, fhstdout, fhstderr);
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_LAUNCHER_HPP)
