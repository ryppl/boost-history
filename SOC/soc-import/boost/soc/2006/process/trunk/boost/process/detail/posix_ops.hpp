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
//! \file boost/process/detail/posix_ops.hpp
//!
//! Provides some convenience functions to start processes under POSIX
//! operating systems.
//!

#if !defined(BOOST_PROCESS_DETAIL_POSIX_OPS_HPP)
/** \cond */
#define BOOST_PROCESS_DETAIL_POSIX_OPS_HPP
/** \endcond */

#include <boost/process/config.hpp>

#if !defined(BOOST_PROCESS_POSIX_API)
#   error "Unsupported platform."
#endif

extern "C" {
#   include <fcntl.h>
#   include <unistd.h>
}

#include <cerrno>
#include <cstdlib>
#include <map>
#include <set>

#include <boost/optional.hpp>
#include <boost/process/detail/command_line_ops.hpp>
#include <boost/process/detail/environment.hpp>
#include <boost/process/detail/file_handle.hpp>
#include <boost/process/detail/pipe.hpp>
#include <boost/process/exceptions.hpp>
#include <boost/throw_exception.hpp>

namespace boost {
namespace process {
namespace detail {

// ------------------------------------------------------------------------

//!
//! \brief Configuration data for a file descriptor.
//!
//! This convenience structure provides a compact way to pass information
//! around on how to configure a file descriptor.  It is used in
//! conjunction with the info_map map to create an unidirectional
//! association between file descriptors and their configuration details.
//!
struct stream_info
{
    enum type { dontclose, usefile, usehandle, usepipe } m_type;

    // Valid when m_type == usefile.
    std::string m_file;

    // Valid when m_type == usehandle.
    file_handle m_handle;

    // Valid when m_type == usepipe.
    boost::optional< pipe > m_pipe;
};

// ------------------------------------------------------------------------

//!
//! Holds a mapping between native file descriptors and their corresponding
//! pipes to set up communication between the parent and the %child process.
//!
typedef std::map< int, stream_info > info_map;

//!
//! Maintains a list of file descriptor pairs, aimed at keeping a list of
//! stream merges (source descriptor, target descriptor).
//!
typedef std::set< std::pair< int, int > > merge_set;

// ------------------------------------------------------------------------

//!
//! \brief Helper class to configure a POSIX %child.
//!
//! This helper class is used to hold all the attributes that configure a
//! new POSIX %child process and to centralize all the actions needed to
//! make them effective.
//!
//! All its fields are public for simplicity.  It is only intended for
//! internal use and it is heavily coupled with the Launcher
//! implementations.
//!
struct posix_setup
{
    //!
    //! \brief The work directory.
    //!
    //! This string specifies the directory in which the %child process
    //! starts execution.  It cannot be empty.
    //!
    std::string m_work_directory;

    //!
    //! \brief The user credentials.
    //!
    //! UID that specifies the user credentials to use to run the %child
    //! process.  Defaults to the current UID.
    //!
    uid_t m_uid;

    //!
    //! \brief The effective user credentials.
    //!
    //! EUID that specifies the effective user credentials to use to run
    //! the %child process.  Defaults to the current EUID.
    //!
    uid_t m_euid;

    //!
    //! \brief The group credentials.
    //!
    //! GID that specifies the group credentials to use to run the %child
    //! process.  Defaults to the current GID.
    //!
    uid_t m_gid;

    //!
    //! \brief The effective group credentials.
    //!
    //! EGID that specifies the effective group credentials to use to run
    //! the %child process.  Defaults to the current EGID.
    //!
    uid_t m_egid;

    //!
    //! \brief The chroot directory, if any.
    //!
    //! Specifies the directory in which the %child process is chrooted
    //! before execution.  Empty if this feature is not desired.
    //!
    std::string m_chroot;

    //!
    //! \brief Creates a new properties set.
    //!
    //! Creates a new object that has sensible default values for all the
    //! properties.
    //!
    posix_setup(void);

