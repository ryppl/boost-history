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

#include <istream>
#include <map>
#include <ostream>
#include <set>

#include <boost/assert.hpp>
#include <boost/process/basic_process.hpp>
#include <boost/process/detail/pipe.hpp>
#include <boost/process/detail/systembuf.hpp>
#include <boost/process/status.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

// XXX This should be inside basic_child.
typedef std::map< desc_t, detail::shared_pipe > pipe_map;

template< class Attributes >
class basic_child : 
    public basic_process< Attributes >
{
public:
    status wait(void);

    id_t get_id(void) const { return basic_process< Attributes >::get_id(); }

    // XXX Should these two be constant?
    boost::weak_ptr< std::ostream > get_input(desc_t desc);
    boost::weak_ptr< std::istream > get_output(desc_t desc);

private:
    typedef std::set< boost::shared_ptr< detail::systembuf > > buf_set;
    typedef std::map< desc_t, boost::shared_ptr< std::ostream > > input_map;
    typedef std::map< desc_t, boost::shared_ptr< std::istream > > output_map;

    buf_set m_buf_set;
    pipe_map m_input_pipes;
    pipe_map m_output_pipes;
    input_map m_input_map;
    output_map m_output_map;

    friend class launcher;
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
    basic_process< Attributes >(id, attrs),
    m_input_pipes(inpipes),
    m_output_pipes(outpipes)
{
    for (pipe_map::const_iterator iter = m_input_pipes.begin();
         iter != m_input_pipes.end(); iter++) {
        detail::systembuf* buf =
            new detail::systembuf((*iter).second->get_write_end());
        m_buf_set.insert(boost::shared_ptr< detail::systembuf >(buf));

        std::ostream* st = new std::ostream(buf);
        m_input_map.insert
            (input_map::value_type((*iter).first,
                                   boost::shared_ptr< std::ostream >(st)));
    }

    for (pipe_map::const_iterator iter = m_output_pipes.begin();
         iter != m_output_pipes.end(); iter++) {
        detail::systembuf* buf =
            new detail::systembuf((*iter).second->get_read_end());
        m_buf_set.insert(boost::shared_ptr< detail::systembuf >(buf));

        std::istream* st = new std::istream(buf);
        m_output_map.insert
            (output_map::value_type((*iter).first,
                                   boost::shared_ptr< std::istream >(st)));
    }
}

// ------------------------------------------------------------------------

template< class Attributes >
inline
status
basic_child< Attributes >::wait(void)
{
    int s;
    ::waitpid(get_id(), &s, 0);
    return status(s);
}

// ------------------------------------------------------------------------

template< class Attributes >
inline
boost::weak_ptr< std::ostream >
basic_child< Attributes >::get_input(desc_t desc)
{
    input_map::const_iterator iter = m_input_map.find(desc);
    BOOST_ASSERT(iter != m_input_map.end());
    return boost::weak_ptr< std::ostream >((*iter).second);
}

// ------------------------------------------------------------------------

template< class Attributes >
inline
boost::weak_ptr< std::istream >
basic_child< Attributes >::get_output(desc_t desc)
{
    output_map::const_iterator iter = m_output_map.find(desc);
    BOOST_ASSERT(iter != m_output_map.end());
    return boost::weak_ptr< std::istream >((*iter).second);
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_BASIC_CHILD_HPP)
