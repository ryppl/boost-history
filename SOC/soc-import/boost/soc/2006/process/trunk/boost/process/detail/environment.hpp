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
#   include <windows.h>
}
#elif defined(BOOST_PROCESS_POSIX_API)
extern "C" {
#   include <unistd.h>
}
#   include <cerrno>
#   include <cstdlib>
#else
#   error "Unknown platform."
#endif

#include <map>
#include <string>

#include <boost/assert.hpp>
#include <boost/noncopyable.hpp>
#include <boost/process/exceptions.hpp>
#include <boost/throw_exception.hpp>

namespace boost {
namespace process {
namespace detail {

// ------------------------------------------------------------------------

class environment_entry
{
    bool m_set;
    std::string m_value;

public:
    explicit
    environment_entry(bool setit, const std::string& value = "") :
        m_set(setit),
        m_value(value)
    {
    }

    bool
    is_set(void)
        const
    {
        return m_set;
    }

    const std::string&
    get_value(void)
        const
    {
        BOOST_ASSERT(m_set);
        return m_value;
    }
};

// ------------------------------------------------------------------------

class environment :
    public std::map< std::string, environment_entry >,
    public boost::noncopyable
{
public:
    void set(const std::string& var, const std::string& value);
    void unset(const std::string& var);

    void setup(void);
};

// ------------------------------------------------------------------------

inline
void
environment::set(const std::string& var, const std::string& value)
{
    BOOST_ASSERT(not var.empty());
    insert(value_type(var, environment_entry(true, value)));
}

// ------------------------------------------------------------------------

inline
void
environment::unset(const std::string& var)
{
    BOOST_ASSERT(not var.empty());
    insert(value_type(var, environment_entry(false)));
}

// ------------------------------------------------------------------------

inline
void
environment::setup(void)
{
    for (const_iterator iter = begin(); iter != end(); iter++) {
        const std::string& var = (*iter).first;
        const environment_entry& e = (*iter).second;

        if (e.is_set()) {
            int res = ::setenv(var.c_str(), e.get_value().c_str(), 1);
            if (res == -1) {
                system_error e("boost::process::launcher::start",
                               "setenv(2) failed", errno);
                ::write(STDERR_FILENO, e.what(), std::strlen(e.what()));
                ::write(STDERR_FILENO, "\n", 1);
                ::exit(EXIT_FAILURE);
            }
        } else
            ::unsetenv(var.c_str());
    }
}

// ------------------------------------------------------------------------

} // namespace detail
} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_DETAIL_ENVIRONMENT_HPP)
