//
// Boost.Process
// Streambuf implementation for POSIX system descriptors.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#if !defined(BOOST_PROCESS_DETAIL_SYSTEMBUF_POSIX_HPP)
#define BOOST_PROCESS_DETAIL_SYSTEMBUF_POSIX_HPP

#include <unistd.h>

#include <streambuf>

#include <boost/assert.hpp>
#include <boost/process/types.hpp>
#include <boost/scoped_array.hpp>

namespace boost {
namespace process {
namespace detail {

// ------------------------------------------------------------------------

class systembuf : public std::streambuf
{
    desc_t m_desc;
    size_t m_bufsize;
    boost::scoped_array< char > m_read_buf;
    boost::scoped_array< char > m_write_buf;

protected:
    virtual int_type underflow(void);
    virtual int_type overflow(int c);
    virtual int sync(void);

public:
    explicit systembuf(desc_t d, size_t bufsize = 8192);
};

// ------------------------------------------------------------------------

inline
systembuf::systembuf(desc_t d, size_t bufsize) :
    m_desc(d),
    m_bufsize(bufsize),
    m_read_buf(new char[bufsize]),
    m_write_buf(new char[bufsize])
{
    setp(m_write_buf.get(), m_write_buf.get() + m_bufsize);
}

// ------------------------------------------------------------------------

inline
systembuf::int_type
systembuf::underflow(void)
{
    BOOST_ASSERT(gptr() >= egptr());
    ssize_t cnt = ::read(m_desc, m_read_buf.get(), m_bufsize);
    if (cnt == -1 || cnt == 0)
        return traits_type::eof();
    else {
        setg(m_read_buf.get(), m_read_buf.get(), m_read_buf.get() + cnt);
        return traits_type::to_int_type(*gptr());
    }
}

// ------------------------------------------------------------------------

inline
systembuf::int_type
systembuf::overflow(int c)
{
    BOOST_ASSERT(pptr() >= epptr());
    if (sync() == -1)
        return traits_type::eof();
    if (!traits_type::eq_int_type(c, traits_type::eof())) {
        traits_type::assign(*pptr(), c);
        pbump(1);
    }
    return traits_type::not_eof(c);
}

// ------------------------------------------------------------------------

inline
int
systembuf::sync(void)
{
    ssize_t cnt = pptr() - pbase();
    int res = ::write(m_desc, pbase(), cnt) == cnt ? 0 : -1;
    pbump(-cnt);
    return res;
}

// ------------------------------------------------------------------------

} // namespace detail
} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_DETAIL_SYSTEMBUF_POSIX_HPP)
