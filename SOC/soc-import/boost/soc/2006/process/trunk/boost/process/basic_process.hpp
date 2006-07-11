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

#include <boost/process/types.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

template< class Attributes >
class basic_process
{
    id_t m_id;
    Attributes m_attributes;

protected:
    basic_process(const id_t& id, const Attributes& a);

public:
    const Attributes& get_attributes(void) const;
    id_t get_id(void) const;
};

// ------------------------------------------------------------------------

template< class Attributes >
inline
basic_process< Attributes >::basic_process(const id_t& id,
                                           const Attributes& a) :
    m_id(id),
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
id_t
basic_process< Attributes >::get_id(void)
    const
{
    return m_id;
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_BASIC_PROCESS_HPP)
