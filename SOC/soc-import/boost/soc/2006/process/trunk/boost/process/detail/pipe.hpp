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
#include <boost/process/detail/file_handle.hpp>
#include <boost/process/exceptions.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/throw_exception.hpp>

namespace boost {
namespace process {
namespace detail {

// ------------------------------------------------------------------------

//
// A pipe is a RAII model for anonymous pipes.  It provides a portable
// constructor that initializes a new pipe and owns the two file handles
// associated to it.  These handles can be retrieved and, if copied,
// removed from the pipe object.
//
// A pipe object can be copied following the same semantics as file_handle.
// That is, copying such an object invalidates the source one.
//
class pipe
{
    file_handle m_read_end;
    file_handle m_write_end;

public:
    pipe(void);

    file_handle& rend(void);
    file_handle& wend(void);
};

// ------------------------------------------------------------------------

inline
pipe::pipe(void)
{
    file_handle::handle_type hs[2];

#if defined(BOOST_PROCESS_WIN32_API)
    SECURITY_ATTRIBUTES sa;
    ZeroMemory(&sa, sizeof(sa));
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = FALSE;

    if (!::CreatePipe(&hs[0], &hs[1], &sa, 0))
        boost::throw_exception
            (system_error("boost::process::detail::pipe::pipe",
                          "CreatePipe failed", ::GetLastError()));
#else
    if (::pipe(hs) == -1)
        boost::throw_exception
            (system_error("boost::process::detail::pipe::pipe",
                          "pipe(2) failed", errno));
#endif

    m_read_end = file_handle(hs[0]);
    m_write_end = file_handle(hs[1]);
}

// ------------------------------------------------------------------------

inline
file_handle&
pipe::rend(void)
{
    return m_read_end;
}

// ------------------------------------------------------------------------

inline
file_handle&
pipe::wend(void)
{
    return m_write_end;
}

// ------------------------------------------------------------------------

} // namespace detail
} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_DETAIL_PIPE_HPP)
