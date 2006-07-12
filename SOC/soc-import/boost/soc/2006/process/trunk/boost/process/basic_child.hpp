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

#include <map>

#include <boost/assert.hpp>
#include <boost/process/basic_process.hpp>
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
    status wait(void);

    // XXX Should these two be constant?
    postream& get_input(desc_t desc);
    pistream& get_output(desc_t desc);

private:
    typedef std::map< desc_t, boost::shared_ptr< postream > > input_map;
    typedef std::map< desc_t, boost::shared_ptr< pistream > > output_map;

    input_map m_input_map;
    output_map m_output_map;

    friend class launcher;
    typedef std::map< desc_t, detail::shared_pipe > pipe_map;
    basic_child(id_t id, const Attributes& attrs,
                const pipe_map& inpipes, const pipe_map& outpipes);
};

// ------------------------------------------------------------------------

template< class Attributes >
inline
basic_child< Attributes >::basic_child(id_t id,
                                       const Attributes& attrs,
                                       const pipe_map& inpipes,
                                       const pipe_map& outpipes) :
    basic_process< Attributes >(id, attrs)
{
    for (pipe_map::const_iterator iter = inpipes.begin();
         iter != inpipes.end(); iter++) {
        boost::shared_ptr< postream > st(new postream((*iter).second));
        m_input_map.insert(input_map::value_type((*iter).first, st));
    }

    for (pipe_map::const_iterator iter = outpipes.begin();
         iter != outpipes.end(); iter++) {
        boost::shared_ptr< pistream > st(new pistream((*iter).second));
        m_output_map.insert(output_map::value_type((*iter).first, st));
    }
}

// ------------------------------------------------------------------------

template< class Attributes >
inline
status
basic_child< Attributes >::wait(void)
{
    int s;
    ::waitpid(basic_child< Attributes >::get_id(), &s, 0);
    return status(s);
}

// ------------------------------------------------------------------------

template< class Attributes >
inline
postream&
basic_child< Attributes >::get_input(desc_t desc)
{
    input_map::const_iterator iter = m_input_map.find(desc);
    BOOST_ASSERT(iter != m_input_map.end());
    return *((*iter).second);
}

// ------------------------------------------------------------------------

template< class Attributes >
inline
pistream&
basic_child< Attributes >::get_output(desc_t desc)
{
    output_map::const_iterator iter = m_output_map.find(desc);
    BOOST_ASSERT(iter != m_output_map.end());
    return *((*iter).second);
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_BASIC_CHILD_HPP)
