//
// Boost.Process
// Implementation of the Process concept.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//
 
#if !defined(BOOST_PROCESS_BASIC_PROCESS_HPP)
#define BOOST_PROCESS_BASIC_PROCESS_HPP

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_WIN32_API)
extern "C" {
#   include <windows.h>
}
#elif defined(BOOST_PROCESS_POSIX_API)
#else
#   error "Unsupported platform."
#endif

namespace boost {
namespace process {

// ------------------------------------------------------------------------

template< class Attributes >
class basic_process
{
public:
#if defined(BOOST_PROCESS_WIN32_API)
    typedef HANDLE handle_type;
#elif defined(BOOST_PROCESS_POSIX_API)
    typedef pid_t handle_type;
#endif

    const Attributes& get_attributes(void) const;
    handle_type get_handle(void) const;

protected:
    basic_process(const handle_type& h, const Attributes& a);

private:
    handle_type m_handle;
    Attributes m_attributes;
};

// ------------------------------------------------------------------------

template< class Attributes >
inline
basic_process< Attributes >::basic_process(const handle_type& h,
                                           const Attributes& a) :
    m_handle(h),
    m_attributes(a)
{
}

// ------------------------------------------------------------------------

template< class Attributes >
inline
const Attributes&
basic_process< Attributes >::get_attributes(void)
    const
{
    return m_attributes;
}

// ------------------------------------------------------------------------

template< class Attributes >
inline
typename basic_process< Attributes >::handle_type
basic_process< Attributes >::get_handle(void)
    const
{
    return m_handle;
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_BASIC_PROCESS_HPP)
