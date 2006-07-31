//
// Boost.Process
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

//!
//! \file boost/process/basic_process.hpp
//!
//! Includes the declaration of the basic_process template.
//!

#if !defined(BOOST_PROCESS_BASIC_PROCESS_HPP)
/** \cond */
#define BOOST_PROCESS_BASIC_PROCESS_HPP
/** \endcond */

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_POSIX_API)
#elif defined(BOOST_PROCESS_WIN32_API)
extern "C" {
#   include <windows.h>
}
#else
#   error "Unsupported platform."
#endif

namespace boost {
namespace process {

// ------------------------------------------------------------------------

//!
//! \brief Implementation of the Process concept.
//!
//! The basic_process template implements the Process concept.  This class
//! is templatized on a type that specifies the class implementing the
//! Command_Line concept, used to represent the command line used to launch
//! the process.
//!
template< class Command_Line >
class basic_process
{
public:
#if defined(BOOST_PROCESS_DOXYGEN)
    //!
    //! \brief Opaque name for the native process' handle type.
    //!
    //! Each operating system identifies processes using a specific type.
    //! The \a handle_type type is used to transparently refer to a process
    //! regarless of the operating system in which this class is used.
    //!
    //! If this class is used in a POSIX system, \a NativeSystemHandle is
    //! an integer type while it is a \a HANDLE in a Win32 system.
    //!
    typedef NativeSystemHandle handle_type;
#elif defined(BOOST_PROCESS_WIN32_API)
    typedef HANDLE handle_type;
#elif defined(BOOST_PROCESS_POSIX_API)
    typedef pid_t handle_type;
#endif

    //!
    //! \brief Returns the system handle that identifies the process.
    //!
    //! Returns the system handle that identifies the process.
    //!
    handle_type get_handle(void) const;

    //!
    //! \brief Returns the command line held by the process.
    //!
    //! Returns a reference to the command line held by the process.
    //!
    const Command_Line& get_command_line(void) const;

protected:
    //!
    //! \brief Creates a new process object that represents a running
    //!        process.
    //!
    //! Creates a new process object that represents the running process
    //! \a h.  The \a cl command line is assigned to this process and it
    //! is assummed that its contents are those that were used to launch
    //! the program's instance.
    //!
    //! This constructor is protected because the library user has no
    //! business in creating representations of live processes himself;
    //! the library takes care of that in all cases.
    //!
    basic_process(handle_type h, const Command_Line& cl);

private:
    //!
    //! \brief The handle that identifies the process.
    //!
    handle_type m_handle;

    //!
    //! \brief The command line that belongs to the process.
    //!
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
