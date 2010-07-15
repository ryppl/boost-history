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
 * \file boost/process/detail/status_impl.hpp
 *
 * Includes the declaration of the status implementation class.
 */

#ifndef BOOST_PROCESS_DETAIL_STATUS_IMPL_HPP
#define BOOST_PROCESS_DETAIL_STATUS_IMPL_HPP

#include <boost/process/config.hpp>
#include <boost/system/error_code.hpp>
#include <boost/ptr_container/ptr_unordered_map.hpp>
#include <boost/bind.hpp>
#include <boost/process/stream_behavior.hpp>
#include <algorithm>

#if defined(BOOST_POSIX_API)
#elif defined(BOOST_WINDOWS_API)
#   include <windows.h>
#else
#   error "Unsupported platform."
#endif

namespace boost {
namespace process {
namespace detail {

struct operation
{
    #if defined(BOOST_WINDOWS_API)
    virtual void operator()(DWORD exit_code)
    #elif defined BOOST_POSIX_API
    virtual void operator()(unsigned int exit_code)
    #endif
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

    #if defined(BOOST_WINDOWS_API)
    void operator()(DWORD exit_code)
    #elif defined BOOST_POSIX_API
    void operator()(unsigned int exit_code)
    #endif
    {
        handler_(boost::system::error_code(), exit_code);
    }

private:
    Handler handler_;
};

class status_impl
{
public:
    template <typename Handler>
    void async_wait(behavior::stream::native_type handle, Handler handler)
    {
        ops_.insert(handle, new wrapped_handler<Handler>(handler));
    }

    #if defined(BOOST_WINDOWS_API)
    void complete(behavior::stream::native_type handle, DWORD exit_code)
    #elif defined(BOOST_POSIX_API)
    void complete(behavior::stream::native_type handle, unsigned int exit_code)
    #endif
    {
        boost::iterator_range<operations_type::iterator> r = ops_.equal_range(handle);
        for (operations_type::iterator it = r.begin(); it != r.end(); ++it)
            (*it->second)(exit_code);
        ops_.erase(r.begin(), r.end());
        #if defined(BOOST_WINDOWS_API)
        if (!::CloseHandle(handle))
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("CloseHandle() failed");
        #elif defined(BOOST_POSIX_API)
        if (::close(handle) == -1)
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("close() failed");
        #endif
    }

private:
    typedef boost::ptr_unordered_multimap<behavior::stream::native_type, operation> operations_type;
    operations_type ops_;
};

}
}
}

#endif
