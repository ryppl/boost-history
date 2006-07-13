//
// Boost.Process
// Implementation of the Child concept.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//
 
#if !defined(BOOST_PROCESS_BASIC_CHILD_POSIX_HPP)
#define BOOST_PROCESS_BASIC_CHILD_POSIX_HPP

#include <sys/types.h>
#include <sys/wait.h>

#include <cerrno>

#include <boost/process/exceptions.hpp>
#include <boost/throw_exception.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

template< class Attributes >
inline
status
basic_child< Attributes >::wait(void)
{
    int s;
    if (::waitpid(basic_child< Attributes >::get_id(), &s, 0) == -1)
        boost::throw_exception
            (system_error("boost::process::basic_child::wait",
                          "waitpid(2) failed", errno));
    return status(s);
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_BASIC_CHILD_POSIX_HPP)
