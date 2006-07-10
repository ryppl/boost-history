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
#include <boost/shared_ptr.hpp>

#if defined(BOOST_PROCESS_WIN32_API)
#   include <boost/process/detail/pipe_win32.hpp>
#else
#   include <boost/process/detail/pipe_posix.hpp>
#endif

namespace boost {
namespace process {
namespace detail {

// ------------------------------------------------------------------------

// shared_pipe is a wrapper around the pipe class to allow its placement
// in containers that require to copy objects.
// XXX: Should the pipe class be reference counted instead?
class shared_pipe :
    public boost::shared_ptr< detail::pipe >
{
public:
    shared_pipe(void) :
        boost::shared_ptr< detail::pipe >(new detail::pipe())
    {
    }
};

// ------------------------------------------------------------------------

} // namespace detail
} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_DETAIL_PIPE_HPP)
