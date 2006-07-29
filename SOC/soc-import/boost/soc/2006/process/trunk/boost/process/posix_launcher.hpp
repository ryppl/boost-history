//
// Boost.Process
// Implementation of the Launcher concept.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#if !defined(BOOST_PROCESS_POSIX_LAUNCHER_HPP)
#define BOOST_PROCESS_POSIX_LAUNCHER_HPP

#include <unistd.h>

#include <cerrno>
#include <cstdlib>
#include <set>

#include <boost/process/basic_posix_child.hpp>
#include <boost/process/detail/environment.hpp>
#include <boost/process/detail/systembuf.hpp>
#include <boost/process/exceptions.hpp>
#include <boost/process/launcher.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

class posix_launcher :
    public launcher
{
    typedef std::set< std::pair< int, int > > merge_set;
    typedef std::set< int > input_set;
    typedef std::set< int > output_set;

    merge_set m_merge_set;
    input_set m_input_set;
    output_set m_output_set;

    uid_t m_uid;
    uid_t m_euid;
    gid_t m_gid;
    gid_t m_egid;
    std::string m_chroot;

public:
    posix_launcher(int flags = launcher::REDIR_ALL);

    posix_launcher& redir_input(int desc);
    posix_launcher& redir_output(int desc);
    posix_launcher& merge_outputs(int from, int to);

    uid_t get_uid(void) const;
    uid_t get_euid(void) const;
    gid_t get_gid(void) const;
    gid_t get_egid(void) const;
    const std::string& get_chroot(void) const;

    posix_launcher& set_uid(uid_t uid);
    posix_launcher& set_euid(uid_t euid);
    posix_launcher& set_gid(gid_t gid);
    posix_launcher& set_egid(gid_t egid);
    posix_launcher& set_chroot(const std::string& dir);

    template< class Command_Line >
    basic_posix_child< Command_Line > start(const Command_Line& cl);
};

// ------------------------------------------------------------------------

inline
posix_launcher::posix_launcher(int flags) :
    launcher(flags),
    m_uid(::getuid()),
    m_euid(::geteuid()),
    m_gid(::getgid()),
    m_egid(::getegid()),
    m_chroot("")
{
    if (flags & REDIR_STDIN)
        redir_input(STDIN_FILENO);
    if (flags & REDIR_STDOUT)
        redir_output(STDOUT_FILENO);
    if (flags & REDIR_STDERR)
        redir_output(STDERR_FILENO);
    if (flags & REDIR_STDERR_TO_STDOUT)
        merge_outputs(STDERR_FILENO, STDOUT_FILENO);
}

// ------------------------------------------------------------------------

inline
posix_launcher&
posix_launcher::redir_input(int desc)
{
    if (desc == STDIN_FILENO)
        set_flags(get_flags() | REDIR_STDIN);
    m_input_set.insert(desc);
    return *this;
}

// ------------------------------------------------------------------------

inline
posix_launcher&
posix_launcher::redir_output(int desc)
{
    if (desc == STDOUT_FILENO)
        set_flags(get_flags() | REDIR_STDOUT);
    else if (desc == STDERR_FILENO)
        set_flags(get_flags() | REDIR_STDERR);
    m_output_set.insert(desc);
    return *this;
}

// ------------------------------------------------------------------------

inline
posix_launcher&
posix_launcher::merge_outputs(int src, int dest)
{
    if (src == STDERR_FILENO && dest == STDOUT_FILENO)
        set_flags(get_flags() | REDIR_STDERR_TO_STDOUT);
    m_merge_set.insert(std::pair< int, int >(src, dest));
    return *this;
}

// ------------------------------------------------------------------------

inline
uid_t
posix_launcher::get_uid(void)
    const
{
    return m_uid;
}

// ------------------------------------------------------------------------

inline
uid_t
posix_launcher::get_euid(void)
    const
{
    return m_euid;
}

// ------------------------------------------------------------------------

inline
gid_t
posix_launcher::get_gid(void)
    const
{
    return m_gid;
}

// ------------------------------------------------------------------------

inline
gid_t
posix_launcher::get_egid(void)
    const
{
    return m_egid;
}

// ------------------------------------------------------------------------

inline
const std::string&
posix_launcher::get_chroot(void)
    const
{
    return m_chroot;
}

// ------------------------------------------------------------------------

inline
posix_launcher&
posix_launcher::set_uid(uid_t uid)
{
    m_uid = uid;
    return *this;
}

// ------------------------------------------------------------------------

inline
posix_launcher&
posix_launcher::set_euid(uid_t euid)
{
    m_euid = euid;
    return *this;
}

// ------------------------------------------------------------------------

inline
posix_launcher&
posix_launcher::set_gid(gid_t gid)
{
    m_gid = gid;
    return *this;
}

// ------------------------------------------------------------------------

inline
posix_launcher&
posix_launcher::set_egid(gid_t egid)
{
    m_egid = egid;
    return *this;
}

