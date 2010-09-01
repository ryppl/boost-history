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
 * \file boost/process/stream_behavior.hpp
 *
 * Includes the declaration of stream behavior classes.
 */

#ifndef BOOST_PROCESS_STREAM_BEHAVIOR_HPP
#define BOOST_PROCESS_STREAM_BEHAVIOR_HPP

#include <boost/process/config.hpp>

#if defined(BOOST_POSIX_API)
#   include <sys/stat.h>
#   include <fcntl.h>
#   include <unistd.h>
#elif defined(BOOST_WINDOWS_API)
#   include <windows.h>
#endif

#include <boost/process/handle.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <string>
#include <algorithm>

namespace boost {
namespace process {
namespace behavior {

/**
 * Base class for stream behaviors.
 *
 * Stream behavior classes are used to configure streams of a child process.
 * They must be derived from this class.
 */
class stream
{
public:
    /**
     * Empty virtual destructor.
     */
    virtual ~stream()
    {
    }

    /**
     * Factory function to create a stream behavior object.
     *
     * Returns a shared pointer to the stream behavior object as this is
     * what a context object expects. The shared pointer guarantees that
     * the object is cleaned up.
     */
    static boost::shared_ptr<stream> create()
    {
        return boost::make_shared<stream>();
    }

    /**
     * Returns the child's end of the stream.
     */
    virtual handle get_child_end()
    {
        return handle();
    }

    /**
     * Returns the parent's end of the stream.
     */
    virtual handle get_parent_end()
    {
        return handle();
    }
};

/**
 * Stream behavior to close streams of a child process.
 *
 * A child process will not be able to use the stream.
 */
typedef stream close;

/**
 * Stream behavior to make a child process inherit streams.
 *
 * A child process will use the very same stream of its parent process.
 */
class inherit : public stream
{
public:
    inherit(handle::native_type child_end)
    {
#if defined(BOOST_POSIX_API)
        child_end_ = dup(child_end);
        if (!child_end_.valid())
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("dup(2) failed");
#elif defined(BOOST_WINDOWS_API)
        HANDLE h;
        if (!DuplicateHandle(GetCurrentProcess(), child_end,
            GetCurrentProcess(), &h, 0, TRUE, DUPLICATE_SAME_ACCESS))
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("DuplicateHandle() failed");
        child_end_ = h;
#endif
    }

    static boost::shared_ptr<inherit> create(handle::native_type child_end)
    {
        return boost::make_shared<inherit>(child_end);
    }

    handle get_child_end()
    {
        return child_end_;
    }

private:
    handle child_end_;
};

/**
 * Stream behavior to redirect streams with a pipe.
 *
 * A child process will be able to communicate with its parent process.
 */
class pipe : public stream
{
public:
    enum stream_type { input_stream, output_stream };

    pipe(stream_type stream)
    {
        handle::native_type ends[2];
#if defined(BOOST_POSIX_API)
        if (::pipe(ends) == -1)
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("pipe(2) failed");
#elif defined(BOOST_WINDOWS_API)
        SECURITY_ATTRIBUTES sa;
        ZeroMemory(&sa, sizeof(sa));
        sa.nLength = sizeof(sa);
        sa.lpSecurityDescriptor = NULL;
        sa.bInheritHandle = FALSE;
        if (!CreatePipe(&ends[0], &ends[1], &sa, 0))
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("CreatePipe() failed");
#endif 
        child_end_ = ends[(stream == input_stream) ? 0 : 1];
        parent_end_ = ends[(stream == input_stream) ? 1 : 0];
#if defined(BOOST_WINDOWS_API)
        if (!SetHandleInformation(child_end_.native(), HANDLE_FLAG_INHERIT,
            HANDLE_FLAG_INHERIT))
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR(
                "SetHandleInformation() failed");
#endif
    }

    static boost::shared_ptr<behavior::pipe> create(stream_type stream)
    {
        return boost::make_shared<pipe>(stream);
    }

    handle get_child_end()
    {
        return child_end_;
    }

    handle get_parent_end()
    {
        return parent_end_;
    }

private:
    handle child_end_;
    handle parent_end_;
};

/**
 * Stream behavior to redirect streams with a named pipe.
 *
 * A child process will be able to communicate with its parent process.
 * On Windows this stream behavior must be used for asynchronous I/O (as only
 * named pipes support asynchronous I/O on Windows).
 */
class named_pipe : public stream
{
public:
    enum stream_type { input_stream, output_stream };

