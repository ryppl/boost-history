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
//! \file boost/process/detail/command_line_ops.hpp
//!
//! Provides some convenience functions to convert command_line objects
//! to the representation used by the underlying operating system.
//!

#if !defined(BOOST_PROCESS_DETAIL_COMMAND_LINE_OPS_HPP)
/** \cond */
#define BOOST_PROCESS_DETAIL_COMMAND_LINE_OPS_HPP
/** \endcond */

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_POSIX_API)
#   include <cstring>
#   include <utility>
#   include <boost/assert.hpp>
#elif defined(BOOST_PROCESS_WIN32_API)
#   include <tchar.h>
#   include <windows.h>
#   include <boost/shared_array.hpp>
#else
#   error "Unsupported platform."
#endif

namespace boost {
namespace process {
namespace detail {

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API) || defined(BOOST_PROCESS_DOXYGEN)
//!
//! \brief Converts the command line to an array of C strings.
//!
//! Converts the command line's list of arguments to the format expected
//! by the \a argv parameter in the POSIX execve() system call.
//!
//! This operation is only available in POSIX systems.
//!
//! \return The first argument of the pair is an integer that indicates
//!         how many strings are stored in the second argument.  The
//!         second argument is a NULL-terminated, dynamically allocated
//!         vector of dynamically allocated strings holding the arguments
//!         to the executable.  The caller is responsible of freeing them.
//!
template< class Command_Line >
inline
std::pair< size_t, char** >
command_line_to_posix_argv(const Command_Line& cl)
{
    size_t nargs = cl.get_arguments().size();
    BOOST_ASSERT(nargs > 0);

    char** argv = new char*[nargs + 1];
    for (size_t i = 0; i < nargs; i++)
        argv[i] = ::strdup(cl.get_arguments()[i].c_str());
    argv[nargs] = NULL;

    return std::pair< size_t, char ** >(nargs, argv);
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_WIN32_API) || defined(BOOST_PROCESS_DOXYGEN)
//!
//! \brief Converts the command line to a plain string.
//!
//! Converts the command line's list of arguments to the format
//! expected by the \a lpCommandLine parameter in the CreateProcess()
//! system call.
//!
//! This operation is only available in Win32 systems.
//!
//! \return A dynamically allocated string holding the command line
//!         to be passed to the executable.  It is returned in a
//!         shared_array object to ensure its release at some point.
//!
template< class Command_Line >
inline
boost::shared_array< TCHAR >
command_line_to_win32_cmdline(const Command_Line& cl)
{
    SIZE_T length = 0;
    for (arguments_vector::const_iterator iter = cl.get_arguments().begin();
         iter != cl.get_arguments().end(); iter++)
        length += (*iter).length() + 1;

    boost::shared_array< TCHAR > cl(new TCHAR[length]);
    ::_tcscpy_s(cl.get(), length, TEXT(""));

    for (arguments_vector::size_type i = 0; i < cl.get_arguments().size();
         i++) {
        ::_tcscat_s(cl.get(), length, TEXT(cl.get_arguments()[i].c_str()));
        if (i != cl.get_arguments().size() - 1)
            ::_tcscat_s(cl.get(), length, TEXT(" "));
    }

    return cl;
}
#endif

// ------------------------------------------------------------------------

} // namespace detail
} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_DETAIL_COMMAND_LINE_OPS_HPP)
