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
//! \file boost/process/basic_children.hpp
//!
//! Includes the declaration of the basic_children template.
//!

#if !defined(BOOST_PROCESS_BASIC_CHILDREN_HPP)
/** \cond */
#define BOOST_PROCESS_BASIC_CHILDREN_HPP
/** \endcond */

#include <vector>

#include <boost/assert.hpp>
#include <boost/process/basic_child.hpp>
#include <boost/process/pistream.hpp>
#include <boost/process/postream.hpp>
#include <boost/process/status.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

//!
//! \brief Representation of a pipelined group of child processes.
//!
//! Represents a group of child process whose standard data streams are
//! connected to form a pipeline.  This higher order structure allows for
//! easy access to the pipeline endpoints and termination synchronization.
//!
template< class Command_Line >
class basic_children :
    public std::vector< basic_child< Command_Line > >
{
public:
    //!
    //! \brief Returns the pipeline's input stream.
    //!
    //! Returns the pipeline's input stream, which is connected to the
    //! stdin of the first process in the chain.
    //!
    //! \pre The pipeline launcher (basic_pipeline) must have configured
    //!      the first process' stdin to the redirect_stream behavior.
    //!
    postream& get_stdin(void) const;

    //!
    //! \brief Returns the pipeline's output stream.
    //!
    //! Returns the pipeline's output stream, which is connected to the
    //! stdout of the last process in the chain.
    //!
    //! \pre The pipeline launcher (basic_pipeline) must have configured
    //!      the last process' stdout to the redirect_stream behavior.
    //!
    pistream& get_stdout(void) const;

    //!
    //! \brief Returns the pipeline's error stream.
    //!
    //! Returns the pipeline's error stream, which is connected to the
    //! stderr of the last process in the chain.
    //!
    //! \pre The pipeline launcher (basic_pipeline) must have configured
    //!      the last process' stderr to the redirect_stream behavior.
    //!
    pistream& get_stderr(void) const;

    //!
    //! \brief Waits for %children finalization.
    //!
    //! Waits until all the processes in the pipeline have finalized
    //! execution.
    //!
    //! \return The exit status of the first failed process or, if all
    //!         was successful, the exit status of the last process.
    //!
    status wait(void);
};

// ------------------------------------------------------------------------

template< class Command_Line >
postream&
basic_children< Command_Line >::get_stdin(void)
    const
{
    BOOST_ASSERT(basic_children< Command_Line >::size() >= 2);

    return (*this)[0].get_stdin();
}

// ------------------------------------------------------------------------

template< class Command_Line >
pistream&
basic_children< Command_Line >::get_stdout(void)
    const
{
    BOOST_ASSERT(basic_children< Command_Line >::size() >= 2);

    return (*this)[basic_children< Command_Line >::size() - 1].get_stdout();
}

// ------------------------------------------------------------------------

template< class Command_Line >
pistream&
basic_children< Command_Line >::get_stderr(void)
    const
{
    BOOST_ASSERT(basic_children< Command_Line >::size() >= 2);

    return (*this)[basic_children< Command_Line >::size() - 1].get_stderr();
}

// ------------------------------------------------------------------------

template< class Command_Line >
status
basic_children< Command_Line >::wait(void)
{
    BOOST_ASSERT(basic_children< Command_Line >::size() >= 2);

    status s(0), s2(0);
    bool update = true;

    typename basic_children< Command_Line >::iterator biter =
        basic_children< Command_Line >::begin();
    typename basic_children< Command_Line >::iterator eiter =
        basic_children< Command_Line >::end();

    for (typename basic_children< Command_Line >::iterator iter = biter;
         iter != eiter; iter++) {
        s2 = (*iter).wait();
        if (!s2.exited() || s2.exit_status() != EXIT_SUCCESS) {
            if (update) {
                s = s2;
                update = false;
            }
        }
    }

    return update ? s2 : s;
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_BASIC_CHILDREN_HPP)