// ------------------------------------------------------------------------

inline
posix_launcher&
posix_launcher::set_chroot(const std::string& dir)
{
    m_chroot = dir;
    return *this;
}

// ------------------------------------------------------------------------

template< class Command_Line >
inline
basic_posix_child< Command_Line >
posix_launcher::start(const Command_Line& cl)
{
    typedef typename
        basic_posix_child< Command_Line >::pipe_map pipe_map;

    pipe_map inpipes;
    for (input_set::const_iterator iter = m_input_set.begin();
         iter != m_input_set.end(); iter++)
        inpipes.insert
            (typename pipe_map::value_type(*iter, detail::pipe()));

    pipe_map outpipes;
    for (output_set::const_iterator iter = m_output_set.begin();
         iter != m_output_set.end(); iter++)
        outpipes.insert
            (typename pipe_map::value_type(*iter, detail::pipe()));

    pid_t pid = ::fork();
    if (pid == -1) {
        boost::throw_exception
            (system_error("boost::process::posix_launcher::start",
                          "fork(2) failed", errno));
    } else if (pid == 0) {
        for (typename pipe_map::iterator iter = inpipes.begin();
             iter != inpipes.end(); iter++) {
            int d = (*iter).first;
            detail::pipe& p = (*iter).second;

            p.wend().close();
            if (d != p.rend().get())
                p.rend().posix_remap(d);
        }

        for (typename pipe_map::iterator iter = outpipes.begin();
             iter != outpipes.end(); iter++) {
            int d = (*iter).first;
            detail::pipe& p = (*iter).second;

            p.rend().close();
            if (d != p.wend().get())
                p.wend().posix_remap(d);
        }

        for (merge_set::const_iterator iter = m_merge_set.begin();
             iter != m_merge_set.end(); iter++) {
            const std::pair< int, int >& p = (*iter);
            detail::file_handle fh =
                detail::file_handle::posix_dup(p.second, p.first);
            fh.disown();
        }

        if (!m_chroot.empty()) {
            if (::chroot(m_chroot.c_str()) == -1)
                boost::throw_exception
                    (system_error("boost::process::posix_launcher::start",
                                  "chroot(2) failed", errno));
        }

        if (m_gid != ::getgid()) {
            if (::setgid(m_gid) == -1)
                boost::throw_exception
                    (system_error("boost::process::posix_launcher::start",
                                  "setgid(2) failed", errno));
        }

        if (m_egid != ::getegid()) {
            if (::setegid(m_egid) == -1)
                boost::throw_exception
                    (system_error("boost::process::posix_launcher::start",
                                  "setegid(2) failed", errno));
        }

        if (m_uid != ::getuid()) {
            if (::setuid(m_uid) == -1)
                boost::throw_exception
                    (system_error("boost::process::posix_launcher::start",
                                  "setuid(2) failed", errno));
        }

        if (m_euid != ::geteuid()) {
            if (::seteuid(m_euid) == -1)
                boost::throw_exception
                    (system_error("boost::process::posix_launcher::start",
                                  "seteuid(2) failed", errno));
        }

        posix_child_entry(cl);
        BOOST_ASSERT(false); // Not reached.
    }

    BOOST_ASSERT(pid > 0);

    for (typename pipe_map::iterator iter = inpipes.begin();
         iter != inpipes.end(); iter++)
        (*iter).second.rend().close();

    for (typename pipe_map::iterator iter = outpipes.begin();
         iter != outpipes.end(); iter++)
        (*iter).second.wend().close();

    detail::file_handle fhstdin, fhstdout, fhstderr;

    if (get_flags() & REDIR_STDIN) {
        typename pipe_map::iterator iter = inpipes.find(STDIN_FILENO);
        if (iter != inpipes.end()) {
            fhstdin = (*iter).second.wend();
            inpipes.erase(iter);
        }
    }

    if (get_flags() & REDIR_STDOUT) {
        typename pipe_map::iterator iter = outpipes.find(STDOUT_FILENO);
        if (iter != outpipes.end()) {
            fhstdout = (*iter).second.rend();
            outpipes.erase(iter);
        }
    }

    if (get_flags() & REDIR_STDERR) {
        typename pipe_map::iterator iter = outpipes.find(STDERR_FILENO);
        if (iter != outpipes.end()) {
            fhstderr = (*iter).second.rend();
            outpipes.erase(iter);
        }
    }

    BOOST_ASSERT(!(get_flags() & REDIR_STDIN) || fhstdin.is_valid());
    BOOST_ASSERT(!(get_flags() & REDIR_STDOUT) || fhstdout.is_valid());
    BOOST_ASSERT(!(get_flags() & REDIR_STDERR) || fhstderr.is_valid());
    return basic_posix_child< Command_Line >
        (pid, cl, fhstdin, fhstdout, fhstderr, inpipes, outpipes);
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_POSIX_LAUNCHER_HPP)
