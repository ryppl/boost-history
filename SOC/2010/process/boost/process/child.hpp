//
// Boost.Process
// ~~~~~~~~~~~~~
//
// Copyright (c) 2006, 2007 Julio M. Merino Vidal
// Copyright (c) 2008 Ilya Sokolov, Boris Schaeling
// Copyright (c) 2009 Boris Schaeling
// Copyright (c) 2010 Felipe Tanus, Boris Schaeling
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

/**
 * \file boost/process/child.hpp
 *
 * Includes the declaration of the child class.
 */

#ifndef BOOST_PROCESS_CHILD_HPP
#define BOOST_PROCESS_CHILD_HPP

#include <boost/process/config.hpp>

#if defined(BOOST_POSIX_API)
#elif defined(BOOST_WINDOWS_API)
#   include <windows.h>
#else
#   error "Unsupported platform." 
#endif

#include <boost/process/process.hpp>
#include <boost/process/pid_type.hpp>
#include <boost/process/pistream.hpp>
#include <boost/process/postream.hpp>
#include <boost/process/detail/file_handle.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/assert.hpp>

namespace boost {
namespace process {

/**
 * The child class provides access to a child process.
 */
class child : public process
{
public:
    /**
     * Creates a new child object that represents the just spawned child
     * process \a id.
     *
     * The \a fhstdin, \a fhstdout and \a fhstderr file handles represent
     * the parent's handles used to communicate with the corresponding
     * data streams. They needn't be valid but their availability must
     * match the redirections configured by the launcher that spawned this
     * process.
     */
    child(pid_type id, detail::file_handle fhstdin,
        detail::file_handle fhstdout, detail::file_handle fhstderr)
        : process(id)
    {
        if (fhstdin.valid())
            stdin_.reset(new postream(fhstdin));
        if (fhstdout.valid())
            stdout_.reset(new pistream(fhstdout));
        if (fhstderr.valid())
            stderr_.reset(new pistream(fhstderr));
    }

    /**
     * Gets a reference to the child's standard input stream.
     *
     * Returns a reference to a postream object that represents the
     * standard input communication channel with the child process.
     */
    postream &get_stdin() const
    {
        BOOST_ASSERT(stdin_);
        return *stdin_;
    }

    /**
     * Gets a reference to the child's standard output stream.
     *
     * Returns a reference to a pistream object that represents the
     * standard output communication channel with the child process.
     */
    pistream &get_stdout() const
    {
        BOOST_ASSERT(stdout_);
        return *stdout_;
    }

    /**
     * Gets a reference to the child's standard error stream.
     *
     * Returns a reference to a pistream object that represents the
     * standard error communication channel with the child process.
     */
    pistream &get_stderr() const
    {
        BOOST_ASSERT(stderr_);
        return *stderr_;
    }

private:
    /**
     * The standard input stream attached to the child process.
     *
     * This postream object holds the communication channel with the
     * child's process standard input. It is stored in a pointer because
     * this field is only valid when the user requested to redirect this
     * data stream.
     */
    boost::shared_ptr<postream> stdin_;

    /**
     * The standard output stream attached to the child process.
     *
     * This postream object holds the communication channel with the
     * child's process standard output. It is stored in a pointer because
     * this field is only valid when the user requested to redirect this
     * data stream.
     */
    boost::shared_ptr<pistream> stdout_;

    /**
     * The standard error stream attached to the child process.
     *
     * This postream object holds the communication channel with the
     * child's process standard error. It is stored in a pointer because
     * this field is only valid when the user requested to redirect this
     * data stream.
     */
    boost::shared_ptr<pistream> stderr_;
};

}
}

#endif
