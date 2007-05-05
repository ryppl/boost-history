// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RPC_HANDLER_RETURN_HPP
#define BOOST_RPC_HANDLER_RETURN_HPP

#include <boost/noncopyable.hpp>
#include <boost/rpc/config.hpp>
#include <boost/rpc/call.hpp>
#include <boost/rpc/call_options.hpp>
#include <boost/future/future.hpp>

namespace boost {
namespace rpc {

template<typename Client, typename T>
class handler_return : private boost::noncopyable
{
    handler_return(Client *client, typename Client::id_type id)
        : client(client), id(id)
    {}
    operator boost::future<T>()
    {
        return client->handler_accepted_as_future(this);
    }
    operator shared_ptr<handler_base>()
    {
        return client->handler_accepted_as_handler(this);
    }
    ~handler_return()
    {
        client->handler_not_accepted();
    }
private:
    Client *client;
    typename Client::id_type id;

    friend class Client;
};

// namespace rpc
} // namespace boost

#endif // BOOST_RPC_CALL_HPP