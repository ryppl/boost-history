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
 * \file boost/process/detail/status_impl.hpp
 *
 * Includes the declaration of the status implementation class.
 */

#ifndef BOOST_PROCESS_DETAIL_STATUS_IMPL_HPP
#define BOOST_PROCESS_DETAIL_STATUS_IMPL_HPP

#include <boost/process/config.hpp>

#if defined(BOOST_POSIX_API)
#   include <sys/types.h>
#   include <sys/wait.h>
#elif defined(BOOST_WINDOWS_API)
#   include <windows.h>
#else
#   error "Unsupported platform."
#endif

#include <boost/process/pid_type.hpp>
#include <boost/system/error_code.hpp>
#include <boost/ptr_container/ptr_unordered_map.hpp>
#include <algorithm>

namespace boost {
namespace process {
namespace detail {

#if defined(BOOST_POSIX_API)
typedef pid_t phandle;
#elif defined(BOOST_WINDOWS_API)
typedef HANDLE phandle;
#endif

struct operation
{
    virtual void operator()(int exit_code)
    {
    }
};

template <typename Handler>
class wrapped_handler : public operation
{
public:
    wrapped_handler(Handler handler)
    : handler_(handler)
    {
    }

    void operator()(int exit_code)
    {
        handler_(boost::system::error_code(), exit_code);
    }

private:
    Handler handler_;
};

/**
 * The status_impl class saves internal data of every status I/O object.
 */
class status_impl
{
public:
    int wait(pid_type pid, boost::system::error_code &ec)
    {
#if defined(BOOST_POSIX_API)
        pid_t p;
        int status;
        do
        {
            p = waitpid(pid, &status, 0);
        } while (p == -1 && errno == EINTR);
        if (p == -1)
        {
            ec = boost::system::error_code(errno,
                    boost::system::get_system_category());
            return -1;
        }
        return status;
#elif defined(BOOST_WINDOWS_API)
        HANDLE h = OpenProcess(PROCESS_QUERY_INFORMATION | SYNCHRONIZE, FALSE, pid);
        if (h == NULL)
        {
            ec = boost::system::error_code(GetLastError(),
                    boost::system::get_system_category());
            return -1;
        }

        if (WaitForSingleObject(h, INFINITE) == WAIT_FAILED)
        {
            CloseHandle(h);
            ec = boost::system::error_code(GetLastError(),
                    boost::system::get_system_category());
            return -1;
        }

        DWORD exit_code;
        if (!GetExitCodeProcess(h, &exit_code))
        {
            CloseHandle(h);
            ec = boost::system::error_code(GetLastError(),
                    boost::system::get_system_category());
            return -1;
        }
        if (!CloseHandle(h))
        {
            ec = boost::system::error_code(GetLastError(),
                    boost::system::get_system_category());
            return -1;
        }
        return exit_code;
#endif
    }

    template <typename Handler>
    void async_wait(phandle ph, Handler handler)
    {
        ops_.insert(ph, new wrapped_handler<Handler>(handler));
    }

    void complete(phandle ph, int exit_code)
    {
        boost::iterator_range<operations_type::iterator> r = ops_.equal_range(ph);
        for (operations_type::iterator it = r.begin(); it != r.end(); ++it)
            (*it->second)(exit_code);
        ops_.erase(r.begin(), r.end());
#if defined(BOOST_WINDOWS_API)
        if (!CloseHandle(ph))
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("CloseHandle() failed");
#endif
    }

private:
    typedef boost::ptr_unordered_multimap<phandle, operation> operations_type;
    operations_type ops_;
};

}
}
}

#endif
