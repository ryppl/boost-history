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
#elif defined(BOOST_PROCESS_WIN32_API)
#   include <tchar.h>
#   include <windows.h>
#else
#   error "Unsupported platform."
#endif

#include <boost/assert.hpp>
#include <boost/optional.hpp>
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

#if defined(BOOST_PROCESS_POSIX_API)
    template< class Command_Line >
    basic_child< Command_Line > start_posix
        (const Command_Line& cl,
         boost::optional< detail::pipe > pstdin,
         boost::optional< detail::pipe > pstdout,
         boost::optional< detail::pipe > pstderr);
#elif defined(BOOST_PROCESS_WIN32_API)
    template< class Command_Line >
    basic_child< Command_Line > start_win32
        (const Command_Line& cl,
         boost::optional< detail::pipe > pstdin,
         boost::optional< detail::pipe > pstdout,
         boost::optional< detail::pipe > pstderr);
#endif

protected:
#if defined(BOOST_PROCESS_POSIX_API) || defined(BOOST_PROCESS_DOXYGEN)
    //!
    //! \brief Default startup procedure for a new child process.
    //!
    //! This auxiliary function contains part of the code used when
    //! launching a child process in a POSIX system.  This is meant to be
    //! executed by the child process after the fork has happened and
    //! takes care to execute the given command line \a cl.
    //!
    template< class Command_Line >
    void posix_child_entry(const Command_Line& cl);
#endif

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
    return start_posix(cl, pstdin, pstdout, pstderr);
#elif defined(BOOST_PROCESS_WIN32_API)
    return start_win32(cl, pstdin, pstdout, pstderr);
#endif
}

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
template< class Command_Line >
inline
basic_child< Command_Line >
launcher::start_posix(const Command_Line& cl,
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

        posix_child_entry(cl);
        BOOST_ASSERT(false); // Not reached.
    }

    BOOST_ASSERT(pid > 0);

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
    return basic_child< Command_Line >
        (pid, cl, fhstdin, fhstdout, fhstderr);
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
template< class Command_Line >
inline
void
launcher::posix_child_entry(const Command_Line& cl)
{
    try {
        if (chdir(m_work_directory.c_str()) == -1)
            boost::throw_exception
                (system_error("boost::process::launcher::posix_child_entry",
                              "chdir(2) failed", errno));
    } catch (const system_error& e) {
        ::write(STDERR_FILENO, e.what(), std::strlen(e.what()));
        ::write(STDERR_FILENO, "\n", 1);
        ::exit(EXIT_FAILURE);
    }

    std::pair< std::size_t, char** > args =
        detail::command_line_to_posix_argv(cl);
    char** envp = m_environment.envp();

    ::execve(cl.get_executable().c_str(), args.second, envp);
    system_error e("boost::process::launcher::start",
                   "execvp(2) failed", errno);

    for (std::size_t i = 0; i < args.first; i++)
        delete [] args.second[i];
    delete [] args.second;

    for (std::size_t i = 0; i < m_environment.size(); i++)
        delete [] envp[i];
    delete [] envp;

    ::write(STDERR_FILENO, e.what(), std::strlen(e.what()));
    ::write(STDERR_FILENO, "\n", 1);
    ::exit(EXIT_FAILURE);
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_WIN32_API)
template< class Command_Line >
inline
basic_child< Command_Line >
launcher::start_win32(const Command_Line& cl,
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

    boost::shared_array< TCHAR > cmdline =
        detail::command_line_to_win32_cmdline(cl);
    boost::scoped_array< TCHAR > executable
        (::_tcsdup(TEXT(cl.get_executable().c_str())));
    boost::scoped_array< TCHAR > workdir
        (::_tcsdup(TEXT(m_work_directory.c_str())));

    boost::shared_array< TCHAR > env = m_environment.win32_strings();
    if (!::CreateProcess(executable.get(), cmdline.get(), NULL, NULL, TRUE,
                         0, env.get(), workdir.get(), &si, &pi)) {
        boost::throw_exception
            (system_error("boost::process::launcher::start",
                          "CreateProcess failed", ::GetLastError()));
    }

    BOOST_ASSERT(!(m_flags & REDIR_STDIN) || fhstdin.is_valid());
    BOOST_ASSERT(!(m_flags & REDIR_STDOUT) || fhstdout.is_valid());
    BOOST_ASSERT(!(m_flags & REDIR_STDERR) || fhstderr.is_valid());
    return basic_child< Command_Line >
        (pi.hProcess, cl, fhstdin, fhstdout, fhstderr);
}
#endif

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_LAUNCHER_HPP)
