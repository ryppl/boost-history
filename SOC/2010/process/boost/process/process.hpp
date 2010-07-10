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
 * \file boost/process/process.hpp
 *
 * Includes the declaration of the process class.
 */

#ifndef BOOST_PROCESS_PROCESS_HPP
#define BOOST_PROCESS_PROCESS_HPP

#include <boost/process/config.hpp>

#if defined(BOOST_POSIX_API)
#   include <cerrno>
#   include <sys/wait.h>
#   include <signal.h>
#elif defined(BOOST_WINDOWS_API)
#   include <cstdlib>
#   include <windows.h>
#else
#   error "Unsupported platform." 
#endif

#include <boost/process/status.hpp>
#include <boost/system/system_error.hpp>
#include <boost/throw_exception.hpp>
#include <boost/shared_ptr.hpp>

namespace boost {
namespace process {

/**
 * The process class provides access to an unrelated process (a process 
 * which is neither a child process nor a parent process nor self). 
 */
class process
{
public:
#if defined(BOOST_PROCESS_DOXYGEN)
    /**
     * Opaque name for the native process' identifier type.
     *
     * Each operating system identifies processes using a specific type.
     * The \a id_type type is used to transparently refer to a process
     * regardless of the operating system in which this class is used.
     *
     * This type is guaranteed to be an integral type on all supported
     * platforms.
     */
    typedef NativeProcessId id_type;
#elif defined(BOOST_POSIX_API)
    typedef pid_t id_type;
#elif defined(BOOST_WINDOWS_API)
    typedef DWORD id_type;
#endif

    /**
     * Constructs a new process object.
     *
     * Creates a new process object that represents a running process
     * within the system.
     */
    process(id_type id)
        : id_(id)
    {
    }

    /**
     * Returns the process' identifier.
     */
    id_type get_id() const
    {
        return id_;
    }

    /**
     * Terminates the process execution.
     *
     * Forces the termination of the process execution. Some platforms
     * allow processes to ignore some external termination notifications
     * or to capture them for a proper exit cleanup. You can set the
     * \a force flag to true in them to force their termination regardless
     * of any exit handler.
     *
     * After this call, accessing this object can be dangerous because the
     * process identifier may have been reused by a different process. It
     * might still be valid, though, if the process has refused to die.
     *
     * \throw boost::system::system_error If the system call used to
     *        terminate the process fails.
     */
    void terminate(bool force = false) const
    {
#if defined(BOOST_POSIX_API)
        if (::kill(id_, force ? SIGKILL : SIGTERM) == -1)
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("kill(2) failed"); 
#elif defined(BOOST_WINDOWS_API)
        HANDLE h = ::OpenProcess(PROCESS_TERMINATE, FALSE, id_);
        if (h == NULL)
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("OpenProcess() failed"); 
        if (!::TerminateProcess(h, EXIT_FAILURE))
        {
            ::CloseHandle(h);
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("TerminateProcess() failed"); 
        }
        if (!::CloseHandle(h))
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("CloseHandle() failed"); 
#endif
    }

    /**
     * Blocks and waits for the child process to terminate.
     *
     * Returns an exit code. The child process object ceases to be
     * valid after this call.
     *
     * \remark Blocking remarks: This call blocks if the child
     *         process has not finalized execution and waits until
     *         it terminates.
     */
    int wait()
    {
#if defined(BOOST_POSIX_API)
        int s;
        if (::waitpid(get_id(), &s, 0) == -1)
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("waitpid(2) failed"); 
        return s;
#elif defined(BOOST_WINDOWS_API)
        HANDLE h = ::OpenProcess(SYNCHRONIZE, FALSE, id_); 
        if (h == NULL) 
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("OpenProcess() failed"); 
        if (::WaitForSingleObject(h, INFINITE) == WAIT_FAILED) 
        { 
            ::CloseHandle(h); 
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("WaitForSingleObject() failed"); 
        } 
        DWORD exit_code; 
        if (!::GetExitCodeProcess(h, &exit_code)) 
        { 
            ::CloseHandle(h); 
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("GetExitCodeProcess() failed"); 
        } 
        if (!::CloseHandle(h)) 
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("CloseHandle() failed"); 
        return exit_code; 
#endif
    }

private:
    /**
     * The process' identifier.
     */
    id_type id_;
};

}
}

#endif
