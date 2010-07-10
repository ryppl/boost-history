//
// Boost.Process
// ~~~~~~~~~~~~~
//
// Copyright (c) 2006, 2007 Julio M. Merino Vidal
// Copyright (c) 2008, 2009 Boris Schaeling
// Copyright (c) 2010 Felipe Tanus, Boris Schaeling
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

/**
 * \file boost/process/status.hpp
 *
 * Includes the declaration of the status class.
 */

#ifndef BOOST_PROCESS_STATUS_HPP
#define BOOST_PROCESS_STATUS_HPP

#if defined(BOOST_WINDOWS_API)
#include <boost/process/config.hpp>
#include <boost/process/detail/basic_status.hpp>
#include <boost/process/detail/basic_status_service.hpp>

namespace boost {
namespace process {

typedef detail::basic_status<detail::basic_status_service<> > status;

}
}
#endif
#if defined(BOOST_POSIX_API)

//Old status implementation
class status
{
    friend class process;

public:
    /**
     * Returns whether the process exited gracefully or not.
     */
    bool exited() const
    {
        return WIFEXITED(flags_);
    }

    /**
     * If exited, returns the exit code.
     *
     * If the process exited, returns the exit code it returned.
     *
     * \pre exited() is true.
     */

    int exit_code() const
    {
        BOOST_ASSERT(exited());

        return WEXITSTATUS(flags_);
    }

protected:
    /**
     * Creates a status object based on exit information.
     *
     * Creates a new status object representing the exit status of a
     * child process.
     *
     * \param flags In a POSIX system this parameter contains the
     *        flags returned by the ::waitpid() call. In a
     *        Windows system it contains the exit code only.
     */
    status(int flags)
        : flags_(flags) {}

    /**
     * OS-specific codification of exit status.
     */
    int flags_;
};
#endif
#endif
