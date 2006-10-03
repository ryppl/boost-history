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
//! \file boost/process/status.hpp
//!
//! Includes the declaration of the status class.
//!

#if !defined(BOOST_PROCESS_STATUS_HPP)
/** \cond */
#define BOOST_PROCESS_STATUS_HPP
/** \endcond */

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_POSIX_API)
extern "C" {
#   include <sys/wait.h>
}
#elif defined(BOOST_PROCESS_WIN32_API)
#else
#   error "Unsupported platform."
#endif

#include <boost/optional.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

//!
//! \brief Status returned by a finalized child process.
//!
//! This class represents the %status returned by a child process after it
//! has terminated.  It only provides that information available under all
//! supported platforms.
//!
//! \see posix_status
//!
class status
{
    //!
    //! \brief Creates a status object based on exit information.
    //!
    //! Creates a new status object representing the exit status of a
    //! child process.
    //!
    //! \param flags In a POSIX system this parameter contains the
    //!              flags returned by the ::waitpid() call.  In a
    //!              Win32 system it contains the exit code only.
    //!
    status(int flags);
    friend class child;
    friend class posix_status;

    //!
    //! \brief OS-specific codification of the exit status.
    //!
    const int m_flags;

public:
    //!
    //! \brief The process' exit status code.
    //!
    //! When defined, this member contains the integer returned by the
    //! child upon exit (be it by a return of main() or by a call to
    //! exit()).  This is undefined if the process did not exit regularly:
    //! e.g. a signal was received under a POSIX platform that caused an
    //! unexpected termination.
    //!
    const boost::optional< int > m_exit_status;
};

// ------------------------------------------------------------------------

inline
status::status(int flags) :
    m_flags(flags),
#if defined(BOOST_PROCESS_POSIX_API)
    m_exit_status(WIFEXITED(m_flags) ? WEXITSTATUS(m_flags) :
                                       boost::optional< int >())
#elif defined(BOOST_PROCESS_WIN32_API)
    m_exit_status(m_flags)
#endif
{
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_STATUS_HPP)
