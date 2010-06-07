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
 * \file boost/process/stream_behavior.hpp
 *
 * Includes the declaration of the stream_behavior enumeration. 
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

namespace boost {
namespace process {

/**
 *
 * Stream behaviors to define how standard streams are passed to a child process.
 *
 * inherit: A stream is inherited from its parent process. 
 * capture: A stream is redirected to its parent process (anonymous pipe). 
 * mute: A stream is redirected to /dev/null, /dev/zero or NUL. 
 * close: A stream is closed. 
 *
 */
// enum stream_behavior { inherit, capture, mute, close }; 

class stream_behavior 
{ 
public: 
#if defined(BOOST_POSIX_API) 
    typedef int native_type; 
#elif defined(BOOST_WINDOWS_API) 
    typedef HANDLE native_type; 
#endif 

    virtual native_type get_child_end() const 
    { 
#if defined(BOOST_POSIX_API) 
        return -1; 
#elif defined(BOOST_WINDOWS_API) 
        return INVALID_HANDLE_VALUE; 
#endif 
    } 

    virtual native_type get_parent_end() const 
    { 
#if defined(BOOST_POSIX_API) 
        return -1; 
#elif defined(BOOST_WINDOWS_API) 
        return INVALID_HANDLE_VALUE; 
#endif 
    } 
}; 

typedef stream_behavior close; 

class inherit : public stream_behavior 
{ 
public: 
    inherit(native_type child_end) 
    : child_end_(child_end) 
    { 
#if defined(BOOST_WINDOWS_API) 
        if (!::DuplicateHandle(::GetCurrentProcess(), child_end_, ::GetCurrentProcess(), &child_end_, 0, TRUE, DUPLICATE_SAME_ACCESS)) 
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("DuplicateHandle() failed"); 
#endif 
    } 

    native_type get_child_end() const 
    { 
        return child_end_; 
    } 

private: 
    native_type child_end_; 
}; 

class capture : public stream_behavior 
{ 
public: 
    enum stream_type { input_stream, output_stream }; 

    capture(stream_type stream) 
    { 
        native_type natives[2]; 
#if defined(BOOST_POSIX_API) 
        if (::pipe(natives) == -1) 
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("pipe(2) failed"); 
#elif defined(BOOST_WINDOWS_API) 
        SECURITY_ATTRIBUTES sa;
        ZeroMemory(&sa, sizeof(sa));
        sa.nLength = sizeof(sa);
        sa.lpSecurityDescriptor = NULL;
        sa.bInheritHandle = TRUE;
        if (!::CreatePipe(&natives[0], &natives[1], &sa, 0)) 
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("CreatePipe() failed"); 
#endif 
        child_end_ = natives[(stream == input_stream) ? 0 : 1]; 
        parent_end_ = natives[(stream == input_stream) ? 1 : 0]; 
#if defined(BOOST_WINDOWS_API) 
        if (!::SetHandleInformation(child_end_, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT)) 
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("SetHandleInformation() failed"); 
#endif 
    } 

    native_type get_child_end() const 
    { 
        return child_end_; 
    } 

    native_type get_parent_end() const 
    { 
        return parent_end_; 
    } 

private: 
    native_type child_end_; 
    native_type parent_end_; 
}; 

class capture_with_named_pipe : public stream_behavior 
{ 
public: 
    enum stream_type { input_stream, output_stream }; 