    named_pipe(stream_type stream, std::string *name = 0)
    {
#if defined(BOOST_POSIX_API)
        std::string s;
        if (name && !name->empty())
            s = *name;
        else
        {
            boost::uuids::random_generator gen;
            boost::uuids::uuid u = gen();
            s = "/tmp/boost_process_" + boost::lexical_cast<std::string>(u);
            if (name)
                *name = s;
        }

        if (mkfifo(s.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) == -1)
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("mkfifo(3) failed");
        child_end_ = open(s.c_str(), O_RDONLY | O_NONBLOCK);
        if (!child_end_.valid())
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("open(2) failed");
        int opts = fcntl(child_end_.native(), F_GETFL);
        if (opts == -1)
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("fcntl(2) failed");
        opts ^= O_NONBLOCK;
        if (fcntl(child_end_.native(), F_SETFL, opts) == -1)
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("fcntl(2) failed");
        parent_end_ = open(s.c_str(), O_WRONLY);
        if (!parent_end_.valid())
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("open(2) failed");
#elif defined(BOOST_WINDOWS_API)
        std::string s;
        if (name && !name->empty())
            s = *name;
        else
        {
            boost::uuids::random_generator gen;
            boost::uuids::uuid u = gen();
            s = "\\\\.\\pipe\\boost_process_" +
                boost::lexical_cast<std::string>(u);
            if (name)
                *name = s;
        }

        SECURITY_ATTRIBUTES sa;
        ZeroMemory(&sa, sizeof(sa));
        sa.nLength = sizeof(sa);
        sa.lpSecurityDescriptor = NULL;
        sa.bInheritHandle = TRUE;
        child_end_ = CreateNamedPipeA(s.c_str(), PIPE_ACCESS_INBOUND |
            FILE_FLAG_OVERLAPPED, 0, 1, 8192, 8192, 0, &sa);
        if (!child_end_.valid())
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("CreateNamedPipe() failed");
        parent_end_ = CreateFileA(s.c_str(), GENERIC_WRITE, 0, NULL,
            OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
        if (!parent_end_.valid())
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("CreateFile() failed");
#endif
        if (stream == output_stream)
            std::swap(child_end_, parent_end_);
#if defined(BOOST_WINDOWS_API)
        if (!SetHandleInformation(child_end_.native(), HANDLE_FLAG_INHERIT,
            HANDLE_FLAG_INHERIT))
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR(
                "SetHandleInformation() failed");
#endif
    }

    static boost::shared_ptr<named_pipe> create(stream_type stream,
        std::string *name = 0)
    {
        return boost::make_shared<named_pipe>(stream, name);
    }

    handle get_child_end()
    {
        return child_end_;
    }

    handle get_parent_end()
    {
        return parent_end_;
    }

private:
    handle child_end_;
    handle parent_end_;
};

/**
 * Stream behavior to mute streams.
 *
 * A child process will be able to use streams. But data written to an
 * output stream is discarded and data read from an input stream is 0.
 */
class null : public stream
{
public:
    enum stream_type { input_stream, output_stream };

    null(stream_type stream)
    {
#if defined(BOOST_POSIX_API)
        std::string filename = (stream == input_stream) ? "/dev/zero" :
            "/dev/null";
        int flag = (stream == input_stream) ? O_RDONLY : O_WRONLY;
        child_end_ = open(filename.c_str(), flag);
        if (!child_end_.valid())
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("open(2) failed");
#elif defined(BOOST_WINDOWS_API)
        DWORD access = (stream == input_stream) ? GENERIC_READ : GENERIC_WRITE;
        child_end_ = CreateFileA("NUL", access, 0, NULL, OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL, NULL);
        if (!child_end_.valid())
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("CreateFile() failed");
        if (!SetHandleInformation(child_end_.native(), HANDLE_FLAG_INHERIT,
            HANDLE_FLAG_INHERIT))
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR(
                "SetHandleInformation() failed");
#endif
    }

    static boost::shared_ptr<null> create(stream_type stream)
    {
        return boost::make_shared<null>(stream);
    }

    handle get_child_end()
    {
        return child_end_;
    }

private:
    handle child_end_;
};

}
}
}

#endif
