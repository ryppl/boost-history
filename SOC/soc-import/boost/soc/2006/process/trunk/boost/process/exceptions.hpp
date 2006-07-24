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

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_POSIX_API)
#   include <cstring>
#elif defined(BOOST_PROCESS_WIN32_API)
extern "C" {
#   include <windows.h>
}
#   include <boost/assert.hpp>
#else
#   error "Unknown platform."
#endif

#include <stdexcept>
#include <string>

#ifdef BOOST_NO_STDC_NAMESPACE
namespace std { using ::strerror; }
#endif

namespace boost {
namespace process {

// ------------------------------------------------------------------------

class system_error : public std::runtime_error
{
public:
#if defined(BOOST_PROCESS_POSIX_API)
    typedef int code_type;
#elif defined(BOOST_PROCESS_WIN32_API)
    typedef DWORD code_type;
#endif

    system_error(const std::string& who, const std::string& message,
                 code_type sys_err);
    virtual ~system_error(void) throw();

    virtual const char* what(void) const throw();

private:
    code_type m_sys_err;
    mutable std::string m_message;
};

// ------------------------------------------------------------------------

inline
system_error::system_error(const std::string& who,
                           const std::string& message,
                           code_type sys_err) :
    std::runtime_error(who + ": " + message),
    m_sys_err(sys_err)
{
}

// ------------------------------------------------------------------------

inline
system_error::~system_error(void)
    throw()
{
}

// ------------------------------------------------------------------------

inline
const char*
system_error::what(void)
    const
    throw()
{
    if (m_message.length() == 0) {
        m_message = std::string(std::runtime_error::what()) + ": ";

#if defined(BOOST_PROCESS_POSIX_API)
        m_message += std::strerror(m_sys_err);
#elif defined(BOOST_PROCESS_WIN32_API)
        TCHAR* msg = NULL;
        if (::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |
                            FORMAT_MESSAGE_ALLOCATE_BUFFER,
                            NULL, m_sys_err, 0,
                            reinterpret_cast<LPTSTR>(&msg),
                            0, NULL) == 0)
            m_message += "Unexpected error in FormatMessage";
        else {
            BOOST_ASSERT(msg != NULL);
            m_message += msg;
            LocalFree(msg);
        }
#endif
    }

    return m_message.c_str();
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_EXCEPTIONS_HPP)
