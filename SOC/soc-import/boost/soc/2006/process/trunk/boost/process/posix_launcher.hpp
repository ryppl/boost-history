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
//! \file boost/process/posix_launcher.hpp
//!
//! Includes the declaration of the posix_launcher class.
//!

#if !defined(BOOST_PROCESS_POSIX_LAUNCHER_HPP)
/** \cond */
#define BOOST_PROCESS_POSIX_LAUNCHER_HPP
/** \endcond */

#include <unistd.h>

#include <cerrno>
#include <cstdlib>
#include <set>

#include <boost/process/posix_child.hpp>
#include <boost/process/detail/environment.hpp>
#include <boost/process/detail/posix_ops.hpp>
#include <boost/process/detail/systembuf.hpp>
#include <boost/process/exceptions.hpp>
#include <boost/process/launcher.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

//!
//! \brief POSIX implementation of the Launcher concept.
//!
//! The launcher class implements the Launcher concept with features only
//! available in POSIX systems.  Among other things, this includes the
//! ability to set up more than three communication pipes or change the
//! privileges of the spawned process.
//!
class posix_launcher :
    public launcher
{
    //!
    //! \brief Type that represents a list of input streams that will be
    //!        redirected.
    //!
    typedef std::set< int > input_set;

    //!
    //! \brief Type that represents a list of output streams that will be
    //!        redirected.
    //!
    typedef std::set< int > output_set;

    //!
    //! \brief List of stream merges (source descriptor - target descriptor).
    //!
    detail::merge_set m_merge_set;

    //!
    //! \brief List of input streams that will be redirected.
    //!
    input_set m_input_set;

    //!
    //! \brief List of output streams that will be redirected.
    //!
    output_set m_output_set;

    //!
    //! \brief POSIX-specific properties passed to the new process.
    //!
    detail::posix_setup m_setup;

public:
    //!
    //! \brief Sets up an input redirection for the \a desc descriptor.
    //!
    //! Configures the launcher to redirect the \a desc descriptor as an
    //! input channel for the new child process.  If \a desc matches
    //! STDIN_FILENO (defined in cstdlib), this mimics the REDIR_INPUT
    //! flag passed to the constructor.
    //!
    //! \return A reference to the launcher to allow daisy-chaining calls
    //!         to redirection functions for simplicity.
    //!
    posix_launcher& redir_input(int desc);

    //!
    //! \brief Sets up an output redirection for the \a desc descriptor.
    //!
    //! Configures the launcher to redirect the \a desc descriptor as an
    //! output channel for the new child process.  If \a desc matches
    //! STDOUT_FILENO or STDERR_FILENO (defined in cstdlib), this mimics
    //! the REDIR_STDOUT and REDIR_STDERR flags passed to the constructor
    //! respectively.
    //!
    //! \return A reference to the launcher to allow daisy-chaining calls
    //!         to redirection functions for simplicity.
    //!
    posix_launcher& redir_output(int desc);

    //!
    //! \brief Sets up a merge of two output streams.
    //!
    //! Configures the launcher to merge to output streams; that is, that
    //! when the child process writes to \a from, the data seems to have
    //! been written to \a to.  If \a from matches STDOUT_FILENO and
    //! \a to matches STDERR_FILENO (both defined in cstdlib), this mimics
    //! the REDIR_STDERR_TO_STDOUT flag passed to the constructor.
    //!
    //! \return A reference to the launcher to allow daisy-chaining calls
    //!         to redirection functions for simplicity.
    //!
    posix_launcher& merge_outputs(int from, int to);

    //!
    //! \brief Gets the UID that will be used to launch the new child.
    //!
    //! Returns the user identifier that will be used to launch the new
    //! child process.  By default, this matches the user of the parent
    //! process at the moment of the creation of the launcher object.
    //!
    uid_t get_uid(void) const;

    //!
    //! \brief Sets the UID credentials used to launch the new process.
    //!
    //! Sets the UID credentials used to launch the new process to those
    //! given in \a uid.
    //!
    //! \return A reference to the launcher to allow daisy-chaining calls
    //!         to configuration function for simplicity.
    //!
    posix_launcher& set_uid(uid_t uid);

    //!
    //! \brief Gets the effective UID that will be used to launch the new
    //!        child.
    //!
    //! Returns the effective user identifier that will be used to launch
    //! the new child process.  By default, this matches the effective user
    //! of the parent process at the moment of the creation of the launcher
    //! object.
    //!
    uid_t get_euid(void) const;

    //!
    //! \brief Sets the effective UID credentials used to launch the new
    //!        process.
    //!
    //! Sets the effective UID credentials used to launch the new process
    //! to those given in \a euid.
    //!
    //! \return A reference to the launcher to allow daisy-chaining calls
    //!         to configuration function for simplicity.
    //!
    posix_launcher& set_euid(uid_t euid);

    //!
    //! \brief Gets the GID that will be used to launch the new child.
    //!
    //! Returns the group identifier that will be used to launch the new
    //! child process.  By default, this matches the group of the parent
    //! process at the moment of the creation of the launcher object.
    //!
    gid_t get_gid(void) const;

    //!
    //! \brief Sets the GID credentials used to launch the new process.
    //!
    //! Sets the GID credentials used to launch the new process to those
    //! given in \a gid.
    //!
    //! \return A reference to the launcher to allow daisy-chaining calls
    //!         to configuration function for simplicity.
    //!
    posix_launcher& set_gid(gid_t gid);

    //!
    //! \brief Gets the effective GID that will be used to launch the new
    //!        child.
    //!
    //! Returns the effective group identifier that will be used to launch
    //! the new child process.  By default, this matches the effective
    //! group of the parent process at the moment of the creation of the
    //! launcher object.
    //!
    gid_t get_egid(void) const;

    //!
    //! \brief Sets the effective GID credentials used to launch the new
    //!        process.
    //!
    //! Sets the effective GID credentials used to launch the new process
    //! to those given in \a egid.
    //!
    //! \return A reference to the launcher to allow daisy-chaining calls
    //!         to configuration function for simplicity.
    //!
    posix_launcher& set_egid(gid_t egid);

    //!
    //! \brief Gets the directory that the new process will be chrooted to.
    //!
    //! Gets a path to the directory that will be used to chroot the
    //! process to during execution.  If the resulting string is empty,
    //! it means that no chroot shall take place (the default).
    //!
    const std::string& get_chroot(void) const;

    //!
    //! \brief Sets the directory to chroot the new process to.
    //!
    //! Sets the directory that will be used to chroot the process to
    //! during execution.  \a dir may be empty to indicate that the process
    //! should not be chrooted (the default).
    //!
    //! \return A reference to the launcher to allow daisy-chaining calls
    //!         to configuration function for simplicity.
    //!
    posix_launcher& set_chroot(const std::string& dir);

    //!
    //! \brief Starts a new child process.
    //!
    //! Given a command line \a cl, starts a new process with all the
    //! parameters configured in the launcher.  The launcher can be
    //! reused afterwards to launch other different command lines.
    //!
    //! \return A handle to the new child process.
    //!
    template< class Command_Line >
    posix_child start(const Command_Line& cl);
};

