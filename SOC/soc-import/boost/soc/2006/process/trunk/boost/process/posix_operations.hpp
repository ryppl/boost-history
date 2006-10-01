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
//! \file boost/process/posix_operations.hpp
//!
//! Provides miscellaneous free functions specific to POSIX operating
//! systems.
//!

#if !defined(BOOST_PROCESS_POSIX_OPERATIONS_HPP)
/** \cond */
#define BOOST_PROCESS_POSIX_OPERATIONS_HPP
/** \endcond */

#include <cstdlib>

#include <boost/process/detail/posix_ops.hpp>
#include <boost/process/posix_child.hpp>
#include <boost/process/posix_context.hpp> // XXX For behavior_map.
#include <boost/process/stream_behavior.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------
//
//!
//! \brief Starts a new child process.
//!
//! Given an executable and the set of arguments passed to it, starts
//! a new process with all the
//! parameters configured in the launcher.  The launcher can be
//! reused afterwards to launch other different processes.
//!
//! \return A handle to the new child process.
//!
template< class Executable, class Arguments, class Posix_Context >
inline
posix_child
posix_launch(const Executable& exe, const Arguments& args,
             const Posix_Context& ctx)
{
    detail::info_map input_info;
    for (behavior_map::const_iterator iter = ctx.m_input_behavior.begin();
         iter != ctx.m_input_behavior.end(); iter++)
        detail::posix_behavior_to_info((*iter).second, (*iter).first,
                                       false, input_info);

    detail::info_map output_info;
    for (behavior_map::const_iterator iter = ctx.m_output_behavior.begin();
         iter != ctx.m_output_behavior.end(); iter++)
        detail::posix_behavior_to_info((*iter).second, (*iter).first,
                                       true, output_info);

    detail::posix_setup s;
    s.m_work_directory = ctx.m_work_directory;
    s.m_uid = ctx.m_uid;
    s.m_euid = ctx.m_euid;
    s.m_gid = ctx.m_gid;
    s.m_egid = ctx.m_egid;
    s.m_chroot = ctx.m_chroot;

    pid_t pid = detail::posix_start(exe, args, ctx.m_environment,
                                    input_info, output_info,
                                    ctx.m_merge_set, s);

    return posix_child(pid, input_info, output_info);
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_POSIX_OPERATIONS_HPP)