    //!
    //! \brief Sets up the execution environment.
    //!
    //! Modifies the current execution environment (that of the %child) so
    //! that the properties become effective.
    //!
    //! \throw system_error If any error ocurred during environment
    //!                     configuration.  The child process should abort
    //!                     execution if this happens because its start
    //!                     conditions cannot be met.
    //!
    void operator()(void) const;
};

// ------------------------------------------------------------------------

inline
posix_setup::posix_setup(void) :
    m_uid(::getuid()),
    m_euid(::geteuid()),
    m_gid(::getgid()),
    m_egid(::getegid())
{
}

// ------------------------------------------------------------------------

inline
void
posix_setup::operator()(void)
    const
{
    if (!m_chroot.empty()) {
        if (::chroot(m_chroot.c_str()) == -1)
            boost::throw_exception
                (system_error("boost::process::detail::posix_setup",
                              "chroot(2) failed", errno));
    }

    if (m_gid != ::getgid()) {
        if (::setgid(m_gid) == -1)
            boost::throw_exception
                (system_error("boost::process::detail::posix_setup",
                              "setgid(2) failed", errno));
    }

    if (m_egid != ::getegid()) {
        if (::setegid(m_egid) == -1)
            boost::throw_exception
                (system_error("boost::process::detail::posix_setup",
                              "setegid(2) failed", errno));
    }

    if (m_uid != ::getuid()) {
        if (::setuid(m_uid) == -1)
            boost::throw_exception
                (system_error("boost::process::detail::posix_setup",
                              "setuid(2) failed", errno));
    }

    if (m_euid != ::geteuid()) {
        if (::seteuid(m_euid) == -1)
            boost::throw_exception
                (system_error("boost::process::detail::posix_setup",
                              "seteuid(2) failed", errno));
    }

    BOOST_ASSERT(m_work_directory != "");
    if (chdir(m_work_directory.c_str()) == -1)
        boost::throw_exception
            (system_error("boost::process::detail::posix_setup",
                          "chdir(2) failed", errno));
}

// ------------------------------------------------------------------------

//!
//! \brief Starts a new child process in a POSIX operating system.
//!
//! This helper functions is provided to simplify the Launcher's task when
//! it comes to starting up a new process in a POSIX operating system.
//! The function hides all the details of the fork/exec pair of calls as
//! well as all the setup of communication pipes and execution environment.
//!
//! \param cl The command line used to execute the child process.
//! \param env The environment variables that the new child process
//!            receives.
//! \param infoin A map describing all input file descriptors to be
//!               redirected.
//! \param infoout A map describing all output file descriptors to be
//!                redirected.
//! \param merges A list of output file descriptors to be merged.
//! \param setup A helper object used to configure the child's execution
//!              environment.
//! \return The new process' PID.  The caller is responsible of creating
//!         an appropriate Child representation for it.
//!
template< class Command_Line >
inline
pid_t
posix_start(const Command_Line& cl,
            const environment& env,
            info_map& infoin,
            info_map& infoout,
            merge_set& merges,
            const posix_setup& setup)
{
    pid_t pid = ::fork();
    if (pid == -1) {
        boost::throw_exception
            (system_error("boost::process::detail::posix_start",
                          "fork(2) failed", errno));
    } else if (pid == 0) {
#if defined(F_MAXFD)
        int maxdescs = ::fcntl(0, F_MAXFD);
        if (maxdescs == -1)
            maxdescs = 128; // XXX
#else
        int maxdescs = 128; // XXX
#endif
        bool closeflags[maxdescs];
        for (int i = 0; i < maxdescs; i++)
            closeflags[i] = true;

        for (info_map::iterator iter = infoin.begin();
             iter != infoin.end(); iter++) {
            int d = (*iter).first;
            stream_info& si = (*iter).second;

            BOOST_ASSERT(d < maxdescs);
            closeflags[d] = false;

            if (si.m_type == stream_info::usefile) {
                int fd = ::open(si.m_file.c_str(), O_RDONLY);
                if (fd == -1)
                    ; // XXX Error!
                if (fd != d) {
                    file_handle h(fd);
                    h.posix_remap(d);
                    h.disown();
                }
            } else if (si.m_type == stream_info::usehandle) {
                if (si.m_handle.get() != d)
                    si.m_handle.posix_remap(d);
            } else if (si.m_type == stream_info::usepipe) {
                si.m_pipe->wend().close();
                if (d != si.m_pipe->rend().get())
                    si.m_pipe->rend().posix_remap(d);
            } else
                BOOST_ASSERT(si.m_type == stream_info::dontclose);
        }

        for (info_map::iterator iter = infoout.begin();
             iter != infoout.end(); iter++) {
            int d = (*iter).first;
            stream_info& si = (*iter).second;

            BOOST_ASSERT(d < maxdescs);
            closeflags[d] = false;

            if (si.m_type == stream_info::usefile) {
                int fd = ::open(si.m_file.c_str(), O_WRONLY);
                if (fd == -1)
                    ; // XXX Error!
                if (fd != d) {
                    file_handle h(fd);
                    h.posix_remap(d);
                    h.disown();
                }
            } else if (si.m_type == stream_info::usehandle) {
                if (si.m_handle.get() != d)
                    si.m_handle.posix_remap(d);
            } else if (si.m_type == stream_info::usepipe) {
                si.m_pipe->rend().close();
                if (d != si.m_pipe->wend().get())
                    si.m_pipe->wend().posix_remap(d);
            } else
                BOOST_ASSERT(si.m_type == stream_info::dontclose);
        }

        for (merge_set::const_iterator iter = merges.begin();
             iter != merges.end(); iter++) {
            const std::pair< int, int >& p = (*iter);
            file_handle fh = file_handle::posix_dup(p.second, p.first);
            fh.disown();
            BOOST_ASSERT(p.first < maxdescs);
            closeflags[p.first] = false;
        }

        for (int i = 0; i < maxdescs; i++)
            if (closeflags[i])
#if defined(F_SETFD) && defined(FD_CLOEXEC)
                ::fcntl(i, F_SETFD, FD_CLOEXEC);
#else
                ::close(i);
#endif

        try {
            setup();
        } catch (const system_error& e) {
            ::write(STDERR_FILENO, e.what(), std::strlen(e.what()));
            ::write(STDERR_FILENO, "\n", 1);
            ::exit(EXIT_FAILURE);
        }

        std::pair< std::size_t, char** > args = command_line_to_posix_argv(cl);
        char** envp = env.envp();

        ::execve(cl.get_executable().c_str(), args.second, envp);
        system_error e("boost::process::detail::posix_start",
                       "execvp(2) failed", errno);

        for (std::size_t i = 0; i < args.first; i++)
            delete [] args.second[i];
        delete [] args.second;

        for (std::size_t i = 0; i < env.size(); i++)
            delete [] envp[i];
        delete [] envp;

        ::write(STDERR_FILENO, e.what(), std::strlen(e.what()));
        ::write(STDERR_FILENO, "\n", 1);
        ::exit(EXIT_FAILURE);
    }

    BOOST_ASSERT(pid > 0);

    for (info_map::iterator iter = infoin.begin();
         iter != infoin.end(); iter++) {
        stream_info& si = (*iter).second;

        if (si.m_type == stream_info::usepipe)
            si.m_pipe->rend().close();
    }

    for (info_map::iterator iter = infoout.begin();
         iter != infoout.end(); iter++) {
        stream_info& si = (*iter).second;

        if (si.m_type == stream_info::usepipe)
            si.m_pipe->wend().close();
    }

    return pid;
}

// ------------------------------------------------------------------------

} // namespace detail
} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_DETAIL_POSIX_OPS_HPP)
