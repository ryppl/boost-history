// Copyright (C) 2002 Michel André (michel@andre.net), Hugo Duncan
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  Michel André (michel@andre.net) and
// Hugo Duncan makes no representations about the suitability of this 
// software for any purpose. It is provided "as is" without express or 
// implied warranty.

#ifdef _MSC_VER
#pragma warning (push, 4)
#pragma warning (disable: 4786 4305 4244)
  // 4786 truncated debug symbolic name
  // 4305 truncation from const double to float
  // 4244: 'argument' : conversion from 'boost::gregorian::months_of_year' to 'boost::CV::constrained_value<value_policies>::value_type', possible loss of data
#pragma warning (disable: 4275) // non dll-interface class
#endif

#if defined(__BORLANDC__)
#pragma hdrstop
#endif


#include "boost/socket/socket_exception.hpp"
#include "boost/socket/socket_errors.hpp"
#include "boost/assert.hpp"
#include "boost/shared_array.hpp"

#include <Winsock2.h> // Microsoft winsock extensions
#include <mswsock.h> // Microsoft winsock extensions

#include "overlapped.hpp"

#ifdef _MSC_VER

#endif


namespace boost
{
    namespace socket
    {
        namespace impl
        {
            namespace 
            {
                struct accept_completer
                {
                    accept_completer(
                        overlapped::completion_callback_t callback, std::pair<void*, size_t> remoteAddr) : 
                        m_remoteAddr(remoteAddr),
                        m_addressbufferSize(2*(remoteAddr.second+16)),
                        m_addressbuffer(new unsigned char[m_addressbufferSize]),
                        m_callback(callback)
                    {
                    }

                    
                    void operator()(socket_errno err, unsigned int bytestransferred)
                    {
                        if (err == Success)
                        {
                            ::SOCKADDR* local = NULL;
                            ::SOCKADDR* remote = NULL;
                            INT localLen = 0;
                            INT remoteLen = 0;
                            ::GetAcceptExSockaddrs(
                                address_buffer(), 0, address_buffer_length(), address_buffer_length(),
                                &local, &localLen, &remote, &remoteLen);
                            std::memcpy(
                                m_remoteAddr.first,
                                remote, std::min<INT>(m_remoteAddr.second, remoteLen));
                        }
                        m_callback(err, 0);
                    }

                    void* address_buffer() { return m_addressbuffer.get(); };
                    size_t address_buffer_length() { return m_addressbufferSize / 2; }

                private:
                    std::pair<void*, size_t>           m_remoteAddr;
                    size_t                             m_addressbufferSize;
                    boost::shared_array<unsigned char> m_addressbuffer;
                    overlapped::completion_callback_t  m_callback;
                };
            }

            default_asynch_socket_impl::default_asynch_socket_impl()
                : default_socket_impl()
            {}

            default_asynch_socket_impl::default_asynch_socket_impl(socket_t socket)
                : default_socket_impl(socket)
            {}

            default_asynch_socket_impl::~default_asynch_socket_impl()
            {}

            socket_errno default_asynch_socket_impl::async_accept(
                    default_asynch_socket_impl& newSocket,
                    std::pair<void *,size_t>& remoteAddress,
                    completion_callback_t completionRoutine)
            {
                BOOST_ASSERT(!completionRoutine.empty());
                
                accept_completer completer(completionRoutine, remoteAddress);
                std::auto_ptr<overlapped> overlapped(new overlapped(completer, NULL, 0));

                DWORD bytesReceived = 0;
                BOOL ret = ::AcceptEx(
                              socket(), 
                              newSocket.socket(),
                              completer.address_buffer(),
                              0,
                              completer.address_buffer_length(),
                              completer.address_buffer_length(),
                              &bytesReceived,
                              overlapped->os_overlapped()); 
                
                if (ret || ::GetLastError() == ERROR_IO_PENDING)
                {
                    overlapped.release(); // release the overlapped structure to the os
                    return Success;
                }

                return translate_error(ret ? 0 : -1);
            }

            socket_errno default_asynch_socket_impl::async_connect(
                std::pair<void *,size_t>&,
                completion_callback_t completionRoutine)
            {
                BOOST_ASSERT(!completionRoutine.empty());
                BOOST_ASSERT(false); // not implemented
                return Success;
            }

            socket_errno default_asynch_socket_impl::async_recv(
                void* data, int len,
                completion_callback_t completionRoutine)
            {
                BOOST_ASSERT(!completionRoutine.empty());
                std::auto_ptr<overlapped> overlapped(
                    new overlapped(completionRoutine, const_cast<void*>(data), len));
                DWORD bytesReceived = 0;
                DWORD flags = 0;
                int ret = ::WSARecv(
                    socket(),
                    overlapped->buffer(),
                    1,
                    &bytesReceived,
                    &flags,
                    overlapped->os_overlapped(),
                    NULL); 
               
                if (ret == 0 || ::GetLastError() == WSA_IO_PENDING)
                {
                    overlapped.release(); // release the overlapped structure to the os
                    return Success;
                }

                return translate_error(ret);
            }

            socket_errno default_asynch_socket_impl::async_send(
                const void* data, int len,
                completion_callback_t completionRoutine)
            {
                BOOST_ASSERT(!completionRoutine.empty());
                std::auto_ptr<overlapped> overlapped(
                    new overlapped(completionRoutine, const_cast<void*>(data), len));
                DWORD bytesSent = 0;
                int ret = ::WSASend(
                    socket(),
                    overlapped->buffer(),
                    1,
                    &bytesSent,
                    0,
                    overlapped->os_overlapped(),
                    NULL); 
   
                if (ret == 0 || ::GetLastError() == WSA_IO_PENDING)
                {
                    overlapped.release(); // release the overlapped structure to the os
                    return Success;
                }

                return translate_error(ret);
            }
        }// namespace
    }// namespace
}// namespace

#ifdef _MSC_VER
#pragma warning (pop)
#endif
