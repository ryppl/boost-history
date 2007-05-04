// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RPC_SERVER_HPP
#define BOOST_RPC_SERVER_HPP

#include <boost/rpc/detail/threaded_service.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/rpc/registry_server.hpp>

namespace boost {
namespace rpc {

/// Sets up new connections with a dedicated rpc::registry_server.
/** \param Registry Type of registry used with the server.
    \param Acceptor Type of object used to accept/authenticate new connections.
*/
template<typename Registry, typename Acceptor>
class server : public detail::threaded_service
{
public:
    /// Initializes the server to use the specified registry and port.
    /** \warning reg must remain valid until the server is destroyed.

        Each successful connection will get a dedicated rpc::registry_server.
    */
    server(Registry &reg, int port)
        : registry_(reg)
        , acceptor(service, port, boost::bind(&server<Registry, Acceptor>::connected, this, _1))
    {
        start_service_thread();
    }
    ~server()
    {
        stop_service_thread();
    }
private:
    void connected(std::auto_ptr<boost::asio::ip::tcp::socket> socket_ptr)
    {
        servers.push_back(new registry_server<Registry>(registry_,
            boost::shared_ptr<boost::asio::ip::tcp::socket>(socket_ptr)));
    }

    Acceptor acceptor;

    Registry &registry_;
    boost::ptr_vector<registry_server<Registry> > servers;
};

} // namespace rpc
} // namespace boost

#endif // BOOST_RPC_SERVER_HPP
