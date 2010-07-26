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
 *
 */

#ifndef BOOST_PROCESS_STREAM_BEHAVIOR_HPP
#define BOOST_PROCESS_STREAM_BEHAVIOR_HPP

#include <boost/process/config.hpp>

#if defined(BOOST_POSIX_API)
#   include <sys/stat.h>
#   include <fcntl.h>
#   include <unistd.h>
#elif defined(BOOST_WINDOWS_API)
#   include <boost/lexical_cast.hpp>
#   include <string>
#   include <windows.h>
#endif

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

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
     * Opaque identifier for a stream's end.
     *
     * Each operating system identifies streams' ends using a specific type.
     * The \a stream_end_type type is used to transparently refer to a stream's
     * end regardless of the operating system in which this class is used.
     *
     * On POSIX systems it is defined as int, on Windows systems as HANDLE.
     */
#if defined(BOOST_PROCESS_DOXYGEN)
    typedef NativeStreamType stream_end_type;
#elif defined(BOOST_POSIX_API)
    typedef int stream_end_type;
#elif defined(BOOST_WINDOWS_API)
    typedef HANDLE stream_end_type;
#endif

    /**
     * Factory function to create a stream behavior object.
     *
     * Returns a shared pointer to the stream behavior object as this is
     * what a context object expects. The shared pointer guarantees that
     * the object is cleaned up.
     */
    static boost::shared_ptr<stream> def()
    {
        return boost::make_shared<stream>(stream());
    }

    /**
     * Returns the child's end of the stream.
     */
    virtual stream_end_type get_child_end()
    {
#if defined(BOOST_POSIX_API)
        return -1;
#elif defined(BOOST_WINDOWS_API)
        return INVALID_HANDLE_VALUE;
#endif
    }

    /**
     * Returns the parent's end of the stream.
     */
    virtual stream_end_type get_parent_end()
    {
#if defined(BOOST_POSIX_API)
        return -1;
#elif defined(BOOST_WINDOWS_API)
        return INVALID_HANDLE_VALUE;
#endif
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
    inherit(stream_end_type child_end)
    : child_end_(child_end)
    {
#if defined(BOOST_POSIX_API)
        child_end_ = dup(child_end);
        if (child_end_ == -1)
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("dup(2) failed");
#elif defined(BOOST_WINDOWS_API)
        if (!DuplicateHandle(GetCurrentProcess(), child_end_,
            GetCurrentProcess(), &child_end_, 0, TRUE, DUPLICATE_SAME_ACCESS))
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("DuplicateHandle() failed");
#endif
    }

    static boost::shared_ptr<inherit> def(stream_end_type child_end)
    {
        return boost::make_shared<inherit>(inherit(child_end));
    }

    stream_end_type get_child_end()
    {
        return child_end_;
    }

private:
    stream_end_type child_end_;
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
        stream_end_type ends[2];
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
        if (!SetHandleInformation(child_end_, HANDLE_FLAG_INHERIT,
            HANDLE_FLAG_INHERIT))
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR(
                "SetHandleInformation() failed");
#endif
    }

    static boost::shared_ptr<pipe> def(stream_type stream)
    {
        return boost::make_shared<pipe>(pipe(stream));
    }

    stream_end_type get_child_end()
    {
        return child_end_;
    }

    stream_end_type get_parent_end()
    {
        return parent_end_;
    }

