//
// Boost.Process
// Output stream.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#if !defined(BOOST_PROCESS_POSTREAM_HPP)
#define BOOST_PROCESS_POSTREAM_HPP

#include <ostream>

#include <boost/noncopyable.hpp>
#include <boost/process/detail/file_handle.hpp>
#include <boost/process/detail/systembuf.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

class postream :
    public std::ostream, boost::noncopyable
{
    detail::file_handle m_handle;
    detail::systembuf m_systembuf;

public:
    explicit postream(detail::file_handle& fh);

    void close(void);
};

// ------------------------------------------------------------------------

inline
postream::postream(detail::file_handle& fh) :
    std::ostream(NULL),
    m_handle(fh),
    m_systembuf(m_handle.get())
{
    rdbuf(&m_systembuf);
}

// ------------------------------------------------------------------------

inline
void
postream::close(void)
{
    m_handle.close();
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_POSTREAM_HPP)
