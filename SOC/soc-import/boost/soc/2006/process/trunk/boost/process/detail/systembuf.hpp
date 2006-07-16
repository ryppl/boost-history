//
// Boost.Process
// Streambuf implementation for system files.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//
 
#if !defined(BOOST_PROCESS_DETAIL_SYSTEMBUF_HPP)
#define BOOST_PROCESS_DETAIL_SYSTEMBUF_HPP

#include <boost/process/config.hpp>

extern "C" {
#if defined(BOOST_PROCESS_WIN32_API)
#   include <windows.h>
#elif defined(BOOST_PROCESS_POSIX_API)
#   include <unistd.h>
#else
#   error "Unknown platform."
#endif
}

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
public:
#if defined(BOOST_PROCESS_WIN32_API)
    typedef HANDLE handle_type;
    typedef DWORD size_type;
    typedef unsigned long ssize_type;
#else
    typedef int handle_type;
    typedef size_t size_type;
    typedef ssize_t ssize_type;
#endif

    explicit systembuf(handle_type h, size_type bufsize = 8192);

private:
    handle_type m_handle;
    size_type m_bufsize;
    boost::scoped_array< char > m_read_buf;
    boost::scoped_array< char > m_write_buf;

protected:
    virtual int_type underflow(void);
    virtual int_type overflow(int c);
    virtual int sync(void);
};

// ------------------------------------------------------------------------

inline
systembuf::systembuf(handle_type h, size_type bufsize) :
    m_handle(h),
    m_bufsize(bufsize),
    m_read_buf(new char[bufsize]),
    m_write_buf(new char[bufsize])
{
#if defined(BOOST_PROCESS_WIN32_API)
    BOOST_ASSERT(m_handle != INVALID_HANDLE_VALUE);
#else
    BOOST_ASSERT(m_handle >= 0);
#endif
    BOOST_ASSERT(m_bufsize > 0);

    setp(m_write_buf.get(), m_write_buf.get() + m_bufsize);
}

// ------------------------------------------------------------------------

inline
systembuf::int_type
systembuf::underflow(void)
{
    BOOST_ASSERT(gptr() >= egptr());

    bool ok;
#if defined(BOOST_PROCESS_WIN32_API)
    DWORD cnt;
    BOOL res = ::ReadFile(m_hfile, m_read_buf.get(), m_bufsize, &cnt, NULL);
    ok = (res && cnt > 0);
#else
    ssize_type cnt = ::read(m_handle, m_read_buf.get(), m_bufsize);
    ok = (cnt != -1 && cnt != 0);
#endif

    if (!ok)
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
    ssize_type cnt = pptr() - pbase();

    bool ok;
#if defined(BOOST_PROCESS_WIN32_API)
    DWORD cnt = pptr() - pbase();
    DWORD rcnt;
    BOOL res = ::WriteFile(m_hfile, pbase(), cnt, &rcnt, NULL);
    ok = (res && rcnt == cnt);
#else
    ok = ::write(m_handle, pbase(), cnt) == cnt;
#endif

    if (ok)
        pbump(-cnt);
    return ok ? 0 : -1;
}

// ------------------------------------------------------------------------

} // namespace detail
} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_DETAIL_SYSTEMBUF_HPP)
