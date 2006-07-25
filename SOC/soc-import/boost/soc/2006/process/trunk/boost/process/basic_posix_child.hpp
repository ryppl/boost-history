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

#if !defined(BOOST_PROCESS_BASIC_POSIX_CHILD_HPP)
#define BOOST_PROCESS_BASIC_POSIX_CHILD_HPP

#include <boost/process/config.hpp>

#if !defined(BOOST_PROCESS_POSIX_API)
#   error "Unsupported platform."
#endif

#include <cstdlib>
#include <map>

#include <boost/process/basic_child.hpp>
#include <boost/process/detail/pipe.hpp>
#include <boost/process/pistream.hpp>
#include <boost/process/postream.hpp>
#include <boost/shared_ptr.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

template< class Command_Line, class Attributes >
class basic_posix_child :
    public basic_child< Command_Line, Attributes >
{
public:
    typedef typename basic_child< Command_Line, Attributes >::handle_type
        handle_type;

    postream& get_input(int desc) const;
    pistream& get_output(int desc) const;

private:
    typedef std::map< int, boost::shared_ptr< postream > > input_map;
    typedef std::map< int, boost::shared_ptr< pistream > > output_map;

    input_map m_input_map;
    output_map m_output_map;

protected:
    friend class posix_launcher;
    typedef std::map< int, detail::pipe > pipe_map;
    basic_posix_child(handle_type h,
                const Command_Line& cl,
                const Attributes& attrs,
                detail::file_handle& fhstdin,
                detail::file_handle& fhstdout,
                detail::file_handle& fhstderr,
                pipe_map& inpipes,
                pipe_map& outpipes);
};

// ------------------------------------------------------------------------

template< class Command_Line, class Attributes >
inline
basic_posix_child< Command_Line, Attributes >::basic_posix_child
    (handle_type h,
     const Command_Line& cl,
     const Attributes& attrs,
     detail::file_handle& fhstdin,
     detail::file_handle& fhstdout,
     detail::file_handle& fhstderr,
     pipe_map& inpipes,
     pipe_map& outpipes) :
    basic_child< Command_Line, Attributes >(h, cl, attrs, fhstdin, fhstdout,
                                            fhstderr)
{
    for (pipe_map::iterator iter = inpipes.begin();
         iter != inpipes.end(); iter++) {
        BOOST_ASSERT((*iter).second.wend().is_valid());
        boost::shared_ptr< postream > st(new postream((*iter).second.wend()));
        m_input_map.insert(input_map::value_type((*iter).first, st));
    }

    for (pipe_map::iterator iter = outpipes.begin();
         iter != outpipes.end(); iter++) {
        BOOST_ASSERT((*iter).second.rend().is_valid());
        boost::shared_ptr< pistream > st(new pistream((*iter).second.rend()));
        m_output_map.insert(output_map::value_type((*iter).first, st));
    }
}

// ------------------------------------------------------------------------

template< class Command_Line, class Attributes >
inline
postream&
basic_posix_child< Command_Line, Attributes >::get_input(int desc)
    const
{
    if (desc == STDIN_FILENO)
        return basic_posix_child< Command_Line, Attributes >::get_stdin();
    else {
        input_map::const_iterator iter = m_input_map.find(desc);
        BOOST_ASSERT(iter != m_input_map.end());
        return *((*iter).second);
    }
}

// ------------------------------------------------------------------------

template< class Command_Line, class Attributes >
inline
pistream&
basic_posix_child< Command_Line, Attributes >::get_output(int desc)
    const
{
    if (desc == STDOUT_FILENO)
        return basic_posix_child< Command_Line, Attributes >::get_stdout();
    else if (desc == STDERR_FILENO)
        return basic_posix_child< Command_Line, Attributes >::get_stderr();
    else {
        output_map::const_iterator iter = m_output_map.find(desc);
        BOOST_ASSERT(iter != m_output_map.end());
        return *((*iter).second);
    }
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_BASIC_POSIX_CHILD_HPP)
