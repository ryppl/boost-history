// Copyright (C) 2002 Michel André (michel@andre.net)

// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  Hugo Duncan makes no representations
// about the suitability of this software for any purpose.
// It is provided "as is" without express or implied warranty.

#ifdef _MSC_VER
#pragma warning (disable: 4786 4305)
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

#include <iostream>
#include <cassert>

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

    BOOST_MESSAGE("starting");

    ip4::address addr;
    addr.port(3234);
    addr.ip("127.0.0.1");
    BOOST_MESSAGE(addr.ip());

    ip4::tcp_protocol protocol;

    typedef connector<> connector_t;
    typedef connector_t::data_connection_t data_connection_t;
 
    connector_t connector;

    boost::socket::time_t timeout(0,0,10,0);
    data_connection_t socket;
    BOOST_CHECK(connector.connect(socket,protocol,addr)==boost::socket::Success);
    BOOST_CHECK(socket.is_open());
//     BOOST_CHECK(socket.setsockopt(boost::socket::option::linger(1000))
//                 ==boost::socket::Success);
    BOOST_MESSAGE("Connected");

    boost::socket::basic_socket_stream<char> s(socket);
    for (int k=0;k<10;++k)
    {
      s << "command";
      s.flush();
    }

    BOOST_CHECK(socket.close()==boost::socket::Success);
  }
  catch (const socket_exception& s)
  {
    std::cerr << s.message() << std::endl;
  }

}

struct client
{
    typedef boost::socket::async_acceptor::data_socket_t data_socket_t;
    
    enum state
    {
        state_disconnected,
        state_receiving,
        state_replying,
        state_done
    };
    
    client() : m_state(state_disconnected) {}
    
    void on_accepted()
    {
        BOOST_CHECK(m_state == state_disconnected):
        m_state = state_receiving;
        m_socket.async_recv(
            &*m_receiveBuffer.begin(), m_receiveBuffer.size(),
            boost::bind(&client::on_read_complete, this));
        
    }
    
    void on_read_complete(boost::socket::socket_errno err, unsigned int bytesRead)
    {
        BOOST_CHECK(bytesRead > 0);
        BOOST_CHECK(err == boost::socket::Success);
        BOOST_CHECK(m_state == state_receiving):
        if (bytesRead == m_receiveBuffer.size())
        {
            m_sendBuffer = "reply";
            m_socket.async_send(
                &*m_sendBuffer.begin(), m_sendBuffer.size(),
                boost::bind(&client::on_write_complete, this));
            m_state = state_sending;
        }
        
    }
    
    void on_write_complete(boost::socket::socket_errno err, unsigned int bytesWritten)
    {
        BOOST_CHECK(bytesWritten == m_sendBuffer.size());
        BOOST_CHECK(err == boost::socket::Success);
        BOOST_CHECK(m_state == state_sending):
        BOOST_CHECK(m_socket.close() == boost::socket::Success);
        m_state == state_done;
    }

    bool is_done() const 
    {
        return m_state == state_done;
    }
    

    data_socket_t& socket() 
    {
        return m_socket;
    }
    
private:
    
    data_socket_t m_socket;
    std::string   m_receiveBuffer;
    std::string   m_sendBuffer;
    state         m_state;
    
};

void accept_test()
{
    
    boost::socket::async_acceptor acceptor;

    ip4::address addr;
    addr.port(3234);
    addr.ip("127.0.0.1");

    boost::socket::proactor proactor;
    BOOST_CHECK(acceptor.open(ip4::tcp_protocol(), addr, proactor) == boost::socket::Success);
    
    std::vector<client> clients(10);
    for(std::vector<client>::iterator iter = clients.begin(); iter != clients.end(); ++iter)
        acceptor.accept(
            iter->socket(),
            boost::bind(&acceptedClient::on_accept, &acceptedClient),
            proactor);
    
}

//----------------------------------------------------------------------------
test_suite* init_unit_test_suite( int , char*[] )
{
  test_suite* test= BOOST_TEST_SUITE("socket.asynch_accept test");
  test->add( BOOST_TEST_CASE( client_test ));
  return test;
}

#ifdef _MSC_VER
#pragma warning (pop)
#endif
