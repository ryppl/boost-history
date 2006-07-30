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
//! \file boost/process/basic_posix_child.hpp
//!
//! Includes the declaration of the basic_posix_child template.
//!

#if !defined(BOOST_PROCESS_BASIC_POSIX_CHILD_HPP)
/** \cond */
#define BOOST_PROCESS_BASIC_POSIX_CHILD_HPP
/** \endcond */

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

//!
//! \brief POSIX implementation of the Child concept.
//!
//! The basic_posix_child template implements the Child concept in a POSIX
//! operating system.  This class is templatized on a type that specifies
//! the class implementing the Command_Line concept, used to represent the
//! command line used to launch the process.
//!
//! A POSIX child differs from a regular child (represented by a
//! basic_child object) in that it supports more than three communication
//! channels with its parent.  These channels are identified by regular
//! file descriptors (integers).
//!
template< class Command_Line >
class basic_posix_child :
    public basic_child< Command_Line >
{
public:
    //!
    //! \brief Process handle type inherited from basic_process.
    //!
    typedef typename basic_child< Command_Line >::handle_type handle_type;

    //!
    //! \brief Gets a reference to the child's input stream \a desc.
    //!
    //! Returns a reference to a postream object that represents one of
    //! the multiple input communication channels with the child process.
    //! The channel is identified by \a desc as seen from the child's
    //! point of view.  The parent can use the returned stream to send
    //! data to the child.
    //!
    //! Giving this function the STDIN_FILENO constant (defined in
    //! cstdlib) is a synonym for the get_stdin() call inherited from
    //! basic_child.
    //!
    postream& get_input(int desc) const;

    //!
    //! \brief Gets a reference to the child's output stream \a desc.
    //!
    //! Returns a reference to a pistream object that represents one of
    //! the multiple output communication channels with the child process.
    //! The channel is identified by \a desc as seen from the child's
    //! point of view.  The parent can use the returned stream to retrieve
    //! data from the child.
    //!
    //! Giving this function the STDOUT_FILENO or STDERR_FILENO constants
    //! (both defined in cstdlib) are synonyms for the get_stdout() and
    //! get_stderr() calls inherited from basic_child, respectively.
    //!
    pistream& get_output(int desc) const;

private:
    //!
    //! \brief Maps child's file descriptors to postream objects.
    //!
    typedef std::map< int, boost::shared_ptr< postream > > input_map;

    //!
    //! \brief Maps child's file descriptors to pistream objects.
    //!
    typedef std::map< int, boost::shared_ptr< pistream > > output_map;

    //!
    //! \brief Contains all relationships between child's input file
    //!        descriptors and their corresponding postream objects.
    //!
    input_map m_input_map;

    //!
    //! \brief Contains all relationships between child's output file
    //!        descriptors and their corresponding pistream objects.
    //!
    output_map m_output_map;

protected:
    friend class posix_launcher;

    //!
    //! \brief Maps child's file descriptors to pipe objects.
    //!
    typedef std::map< int, detail::pipe > pipe_map;

    //!
    //! \brief Searches for a file handle in a %pipe map.
    //!
    //! This helper function searches for the \a desc file descriptor
    //! in the \a pm %pipe map and returns its corresponding file handle.
    //! If \a out is true, it is assummed that the file handle corresponds
    //! to a output stream, so the pipe's read end is returned; otherwise
    //! the write end is returned.
    //!
    static detail::file_handle get_handle(pipe_map& pm, int desc, bool out);

    //!
    //! \brief Constructs a new POSIX child object representing a just
    //!        spawned child process.
    //!
    //! Creates a new child object that represents the just spawned process
    //! \a h.  The \a cl command line is assigned to this process and it
    //! is assummed that its contents are those that were used to launch
    //! the program's instance.
    //!
    //! The \a inpipes and \a outpipes maps contain the pipes used to
    //! handle the redirections of the child process.  If the launcher was
    //! asked to redirect any of the three standard flows, their pipes
    //! must be present in these maps.
    //!
    //! This constructor is protected because the library user has no
    //! business in creating representations of live processes himself;
    //! the library takes care of that in all cases.
    //!
    basic_posix_child(handle_type h,
                const Command_Line& cl,
                pipe_map& inpipes,
                pipe_map& outpipes);
};

// ------------------------------------------------------------------------

template< class Command_Line >
inline
basic_posix_child< Command_Line >::basic_posix_child
    (handle_type h,
     const Command_Line& cl,
     pipe_map& inpipes,
     pipe_map& outpipes) :
    basic_child< Command_Line >
        (h, cl,
         basic_posix_child< Command_Line >::get_handle(inpipes, STDIN_FILENO,
                                                       false),
         basic_posix_child< Command_Line >::get_handle(outpipes, STDOUT_FILENO,
                                                       true),
         basic_posix_child< Command_Line >::get_handle(outpipes, STDERR_FILENO,
                                                       true))
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

template< class Command_Line >
inline
detail::file_handle
basic_posix_child< Command_Line >::get_handle(pipe_map& pm, int desc,
                                              bool out)
{
    detail::file_handle fh;

    typename pipe_map::iterator iter = pm.find(desc);
    if (iter != pm.end()) {
        if (out)
            fh = (*iter).second.rend();
        else
            fh = (*iter).second.wend();
        BOOST_ASSERT(fh.is_valid());
        pm.erase(iter);
    }

    return fh;
}

// ------------------------------------------------------------------------

template< class Command_Line >
inline
postream&
basic_posix_child< Command_Line >::get_input(int desc)
    const
{
    if (desc == STDIN_FILENO)
        return basic_posix_child< Command_Line >::get_stdin();
    else {
        input_map::const_iterator iter = m_input_map.find(desc);
        BOOST_ASSERT(iter != m_input_map.end());
        return *((*iter).second);
    }
}

// ------------------------------------------------------------------------

template< class Command_Line >
inline
pistream&
basic_posix_child< Command_Line >::get_output(int desc)
    const
{
    if (desc == STDOUT_FILENO)
        return basic_posix_child< Command_Line >::get_stdout();
    else if (desc == STDERR_FILENO)
        return basic_posix_child< Command_Line >::get_stderr();
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
