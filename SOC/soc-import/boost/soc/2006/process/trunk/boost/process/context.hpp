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
//! \file boost/process/context.hpp
//!
//! Includes the declaration of the context class.
//!

#if !defined(BOOST_PROCESS_CONTEXT_HPP)
/** \cond */
#define BOOST_PROCESS_CONTEXT_HPP
/** \endcond */

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_POSIX_API)
#elif defined(BOOST_PROCESS_WIN32_API)
#   include <tchar.h>
#   include <windows.h>
#else
#   error "Unsupported platform."
#endif

#include <boost/assert.hpp>
#include <boost/process/detail/file_handle.hpp>
#include <boost/process/environment.hpp>
#include <boost/process/exceptions.hpp>
#include <boost/process/stream_behavior.hpp>
#include <boost/throw_exception.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

//!
//! \brief Process startup execution context.
//!
//! The context class groups all the parameters needed to configure a
//! process' environment during its creation.
//!
template< class String >
class basic_context
{
public:
    //!
    //! \brief Constructs a new context.
    //!
    //! Constructs a new context.  It is configured as follows:
    //! * All communcation channels with the child process are closed.
    //! * There is no stderr merging to stdout.
    //! * The initial work directory of the child processes is set to the
    //!   current working directory.
    //! * The environment variables table is empty.
    //!
    basic_context(void);

    //!
    //! \brief Child's stdin behavior.
    //!
    stream_behavior m_stdin_behavior;

    //!
    //! \brief Child's stdout behavior.
    //!
    stream_behavior m_stdout_behavior;

    //!
    //! \brief Child's stderr behavior.
    //!
    stream_behavior m_stderr_behavior;

    //!
    //! \brief Whether the child's stderr should be redirected to stdout.
    //!
    bool m_merge_stderr_with_stdout;

    //!
    //! \brief The process' environment.
    //!
    //! Contains the list of environment variables, alongside with their
    //! values, that will be passed to the spawned child process.
    //!
    environment m_environment;

    //!
    //! \brief The process' initial work directory.
    //!
    //! The work directory is the directory in which the process starts
    //! execution.
    //!
    String m_work_directory;
};

typedef basic_context< std::string > context;

// ------------------------------------------------------------------------

template< class String >
inline
basic_context< String >::basic_context(void) :
    m_stdin_behavior(close_stream),
    m_stdout_behavior(close_stream),
    m_stderr_behavior(close_stream),
    m_merge_stderr_with_stdout(false)
{
#if defined(BOOST_PROCESS_POSIX_API)
    const char* buf = ::getcwd(NULL, 0);
    if (buf == NULL)
        boost::throw_exception
            (system_error
             ("boost::process::context::context",
              "getcwd(2) failed", errno));
    m_work_directory = buf;
#elif defined(BOOST_PROCESS_WIN32_API)
    DWORD length = ::GetCurrentDirectory(0, NULL);
    TCHAR* buf = new TCHAR[length * sizeof(TCHAR)];
    if (::GetCurrentDirectory(length, buf) == 0) {
        delete buf;
        boost::throw_exception
            (system_error
             ("boost::process::context::context",
              "GetCurrentDirectory failed", ::GetLastError()));
    }
    m_work_directory = buf;
    delete buf;
#endif
    BOOST_ASSERT(!m_work_directory.empty());
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_CONTEXT_HPP)
