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
//! \file boost/process/posix_status.hpp
//!
//! Includes the declaration of the posix_status class.
//!

#if !defined(BOOST_PROCESS_POSIX_STATUS_HPP)
/** \cond */
#define BOOST_PROCESS_POSIX_STATUS_HPP
/** \endcond */

#include <boost/process/config.hpp>

#if !defined(BOOST_PROCESS_POSIX_API)
#   error "Unsupported platform."
#endif

#include <boost/optional.hpp>
#include <boost/process/status.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

//!
//! \brief Status returned by a finalized child process on a POSIX system.
//!
//! This class represents the %status returned by a child process after it
//! has terminated.  It contains some methods not available in the status
//! class that provide information only available in POSIX systems.
//!
class posix_status :
    public status
{
public:
    //!
    //! \brief Creates a posix_status object from an existing status
    //!        object.
    //!
    //! Creates a new status object representing the exit status of a
    //! child process.  The construction is done based on an existing
    //! status object which already contains all the available
    //! information: this class only provides controlled access to it.
    //!
    posix_status(const status& s);

    //!
    //! \brief POSIX-specific codification of the exit status.
    //!
    //! POSIX-specific codification of the exit status as returned by
    //! the wait() family of calls.  This should not generally be
    //! needed but may come helpful for those situations that were not
    //! considered by this interface.
    //!
    const int m_flags;

    //!
    //! \brief The signal that terminated the process.
    //!
    //! When defined, this member contains the signal number that caused
    //! the process to terminate.  This is undefined if the process did
    //! not terminate because of the reception of a signal.
    //!
    const boost::optional< int > m_term_signal;

    //!
    //! \brief Whether the process dumped core or not.
    //!
    //! When defined, indicates if the process dumped a core image or not.
    //! Because a core image is only dumped upon the reception of some
    //! specific signals, this member is only defined when any signal is
    //! received by the process and it terminates because of it.
    //!
    const boost::optional< bool > m_dumped_core;

    //!
    //! \brief The signal that stopped the process.
    //!
    //! When defined, contains the signal number that caused the process
    //! to stop execution.  This is undefined if the process was not
    //! stopped.
    //!
    const boost::optional< int > m_stop_signal;
};

// ------------------------------------------------------------------------

inline
posix_status::posix_status(const status& s) :
    status(s),
    m_flags(s.m_flags),
    m_term_signal(WIFSIGNALED(m_flags) ? WTERMSIG(m_flags) :
                                         boost::optional< int >()),
    m_dumped_core(WIFSIGNALED(m_flags) ? WCOREDUMP(m_flags) :
                                         boost::optional< bool >()),
    m_stop_signal(WIFSTOPPED(m_flags) ? WSTOPSIG(m_flags) :
                                        boost::optional< int >())
{
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_POSIX_STATUS_HPP)
