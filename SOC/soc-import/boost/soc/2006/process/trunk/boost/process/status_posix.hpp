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
 
#if !defined(BOOST_PROCESS_STATUS_POSIX_HPP)
#define BOOST_PROCESS_STATUS_POSIX_HPP

#include <sys/wait.h>

#include <boost/assert.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

class status
{
    int m_flags;

public:
    status(int flags);

    bool exited(void) const;
    int exit_status(void) const;

    bool signaled(void) const;
    int term_signal(void) const;
    bool dumped_core(void) const;

    bool stopped(void) const;
    int stop_signal(void) const;
};

// ------------------------------------------------------------------------

inline
status::status(int flags) :
    m_flags(flags)
{
}

// ------------------------------------------------------------------------

inline
bool
status::exited(void)
    const
{
    return WIFEXITED(m_flags);
}

// ------------------------------------------------------------------------

inline
int
status::exit_status(void)
    const
{
    BOOST_ASSERT(exited());
    return WEXITSTATUS(m_flags);
}

// ------------------------------------------------------------------------

inline
bool
status::signaled(void)
    const
{
    return WIFSIGNALED(m_flags);
}

// ------------------------------------------------------------------------

inline
int
status::term_signal(void)
    const
{
    BOOST_ASSERT(signaled());
    return WTERMSIG(m_flags);
}

// ------------------------------------------------------------------------

inline
bool
status::dumped_core(void)
    const
{
    BOOST_ASSERT(signaled());
    return WCOREDUMP(m_flags);
}

// ------------------------------------------------------------------------

inline
bool
status::stopped(void)
    const
{
    return WIFSTOPPED(m_flags);
}

// ------------------------------------------------------------------------

inline
int
status::stop_signal(void)
    const
{
    BOOST_ASSERT(stopped());
    return WSTOPSIG(m_flags);
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_STATUS_POSIX_HPP)
