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
#include <boost/optional.hpp>
#include <boost/process/basic_process.hpp>
#include <boost/process/detail/pipe.hpp>
#include <boost/process/pistream.hpp>
#include <boost/process/postream.hpp>
#include <boost/process/status.hpp>
#include <boost/shared_ptr.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

template< class Attributes >
class basic_child : 
    public basic_process< Attributes >
{
public:
    typedef typename basic_process< Attributes >::handle_type handle_type;

    status wait(void);

    postream& get_stdin(void) const;
    pistream& get_stdout(void) const;
    pistream& get_stderr(void) const;

private:
    boost::optional< detail::shared_pipe > m_pstdin;
    boost::optional< detail::shared_pipe > m_pstdout;
    boost::optional< detail::shared_pipe > m_pstderr;

    boost::shared_ptr< postream > m_sstdin;
    boost::shared_ptr< pistream > m_sstdout;
    boost::shared_ptr< pistream > m_sstderr;

    friend class launcher;
    basic_child(const handle_type& h,
                const Attributes& attrs,
                boost::optional< detail::shared_pipe > m_pstdin,
                boost::optional< detail::shared_pipe > m_pstdout,
                boost::optional< detail::shared_pipe > m_pstderr);
};

// ------------------------------------------------------------------------

template< class Attributes >
inline
basic_child< Attributes >::basic_child
    (const handle_type& h,
     const Attributes& attrs,
     boost::optional< detail::shared_pipe > m_pstdin,
     boost::optional< detail::shared_pipe > m_pstdout,
     boost::optional< detail::shared_pipe > m_pstderr) :
    basic_process< Attributes >(h, attrs)
{
    if (m_pstdin)
        m_sstdin.reset(new postream(*m_pstdin));
    if (m_pstdout)
        m_sstdout.reset(new pistream(*m_pstdout));
    if (m_pstderr)
        m_sstderr.reset(new pistream(*m_pstderr));
}

// ------------------------------------------------------------------------

template< class Attributes >
inline
postream&
basic_child< Attributes >::get_stdin(void)
    const
{
    BOOST_ASSERT(m_sstdin);
    return *m_sstdin;
}

// ------------------------------------------------------------------------

template< class Attributes >
inline
pistream&
basic_child< Attributes >::get_stdout(void)
    const
{
    BOOST_ASSERT(m_sstdout);
    return *m_sstdout;
}

// ------------------------------------------------------------------------

template< class Attributes >
inline
pistream&
basic_child< Attributes >::get_stderr(void)
    const
{
    BOOST_ASSERT(m_sstderr);
    return *m_sstderr;
}

// ------------------------------------------------------------------------

template< class Attributes >
inline
status
basic_child< Attributes >::wait(void)
{
#if defined(BOOST_PROCESS_POSIX_API)
    int s;
    if (::waitpid(basic_child< Attributes >::get_handle(), &s, 0) == -1)
        boost::throw_exception
            (system_error("boost::process::basic_child::wait",
                          "waitpid(2) failed", errno));
    return status(s);
#endif
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_BASIC_CHILD_HPP)
