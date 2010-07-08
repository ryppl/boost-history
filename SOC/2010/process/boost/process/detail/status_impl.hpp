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
    virtual void operator()(DWORD exit_code)
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

    void operator()(DWORD exit_code)
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
    void async_wait(HANDLE handle, Handler handler)
    {
        ops_.insert(handle, new wrapped_handler<Handler>(handler));
    }

    void complete(HANDLE handle, DWORD exit_code)
    {
        boost::iterator_range<operations_type::iterator> r = ops_.equal_range(handle);
        for (operations_type::iterator it = r.begin(); it != r.end(); ++it)
            (*it->second)(exit_code);
        ops_.erase(r.begin(), r.end());
        if (!::CloseHandle(handle))
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("CloseHandle() failed");
    }

private:
    typedef boost::ptr_unordered_multimap<HANDLE, operation> operations_type;
    operations_type ops_;
};

}
}
}

#endif
