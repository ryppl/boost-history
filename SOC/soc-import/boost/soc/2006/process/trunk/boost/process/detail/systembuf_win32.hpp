//
// Boost.Process
// Streambuf implementation for Win32 system descriptors.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#if !defined(BOOST_PROCESS_DETAIL_SYSTEMBUF_WIN32_HPP)
#define BOOST_PROCESS_DETAIL_SYSTEMBUF_WIN32_HPP

#include <windows.h>

#include <streambuf>

#include <boost/assert.hpp>
#include <boost/noncopyable.hpp>
#include <boost/process/types.hpp>
#include <boost/scoped_array.hpp>

namespace boost {
namespace process {
namespace detail {

// ------------------------------------------------------------------------

class systembuf :
    public std::streambuf, boost::noncopyable
{
    HANDLE m_hfile;
    DWORD m_bufsize;
    boost::scoped_array< char > m_read_buf;
    boost::scoped_array< char > m_write_buf;

protected:
    virtual int_type underflow(void);
    virtual int_type overflow(int c);
    virtual int sync(void);

public:
    explicit systembuf(HANDLE hfile, DWORD bufsize = 8192);
};

// ------------------------------------------------------------------------

inline
systembuf::systembuf(HANDLE hfile, DWORD bufsize) :
    m_hfile(hfile),
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
    DWORD cnt;
    BOOL res = ::ReadFile(m_hfile, m_read_buf.get(), m_bufsize, &cnt, NULL);
    if (!res || cnt == 0)
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
    DWORD cnt = pptr() - pbase();
    DWORD rcnt;
    BOOL res = ::WriteFile(m_hfile, pbase(), cnt, &rcnt, NULL);
    if (!res || rcnt < cnt)
        return -1;
    pbump(-static_cast<int>(cnt));
    return 0;
}

// ------------------------------------------------------------------------

} // namespace detail
} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_DETAIL_SYSTEMBUF_WIN32_HPP)
