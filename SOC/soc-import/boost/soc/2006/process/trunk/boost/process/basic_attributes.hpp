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
 
#if !defined(BOOST_PROCESS_BASIC_ATTRIBUTES_HPP)
#define BOOST_PROCESS_BASIC_ATTRIBUTES_HPP

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
#   error "Unsupported platform."
#endif

#include <string>

#include <boost/assert.hpp>
#include <boost/process/exceptions.hpp>
#include <boost/throw_exception.hpp>

namespace boost {
namespace process {

class launcher;

// ------------------------------------------------------------------------

template< class Command_Line >
class basic_attributes
{
    Command_Line m_command_line;
    std::string m_work_directory;

protected:
    friend class launcher;
    void setup(void) const;

public:
    explicit basic_attributes(const Command_Line& cl,
                              const std::string& work_directory = "");

    const Command_Line& get_command_line(void) const;

    const std::string& get_work_directory(void) const;
};

// ------------------------------------------------------------------------

template< class Command_Line >
inline
basic_attributes< Command_Line >::basic_attributes
    (const Command_Line& cl, const std::string& work_directory) :
    m_command_line(cl)
{
    if (work_directory == "") {
#if defined(BOOST_PROCESS_WIN32_API)
        DWORD length = ::GetCurrentDirectory(0, NULL);
        TCHAR* buf = new TCHAR[length * sizeof(TCHAR)];
        if (::GetCurrentDirectory(length, buf) == 0) {
            delete buf;
            boost::throw_exception
                (system_error
                 ("boost::process::basic_attributes::basic_attributes",
                  "GetCurrentDirectory failed", ::GetLastError()));
        }
        m_work_directory = buf;
        delete buf;
#else
        const char* buf = ::getcwd(NULL, 0);
        if (buf == NULL)
            boost::throw_exception
                (system_error
                 ("boost::process::basic_attributes::basic_attributes",
                  "getcwd(2) failed", errno));
        m_work_directory = buf;
#endif
    } else
        m_work_directory = work_directory;
    BOOST_ASSERT(!m_work_directory.empty());
}

// ------------------------------------------------------------------------

template< class Command_Line >
inline
const Command_Line&
basic_attributes< Command_Line >::get_command_line(void)
    const
{
    return m_command_line;
}

// ------------------------------------------------------------------------

template< class Command_Line >
inline
const std::string&
basic_attributes< Command_Line >::get_work_directory(void)
    const
{
    return m_work_directory;
}

// ------------------------------------------------------------------------

template< class Command_Line >
inline void
basic_attributes< Command_Line >::setup(void)
    const
{
    if (m_work_directory.length() > 0) {
#if defined(BOOST_PROCESS_WIN32_API)
        if (::SetCurrentDirectory(TEXT(m_work_directory.c_str())) == 0)
            boost::throw_exception
                (system_error
                 ("boost::process::basic_attributes::basic_attributes",
                  "SetCurrentDirectory failed", ::GetLastError()));
#else
        if (chdir(m_work_directory.c_str()) == -1)
            boost::throw_exception
                (system_error("boost::process::basic_attributes::setup",
                              "chdir(2) failed", errno));
#endif
    }
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_BASIC_ATTRIBUTES_HPP)
