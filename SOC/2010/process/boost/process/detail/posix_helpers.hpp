//
// Boost.Process
// ~~~~~~~~~~~~~
//
// Copyright (c) 2006, 2007 Julio M. Merino Vidal
// Copyright (c) 2008, 2009 Boris Schaeling
// Copyright (c) 2010 Boris Schaeling, Felipe Tanus
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

/**
 * \file boost/process/detail/posix_helper.hpp
 *
 * Includes the declaration of helper functions for the operations in a posix
 * system. It's for internal purposes.
 *
 */


#if defined(BOOST_POSIX_API)
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#endif
#include <map>
#include <string.h>
#include <boost/optional.hpp>
#include <boost/process/stream_behavior.hpp>
#include <boost/process/detail/stream_detail.hpp>


#ifndef BOOST_PROCESS_POSIX_HELPERS_HPP
#define BOOST_PROCESS_POSIX_HELPERS_HPP
namespace boost
{
namespace process
{
namespace detail
{



/*
 * This is the declaration of configure_stream function.
 * This function, given an stream_detail inherit from the current process
 * father, configure the stream.
 *
 * Note that this function is meant to be called on a brand-new child.
 *
 */
inline configure_child_posix_stream(stream_detail &s)
{

    switch(s.behavior)
    {
    case dummy:
    {

        std::string null_file;
        if(s.stream_type == stdin_type)
            null_file = "/dev/null";
        else
            null_file = "/dev/zero";

        int fd;
        if(s.stream_type == stdin_type)
            fd = ::open(null_file.c_str(), O_RDONLY);
        else
            fd = ::open(null_file.c_str(), O_WRONLY);

        if (fd == -1)
            boost::throw_exception(boost::system::system_error(
                        boost::system::error_code(errno,
                            boost::system::get_system_category()),
                        "boost::process::detail::setup_input: open(2) of "
                        + s.object.file_ + " failed"));

        s.object.handle_ = file_handle(fd);
        s.object.handle_.posix_remap(s.stream_handle);
        s.object.handle_.release();

        break;
    }

    case closed:
        ::close(s.stream_handle);
        break;

    case inherit:
        //do nothing since is default?
        break;

    case capture:
        if(s.stream_type == stdin_type)
        {
            s.object.pipe_.wend().close();
            s.object.pipe_.rend().posix_remap(s.stream_handle);
        }
        else
        {
            s.object.pipe_.rend().close();
            s.object.pipe_.wend().posix_remap(s.stream_handle);
        }

        break;

    default:
        BOOST_ASSERT(false);
    }

}


/**
 * Converts an environment to a char** table as used by execve().
 *
 * Converts the environment's contents to the format used by the
 * execve() system call. The returned char** array is allocated
 * in dynamic memory; the caller must free it when not used any
 * more. Each entry is also allocated in dynamic memory and is a
 * NULL-terminated string of the form var=value; these must also be
 * released by the caller.
 *
 * \return A dynamically allocated char** array that represents
 *         the environment's content. Each array entry is a
 *         NULL-terminated string of the form var=value.
 */
inline char ** environment_to_envp( environment_t env)
{
    char **envp = new char*[env.size() + 1];

    unsigned int i = 0;
    for (environment_t::const_iterator it = env.begin(); it != env.end(); ++it)
    {
        std::string s = (*it).first + "=" + (*it).second;
        envp[i] = new char[s.size() + 1];
        ::strncpy(envp[i], s.c_str(), s.size() + 1);
        ++i;
    }
    envp[i] = 0;
    return envp;
}

/**
 * Converts the command line to an array of C strings.
 *
 * Converts the command line's list of arguments to the format expected
 * by the \a argv parameter in the POSIX execve() system call.
 *
 * This operation is only available on POSIX systems.
 *
 * \return The first argument of the pair is an integer that indicates
 *         how many strings are stored in the second argument. The
 *         second argument is a NULL-terminated, dynamically allocated
 *         array of dynamically allocated strings holding the arguments
 *         to the executable. The caller is responsible of freeing them.
 */
template <class Arguments>
inline std::pair<std::size_t, char**> collection_to_posix_argv(const Arguments
        &args)
{
    std::size_t nargs = args.size();
    BOOST_ASSERT(nargs >= 0);

    char **argv = new char*[nargs + 1];
    typename Arguments::size_type i = 0;
    for (typename Arguments::const_iterator it = args.begin();
            it != args.end(); ++it)
    {
        argv[i] = new char[it->size() + 1];
        ::strncpy(argv[i], it->c_str(), it->size() + 1);
        ++i;
    }
    argv[nargs] = 0;

    return std::pair<std::size_t, char**>(nargs, argv);
}

}
}
}


#endif
