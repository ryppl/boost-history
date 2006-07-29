//
// Boost.Process
// Implementation of the Process concept.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#if !defined(BOOST_PROCESS_BASIC_PROCESS_HPP)
#define BOOST_PROCESS_BASIC_PROCESS_HPP

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_WIN32_API)
extern "C" {
#   include <windows.h>
}
#elif defined(BOOST_PROCESS_POSIX_API)
#else
#   error "Unsupported platform."
#endif

namespace boost {
namespace process {

// ------------------------------------------------------------------------

template< class Command_Line >
class basic_process
{
public:
#if defined(BOOST_PROCESS_WIN32_API)
    typedef HANDLE handle_type;
#elif defined(BOOST_PROCESS_POSIX_API)
    typedef pid_t handle_type;
#endif

    const Command_Line& get_command_line(void) const;
    handle_type get_handle(void) const;

protected:
    basic_process(handle_type h, const Command_Line& cl);

private:
    handle_type m_handle;
    Command_Line m_command_line;
};

// ------------------------------------------------------------------------

template< class Command_Line >
inline
basic_process< Command_Line >::basic_process
    (handle_type h, const Command_Line& cl) :
    m_handle(h),
    m_command_line(cl)
{
}

// ------------------------------------------------------------------------

template< class Command_Line >
inline
const Command_Line&
basic_process< Command_Line >::get_command_line(void)
    const
{
    return m_command_line;
}

// ------------------------------------------------------------------------

template< class Command_Line >
inline
typename basic_process< Command_Line >::handle_type
basic_process< Command_Line >::get_handle(void)
    const
{
    return m_handle;
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_BASIC_PROCESS_HPP)
