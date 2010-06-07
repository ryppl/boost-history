//
// Boost.Process
// ~~~~~~~~~~~~~
//
// Copyright (c) 2006, 2007 Julio M. Merino Vidal
// Copyright (c) 2008, 2009 Boris Schaeling
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
#   include <boost/process/detail/pipe.hpp>
#   include <boost/process/detail/posix_ops.hpp>
#   include <boost/process/detail/stream_info.hpp>
#   include <map>
#   include <unistd.h>
#elif defined(BOOST_WINDOWS_API)
#   include <windows.h>
#else
#   error "Unsupported platform." 
#endif

#include <boost/process/process.hpp>
#include <boost/process/pistream.hpp>
#include <boost/process/postream.hpp>
#include <boost/process/detail/file_handle.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/assert.hpp>
#include <vector>

namespace boost {
namespace process {

/**
 * Generic implementation of the Child concept.
 *
 * The child class implements the Child concept in an operating system
 * agnostic way.
 */
class child : public process
{
public:
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

    /**
     * Creates a new child object that represents the just spawned child
     * process \a id.
     *
     * The \a fhstdin, \a fhstdout and \a fhstderr file handles represent
     * the parent's handles used to communicate with the corresponding
     * data streams. They needn't be valid but their availability must
     * match the redirections configured by the launcher that spawned this
     * process.
     *
     * The \a fhprocess handle represents a handle to the child process.
     * It is only used on Windows as the implementation of wait() needs a
     * process handle.
     */
    child(id_type id, detail::file_handle &fhstdin, detail::file_handle &fhstdout, detail::file_handle &fhstderr, detail::file_handle fhprocess = detail::file_handle())
        : process(id)
#if defined(BOOST_WINDOWS_API) 
        ,process_handle_(fhprocess.release(), ::CloseHandle) 
#endif 
    {
        if (fhstdin.valid())
            stdin_.reset(new postream(fhstdin));
        if (fhstdout.valid())
            stdout_.reset(new pistream(fhstdout));
        if (fhstderr.valid())
            stderr_.reset(new pistream(fhstderr));
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

#if defined(BOOST_WINDOWS_API) 
    /** 
     * Process handle owned by RAII object. 
     */ 
    boost::shared_ptr<void> process_handle_; 
#endif 

#if defined(BOOST_POSIX_API) 
public:
    /**
     * Gets a reference to the child's input stream \a desc.
     *
     * Returns a reference to a postream object that represents one of
     * the multiple input communication channels with the child process.
     * The channel is identified by \a desc as seen from the child's
     * point of view. The parent can use the returned stream to send
     * data to the child.
     *
     * Giving this function the STDIN_FILENO constant (defined in
     * unistd.h) is a synonym for the get_stdin() call inherited from
     * child.
     */
    postream &get_input(int desc) const
    {
        if (desc == STDIN_FILENO)
            return posix_child::get_stdin();
        else
        {
            input_map_t::const_iterator it = input_map_.find(desc);
            BOOST_ASSERT(it != input_map_.end());
            return *it->second;
        }
    }

    /**
     * Gets a reference to the child's output stream \a desc.
     *
     * Returns a reference to a pistream object that represents one of
     * the multiple output communication channels with the child process.
     * The channel is identified by \a desc as seen from the child's
     * point of view. The parent can use the returned stream to retrieve
     * data from the child.
     *
     * Giving this function the STDOUT_FILENO or STDERR_FILENO constants
     * (both defined in unistd.h) are synonyms for the get_stdout() and
     * get_stderr() calls inherited from child, respectively.
     */
    pistream &get_output(int desc) const
    {
        if (desc == STDOUT_FILENO)
            return posix_child::get_stdout();
        else if (desc == STDERR_FILENO)
            return posix_child::get_stderr();
        else
        {
            output_map_t::const_iterator it = output_map_.find(desc);
            BOOST_ASSERT(it != output_map_.end());
            return *it->second;
        }
    }

    /**
     * Creates a new child object on a POSIX platform.
     *
     * The \a infoin and \a infoout maps contain the pipes used to handle
     * the redirections of the child process; at the moment, no other
     * stream_info types are supported. If the launcher was asked to
     * redirect any of the three standard flows, their pipes must be
     * present in these maps.
     */
    child(id_type id, detail::info_map &infoin, detail::info_map &infoout)
        : child(id,
        detail::posix_info_locate_pipe(infoin, STDIN_FILENO, false),
        detail::posix_info_locate_pipe(infoout, STDOUT_FILENO, true),
        detail::posix_info_locate_pipe(infoout, STDERR_FILENO, true))
    {
        for (detail::info_map::iterator it = infoin.begin();
                it != infoin.end(); ++it)
        {
            detail::stream_info &si = it->second;
            if (si.type_ == detail::stream_info::use_pipe)
            {
                BOOST_ASSERT(si.pipe_->wend().valid());
                boost::shared_ptr<postream> st(new postream(si.pipe_->wend()));
                input_map_.insert(input_map_t::value_type(it->first, st));
            }
        }

        for (detail::info_map::iterator it = infoout.begin();
                it != infoout.end(); ++it)
        {
            detail::stream_info &si = it->second;
            if (si.type_ == detail::stream_info::use_pipe)
            {
                BOOST_ASSERT(si.pipe_->rend().valid());
                boost::shared_ptr<pistream> st(new pistream(si.pipe_->rend()));
                output_map_.insert(output_map_t::value_type(it->first, st));
            }
        }
    }

private:
    /**
     * Maps child's file descriptors to postream objects.
     */
    typedef std::map<int, boost::shared_ptr<postream> > input_map_t;

    /**
     * Contains all relationships between child's input file
     * descriptors and their corresponding postream objects.
     */
    input_map_t input_map_;

    /**
     * Maps child's file descriptors to pistream objects.
     */
    typedef std::map<int, boost::shared_ptr<pistream> > output_map_t;

    /**
     * Contains all relationships between child's output file
     * descriptors and their corresponding pistream objects.
     */
    output_map_t output_map_;
#endif 
};

/**
 * Collection of child objects.
 *
 * This convenience type represents a collection of child objects backed
 * by a vector.
 */
typedef std::vector<child> children;

}
}

#endif
