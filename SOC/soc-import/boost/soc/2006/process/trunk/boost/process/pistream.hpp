//
// Boost.Process
// Input stream.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//
 
#if !defined(BOOST_PROCESS_PISTREAM_HPP)
#define BOOST_PROCESS_PISTREAM_HPP

#include <istream>

#include <boost/noncopyable.hpp>
#include <boost/process/detail/pipe.hpp>
#include <boost/process/detail/systembuf.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

class pistream :
    public std::istream, boost::noncopyable
{
    detail::shared_pipe m_pipe;
    detail::systembuf m_systembuf;

public:
    explicit pistream(detail::shared_pipe p);

    void close(void);
};

// ------------------------------------------------------------------------

inline
pistream::pistream(detail::shared_pipe p) :
    std::istream(NULL),
    m_pipe(p),
    m_systembuf(p->get_read_end())
{
    rdbuf(&m_systembuf);
}

// ------------------------------------------------------------------------

inline
void
pistream::close(void)
{
    m_pipe->close_read_end();
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_PISTREAM_HPP)
