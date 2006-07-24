//
// Boost.Process
// Implementation of the Attributes concept for POSIX systems.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#if !defined(BOOST_PROCESS_POSIX_ATTRIBUTES_HPP)
#define BOOST_PROCESS_POSIX_ATTRIBUTES_HPP

#include <boost/process/config.hpp>

#if !defined(BOOST_PROCESS_POSIX_API)
#   error "Unsupported platform."
#endif

extern "C" {
#include <sys/types.h>
#include <unistd.h>
}

#include <cerrno>
#include <string>

#include <boost/assert.hpp>
#include <boost/process/attributes.hpp>
#include <boost/process/exceptions.hpp>
#include <boost/throw_exception.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

class launcher;

class posix_attributes :
    public attributes
{
    uid_t m_uid;
    uid_t m_euid;
    gid_t m_gid;
    gid_t m_egid;
    std::string m_chroot;

protected:
    friend class launcher;
    void setup(void) const;

public:
    explicit posix_attributes(const std::string& work_directory = "");

    uid_t get_uid(void) const;
    uid_t get_euid(void) const;
    gid_t get_gid(void) const;
    gid_t get_egid(void) const;
    const std::string& get_chroot(void) const;

    posix_attributes& set_uid(uid_t uid);
    posix_attributes& set_euid(uid_t euid);
    posix_attributes& set_gid(gid_t gid);
    posix_attributes& set_egid(gid_t egid);
    posix_attributes& set_chroot(const std::string& dir);
};

// ------------------------------------------------------------------------

inline
posix_attributes::posix_attributes(const std::string& work_directory) :
    attributes(work_directory),
    m_uid(::getuid()),
    m_euid(::geteuid()),
    m_gid(::getgid()),
    m_egid(::getegid()),
    m_chroot("")
{
}

// ------------------------------------------------------------------------

inline
uid_t
posix_attributes::get_uid(void)
    const
{
    return m_uid;
}

// ------------------------------------------------------------------------

inline
uid_t
posix_attributes::get_euid(void)
    const
{
    return m_euid;
}

// ------------------------------------------------------------------------

inline
gid_t
posix_attributes::get_gid(void)
    const
{
    return m_gid;
}

// ------------------------------------------------------------------------

inline
gid_t
posix_attributes::get_egid(void)
    const
{
    return m_egid;
}

// ------------------------------------------------------------------------

inline
const std::string&
posix_attributes::get_chroot(void)
    const
{
    return m_chroot;
}

// ------------------------------------------------------------------------

inline
posix_attributes&
posix_attributes::set_uid(uid_t uid)
{
    m_uid = uid;
    return *this;
}

// ------------------------------------------------------------------------

inline
posix_attributes&
posix_attributes::set_euid(uid_t euid)
{
    m_euid = euid;
    return *this;
}

// ------------------------------------------------------------------------

inline
posix_attributes&
posix_attributes::set_gid(gid_t gid)
{
    m_gid = gid;
    return *this;
}

// ------------------------------------------------------------------------

inline
posix_attributes&
posix_attributes::set_egid(gid_t egid)
{
    m_egid = egid;
    return *this;
}

// ------------------------------------------------------------------------

inline
posix_attributes&
posix_attributes::set_chroot(const std::string& dir)
{
    m_chroot = dir;
    return *this;
}

// ------------------------------------------------------------------------

inline
void
posix_attributes::setup(void)
    const
{
    attributes::setup();

    if (!m_chroot.empty()) {
        if (::chroot(m_chroot.c_str()) == -1)
            boost::throw_exception
                (system_error("boost::process::posix_attributes::setup",
                              "chroot(2) failed", errno));
    }

    if (m_gid != ::getgid()) {
        if (::setgid(m_gid) == -1)
            boost::throw_exception
                (system_error("boost::process::posix_attributes::setup",
                              "setgid(2) failed", errno));
    }

    if (m_egid != ::getegid()) {
        if (::setegid(m_egid) == -1)
            boost::throw_exception
                (system_error("boost::process::posix_attributes::setup",
                              "setegid(2) failed", errno));
    }

    if (m_uid != ::getuid()) {
        if (::setuid(m_uid) == -1)
            boost::throw_exception
                (system_error("boost::process::posix_attributes::setup",
                              "setuid(2) failed", errno));
    }

    if (m_euid != ::geteuid()) {
        if (::seteuid(m_euid) == -1)
            boost::throw_exception
                (system_error("boost::process::posix_attributes::setup",
                              "seteuid(2) failed", errno));
    }
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_POSIX_ATTRIBUTES_HPP)
