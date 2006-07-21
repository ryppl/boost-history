//
// Boost.Process
// Exit status representation.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//
 
#if !defined(BOOST_PROCESS_STATUS_HPP)
#define BOOST_PROCESS_STATUS_HPP

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

class status
{
    bool m_exited;
    int m_exit_status;

    bool m_signaled;
    int m_term_signal;
    bool m_dumped_core;

    bool m_stopped;
    int m_stop_signal;

public:
#if defined(BOOST_PROCESS_POSIX_API)
    status(int flags);
#elif defined(BOOST_PROCESS_WIN32_API)
    status(int code);
#endif

    bool exited(void) const;
    int exit_status(void) const;

    bool signaled(void) const;
    int term_signal(void) const;
    bool dumped_core(void) const;

    bool stopped(void) const;
    int stop_signal(void) const;
};

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
inline
status::status(int flags) :
    m_exited(WIFEXITED(flags)),
    m_exit_status(WEXITSTATUS(flags)),
    m_signaled(WIFSIGNALED(flags)),
    m_term_signal(WTERMSIG(flags)),
    m_dumped_core(WCOREDUMP(flags)),
    m_stopped(WIFSTOPPED(flags)),
    m_stop_signal(WSTOPSIG(flags))
{
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_WIN32_API)
inline
status::status(int code) :
    m_exited(true),
    m_exit_status(code),
    m_signaled(false),
    m_term_signal(0),
    m_dumped_core(false),
    m_stopped(false),
    m_stop_signal(0)
{
}
#endif

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
