//
// Boost.Process
// Implementation of the Attributes concept.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//
 
#if !defined(BOOST_PROCESS_POSIX_BASIC_ATTRIBUTES_HPP)
#define BOOST_PROCESS_POSIX_BASIC_ATTRIBUTES_HPP

#include <unistd.h>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

template< class Command_Line >
inline
std::string
basic_attributes< Command_Line >::get_current_directory(void)
    const
{
    const char* buf = getcwd(NULL, 0);
    if (buf == NULL)
        ; // XXX Handle this.
    std::string curdir(buf);
    delete buf;
    return curdir;
}

// ------------------------------------------------------------------------

template< class Command_Line >
inline void
basic_attributes< Command_Line >::setup(void)
    const
{
    if (m_work_directory.length() > 0) {
        if (chdir(m_work_directory.c_str()) == -1)
            ; // XXX Handle this.
    }
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_POSIX_PROCESS_BASIC_ATTRIBUTES_HPP)
