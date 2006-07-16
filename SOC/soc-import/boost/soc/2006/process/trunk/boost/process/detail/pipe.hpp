//
// Boost.Process
// Abstraction for anonymous pipes.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//
 
#if !defined(BOOST_PROCESS_DETAIL_PIPE_HPP)
#define BOOST_PROCESS_DETAIL_PIPE_HPP

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_WIN32_API)
extern "C" {
#   include <windows.h>
}
#elif defined(BOOST_PROCESS_POSIX_API)
extern "C" {
#   include <unistd.h>
}
#   include <cerrno>
#else
#   error "Unknown platform."
#endif

#include <boost/assert.hpp>
#include <boost/noncopyable.hpp>
#include <boost/process/exceptions.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/throw_exception.hpp>

namespace boost {
namespace process {
namespace detail {

// ------------------------------------------------------------------------

class pipe :
    public boost::noncopyable
{
public:
#if defined(BOOST_PROCESS_WIN32_API)
    typedef HANDLE handle_type;
#else
    typedef int handle_type;
#endif

    pipe(void);
    ~pipe(void);

    void close_read_end(void);
    void close_write_end(void);

    handle_type get_read_end(void) const;
    handle_type get_write_end(void) const;

    // XXX I'm afraid this looks very ugly.  However, as this class is
    // implementation-specific and not exposed to the user, we can use
    // this for now.  And maybe it's good enough to stay...
#if defined(BOOST_PROCESS_POSIX_API)
    void remap_read_end(handle_type h);
    void remap_write_end(handle_type h);
#endif

private:
    bool m_read_open;
    handle_type m_read_end;
    bool m_write_open;
    handle_type m_write_end;

    void close_handle(handle_type h) const;
};

// ------------------------------------------------------------------------

inline
pipe::pipe(void)
{
#if defined(BOOST_PROCESS_WIN32_API)
    SECURITY_ATTRIBUTES sa;

    ZeroMemory(sa, sizeof(sa));
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    if (!::CreatePipe(&m_read_end, &m_write_end, &sa, 0))
        boost::throw_exception
            (system_error("boost::process::detail::pipe::pipe",
                          "CreatePipe failed", ::GetLastError()));
#else
    int pfd[2];

    if (::pipe(pfd) == -1)
        boost::throw_exception
            (system_error("boost::process::detail::pipe::pipe",
                          "pipe(2) failed", errno));

    m_read_end = pfd[0];
    m_write_end = pfd[1];
#endif

    m_read_open = true;
    m_write_open = true;
}

// ------------------------------------------------------------------------

inline
pipe::~pipe(void)
{
    if (m_read_open)
        close_handle(m_read_end);
    if (m_write_open)
        close_handle(m_write_end);
}

// ------------------------------------------------------------------------

inline
void
pipe::close_read_end(void)
{
    BOOST_ASSERT(m_read_open);
    close_handle(m_read_end);
    m_read_open = false;
}

// ------------------------------------------------------------------------

inline
void
pipe::close_write_end(void)
{
    BOOST_ASSERT(m_write_open);
    close_handle(m_write_end);
    m_write_open = false;
}

// ------------------------------------------------------------------------

inline
pipe::handle_type
pipe::get_read_end(void)
    const
{
    BOOST_ASSERT(m_read_open);
    return m_read_end;
}

// ------------------------------------------------------------------------

inline
pipe::handle_type
pipe::get_write_end(void)
    const
{
    BOOST_ASSERT(m_write_open);
    return m_write_end;
}

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
inline
void
pipe::remap_read_end(handle_type h)
{
    BOOST_ASSERT(m_read_open);
    ::close(h);
    ::dup2(m_read_end, h);
    ::close(m_read_end);
    m_read_end = h;
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
inline
void
pipe::remap_write_end(handle_type h)
{
    BOOST_ASSERT(m_write_open);
    ::close(h);
    ::dup2(m_write_end, h);
    ::close(m_write_end);
    m_write_end = h;
}
#endif

// ------------------------------------------------------------------------

inline
void
pipe::close_handle(handle_type h)
    const
{
#if defined(BOOST_PROCESS_WIN32_API)
    ::CloseHandle(h);
#else
    ::close(h);
#endif
}

// ------------------------------------------------------------------------

// shared_pipe is a wrapper around the pipe class to allow its placement
// in containers that require to copy objects.
// XXX: Should the pipe class be reference counted instead?
class shared_pipe :
    public boost::shared_ptr< pipe >
{
public:
    shared_pipe(void) :
        boost::shared_ptr< pipe >(new pipe())
    {
    }
};

// ------------------------------------------------------------------------

} // namespace detail
} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_DETAIL_PIPE_HPP)
