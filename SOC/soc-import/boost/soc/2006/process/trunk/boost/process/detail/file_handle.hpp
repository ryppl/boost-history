//
// Boost.Process
// RAII model for system file handles.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//
 
#if !defined(BOOST_PROCESS_DETAIL_FILE_HANDLE_HPP)
#define BOOST_PROCESS_DETAIL_FILE_HANDLE_HPP

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_POSIX_API)
extern "C" {
#   include <unistd.h>
}
#   include <cerrno>
#elif defined(BOOST_PROCESS_WIN32_API)
extern "C" {
#   include <windows.h>
}
#else
#   error "Unknown platform."
#endif

#include <boost/assert.hpp>
#include <boost/process/exceptions.hpp>
#include <boost/throw_exception.hpp>

namespace boost {
namespace process {
namespace detail {

// ------------------------------------------------------------------------

//
// A file_handle is a RAII model for system file handles.  This class
// wraps one of such handles, grabbing its ownership, and automaticaly
// closes it upon destruction.
//
// A file_handle object can be copied but doing so invalidates the source
// object.  That is, there can be a single valid file_handle object for a
// given system file handle.
//
// This class also provides some convenience methods to issue special file
// operations under their respective platforms.
//
class file_handle
{
public:
#if defined(BOOST_PROCESS_POSIX_API)
    typedef int handle_type;
#elif defined(BOOST_PROCESS_WIN32_API)
    typedef HANDLE handle_type;
#endif

    file_handle(void);
    file_handle(const handle_type& h);
    file_handle(const file_handle& fh);
    ~file_handle(void);

    bool is_valid(void) const;
    void close(void);
    handle_type disown(void);
    handle_type get(void) const;

#if defined(BOOST_PROCESS_POSIX_API)
    void posix_remap(const handle_type& h);
    static file_handle posix_dup(int h1, int h2);
#elif defined(BOOST_PROCESS_WIN32_API)
    static file_handle win32_dup(HANDLE h, bool inheritable);
    static file_handle win32_std(DWORD d, bool inheritable);
    void win32_set_inheritable(bool i);
#endif

    file_handle& operator=(const file_handle& fh);

private:
    // This needs to be mutable because the copy constructor and the
    // assignment operator invalidate the source object.
    mutable handle_type m_handle;

#if defined(BOOST_PROCESS_POSIX_API)
    static const handle_type INVALID_VALUE = -1;
#elif defined(BOOST_PROCESS_WIN32_API)
    static const handle_type INVALID_VALUE = INVALID_HANDLE_VALUE;
#endif
};

// ------------------------------------------------------------------------

inline
file_handle::file_handle(void) :
    m_handle(INVALID_VALUE)
{
}

// ------------------------------------------------------------------------

inline
file_handle::file_handle(const handle_type& h) :
    m_handle(h)
{
    BOOST_ASSERT(m_handle != INVALID_VALUE);
}

// ------------------------------------------------------------------------

inline
file_handle::file_handle(const file_handle& fh) :
    m_handle(fh.m_handle)
{
    BOOST_ASSERT(fh.is_valid());

    fh.m_handle = INVALID_VALUE;
}

// ------------------------------------------------------------------------

inline
file_handle::~file_handle(void)
{
    if (is_valid())
        close();
}

// ------------------------------------------------------------------------

inline
bool
file_handle::is_valid(void)
    const
{
    return m_handle != INVALID_VALUE;
}

// ------------------------------------------------------------------------

inline
void
file_handle::close(void)
{
    BOOST_ASSERT(is_valid());

#if defined(BOOST_PROCESS_POSIX_API)
    ::close(m_handle);
#elif defined(BOOST_PROCESS_WIN32_API)
    ::CloseHandle(m_handle);
#endif

    m_handle = INVALID_VALUE;
}

// ------------------------------------------------------------------------

inline
file_handle::handle_type
file_handle::disown(void)
{
    BOOST_ASSERT(is_valid());

    handle_type h = m_handle;
    m_handle = INVALID_VALUE;
    return h;
}

// ------------------------------------------------------------------------

inline
file_handle::handle_type
file_handle::get(void)
    const
{
    BOOST_ASSERT(is_valid());

    return m_handle;
}

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
inline
void
file_handle::posix_remap(const handle_type& h)
{
    BOOST_ASSERT(is_valid());

    if (::dup2(m_handle, h) == -1)
        boost::throw_exception
            (system_error("boost::process::detail::file_handle::remap",
                          "dup2(2) failed", errno));

    if (::close(m_handle) == -1)
        boost::throw_exception
            (system_error("boost::process::detail::file_handle::remap",
                          "close(2) failed", errno));

    m_handle = h;
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
file_handle
file_handle::posix_dup(int h1, int h2)
{
    if (::dup2(h1, h2) == -1)
        boost::throw_exception
            (system_error("boost::process::detail::file_handle::posix_dup",
                          "dup2(2) failed", errno));

    return file_handle(h2);
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_WIN32_API)
file_handle
file_handle::win32_dup(HANDLE h, bool inheritable)
{
    HANDLE h2;
    if (::DuplicateHandle(::GetCurrentProcess(), h,
                          ::GetCurrentProcess(), &h2,
                          0, inheritable ? TRUE : FALSE,
                          DUPLICATE_SAME_ACCESS) == 0)
        boost::throw_exception
            (system_error("boost::process::detail::file_handle::win32_dup",
                          "DuplicateHandle failed", ::GetLastError()));

    return file_handle(h2);
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_WIN32_API)
file_handle
file_handle::win32_std(DWORD d, bool inheritable)
{
    HANDLE h = ::GetStdHandle(d);
    if (h == INVALID_HANDLE_VALUE)
        boost::throw_exception
            (system_error("boost::process::detail::file_handle::win32_std",
                          "GetStdHandle failed", ::GetLastError()));

    return win32_dup(h, inheritable);
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_WIN32_API)
inline
void
file_handle::win32_set_inheritable(bool b)
{
    BOOST_ASSERT(is_valid());

    handle_type hnew;
    if (::DuplicateHandle(::GetCurrentProcess(), m_handle,
                          ::GetCurrentProcess(), &hnew,
                          0, b ? TRUE : FALSE, DUPLICATE_SAME_ACCESS) == 0)
        boost::throw_exception
            (system_error("boost::process::detail::file_handle::"
                          "win32_set_inheritable",
                          "DuplicateHandle failed", ::GetLastError()));

    if (::CloseHandle(m_handle) == 0)
        boost::throw_exception
            (system_error("boost::process::detail::file_handle::"
                          "win32_set_inheritable",
                          "CloseHandle failed", ::GetLastError()));

    m_handle = hnew;
}
#endif

// ------------------------------------------------------------------------

inline
file_handle&
file_handle::operator=(const file_handle& fh)
{
    BOOST_ASSERT(fh.is_valid());

    m_handle = fh.m_handle;
    fh.m_handle = INVALID_VALUE;

    return *this;
}

// ------------------------------------------------------------------------

} // namespace detail
} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_DETAIL_FILE_HANDLE_HPP)
