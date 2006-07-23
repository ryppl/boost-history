//
// Boost.Process
// Command line representation.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//
 
#if !defined(BOOST_PROCESS_COMMAND_LINE_HPP)
#define BOOST_PROCESS_COMMAND_LINE_HPP

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_POSIX_API)
#   include <cstring>
#   include <utility>
#   include <boost/assert.hpp>
#elif defined(BOOST_PROCESS_WIN32_API)
#   include <tchar.h>
#   include <windows.h>
#   include <boost/assert.hpp>
#   include <boost/process/exceptions.hpp>
#   include <boost/shared_array.hpp>
#   include <boost/throw_exception.hpp>
#else
#   error "Unsupported platform."
#endif

#include <sstream>
#include <string>
#include <vector>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

class command_line
{
public:
    typedef std::vector< std::string > arguments_vector;

private:
    std::string m_executable;
    arguments_vector m_arguments;

    friend class launcher;
#if defined(BOOST_PROCESS_POSIX_API)
    std::pair< size_t, char ** > posix_argv(void) const;
#elif defined(BOOST_PROCESS_WIN32_API)
    boost::shared_array< TCHAR > win32_cmdline(void) const;
#endif

public:
    explicit command_line(const std::string& executable);

    static command_line shell(const std::string& command);

    template< typename T >
    command_line& argument(const T& argument);

    const arguments_vector& get_arguments(void) const;
    const std::string& get_executable(void) const;
};

// ------------------------------------------------------------------------

inline
command_line::command_line(const std::string& executable) :
    m_executable(executable)
{
}

// ------------------------------------------------------------------------

inline
command_line
command_line::shell(const std::string& command)
{
#if defined(BOOST_PROCESS_POSIX_API)
    command_line cl("/bin/sh");
    return cl.argument("sh").argument("-c").argument(command);
#elif defined(BOOST_PROCESS_WIN32_API)
    TCHAR buf[MAX_PATH];
    UINT res = ::GetWindowsDirectory(buf, MAX_PATH);
    if (res == 0)
        boost::throw_exception
            (system_error("boost::process::shell_command_line::"
                          "shell_command_line",
                          "GetWindowsDirectory failed", ::GetLastError()));
    BOOST_ASSERT(res < MAX_PATH);

    command_line cl(std::string(buf) + "\\cmd.exe");
    return cl.argument("cmd").argument("/c").argument(command);
#endif
}

// ------------------------------------------------------------------------

template< typename T >
inline
command_line&
command_line::argument(const T& argument)
{
    std::ostringstream tmp;
    tmp << argument;
    m_arguments.push_back(tmp.str());
    return *this;
}

// ------------------------------------------------------------------------

inline
const command_line::arguments_vector&
command_line::get_arguments(void)
    const
{
    return m_arguments;
}

// ------------------------------------------------------------------------

inline
const std::string&
command_line::get_executable(void)
    const
{
    return m_executable;
}

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
inline
std::pair< size_t, char** >
command_line::posix_argv(void)
    const
{
    size_t nargs = m_arguments.size();
    BOOST_ASSERT(nargs > 0);

    char** argv = new char*[nargs + 1];
    for (size_t i = 0; i < nargs; i++)
        argv[i] = ::strdup(m_arguments[i].c_str());
    argv[nargs] = NULL;

    return std::pair< size_t, char ** >(nargs, argv);
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_WIN32_API)
inline
boost::shared_array< TCHAR >
command_line::win32_cmdline(void)
    const
{
    SIZE_T length = 0;
    for (arguments_vector::const_iterator iter = m_arguments.begin();
         iter != m_arguments.end(); iter++)
        length += (*iter).length() + 1;

    boost::shared_array< TCHAR > cl(new TCHAR[length]);
    ::_tcscpy_s(cl.get(), length, TEXT(""));

    for (arguments_vector::size_type i = 0; i < m_arguments.size(); i++) {
        ::_tcscat_s(cl.get(), length, TEXT(m_arguments[i].c_str()));
        if (i != m_arguments.size() - 1)
            ::_tcscat_s(cl.get(), length, TEXT(" "));
    }

    return cl;
}
#endif

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_COMMAND_LINE_HPP)
