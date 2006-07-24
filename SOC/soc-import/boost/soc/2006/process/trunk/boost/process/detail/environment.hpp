//
// Boost.Process
// Abstraction for environment variables.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#if !defined(BOOST_PROCESS_DETAIL_ENVIRONMENT_HPP)
#define BOOST_PROCESS_DETAIL_ENVIRONMENT_HPP

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_WIN32_API)
extern "C" {
#   include <tchar.h>
#   include <windows.h>
}
#elif defined(BOOST_PROCESS_POSIX_API)
#   include <cstring>
#else
#   error "Unknown platform."
#endif

#include <map>
#include <string>

#include <boost/assert.hpp>
#include <boost/process/exceptions.hpp>
#include <boost/shared_array.hpp>
#include <boost/throw_exception.hpp>

#if defined(BOOST_PROCESS_POSIX_API)
extern "C" {
    extern char** environ;
};
#endif

namespace boost {
namespace process {
namespace detail {

// ------------------------------------------------------------------------

class environment :
    public std::map< std::string, std::string >
{
public:
    environment(void);

    void set(const std::string& var, const std::string& value);
    void unset(const std::string& var);

    char** envp(void) const;
#if defined(BOOST_PROCESS_WIN32_API)
    boost::shared_array< TCHAR > strings(void) const;
#endif
};

// ------------------------------------------------------------------------

inline
environment::environment(void)
{
#if defined(BOOST_PROCESS_POSIX_API)
    char** ptr = ::environ;
    while (*ptr != NULL) {
        std::string str = *ptr;
        std::string::size_type pos = str.find('=');
        insert(value_type(str.substr(0, pos),
                          str.substr(pos + 1, str.length())));
        ptr++;
    }
#elif defined(BOOST_PROCESS_WIN32_API)
    TCHAR* es = ::GetEnvironmentStrings();
    if (es == NULL)
        boost::throw_exception
            (system_error("boost::process::detail::environment::environment",
                          "GetEnvironmentStrings failed", ::GetLastError()));

    try {
        while (*es != '\0') {
            std::string str = es;
            std::string::size_type pos = str.find('=');
            insert(value_type(str.substr(0, pos),
                              str.substr(pos + 1, str.length())));
            es += str.length() + 1;
        }
    } catch (...) {
        ::FreeEnvironmentStrings(es);
        throw;
    }

    ::FreeEnvironmentStrings(es);
#endif
}

// ------------------------------------------------------------------------

inline
void
environment::set(const std::string& var, const std::string& value)
{
    insert(value_type(var, value));
}

// ------------------------------------------------------------------------

inline
void
environment::unset(const std::string& var)
{
    erase(var);
}

// ------------------------------------------------------------------------

inline
char**
environment::envp(void)
    const
{
    char** ep = new char*[size() + 1];

    size_type i = 0;
    for (const_iterator iter = begin(); iter != end(); iter++) {
        std::string tmp = (*iter).first + "=" + (*iter).second;

        char* cstr = new char[tmp.length() + 1];
#if defined(BOOST_PROCESS_POSIX_API)
        std::strncpy(cstr, tmp.c_str(), tmp.length());
#elif defined(BOOST_PROCESS_WIN32_API)
        ::strcpy_s(cstr, tmp.length() + 1, tmp.c_str());
#endif
        cstr[tmp.length()] = '\0';

        ep[i++] = cstr;
    }

    ep[i] = NULL;

    return ep;
}

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_WIN32_API)
inline
boost::shared_array< TCHAR >
environment::strings(void)
    const
{
    boost::shared_array< TCHAR > strs(NULL);

    if (size() == 0) {
        strs.reset(new TCHAR[2]);
        ::ZeroMemory(strs.get(), sizeof(TCHAR) * 2);
    } else {
        std::string::size_type len = sizeof(TCHAR);
        for (const_iterator iter = begin(); iter != end(); iter++)
            len += ((*iter).first.length() + 1 + (*iter).second.length() +
                    1) * sizeof(TCHAR);

        strs.reset(new TCHAR[len]);

        TCHAR* ptr = strs.get();
        for (const_iterator iter = begin(); iter != end(); iter++) {
            std::string tmp = (*iter).first + "=" + (*iter).second;
            _tcscpy_s(ptr, len - (ptr - strs.get()) * sizeof(TCHAR),
                      TEXT(tmp.c_str()));
            ptr += (tmp.length() + 1) * sizeof(TCHAR);

            BOOST_ASSERT(static_cast<std::string::size_type>
                (ptr - strs.get()) * sizeof(TCHAR) < len);
        }
        *ptr = '\0';
    }

    BOOST_ASSERT(strs.get() != NULL);
    return strs;
}
#endif

// ------------------------------------------------------------------------

} // namespace detail
} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_DETAIL_ENVIRONMENT_HPP)
