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
public:
    //!
    //! \brief Describes the possible states for a communication stream.
    //!
    //! Describes the possible states for a communication stream as follows:
    //!
    //! close_stream means that the stream is closed and hence the child
    //! process will not be allowed to access it.
    //!
    //! redirect_stream means that the child is connected to the parent so
    //! that they can send and receive data through the stream.
    //!
    enum stream_behavior { close_stream, redirect_stream };

    //!
    //! \brief Constructs a new launcher with redirections.
    //!
    //! Constructs a new launcher object ready to spawn a new child
    //! process.  The launcher is configured to redirect the data streams
    //! described by the parameters described below.
    //!
    //! It is important to note that the initial work directory of the
    //! child processes is set to the currently working directory.  See
    //! set_work_directory() for more details.
    //!
    //! \param in The child's standard input stream behavior.
    //! \param out The child's standard output stream behavior.
    //! \param err The child's standard error stream behavior.
    //! \param merge_out_err If true, the child is configured to merge the
    //!                      standard output and standard error streams
    //!                      into a single one.  In other words, the data
    //!                      sent to stderr is automatically redirected to
    //!                      stdout, providing the parent a single data
    //!                      flow.
    //! \pre If merge_out_err is true, err must be close_stream and out
    //!      must be different than close_stream.
    //!
    launcher(stream_behavior in = close_stream,
             stream_behavior out = close_stream,
             stream_behavior err = close_stream,
             bool merge_out_err = false);

    //!
    //! \brief Sets the standard input stream behavior.
    //!
    //! Sets the standard input stream behavior to the value specified
    //! by b.
    //!
    launcher& set_stdin_behavior(stream_behavior b);

    //!
    //! \brief Sets the standard output stream behavior.
    //!
    //! Sets the standard output stream behavior to the value specified
    //! by b.
    //!
    launcher& set_stdout_behavior(stream_behavior b);

    //!
    //! \brief Sets the standard error stream behavior.
    //!
    //! Sets the standard error stream behavior to the value specified
    //! by b.
    //!
    launcher& set_stderr_behavior(stream_behavior b);

    //!
    //! \brief Sets the redirection of stderr to stdout.
    //!
    //! Enables or disables the redirection of the standard error stream
    //! to the standard output stream according to the value of b.
    //!
    //! \pre The standard output stream behavior is set to something
    //!      different than close_stream.
    //! \pre The standard error stream behavior is set to close_stream.
    //!
    launcher& set_merge_out_err(bool b);

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

private:
    //!
    //! \brief Child's stdin behavior.
    //!
    stream_behavior m_behavior_in;

    //!
    //! \brief Child's stdout behavior.
    //!
    stream_behavior m_behavior_out;

    //!
    //! \brief Child's stderr behavior.
    //!
    stream_behavior m_behavior_err;

    //!
    //! \brief Whether the child's stderr should be redirected to stdout.
    //!
    bool m_merge_out_err;

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
};

// ------------------------------------------------------------------------

inline
launcher::launcher(stream_behavior in,
                   stream_behavior out,
                   stream_behavior err,
                   bool merge_out_err)
{
    // The corresponding fields for the following calls are initialized
    // through their setters instead of from an initializer list to avoid
    // repeating the precondition checks code here.
    set_stdin_behavior(in);
    set_stdout_behavior(out);
    set_stderr_behavior(err);
    set_merge_out_err(merge_out_err);

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

launcher&
launcher::set_stdin_behavior(stream_behavior b)
{
    m_behavior_in = b;
    return *this;
}

// ------------------------------------------------------------------------

launcher&
launcher::set_stdout_behavior(stream_behavior b)
{
    m_behavior_out = b;
    return *this;
}

// ------------------------------------------------------------------------

launcher&
launcher::set_stderr_behavior(stream_behavior b)
{
    m_behavior_err = b;
    return *this;
}

// ------------------------------------------------------------------------

launcher&
launcher::set_merge_out_err(bool b)
{
    BOOST_ASSERT(!b || m_behavior_err == close_stream);
    BOOST_ASSERT(!b || m_behavior_out != close_stream);
    m_merge_out_err = b;
    return *this;
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

    if (m_behavior_in == redirect_stream)
        inpipes.insert(detail::pipe_map::value_type(STDIN_FILENO,
                                                    detail::pipe()));

    if (m_behavior_out == redirect_stream)
        outpipes.insert(detail::pipe_map::value_type(STDOUT_FILENO,
                                                     detail::pipe()));

    if (m_behavior_err == redirect_stream)
        outpipes.insert(detail::pipe_map::value_type(STDERR_FILENO,
                                                     detail::pipe()));

    if (m_merge_out_err)
        merges.insert(std::pair< int, int >(STDERR_FILENO, STDOUT_FILENO));

    detail::posix_setup s;
    s.m_work_directory = m_work_directory;

    ph = detail::posix_start(cl, m_environment, inpipes, outpipes, merges, s);

    if (m_behavior_in == redirect_stream) {
        detail::pipe_map::iterator iter = inpipes.find(STDIN_FILENO);
        BOOST_ASSERT(iter != inpipes.end());
        fhstdin = (*iter).second.wend().disown();
        BOOST_ASSERT(fhstdin.is_valid());
    }

    if (m_behavior_out == redirect_stream) {
        detail::pipe_map::iterator iter = outpipes.find(STDOUT_FILENO);
        BOOST_ASSERT(iter != outpipes.end());
        fhstdout = (*iter).second.rend().disown();
        BOOST_ASSERT(fhstdout.is_valid());
    }

    if (m_behavior_err == redirect_stream) {
        detail::pipe_map::iterator iter = outpipes.find(STDERR_FILENO);
        BOOST_ASSERT(iter != outpipes.end());
        fhstderr = (*iter).second.rend().disown();
        BOOST_ASSERT(fhstderr.is_valid());
    }
#elif defined(BOOST_PROCESS_WIN32_API)
    boost::optional< detail::pipe > pstdin;
    if (m_behavior_in == redirect_stream)
        pstdin = detail::pipe();

    boost::optional< detail::pipe > pstdout;
    if (m_behavior_out == redirect_stream)
        pstdout = detail::pipe();

    boost::optional< detail::pipe > pstderr;
    if (m_behavior_err == redirect_stream)
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
