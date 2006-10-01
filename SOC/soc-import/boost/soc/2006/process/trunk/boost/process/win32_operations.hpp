//
// Boost.Process
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

//!
//! \file boost/process/win32_operations.hpp
//!
//! Provides miscellaneous free functions specific to Win32 operating
//! systems.
//!

#if !defined(BOOST_PROCESS_WIN32_OPERATIONS_HPP)
/** \cond */
#define BOOST_PROCESS_WIN32_OPERATIONS_HPP
/** \endcond */

#include <boost/process/config.hpp>

#if !defined(BOOST_PROCESS_WIN32_API)
#   error "Unsupported platform."
#endif

extern "C" {
#include <windows.h>
}

#include <boost/process/detail/file_handle.hpp>
#include <boost/process/win32_child.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

template< class Executable, class Arguments, class Win32_Context >
inline
win32_child
win32_start(const Executable& exe,
            const Arguments& args,
            const Win32_Context& ctx)
{
    detail::file_handle fhstdin, fhstdout, fhstderr;

    detail::stream_info behin =
        win32_behavior_to_info(ctx.m_stdin_behavior, false, fhstdin);
    detail::stream_info behout =
        win32_behavior_to_info(ctx.m_stdout_behavior, true, fhstdout);
    detail::stream_info beherr =
        win32_behavior_to_info(ctx.m_stderr_behavior, true, fhstderr);

    detail::win32_setup s;
    s.m_work_directory = ctx.m_work_directory;
    s.m_startupinfo = m_startupinfo;

    PROCESS_INFORMATION pi =
        detail::win32_start(exe, args, ctx.m_environment,
                            behin, behout, beherr,
                            ctx.m_merge_stderr_with_stdout, s);

    return win32_child(pi, fhstdin, fhstdout, fhstderr);
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_WIN32_OPERATIONS_HPP)
