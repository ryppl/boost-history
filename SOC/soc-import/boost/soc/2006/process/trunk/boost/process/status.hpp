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

#include <boost/assert.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

//!
//! \brief Status returned by a finalized child process.
//!
//! This class represents the %status returned by a child process after it
//! has terminated.  It contains many methods that may make no sense in
//! some platforms, but this is done for simplicity.  These methods are
//! supported everywhere, although they may simply return fake values.
//!
class status
{
    //!
    //! \brief Indicates whether the process has exited on purpose.
    //!
    bool m_exited;

    //!
    //! \brief If exited, indicates the exit code.
    //!
    int m_exit_status;

    //!
    //! \brief Indicates whether the process was signaled.
    //!
    bool m_signaled;

    //!
    //! \brief If signaled, indicates the termination signal.
    //!
    int m_term_signal;

    //!
    //! \brief If signaled, indicates whether the process dumped core.
    //!
    bool m_dumped_core;

    //!
    //! \brief Indicates whether the process was stopped.
    //!
    bool m_stopped;

    //!
    //! \brief If stopped, indicates the stop signal.
    //!
    int m_stop_signal;

public:
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

    //!
    //! \brief Returns whether the process exited gracefully or not.
    //!
    //! Returns whether the process exited gracefully or not.
    //!
    bool exited(void) const;

    //!
    //! \brief If exited, returns the exit code.
    //!
    //! If the process exited, returns the exit code it returned.
    //!
    //! \pre exited() is true.
    //!
    int exit_status(void) const;

    //!
    //! \brief Returns whether the process exited due to an external
    //!        signal.
    //!
    //! Returns whether the process exited due to an external signal.
    //! The result is always false in Win32 systems.
    //!
    bool signaled(void) const;

    //!
    //! \brief If signaled, returns the terminating signal code.
    //!
    //! If the process was signaled, returns the terminating signal code.
    //! Cannnot be called under Win32 because the preconditions will not
    //! ever be met.
    //!
    //! \pre signaled() is true.
    //!
    int term_signal(void) const;

    //!
    //! \brief If signaled, returns whether the process dumped core.
    //!
    //! If the process was signaled, returns whether the process
    //! produced a core dump.
    //! Cannnot be called under Win32 because the preconditions will not
    //! ever be met.
    //!
    //! \pre signaled() is true.
    //!
    bool dumped_core(void) const;

    //!
    //! \brief Returns whether the process was stopped by an external
    //!        signal.
    //!
    //! Returns whether the process was stopped by an external signal.
    //! The result is always false in Win32 systems.
    //!
    bool stopped(void) const;

    //!
    //! \brief If stpped, returns the stop signal code.
    //!
    //! If the process was stopped, returns the stop signal code.
    //! Cannnot be called under Win32 because the preconditions will not
    //! ever be met.
    //!
    //! \pre signaled() is true.
    //!
    int stop_signal(void) const;
};

// ------------------------------------------------------------------------

inline
status::status(int flags) :
#if defined(BOOST_PROCESS_POSIX_API)
    m_exited(WIFEXITED(flags)),
    m_exit_status(WEXITSTATUS(flags)),
    m_signaled(WIFSIGNALED(flags)),
    m_term_signal(WTERMSIG(flags)),
    m_dumped_core(WCOREDUMP(flags)),
    m_stopped(WIFSTOPPED(flags)),
    m_stop_signal(WSTOPSIG(flags))
#elif defined(BOOST_PROCESS_WIN32_API)
    m_exited(true),
    m_exit_status(flags),
    m_signaled(false),
    m_term_signal(0),
    m_dumped_core(false),
    m_stopped(false),
    m_stop_signal(0)
#endif
{
}

// ------------------------------------------------------------------------

inline
bool
status::exited(void)
    const
{
    return m_exited;
}

// ------------------------------------------------------------------------

inline
int
status::exit_status(void)
    const
{
    BOOST_ASSERT(exited());
    return m_exit_status;
}

// ------------------------------------------------------------------------

inline
bool
status::signaled(void)
    const
{
    return m_signaled;
}

// ------------------------------------------------------------------------

inline
int
status::term_signal(void)
    const
{
    BOOST_ASSERT(signaled());
    return m_term_signal;
}

// ------------------------------------------------------------------------

inline
bool
status::dumped_core(void)
    const
{
    BOOST_ASSERT(signaled());
    return m_dumped_core;
}

// ------------------------------------------------------------------------

inline
bool
status::stopped(void)
    const
{
    return m_stopped;
}

// ------------------------------------------------------------------------

inline
int
status::stop_signal(void)
    const
{
    BOOST_ASSERT(stopped());
    return m_stop_signal;
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_STATUS_HPP)
