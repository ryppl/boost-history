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

#include <string>

#include <boost/process/config.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

template< class Command_Line >
class basic_attributes
{
    Command_Line m_command_line;
    std::string m_work_directory;

    std::string get_current_directory(void) const;

protected:
    void setup(void) const;

public:
    explicit basic_attributes(const Command_Line& cl,
                              const std::string& work_directory = "");

    const Command_Line& get_command_line(void) const;

    const std::string& get_work_directory(void);
};

// ------------------------------------------------------------------------

template< class Command_Line >
inline
basic_attributes< Command_Line >::basic_attributes
    (const Command_Line& cl, const std::string& work_directory) :
    m_command_line(cl)
{
    if (work_directory == "")
        m_work_directory = get_current_directory();
    else
        m_work_directory = work_directory;
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
{
    return m_work_directory;
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#if defined(BOOST_PROCESS_WIN32_API)
#   include <boost/process/basic_attributes_win32.hpp>
#else
#   include <boost/process/basic_attributes_posix.hpp>
#endif

#endif // !defined(BOOST_PROCESS_BASIC_ATTRIBUTES_HPP)
