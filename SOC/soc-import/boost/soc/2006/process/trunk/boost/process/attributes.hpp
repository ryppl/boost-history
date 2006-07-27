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
//! \file boost/process/attributes.hpp
//!
//! Includes the declaration of the attributes class.
//!

#if !defined(BOOST_PROCESS_ATTRIBUTES_HPP)
/** \cond */
#define BOOST_PROCESS_ATTRIBUTES_HPP
/** \endcond */

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_POSIX_API)
extern "C" {
#   include <unistd.h>
}
#   include <cerrno>
#elif defined(BOOST_PROCESS_WIN32_API)
extern "C" {
#   include <windows.h>
}
#else
#   error "Unsupported platform."
#endif

#include <string>

#include <boost/assert.hpp>
#include <boost/process/exceptions.hpp>
#include <boost/throw_exception.hpp>

namespace boost {
namespace process {

class launcher;

// ------------------------------------------------------------------------

//!
//! \brief Portable and simple implementation of the Attributes concept.
//!
//! This class provides a portable and simple implementation of the
//! Attributes concept.  It includes the minimum common set of  attributes
//! that are portable across the different operating systems supported by
//! the library.
//!
class attributes
{
    //!
    //! \brief The process' current work directory.
    //!
    //! The work directory is the directory in which the process is
    //! currently running.  This may be changed during the process'
    //! lifetime, although its main purpose is to set up a new child
    //! process.
    //!
    //! Ideally this could be of boost::filesystem::path type, but it
    //! is a regular string to avoid depending on Boost.Filesystem.
    //!
    std::string m_work_directory;

protected:
    friend class launcher;

    //!
    //! \brief Sets up the running process according to the attributes.
    //!
    //! Sets up the currently running process according to the attributes
    //! described by this object.  This function is aimed basically at
    //! POSIX systems which need to modify the current process before
    //! doing the real execve(2) call.  That is, this function is called
    //! in between the fork(2) and execve(2) calls, in the newly created
    //! child process.
    //!
    //! This function does not change any object but has a lot of side
    //! effects in the currently running process because it changes its
    //! execution context.
    //!
    //! The launcher class is allowed to directly call this function.
    //!
    void setup(void) const;

public:
    //!
    //! \brief Creates a new set of attributes.
    //!
    //! Creates a new set of attributes that will be presumably used to
    //! start a new child process.
    //!
    //! If \a work_directory is empty, the attributes' work directory is
    //! set to point to the current working directory.  Otherwise it is
    //! set to the given path.
    //!
    explicit attributes(const std::string& work_directory = "");

    //!
    //! \brief Returns the work directory.
    //!
    //! Returns the work directory for an attributes object.
    //!
    const std::string& get_work_directory(void) const;
};

// ------------------------------------------------------------------------

inline
attributes::attributes(const std::string& work_directory)
{
    if (work_directory == "") {
#if defined(BOOST_PROCESS_POSIX_API)
        const char* buf = ::getcwd(NULL, 0);
        if (buf == NULL)
            boost::throw_exception
                (system_error
                 ("boost::process::attributes::attributes",
                  "getcwd(2) failed", errno));
        m_work_directory = buf;
#elif defined(BOOST_PROCESS_WIN32_API)
        DWORD length = ::GetCurrentDirectory(0, NULL);
        TCHAR* buf = new TCHAR[length * sizeof(TCHAR)];
        if (::GetCurrentDirectory(length, buf) == 0) {
            delete buf;
            boost::throw_exception
                (system_error
                 ("boost::process::attributes::attributes",
                  "GetCurrentDirectory failed", ::GetLastError()));
        }
        m_work_directory = buf;
        delete buf;
#endif
    } else
        m_work_directory = work_directory;
    BOOST_ASSERT(!m_work_directory.empty());
}

// ------------------------------------------------------------------------

inline
const std::string&
attributes::get_work_directory(void)
    const
{
    return m_work_directory;
}

// ------------------------------------------------------------------------

inline
void
attributes::setup(void)
    const
{
    if (m_work_directory.length() > 0) {
#if defined(BOOST_PROCESS_POSIX_API)
        if (chdir(m_work_directory.c_str()) == -1)
            boost::throw_exception
                (system_error("boost::process::attributes::setup",
                              "chdir(2) failed", errno));
#elif defined(BOOST_PROCESS_WIN32_API)
        if (::SetCurrentDirectory(TEXT(m_work_directory.c_str())) == 0)
            boost::throw_exception
                (system_error
                 ("boost::process::attributes::attributes",
                  "SetCurrentDirectory failed", ::GetLastError()));
#endif
    }
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_ATTRIBUTES_HPP)
