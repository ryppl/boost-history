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
//! \file boost/process/basic_child.hpp
//!
//! Includes the declaration of the basic_child template.
//!

#if !defined(BOOST_PROCESS_BASIC_CHILD_HPP)
/** \cond */
#define BOOST_PROCESS_BASIC_CHILD_HPP
/** \endcond */

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_POSIX_API)
extern "C" {
#   include <sys/types.h>
#   include <sys/wait.h>
}
#   include <cerrno>
#   include <boost/process/exceptions.hpp>
#   include <boost/throw_exception.hpp>
#elif defined(BOOST_PROCESS_WIN32_API)
#   include <windows.h>
#else
#   error "Unsupported platform."
#endif

#include <map>

#include <boost/assert.hpp>
#include <boost/process/basic_process.hpp>
#include <boost/process/detail/pipe.hpp>
#include <boost/process/pistream.hpp>
#include <boost/process/postream.hpp>
#include <boost/process/status.hpp>
#include <boost/shared_ptr.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

//!
//! \brief Generic implementation of the Child concept.
//!
//! The basic_child template implements the Child concept in an operating
//! system agnostic way.  This class is templatized on a type that
//! specifies the class implementing the Command_Line concept, used to
//! represent the command line used to launch the process.
//!
template< class Command_Line >
class basic_child :
    public basic_process< Command_Line >
{
public:
    //!
    //! \brief Process handle type inherited from basic_process.
    //!
    typedef typename basic_process< Command_Line >::handle_type handle_type;

    //!
    //! \brief Gets a reference to the child's standard input stream.
    //!
    //! Returns a reference to a postream object that represents the
    //! standard input communication channel with the child process.
    //!
    postream& get_stdin(void) const;

    //!
    //! \brief Gets a reference to the child's standard output stream.
    //!
    //! Returns a reference to a pistream object that represents the
    //! standard output communication channel with the child process.
    //!
    pistream& get_stdout(void) const;

    //!
    //! \brief Gets a reference to the child's standard error stream.
    //!
    //! Returns a reference to a pistream object that represents the
    //! standard error communication channel with the child process.
    //!
    pistream& get_stderr(void) const;

    //!
    //! \brief Blocks and waits for the child process to terminate.
    //!
    //! Blocks (if necessary) until the child process terminates execution
    //! and returns a status object that represents the finalization
    //! condition.
    //!
    //! The child process object ceases to be valid after this call.
    //!
    status wait(void);

private:
    //!
    //! \brief The standard input stream attached to the child process.
    //!
    //! This postream object holds the communication channel with the
    //! child's process standard input.  It is stored in a pointer because
    //! this field is only valid when the user requested to redirect this
    //! data stream.
    //!
    boost::shared_ptr< postream > m_sstdin;

    //!
    //! \brief The standard output stream attached to the child process.
    //!
    //! This postream object holds the communication channel with the
    //! child's process standard output.  It is stored in a pointer because
    //! this field is only valid when the user requested to redirect this
    //! data stream.
    //!
    boost::shared_ptr< pistream > m_sstdout;

    //!
    //! \brief The standard error stream attached to the child process.
    //!
    //! This postream object holds the communication channel with the
    //! child's process standard error.  It is stored in a pointer because
    //! this field is only valid when the user requested to redirect this
    //! data stream.
    //!
    boost::shared_ptr< pistream > m_sstderr;

protected:
    friend class launcher;

    //!
    //! \brief Constructs a new child object representing a just spawned
    //!        child process.
    //!
    //! Creates a new child object that represents the just spawned process
    //! \a h.  The \a cl command line is assigned to this process and it
    //! is assummed that its contents are those that were used to launch
    //! the program's instance.
    //!
    //! The \a fhstdin, \a fhstdout and \a fhstderr file handles represent
    //! the parent's handles used to communicate with the corresponding
    //! data streams.  They needn't be valid but their availability must
    //! match the redirections configured by the launcher that spawned this
    //! process.
    //!
    //! This constructor is protected because the library user has no
    //! business in creating representations of live processes himself;
    //! the library takes care of that in all cases.
    //!
    basic_child(handle_type h,
                const Command_Line& cl,
                detail::file_handle fhstdin,
                detail::file_handle fhstdout,
                detail::file_handle fhstderr);
};

// ------------------------------------------------------------------------

template< class Command_Line >
inline
basic_child< Command_Line >::basic_child
    (handle_type h,
     const Command_Line& cl,
     detail::file_handle fhstdin,
     detail::file_handle fhstdout,
     detail::file_handle fhstderr) :
    basic_process< Command_Line >(h, cl)
{
    if (fhstdin.is_valid())
        m_sstdin.reset(new postream(fhstdin));
    if (fhstdout.is_valid())
        m_sstdout.reset(new pistream(fhstdout));
    if (fhstderr.is_valid())
        m_sstderr.reset(new pistream(fhstderr));
}

// ------------------------------------------------------------------------

template< class Command_Line >
inline
postream&
basic_child< Command_Line >::get_stdin(void)
    const
{
    BOOST_ASSERT(m_sstdin);
    return *m_sstdin;
}

// ------------------------------------------------------------------------

template< class Command_Line >
inline
pistream&
basic_child< Command_Line >::get_stdout(void)
    const
{
    BOOST_ASSERT(m_sstdout);
    return *m_sstdout;
}

// ------------------------------------------------------------------------

template< class Command_Line >
inline
pistream&
basic_child< Command_Line >::get_stderr(void)
    const
{
    BOOST_ASSERT(m_sstderr);
    return *m_sstderr;
}

// ------------------------------------------------------------------------

template< class Command_Line >
inline
status
basic_child< Command_Line >::wait(void)
{
    handle_type h = basic_child< Command_Line >::get_handle();

#if defined(BOOST_PROCESS_POSIX_API)
    int s;
    if (::waitpid(h, &s, 0) == -1)
        boost::throw_exception
            (system_error("boost::process::basic_child::wait",
                          "waitpid(2) failed", errno));
    return status(s);
#elif defined(BOOST_PROCESS_WIN32_API)
    DWORD code;
    // XXX This loop should go away in favour of a passive wait.
    do {
        ::GetExitCodeProcess(h, &code);
        ::Sleep(500);
    } while (code == STILL_ACTIVE);
    ::WaitForSingleObject(h, 0);
    return status(code);
#endif
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_BASIC_CHILD_HPP)