    capture_with_named_pipe(stream_type stream) 
    { 
        native_type natives[2]; 
#if defined(BOOST_POSIX_API) 
        // TODO: Use RAII to close HANDLEs if an exception is thrown. 
        if (::mkfifo("/tmp/fifo.1", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) == -1) 
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("mkfifo(3) failed"); 
        if (stream == input_stream) 
        { 
            child_end_ = ::open("/tmp/fifo.1", O_RDONLY); 
            if (child_end_ == -1) 
                BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("open(2) failed"); 
            parent_end_ = ::open("/tmp/fifo.1", O_WRONLY); 
            if (parent_end_ == -1) 
                BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("open(2) failed"); 
        } 
        else 
        { 
            child_end_ = ::open("/tmp/fifo.1", O_WRONLY); 
            if (child_end_ == -1) 
                BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("open(2) failed"); 
            parent_end_ = ::open("/tmp/fifo.1", O_RDONLY); 
            if (parent_end_ == -1) 
                BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("open(2) failed"); 
        } 
#elif defined(BOOST_WINDOWS_API) 
        // TODO: Use RAII to close HANDLEs if an exception is thrown. 
        SECURITY_ATTRIBUTES sa;
        ZeroMemory(&sa, sizeof(sa));
        sa.nLength = sizeof(sa);
        sa.lpSecurityDescriptor = NULL;
        sa.bInheritHandle = TRUE;
        // TODO: Make this thread-safe (create unique filename differently). 
        static unsigned int nextid = 0; 
        std::string pipe = "\\\\.\\pipe\\boost_process_" + boost::lexical_cast<std::string>(::GetCurrentProcessId()) + "_" + boost::lexical_cast<std::string>(nextid++); 
        natives[0] = ::CreateNamedPipeA(pipe.c_str(), PIPE_ACCESS_INBOUND | FILE_FLAG_OVERLAPPED, 0, 1, 8192, 8192, 0, &sa); 
        if (natives[0] == INVALID_HANDLE_VALUE) 
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("CreateNamedPipe() failed"); 
        natives[1] = ::CreateFileA(pipe.c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL); 
        if (natives[1] == INVALID_HANDLE_VALUE) 
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("CreateFile() failed"); 

        OVERLAPPED overlapped; 
        ZeroMemory(&overlapped, sizeof(overlapped)); 
        overlapped.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL); 
        if (!overlapped.hEvent) 
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("CreateEvent() failed"); 
        BOOL b = ::ConnectNamedPipe(natives[0], &overlapped); 
        if (!b) 
        { 
            if (::GetLastError() == ERROR_IO_PENDING) 
            { 
                if (::WaitForSingleObject(overlapped.hEvent, INFINITE) == WAIT_FAILED) 
                { 
                    ::CloseHandle(overlapped.hEvent); 
                    BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("WaitForSingleObject() failed"); 
                } 
            } 
            else if (::GetLastError() != ERROR_PIPE_CONNECTED) 
            { 
                ::CloseHandle(overlapped.hEvent); 
                BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("ConnectNamedPipe() failed"); 
            } 
        } 
        ::CloseHandle(overlapped.hEvent); 
#endif 
        child_end_ = natives[(stream == input_stream) ? 0 : 1]; 
        parent_end_ = natives[(stream == input_stream) ? 1 : 0]; 
#if defined(BOOST_WINDOWS_API) 
        if (!::SetHandleInformation(child_end_, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT)) 
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("SetHandleInformation() failed"); 
#endif 
    } 

    native_type get_child_end() const 
    { 
        return child_end_; 
    } 

    native_type get_parent_end() const 
    { 
        return parent_end_; 
    } 

private: 
    native_type child_end_; 
    native_type parent_end_; 
}; 

class mute : public stream_behavior 
{ 
public: 
    enum stream_type { input_stream, output_stream }; 

    mute(stream_type stream) 
    { 
#if defined(BOOST_POSIX_API) 
        std::string filename = (stream == input_stream) ? "/dev/zero" : "/dev/null"; 
        child_end_ = ::open(filename.c_str(), O_RDONLY); 
        if (child_end_ == -1) 
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("open(2) failed"); 
#elif defined(BOOST_WINDOWS_API) 
        child_end_ = ::CreateFileA("NUL", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); 
        if (child_end_ == INVALID_HANDLE_VALUE) 
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("CreateFile() failed"); 
#endif 
    } 

    native_type get_child_end() const 
    { 
        return child_end_; 
    } 

private: 
    native_type child_end_; 
}; 

}
}

#endif
