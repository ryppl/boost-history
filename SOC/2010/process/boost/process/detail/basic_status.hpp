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
 * \file boost/process/detail/basic_status.hpp
 *
 * Includes the declaration of the basic status class.
 */

#ifndef BOOST_PROCESS_DETAIL_BASIC_STATUS_HPP
#define BOOST_PROCESS_DETAIL_BASIC_STATUS_HPP

#include <boost/process/config.hpp>
#include <boost/process/pid_type.hpp>
#include <boost/asio.hpp>

namespace boost {
namespace process {
namespace detail {

template <typename Service>
class basic_status
    : public boost::asio::basic_io_object<Service>
{
public:
    explicit basic_status(boost::asio::io_service &io_service)
    : boost::asio::basic_io_object<Service>(io_service)
    {
    }

    int wait(pid_type pid)
    {
        return this->service.wait(this->implementation, pid);
    }

    template <typename Handler>
    void async_wait(pid_type pid, Handler handler)
    {
        this->service.async_wait(this->implementation, pid, handler);
    }
};

}
}
}

#endif 
