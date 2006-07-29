//
// Boost.Process
// Implementation of the Child concept.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#if !defined(BOOST_PROCESS_BASIC_CHILD_HPP)
#define BOOST_PROCESS_BASIC_CHILD_HPP

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

template< class Command_Line >
class basic_child :
    public basic_process< Command_Line >
{
public:
    typedef typename basic_process< Command_Line >::handle_type handle_type;

    status wait(void);

    postream& get_stdin(void) const;
    pistream& get_stdout(void) const;
    pistream& get_stderr(void) const;

private:
    boost::shared_ptr< postream > m_sstdin;
    boost::shared_ptr< pistream > m_sstdout;
    boost::shared_ptr< pistream > m_sstderr;

protected:
    friend class launcher;
    basic_child(handle_type h,
                const Command_Line& cl,
                detail::file_handle& fhstdin,
                detail::file_handle& fhstdout,
                detail::file_handle& fhstderr);
};

// ------------------------------------------------------------------------

template< class Command_Line >
inline
basic_child< Command_Line >::basic_child
    (handle_type h, const Command_Line& cl,
     detail::file_handle& fhstdin, detail::file_handle& fhstdout,
     detail::file_handle& fhstderr) :
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
