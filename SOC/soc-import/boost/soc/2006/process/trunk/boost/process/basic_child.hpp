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
    typedef typename basic_process< Attributes >::handle_type handle_type;

    status wait(void);

    postream& get_input(int desc) const;
    pistream& get_output(int desc) const;

private:
    typedef std::map< int, boost::shared_ptr< postream > > input_map;
    typedef std::map< int, boost::shared_ptr< pistream > > output_map;

    input_map m_input_map;
    output_map m_output_map;

    friend class launcher;
    typedef std::map< int, detail::shared_pipe > pipe_map;
    basic_child(const handle_type& h, const Attributes& attrs,
                const pipe_map& inpipes, const pipe_map& outpipes);
};

// ------------------------------------------------------------------------

template< class Attributes >
inline
basic_child< Attributes >::basic_child(const handle_type& h,
                                       const Attributes& attrs,
                                       const pipe_map& inpipes,
                                       const pipe_map& outpipes) :
    basic_process< Attributes >(h, attrs)
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
postream&
basic_child< Attributes >::get_input(int desc)
    const
{
    input_map::const_iterator iter = m_input_map.find(desc);
    BOOST_ASSERT(iter != m_input_map.end());
    return *((*iter).second);
}

// ------------------------------------------------------------------------

template< class Attributes >
inline
pistream&
basic_child< Attributes >::get_output(int desc)
    const
{
    output_map::const_iterator iter = m_output_map.find(desc);
    BOOST_ASSERT(iter != m_output_map.end());
    return *((*iter).second);
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#if defined(BOOST_PROCESS_WIN32_API)
#   include <boost/process/basic_child_win32.hpp>
#else
#   include <boost/process/basic_child_posix.hpp>
#endif

#endif // !defined(BOOST_PROCESS_BASIC_CHILD_HPP)
