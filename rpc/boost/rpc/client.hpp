#ifndef BOOST_RPC_CLIENT_HPP
#define BOOST_RPC_CLIENT_HPP

#include <boost/rpc/config.hpp>
#include <boost/rpc/detail/protocol.hpp>
#include <boost/rpc/detail/threaded_service.hpp>
#include <boost/rpc/detail/expanding_buffer.hpp>
#include <boost/rpc/call_options.hpp>
#include <boost/rpc/call.hpp>
#include <boost/future/future.hpp>

namespace boost {
namespace rpc {

/// A client used to communicate with a rpc::registry_server over the network.
/**   \param Registry underlying Registry type used.

*/
template<typename Registry>
class client : public detail::threaded_service
{
public:
    typedef typename Registry::id_type id_type;
    typedef typename Registry::archive_type archive_type;

    typedef std::map<protocol::request_id_type, boost::shared_ptr<handler_base> > requests_type;

    /// Initializes the client to communicate with the specified Boost.Asio endpoint.
    /** \todo Handle protocol error.
    */
    client()
        : next_request_id(0),
        result_marshal_buffer(BOOST_RPC_RPC_BUFFER_SIZE),
        socket(service),
        stream(std::ios::in | std::ios::out | std::ios::binary),
        default_options(call_options::all_out_parameters, call_options::no_exception_handling)
    {
        // Start the thread.
        start_service_thread();
    }
    /// Stops the client service.
    ~client()
    {
        stop_service_thread();
    }
    template<typename ReturnType>
    future<ReturnType> operator()(async_returning_call<ReturnType> &call, call_options options)
    {
        boost::shared_ptr<handler_base> handler;
        async_returning_handler<ReturnType> *r_handler;
        specify(options);
        {
            boost::mutex::scoped_lock lock(mutex);

            handler = process_call(call, options);
            r_handler = (async_returning_handler<ReturnType> *)handler.get();
        }
        // any out parameter passed by reference should be assigned to the call's promises
        r_handler->assign_promises();
        return r_handler->return_promise();
    }
protected:
    void prepare_async_read()
    {
        boost::asio::async_read(socket, result_header.as_buffer(),
        boost::bind(&client::read_result_header, this, boost::asio::placeholders::error));
    }
private:
    boost::shared_ptr<handler_base> process_call(call_base &call, call_options &options)
    {
        boost::shared_ptr<handler_base> handler;

        // send the header
        boost::asio::write(socket,
            protocol::call_header(next_request_id, options, call.parameters().size()).as_buffer(),
            boost::asio::transfer_all());
        // send the marshal
        boost::asio::write(socket, asio::buffer(call.parameters()), boost::asio::transfer_all());

        handler = call.spawn_handler();
        if (options.marshal_option > call_options::no_results)
            requests[next_request_id] = handler;

        next_request_id++;
        return handler;
    }
    void read_result_header(const boost::system::error_code& error)
    {
        boost::mutex::scoped_lock lock(mutex);

        if (!error)
        {
            boost::asio::async_read(socket,
                boost::asio::buffer(result_marshal_buffer,result_header.marshal_size),
                boost::bind(&client::read_result_marshal, this, boost::asio::placeholders::error));
        }
        // ready to receive the next header
        prepare_async_read();
    }
    void read_result_marshal(const boost::system::error_code& error)
    {
        boost::mutex::scoped_lock lock(mutex);

        // get the request call
        requests_type::iterator it = requests.find(result_header.request_id);
        if (it == requests.end())
        {
            // unknown request!!!
            std::cerr << "unknown request" << result_header.request_id << std::endl;
        }
        boost::shared_ptr<handler_base> handler_ptr = it->second;
        requests.erase(it);

        // process the results
        if (result_header.options.marshal_option >= call_options::return_only)
        {
            handler_ptr->result_string(std::string(result_marshal_buffer, result_header.marshal_size),
                result_header.options);
        }
        handler_ptr->complete();
    }
    void specify(call_options &options)
    {
        if(options.marshal_option == call_options::marshal_unspecified)
            options.marshal_option = default_options.marshal_option;
        if(options.exception_option == call_options::exception_unspecified)
            options.exception_option = default_options.exception_option;
    }
protected:
    boost::asio::ip::tcp::socket socket;

private:
    protocol::request_id_type next_request_id;

    protocol::call_header result_header;
    detail::expanding_buffer result_marshal_buffer;

    std::stringstream stream;

    call_options default_options;

    boost::mutex mutex;
    boost::condition condition;
    requests_type requests;
};

/// A client with the capability to connect to a server.
/**
    \param Registry underlying Registry type used.
    \param Connector underlying Connector type used.

    The client class negotiates a connection to the server and can service calls to the
    registry on the server side.
*/
template<typename Registry, typename Connector>
class connecting_client : public client<Registry>
{
public:
    connecting_client(boost::asio::ip::tcp::endpoint endpoint) : connector(endpoint)
    {
        // Connect the socket.
        connector.connect(socket);
//        protocol::message_reader<boost::asio::ip::tcp::socket> reader(socket);


        // Wait for acknowledgement on the socket.
        //if (reader.message() != protocol::message::ok)
        if (protocol::read_message(socket) != protocol::message::ok)
        {
           // protocol error
           std::cerr << "client protocol error" << std::endl;
        }

        prepare_async_read();
    }
private:
    Connector connector;
};

} // namespace rpc
} // namespace boost

#endif BOOST_RPC_CLIENT_HPP