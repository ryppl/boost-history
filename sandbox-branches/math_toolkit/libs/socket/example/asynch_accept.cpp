// Copyright (C) 2002 Michel André (michel@andre.net)

// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  Hugo Duncan makes no representations
// about the suitability of this software for any purpose.
// It is provided "as is" without express or implied warranty.

#ifdef _MSC_VER
#pragma warning (disable: 4786 4305 4355)
// 4786 truncated debug symbolic name
// 4305 truncation from const double to float
#endif

#if defined(__BORLANDC__)
#pragma hdrstop
#endif

#include "boost/socket/ip4.hpp"
#include "boost/socket/socketstream.hpp"
#include "boost/socket/socket_exception.hpp"
#include "boost/socket/connector_socket.hpp"
#include "boost/socket/asynch_acceptor.hpp"
#include "boost/socket/proactor.hpp"

#include "boost/utility.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/thread/thread.hpp"
#include <iostream>
#include <cassert>
#include <list>

#include "boost/test/unit_test.hpp"
using namespace boost::unit_test_framework;

#ifdef _MSC_VER
#pragma warning (push, 4)
#pragma warning (disable: 4786 4305)
#endif

using namespace boost::socket;

void client_thread()
{
    try
    {
        
        BOOST_MESSAGE("client: starting");
        
        ip4::address addr;
        addr.port(6666);
        addr.ip("127.0.0.1");
        
        ip4::tcp_protocol protocol;
        
        typedef connector<> connector_t;
        typedef connector_t::data_connection_t data_connection_t;

        connector_t connector;

        //boost::socket::time_t timeout(0,0,10,0);
        data_connection_t socket;
        BOOST_CHECK(connector.connect(socket,protocol,addr)==boost::socket::Success);
        BOOST_CHECK(socket.is_open());
//     BOOST_CHECK(socket.setsockopt(boost::socket::option::linger(1000))
//                 ==boost::socket::Success);
        BOOST_MESSAGE("client: connected");
        
        boost::socket::basic_socket_stream<char> s(socket);
        //for (int k=0;k<10;++k)
        {
            BOOST_MESSAGE("client: sending");
            s << "command";
            s.flush();
            std::string reply;
            s >> reply;
            
            BOOST_CHECK_EQUAL(reply, "reply");
            BOOST_MESSAGE("client: reply");
        }

        BOOST_CHECK(!socket.is_open());
        BOOST_MESSAGE("client: done");
    }
    catch (const socket_exception& s)
    {
        std::cerr << s.message() << std::endl;
    }
}

struct client : boost::noncopyable
{
    typedef boost::socket::asynch_acceptor<boost::socket::proactor>::data_connection_t data_connection_t;
    
    enum state
    {
        state_disconnected,
        state_receiving,
        state_replying,
        state_done
    };
    
    client(boost::socket::proactor& proactor) :
        m_connection(proactor),
        m_receiveBuffer(10, ' '),
        m_state(state_disconnected)
    {
        m_connection.open(ip4::tcp_protocol());  
        proactor.attach(m_connection.socket());
    }
    
    void on_accepted(socket_errno err)
    {
        BOOST_MESSAGE("server: accepted client\n");
        BOOST_CHECK(m_state == state_disconnected);
        BOOST_CHECK(err == boost::socket::Success);
        
        m_state = state_receiving;
        m_connection.asynch_recv(
            &*m_receiveBuffer.begin(),
            m_receiveBuffer.size(),
            boost::bind(&client::on_read_complete, this, _1, _2)
            );
        
    }
    
    void on_read_complete(boost::socket::socket_errno err, unsigned int bytesRead)
    {
        BOOST_MESSAGE("server: data received from client");
        BOOST_CHECK(bytesRead > 0);
        BOOST_CHECK(err == boost::socket::Success);
        BOOST_CHECK(m_state == state_receiving);
        
        //if (m_receiveBuffer.size())
        {
            m_state = state_replying;
            m_sendBuffer = "reply";
            m_connection.asynch_send(
                &*m_sendBuffer.begin(),
                m_sendBuffer.size(),
                boost::bind(&client::on_write_complete, this, _1, _2)
                );
        }
        
    }
    
    void on_write_complete(boost::socket::socket_errno err, unsigned int bytesWritten)
    {
        BOOST_MESSAGE("server: data written to client");
        BOOST_CHECK_EQUAL(bytesWritten, m_sendBuffer.size());
        BOOST_CHECK_EQUAL(err, boost::socket::Success);
        BOOST_CHECK_EQUAL(m_state, state_replying);
        BOOST_CHECK_EQUAL(m_connection.close(), boost::socket::Success);
        m_state = state_done;
    }

    bool is_done() const 
    {
        return m_state == state_done;
    }
    

    data_connection_t& connection() 
    {
        return m_connection;
    }

    ip4::address& address()
    {
        return m_remoteAddress;
        
    }
    
private:
    
    data_connection_t m_connection;
    ip4::address      m_remoteAddress;
    
    std::string   m_receiveBuffer;
    std::string   m_sendBuffer;
    state         m_state;
    
};

void accept_test()
{
    boost::socket::proactor proactor;
    boost::socket::asynch_acceptor<boost::socket::proactor> acceptor(proactor);

    ip4::address addr;
    addr.port(6666);
    addr.ip("127.0.0.1");
    
    BOOST_CHECK(acceptor.open(ip4::tcp_protocol(), addr) == boost::socket::Success);

    typedef boost::shared_ptr<client> client_ptr;
    typedef std::list<client_ptr> client_list_t;
    client_list_t clients;
    boost::thread_group clientThreads;
    for(int i=0;i<10;++i)
    {
        clients.push_back(client_ptr(new client(proactor)));
        clientThreads.create_thread(&client_thread);
    }
    for(client_list_t::iterator iter = clients.begin(); iter != clients.end(); ++iter)
    {
        acceptor.asynch_accept(
            (*iter)->connection(),
            (*iter)->address(),
            boost::bind(&client::on_accepted, iter->get(), _1));
    }
    for(std::size_t i=0;i < clients.size()*3;++i) // 3 ops per client 1 accept/read/write
    {
        boost::xtime timeout;
        boost::xtime_get(&timeout, boost::TIME_UTC);
        timeout.sec += 10;
        if(proactor.dispatch(timeout))
            std::cout << "proactor: timed out" << std::endl;
        std::cout << "proactor: op" << i << std::endl;
    }

    clientThreads.join_all();
}

//----------------------------------------------------------------------------
test_suite* init_unit_test_suite( int , char*[] )
{
    test_suite* test= BOOST_TEST_SUITE("socket.asynch_accept test");
    test->add( BOOST_TEST_CASE( accept_test ));
    return test;
}

#ifdef _MSC_VER
#pragma warning (pop)
#endif