// ------------------------------------------------------------------------

inline
posix_launcher&
posix_launcher::redir_input(int desc)
{
    if (desc == STDIN_FILENO)
        set_stdin_behavior(redirect_stream);
    m_input_set.insert(desc);
    return *this;
}

// ------------------------------------------------------------------------

inline
posix_launcher&
posix_launcher::redir_output(int desc)
{
    if (desc == STDOUT_FILENO)
        set_stdout_behavior(redirect_stream);
    else if (desc == STDERR_FILENO)
        set_stderr_behavior(redirect_stream);
    m_output_set.insert(desc);
    return *this;
}

// ------------------------------------------------------------------------

inline
posix_launcher&
posix_launcher::merge_outputs(int src, int dest)
{
    if (src == STDERR_FILENO && dest == STDOUT_FILENO)
        set_merge_out_err(true);
    m_merge_set.insert(std::pair< int, int >(src, dest));
    return *this;
}

// ------------------------------------------------------------------------

inline
uid_t
posix_launcher::get_uid(void)
    const
{
    return m_setup.m_uid;
}

// ------------------------------------------------------------------------

inline
uid_t
posix_launcher::get_euid(void)
    const
{
    return m_setup.m_euid;
}

// ------------------------------------------------------------------------

inline
gid_t
posix_launcher::get_gid(void)
    const
{
    return m_setup.m_gid;
}

// ------------------------------------------------------------------------

inline
gid_t
posix_launcher::get_egid(void)
    const
{
    return m_setup.m_egid;
}

// ------------------------------------------------------------------------

inline
const std::string&
posix_launcher::get_chroot(void)
    const
{
    return m_setup.m_chroot;
}

// ------------------------------------------------------------------------

inline
posix_launcher&
posix_launcher::set_uid(uid_t uid)
{
    m_setup.m_uid = uid;
    return *this;
}

// ------------------------------------------------------------------------

inline
posix_launcher&
posix_launcher::set_euid(uid_t euid)
{
    m_setup.m_euid = euid;
    return *this;
}

// ------------------------------------------------------------------------

inline
posix_launcher&
posix_launcher::set_gid(gid_t gid)
{
    m_setup.m_gid = gid;
    return *this;
}

// ------------------------------------------------------------------------

inline
posix_launcher&
posix_launcher::set_egid(gid_t egid)
{
    m_setup.m_egid = egid;
    return *this;
}

// ------------------------------------------------------------------------

inline
posix_launcher&
posix_launcher::set_chroot(const std::string& dir)
{
    m_setup.m_chroot = dir;
    return *this;
}

// ------------------------------------------------------------------------

template< class Command_Line >
inline
posix_child
posix_launcher::start(const Command_Line& cl)
{
    detail::info_map infoin;
    for (input_set::const_iterator iter = m_input_set.begin();
         iter != m_input_set.end(); iter++) {
        detail::stream_info si;
        si.m_type = detail::stream_info::usepipe;
        si.m_pipe = detail::pipe();
        infoin.insert(detail::info_map::value_type(*iter, si));
    }

    detail::info_map infoout;
    for (output_set::const_iterator iter = m_output_set.begin();
         iter != m_output_set.end(); iter++) {
        detail::stream_info si;
        si.m_type = detail::stream_info::usepipe;
        si.m_pipe = detail::pipe();
        infoout.insert(detail::info_map::value_type(*iter, si));
    }

    detail::posix_setup s = m_setup;
    s.m_work_directory = get_work_directory();

    pid_t pid = detail::posix_start(cl, posix_launcher::get_environment(),
                                    infoin, infoout, m_merge_set, s);

    return detail::factories::create_posix_child(pid, infoin, infoout);
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_POSIX_LAUNCHER_HPP)
