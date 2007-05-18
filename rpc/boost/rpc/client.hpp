#ifndef BOOST_RPC_CLIENT_HPP
#define BOOST_RPC_CLIENT_HPP

#include <boost/rpc/config.hpp>
#include <boost/rpc/detail/protocol.hpp>
#include <boost/rpc/detail/threaded_service.hpp>
#include <boost/rpc/detail/expanding_buffer.hpp>
#include <boost/rpc/call_options.hpp>
#include <boost/rpc/call.hpp>
//#include <boost/rpc/handler_return.hpp>
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
        : socket(service),
        next_request_id(0),
        result_marshal_buffer(BOOST_RPC_RPC_BUFFER_SIZE),
        stream(std::ios::in | std::ios::out | std::ios::binary),
        default_options(call_options::all_out_parameters, call_options::no_exception_handling),
        mutex_lock(mutex), sending(false)
    {
        mutex_lock.unlock();
        // Start the thread.
        start_service_thread();
    }
    /// Stops the client service.
    ~client()
    {
        stop_service_thread();
    }
private:
    template<typename ReturnType>
    class handler_return;
public:
    /// Prepares and sends a complete rpc call.
    /** The call is made in the following way:
        \li This method will lock the client's mutex, and spawn a handler for the call.
        \li It will then send the call header and marshal to the server
        \li Then, it will post the process_call method to the io_service, which will block.
        \li The handler will be returned via the handler_return class.
        \li The handler will notify the client about whether and how the handler was accepted.
        \li The client will store that in the handler's record and unlock the mutex.
        \li The process_call method can now finish the call.
    */
    template<typename ReturnType>
    handler_return<ReturnType> operator()(async_returning_call<ReturnType> &call)
    {
        mutex_lock.lock();
        if (sending)
            clear_to_send.wait(mutex_lock);
        sending = true;

        boost::shared_ptr<handler_base> handler(call.spawn_handler());
        requests[next_request_id] = handler;

        handler_return<ReturnType> handler_r(this,next_request_id);

        // send the header
        boost::asio::write(socket,
            protocol::call_header(next_request_id, call.parameters().size()).as_buffer(),
            boost::asio::transfer_all());
        // send the marshal
        boost::asio::write(socket, asio::buffer(call.parameters()), boost::asio::transfer_all());

        // queue up the finising of the call
        service.post(boost::bind(&client::complete_call, this, next_request_id));

        // done with this request_id
        next_request_id++;

        return handler_r;
    }
protected:
    void prepare_async_read()
    {
        boost::asio::async_read(socket, result_header.as_buffer(),
        boost::bind(&client::read_result_header, this, boost::asio::placeholders::error));
    }
private:
    requests_type::iterator find_request(id_type id)
    {
        requests_type::iterator it = requests.find(id);
        if (it == requests.end())
        {
            // unknown request!!!
            std::cerr << "unknown request " << result_header.request_id << std::endl;
        }
        return it;
    }
    void complete_call(id_type id)
    {
        boost::mutex::scoped_lock lock(mutex);

        requests_type::iterator it = find_request(id);
        boost::shared_ptr<handler_base> handler_ptr = it->second;
        if (handler_ptr->options.marshal_option == call_options::no_results)
            requests.erase(it);

        protocol::call_footer footer(handler_ptr->options);
        boost::asio::write(socket,footer.as_buffer(), boost::asio::transfer_all());

        sending = false;
        clear_to_send.notify_one();
    }

    template<typename ReturnType>
    shared_ptr<handler_base> handler_accepted_as(id_type id,
        call_options::marshal_options marshal_option)
    {
        BOOST_ASSERT(mutex_lock.locked());

        requests_type::iterator it = find_request(id);
        if (it->second->options.marshal_option < marshal_option)
            it->second->options.marshal_option = marshal_option;
        shared_ptr<handler_base> handler_ptr = it->second;
        if (handler_ptr->has_out_parameters())
            it->second->options.marshal_option = call_options::all_out_parameters;
        mutex_lock.unlock();
        static_cast<async_returning_handler<ReturnType> *>(handler_ptr.get())
            ->assign_promises();
        return handler_ptr;
    };

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
        requests_type::iterator it = find_request(result_header.request_id);
        boost::shared_ptr<handler_base> handler_ptr = it->second;
        requests.erase(it);

        // process the results
        if (handler_ptr->options.marshal_option >= call_options::return_only)
        {
            handler_ptr->result_string(
                std::string(result_marshal_buffer, result_header.marshal_size));
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
    boost::mutex::scoped_lock mutex_lock;
    boost::condition clear_to_send;
    requests_type requests;

    volatile bool sending;

//    template<typename ReturnType>
//    friend class handler_return;

    template<typename T>
    class handler_return
    {
    public:
        typedef client<Registry> client_type;
        handler_return(client_type *client_ptr, typename client_type::id_type id)
            : client_ptr(client_ptr), id(id), called(false)
        {}
        handler_return(const handler_return<T> &h_r)
            : client_ptr(h_r.client_ptr), id(h_r.id), called(h_r.called)
        {
            h_r.called = true;
        }
        operator T ()
        {
            BOOST_ASSERT(!called);
            called = true;
            boost::future<T> future
                (static_pointer_cast<async_returning_handler<T>, handler_base>
                    (client_ptr->handler_accepted_as<T>(id, call_options::return_only))->return_promise());
            return future;
        }
        operator boost::future<T> ()
        {
            BOOST_ASSERT(!called);
            called = true;
            return static_pointer_cast<async_returning_handler<T>, handler_base>
                (client_ptr->handler_accepted_as<T>(id, call_options::return_only))->return_promise();
        }
        operator boost::shared_ptr<async_returning_handler<T> >()
        {
            BOOST_ASSERT(!called);
            called = true;
            return static_pointer_cast<async_returning_handler<T>, handler_base>
                (client_ptr->handler_accepted_as<T>(id, call_options::return_only));
        }
        operator shared_ptr<acknowledgement>()
        {
            BOOST_ASSERT(!called);
            called = true;
            shared_ptr<acknowledgement> ack = static_pointer_cast<acknowledgement, handler_base>
                (client_ptr->handler_accepted_as<T>(id, call_options::return_only));
            return ack;
        }
        ~handler_return()
        {
            if (!called) client_ptr->handler_accepted_as<T>(id, call_options::no_results);
        }
    private:
        client_type *client_ptr;
        typename client_type::id_type id;
        mutable bool called;

//        friend class client<Registry>;
    };
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
        connector.connect(client<Registry>::socket);
//        protocol::message_reader<boost::asio::ip::tcp::socket> reader(socket);


        // Wait for acknowledgement on the socket.
        //if (reader.message() != protocol::message::ok)
        if (protocol::read_message(client<Registry>::socket) != protocol::message::ok)
        {
           // protocol error
           std::cerr << "client protocol error" << std::endl;
        }

        client<Registry>::prepare_async_read();
    }
private:
    Connector connector;
};

} // namespace rpc
} // namespace boost

#endif BOOST_RPC_CLIENT_HPP