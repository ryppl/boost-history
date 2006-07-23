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
#include <boost/process/detail/file_handle.hpp>
#include <boost/process/detail/systembuf.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

class pistream :
    public std::istream, boost::noncopyable
{
    detail::file_handle m_handle;
    detail::systembuf m_systembuf;

public:
    explicit pistream(detail::file_handle& fh);

    void close(void);
};

// ------------------------------------------------------------------------

inline
pistream::pistream(detail::file_handle& fh) :
    std::istream(NULL),
    m_handle(fh),
    m_systembuf(m_handle.get())
{
    rdbuf(&m_systembuf);
}

// ------------------------------------------------------------------------

inline
void
pistream::close(void)
{
    m_handle.close();
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_PISTREAM_HPP)
