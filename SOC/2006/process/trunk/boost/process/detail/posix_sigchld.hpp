//
// Boost.Process
//
// Copyright (c) 2008 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

//!
//! \file boost/process/detail/posix_sigchld.hpp
//!
//! A signal handler for SIGCHLD.
//!

#if !defined(BOOST_PROCESS_DETAIL_POSIX_SIGCHLD_HPP)
/** \cond */
#define BOOST_PROCESS_DETAIL_POSIX_SIGCHLD_HPP
/** \endcond */

#include <boost/process/config.hpp>

#if !defined(BOOST_PROCESS_POSIX_API)
#   error "Unsupported platform."
#endif

extern "C" {
#   include <signal.h>
}

#   include <boost/assert.hpp>

namespace boost {
namespace process {
namespace detail {

// ------------------------------------------------------------------------

//!
//! \brief A model to reset SIGCHLD while using this library.
//!
//! The \a posix_sigchld_programmer class provides a model to handle the
//! programming of the SIGCHLD signal handler while this library is
//! managing children processes.
//!
//! The SIGCHLD signal must be handled in some way to prevent aborts of
//! system calls when the handler is programmed (e.g. by Boost.Test or by
//! a debugger).  There basically are two possibilities.  First, we reset
//! the signal to its default behavior, which is to simply ignore it.  In
//! this way, we do not get any system call premature aborts due to EINTR.
//! The other approach is to capture the signal and do something useful
//! with it, making all the blocking system calls we use safe to EINTR.
//! We currently implement the former mechanism because we have no use for
//! SIGCHLD yet.
//!
//! Note that handling EINTR but still not handling SIGCHLD in some way
//! causes the tests to fail because Boost.Test installs a signal handler
//! to monitor for unhandled signals.
//!
class posix_sigchld_programmer {
    //!
    //! \brief The amount of children processes the library is managing.
    //!
    int m_children_count;

    //!
    //! \brief The signal handler that was installed before reprogramming.
    //!
    struct sigaction m_old_handler;

    //!
    //! \brief Programs our own signal handler.
    //!
    //! This call is idempotent for simplicity of the callers.
    //!
    void program(void);

    //!
    //! \brief Restores the previous signal handler.
    //!
    //! This call is idempotent for simplicity of the callers.
    //!
    void unprogram(void);

public:
    posix_sigchld_programmer(void);
    ~posix_sigchld_programmer(void);

    //!
    //! \brief Tells this model that we are about to spawn a child.
    //!
    void forking_child(void);

    //!
    //! \brief Tells this model that spawning a child failed, thus
    //! canceling the actions of forking_child.
    //!
    void forking_failed(void);

    //!
    //! \brief Tells this model that a child's status has been collected
    //! and is no further in zombie status.
    //!
    void child_awaited(void);
};

// ------------------------------------------------------------------------

//!
//! \brief The library-wide \a posix_sigchld_programmer instance.
//!
posix_sigchld_programmer the_posix_sigchld_programmer;

// ------------------------------------------------------------------------

inline
posix_sigchld_programmer::posix_sigchld_programmer(void) :
    m_children_count(0)
{
}

// ------------------------------------------------------------------------

inline
posix_sigchld_programmer::~posix_sigchld_programmer(void)
{
}

// ------------------------------------------------------------------------

inline
void
posix_sigchld_programmer::program(void)
{
    if (m_children_count == 1) {
        struct sigaction sa;
        sa.sa_handler = SIG_DFL;
        sa.sa_flags = 0;
        sigemptyset(&sa.sa_mask);

        ::sigaction(SIGCHLD, &sa, &m_old_handler);
    }
}

// ------------------------------------------------------------------------

inline
void
posix_sigchld_programmer::unprogram(void)
{
    if (m_children_count == 0)
        ::sigaction(SIGCHLD, &m_old_handler, NULL);
}

// ------------------------------------------------------------------------

inline
void
posix_sigchld_programmer::forking_child(void)
{
    m_children_count++;
    program();
}

// ------------------------------------------------------------------------

inline
void
posix_sigchld_programmer::forking_failed(void)
{
    BOOST_ASSERT(m_children_count > 0);
    m_children_count--;
    unprogram();
}

// ------------------------------------------------------------------------

inline
void
posix_sigchld_programmer::child_awaited(void)
{
    BOOST_ASSERT(m_children_count > 0);
    m_children_count--;
    unprogram();
}

// ------------------------------------------------------------------------

} // namespace detail
} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_DETAIL_POSIX_SIGCHLD_HPP)
