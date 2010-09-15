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
#   include <rpc.h>
#endif

#include <boost/process/stream_ends.hpp>
#include <boost/process/handle.hpp>
#include <string>
#include <algorithm>

namespace boost {
namespace process {
namespace behavior {

/**
 * Stream behavior to close streams of a child process.
 *
 * A child process will not be able to use the stream.
 */
class close
{
public:
    stream_ends operator()(bool) const
    {
        return stream_ends();
    }
};

/**
 * Stream behavior to make a child process inherit streams.
 *
 * A child process will use the very same stream of its parent process.
 */
class inherit
{
public:
    inherit(handle::native_type h)
    : h_(h, handle::dont_close)
    {
    }

    stream_ends operator()(bool) const
    {
        return stream_ends(h_, handle());
    }

private:
    handle h_;
};

/**
 * Stream behavior to redirect streams with a pipe.
 *
 * A child process will be able to communicate with its parent process.
 */
class pipe
{
public:
    stream_ends operator()(bool in) const
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
        handle child_end = ends[in ? 0 : 1];
        handle parent_end = ends[in ? 1 : 0];
#if defined(BOOST_WINDOWS_API)
        if (!SetHandleInformation(child_end.native(), HANDLE_FLAG_INHERIT,
            HANDLE_FLAG_INHERIT))
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR(
                "SetHandleInformation() failed");
#endif
        return stream_ends(child_end, parent_end);
    }
};

/**
 * Stream behavior to redirect streams with a named pipe.
 *
 * A child process will be able to communicate with its parent process.
 * On Windows this stream behavior must be used for asynchronous I/O (as only
 * named pipes support asynchronous I/O on Windows).
 */
class named_pipe
{
public:
    named_pipe(const std::string &name)
    : name_(name)
    {
    }

    stream_ends operator()(bool in) const
    {
#if defined(BOOST_POSIX_API)
        if (mkfifo(name_.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) == -1)
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("mkfifo(3) failed");
        handle child_end = open(name_.c_str(), O_RDONLY | O_NONBLOCK);
        if (!child_end.valid())
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("open(2) failed");
        int opts = fcntl(child_end.native(), F_GETFL);
        if (opts == -1)
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("fcntl(2) failed");
        opts ^= O_NONBLOCK;
        if (fcntl(child_end.native(), F_SETFL, opts) == -1)
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("fcntl(2) failed");
        handle parent_end = open(name_.c_str(), O_WRONLY);
        if (!parent_end.valid())
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("open(2) failed");
#elif defined(BOOST_WINDOWS_API)
        SECURITY_ATTRIBUTES sa;
        ZeroMemory(&sa, sizeof(sa));
        sa.nLength = sizeof(sa);
        sa.lpSecurityDescriptor = NULL;
        sa.bInheritHandle = TRUE;
        handle child_end = CreateNamedPipeA(name_.c_str(), PIPE_ACCESS_INBOUND |
            FILE_FLAG_OVERLAPPED, 0, 1, 8192, 8192, 0, &sa);
        if (!child_end.valid())
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("CreateNamedPipe() failed");
        handle parent_end = CreateFileA(name_.c_str(), GENERIC_WRITE, 0, NULL,
            OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
        if (!parent_end.valid())
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("CreateFile() failed");
#endif
        if (!in)
            std::swap(child_end, parent_end);
#if defined(BOOST_WINDOWS_API)
        if (!SetHandleInformation(child_end.native(), HANDLE_FLAG_INHERIT,
            HANDLE_FLAG_INHERIT))
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR(
                "SetHandleInformation() failed");
#endif
        return stream_ends(child_end, parent_end);
    }

private:
    std::string name_;
};

/**
 * Stream behavior to redirect streams with a pipe which supports asynchronous
 * I/O.
 *
 * As platforms require different types of pipes for asynchronous I/O this
 * stream behavior is provided for convenience. It uses the minimum required
 * pipe type on a platform in order to be able to use asynchronous I/O.
 */
#if defined(BOOST_POSIX_API)
typedef pipe async_pipe;
#elif defined(BOOST_WINDOWS_API)
class async_pipe
{
public:
    stream_ends operator()(bool in) const
    {
        UUID uuid;
        RPC_STATUS s = UuidCreateSequential(&uuid);
        if (s != RPC_S_OK && s != RPC_S_UUID_LOCAL_ONLY)
            BOOST_PROCESS_THROW_ERROR(s, "UuidCreateSequential() failed");
        unsigned char *c;
        s = UuidToStringA(&uuid, &c);
        if (s != RPC_S_OK)
            BOOST_PROCESS_THROW_ERROR(s, "UuidToString() failed");
        std::string name;
        try
        {
            name = reinterpret_cast<char*>(c);
        }
        catch (...)
        {
            RpcStringFreeA(&c);
            throw;
        }
        RpcStringFreeA(&c);
        named_pipe p("\\\\.\\pipe\\boost_process_" + name);
        return p(in);
    }
};
#endif

/**
 * Stream behavior to mute streams.
 *
 * A child process will be able to use streams. But data written to an
 * output stream is discarded and data read from an input stream is 0.
 */
class null
{
public:
    stream_ends operator()(bool in) const
    {
#if defined(BOOST_POSIX_API)
        std::string filename = in ? "/dev/zero" : "/dev/null";
        int flag = in ? O_RDONLY : O_WRONLY;
        handle child_end = open(filename.c_str(), flag);
        if (!child_end.valid())
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("open(2) failed");
#elif defined(BOOST_WINDOWS_API)
        DWORD access = in ? GENERIC_READ : GENERIC_WRITE;
        handle child_end = CreateFileA("NUL", access, 0, NULL, OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL, NULL);
        if (!child_end.valid())
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("CreateFile() failed");
        if (!SetHandleInformation(child_end.native(), HANDLE_FLAG_INHERIT,
            HANDLE_FLAG_INHERIT))
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("SetHandleInformation() failed");
#endif
        return stream_ends(child_end, handle());
    }
};

}
}
}

#endif
