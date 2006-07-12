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

public:
    explicit command_line(const std::string& executable);

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

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_COMMAND_LINE_HPP)