private:
    stream_end_type child_end_;
    stream_end_type parent_end_;
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

    named_pipe(stream_type stream, std::string name = "")
    {
        stream_end_type ends[2];
#if defined(BOOST_POSIX_API)
        if (name.empty())
            // TODO: Generate unique filename.
            name = "/tmp/fifo.1";

        // TODO: Use RAII to close HANDLEs if an exception is thrown.
        if (mkfifo(name.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) == -1)
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("mkfifo(3) failed");
        if (stream == input_stream)
        {
            child_end_ = open(name.c_str(), O_RDONLY);
            if (child_end_ == -1)
                BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("open(2) failed");
            parent_end_ = open(name.c_str(), O_WRONLY);
            if (parent_end_ == -1)
                BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("open(2) failed");
        }
        else
        {
            child_end_ = open(name.c_str(), O_WRONLY);
            if (child_end_ == -1)
                BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("open(2) failed");
            parent_end_ = open(name.c_str(), O_RDONLY);
            if (parent_end_ == -1)
                BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("open(2) failed");
        }
#elif defined(BOOST_WINDOWS_API)
        if (name.empty())
        {
            // TODO: Make this thread-safe (create unique filename differently).
            static unsigned int nextid = 0;
            name = "\\\\.\\pipe\\boost_process_" + boost::lexical_cast<std::string>(::GetCurrentProcessId()) + "_" + boost::lexical_cast<std::string>(nextid++);
        }

        // TODO: Use RAII to close HANDLEs if an exception is thrown.
        SECURITY_ATTRIBUTES sa;
        ZeroMemory(&sa, sizeof(sa));
        sa.nLength = sizeof(sa);
        sa.lpSecurityDescriptor = NULL;
        sa.bInheritHandle = TRUE;
        ends[0] = CreateNamedPipeA(name.c_str(), PIPE_ACCESS_INBOUND | FILE_FLAG_OVERLAPPED, 0, 1, 8192, 8192, 0, &sa);
        if (ends[0] == INVALID_HANDLE_VALUE)
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("CreateNamedPipe() failed");
        ends[1] = ::CreateFileA(name.c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
        if (ends[1] == INVALID_HANDLE_VALUE)
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("CreateFile() failed");

        OVERLAPPED overlapped;
        ZeroMemory(&overlapped, sizeof(overlapped));
        overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        if (!overlapped.hEvent)
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("CreateEvent() failed");
        BOOL b = ConnectNamedPipe(ends[0], &overlapped);
        if (!b)
        {
            if (GetLastError() == ERROR_IO_PENDING)
            {
                if (WaitForSingleObject(overlapped.hEvent, INFINITE) == WAIT_FAILED)
                {
                    CloseHandle(overlapped.hEvent);
                    BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("WaitForSingleObject() failed");
                }
            }
            else if (GetLastError() != ERROR_PIPE_CONNECTED)
            {
                CloseHandle(overlapped.hEvent);
                BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("ConnectNamedPipe() failed");
            }
        }
        CloseHandle(overlapped.hEvent);
#endif
        child_end_ = ends[(stream == input_stream) ? 0 : 1];
        parent_end_ = ends[(stream == input_stream) ? 1 : 0];
#if defined(BOOST_WINDOWS_API)
        if (!SetHandleInformation(child_end_, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT))
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("SetHandleInformation() failed");
#endif
    }

    static boost::shared_ptr<named_pipe> def(stream_type stream)
    {
        return boost::make_shared<named_pipe>(named_pipe(stream));
    }

    stream_end_type get_child_end()
    {
        return child_end_;
    }

    stream_end_type get_parent_end()
    {
        return parent_end_;
    }

private:
    stream_end_type child_end_;
    stream_end_type parent_end_;
};

/**
 * Stream behavior to mute streams.
 *
 * A child process will be able to use streams. But data written to an
 * output stream is discarded and data read from an input stream is 0.
 */
class dummy : public stream
{
public:
    enum stream_type { input_stream, output_stream };

    dummy(stream_type stream)
    {
#if defined(BOOST_POSIX_API)
        std::string filename = (stream == input_stream) ? "/dev/zero" : "/dev/null";
        child_end_ = open(filename.c_str(), O_RDONLY);
        if (child_end_ == -1)
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("open(2) failed");
#elif defined(BOOST_WINDOWS_API)
        DWORD access = (stream == input_stream) ? GENERIC_READ : GENERIC_WRITE;
        child_end_ = CreateFileA("NUL", access, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (child_end_ == INVALID_HANDLE_VALUE)
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("CreateFile() failed");
#endif
    }

    static boost::shared_ptr<dummy> def(stream_type stream)
    {
        return boost::make_shared<dummy>(dummy(stream));
    }

    stream_end_type get_child_end()
    {
        return child_end_;
    }

private:
    stream_end_type child_end_;
};

}
}
}

#endif
