//
// Boost.Process
// Exceptions.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//
 
#if !defined(BOOST_PROCESS_EXCEPTIONS_HPP)
#define BOOST_PROCESS_EXCEPTIONS_HPP

#include <cstring>
#include <stdexcept>

#ifdef BOOST_NO_STDC_NAMESPACE
namespace std { using ::strerror; }
#endif

namespace boost {
namespace process {

// ------------------------------------------------------------------------

class system_error : public std::runtime_error
{
    int m_sys_err;

public:
    system_error(const std::string& who, const std::string& message,
                 int sys_err);
};

// ------------------------------------------------------------------------

inline
system_error::system_error(const std::string& who,
                           const std::string& message, int sys_err) :
    std::runtime_error(who + ": " + message + ": " + std::strerror(sys_err)),
    m_sys_err(sys_err)
{
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_EXCEPTIONS_HPP)
