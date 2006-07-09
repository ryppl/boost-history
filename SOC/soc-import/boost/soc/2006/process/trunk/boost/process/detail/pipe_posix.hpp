//
// Boost.Process
// Abstraction for POSIX anonymous pipes.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#if !defined(BOOST_PROCESS_DETAIL_PIPE_POSIX_HPP)
#define BOOST_PROCESS_DETAIL_PIPE_POSIX_HPP

#include <unistd.h>

#include <cerrno>

#include <boost/assert.hpp>
#include <boost/process/exceptions.hpp>
#include <boost/process/types.hpp>
#include <boost/throw_exception.hpp>

namespace boost {
namespace process {
namespace detail {

// ------------------------------------------------------------------------

class pipe
{
    bool m_read_open;
    desc_t m_read_end;
    bool m_write_open;
    desc_t m_write_end;

public:
    pipe(void);
    ~pipe(void);

    void close_read_end(void);
    void close_write_end(void);

    desc_t get_read_end(void) const;
    desc_t get_write_end(void) const;
};

// ------------------------------------------------------------------------

inline
pipe::pipe(void)
{
    int pfd[2];

    if (::pipe(pfd) == -1)
        boost::throw_exception
            (system_error("boost::process::detail::pipe::pipe",
                          "pipe(2) failed", errno));

    m_read_open = true;
    m_read_end = pfd[0];
    m_write_open = true;
    m_write_end = pfd[1];
}

// ------------------------------------------------------------------------

inline
pipe::~pipe(void)
{
    if (m_read_open)
        ::close(m_read_end);
    if (m_write_open)
        ::close(m_write_end);
}

// ------------------------------------------------------------------------

inline
void
pipe::close_read_end(void)
{
    BOOST_ASSERT(m_read_open);
    ::close(m_read_end);
    m_read_open = false;
}

// ------------------------------------------------------------------------

inline
void
pipe::close_write_end(void)
{
    BOOST_ASSERT(m_write_open);
    ::close(m_write_end);
    m_write_open = false;
}

// ------------------------------------------------------------------------

inline
desc_t
pipe::get_read_end(void)
    const
{
    BOOST_ASSERT(m_read_open);
    return m_read_end;
}

// ------------------------------------------------------------------------

inline
desc_t
pipe::get_write_end(void)
    const
{
    BOOST_ASSERT(m_write_open);
    return m_write_end;
}

// ------------------------------------------------------------------------

} // namespace detail
} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_DETAIL_PIPE_POSIX_HPP)
