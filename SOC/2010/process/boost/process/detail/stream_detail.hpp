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
 * \file boost/process/detail/stream_detail.hpp
 *
 * Includes the declaration of the stream_detail struct.
 * It's for internal purposes
 *
 */

#include <boost/process/config.hpp>
#include <boost/process/stream_behavior.hpp>
#include <boost/process/detail/file_handle.hpp>
#include <boost/process/detail/pipe.hpp>
#include <boost/optional.hpp>
#include <map>

#ifndef BOOST_PROCESS_STREAM_DETAIL_HPP
#define BOOST_PROCESS_STREAM_DETAIL_HPP

namespace boost {
namespace process {
namespace detail {

/*
 * This is the declaration of stream_object.
 * It represents all types that can be assign of an stream object
 * representation. For example, it can point to another stream or
 * to a pipe.
 */
struct stream_object
{
    int desc_to;
    std::string file_;
    file_handle handle_;
    pipe pipe_;
};

/*
 * This defines the std_stream_type enum.
 * It lists all possible std stream types.
 */
enum std_stream_type { stdin_type = 0, stdout_type = 1, stderr_type = 2 };

/*
 * This is the declaration of a stream detail.
 * A stream detail contains all details from one standard stream
 * (in, out or err) that are needed for adjust it
 * when a new process is created.
 *
 * stream_end: The adress of the stream
 * stream_type: Defines if the stream is a stdin, out or error.
 * behavior: The behavior this stream will assume
 * object: The object required to this behavior (see stream_object above)
 *
 */
struct stream_detail
{
#if defined(BOOST_POSIX_API)
    int stream_handle;
#elif defined(BOOST_WINDOWS_API)
    DWORD stream_handle;
#endif

    struct stream_object object;
    std_stream_type stream_type;
    stream_behavior behavior;

    stream_detail(std_stream_type st)
    {
        stream_type = st;
        object.pipe_ = pipe();
        switch(st)
        {
        case stdin_type:
        {
#if defined(BOOST_POSIX_API)
            stream_handle = STDIN_FILENO;
#elif defined(BOOST_WINDOWS_API)
            stream_handle = STD_INPUT_HANDLE;
#endif
            break;
        }
        case stdout_type:
        {
#if defined(BOOST_POSIX_API)
            stream_handle = STDOUT_FILENO;
#elif defined(BOOST_WINDOWS_API)
            stream_handle = STD_OUTPUT_HANDLE;
#endif
            break;
        }
        case stderr_type:
        {
#if defined(BOOST_POSIX_API)
            stream_handle = STDERR_FILENO;
#elif defined(BOOST_WINDOWS_API)
            stream_handle = STD_ERROR_HANDLE;
#endif
            break;
        }
        default:
        {
            BOOST_ASSERT(false);
        }
        }
    }
};

}
}
}

#endif
