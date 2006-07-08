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

#include <boost/process/exceptions.hpp>
#include <boost/process/types.hpp>
#include <boost/throw_exception.hpp>

namespace boost {
namespace process {
namespace detail {

// ------------------------------------------------------------------------

class pipe
{
    desc_t m_read_end;
    desc_t m_write_end;

public:
    pipe(void);
    ~pipe(void);

    desc_t read_end(void) const;
    desc_t write_end(void) const;
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

    m_read_end = pfd[0];
    m_write_end = pfd[1];
}

// ------------------------------------------------------------------------

inline
pipe::~pipe(void)
{
    ::close(m_read_end);
    ::close(m_write_end);
}

// ------------------------------------------------------------------------

inline
desc_t
pipe::read_end(void)
    const
{
    return m_read_end;
}

// ------------------------------------------------------------------------

inline
desc_t
pipe::write_end(void)
    const
{
    return m_write_end;
}

// ------------------------------------------------------------------------

} // namespace detail
} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_DETAIL_PIPE_POSIX_HPP)
