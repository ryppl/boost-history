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
#endif

#if defined(__BORLANDC__)
#pragma hdrstop
#endif

#include "boost/socket/impl/default_socket_proactor.hpp"
#include "boost/socket/socket_exception.hpp"
#include "boost/socket/socket_errors.hpp"
#include "boost/assert.hpp"
#include "boost/thread/mutex.hpp"

#include <Winsock2.h>
#include <mswsock.h> // Microsoft winsock extensions


#include "overlapped.hpp"
#include <queue>

namespace boost
{
    namespace socket
    {
        namespace impl
        {
            namespace
            {

                DWORD msecs_from_now(boost::posix_time::ptime time)
                {
                    boost::posix_time::time_duration duration = time - boost::posix_time::second_clock::universal_time();
                    static const __int64 nanosPerMsec = 100;
                    if (duration.ticks() > 0)
                        return duration.ticks() / nanosPerMsec;
                    else
                        return 0;
                }

                struct timer
                {

                    timer(default_socket_proactor::ptime fireTime, 
                          default_socket_proactor::timer_callback_t callback) :
                        m_fireTime(fireTime),
                        m_callback(callback)
                    {}

                    bool operator<(const timer& rhs) const { return m_fireTime < rhs.m_fireTime; }

                    default_socket_proactor::ptime time() const 
                    {
                        return m_fireTime;
                    }


                    default_socket_proactor::ptime fire() 
                    {
                        m_fireTime = m_callback();
                        return m_fireTime;
                    }
                    
                private:
                    default_socket_proactor::ptime             m_fireTime;
                    default_socket_proactor::timer_callback_t  m_callback;

                };
            }

            class default_socket_proactor::impl : boost::noncopyable
            {
            public:

                impl()
                {
                    m_completionPort = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
                    BOOST_ASSERT(m_completionPort != NULL);
                }

                ~impl()
                {
                    ::CloseHandle(m_completionPort);
                }

                bool attach(default_asynch_socket_impl::socket_t socket)
                {
                    HANDLE port = ::CreateIoCompletionPort((HANDLE)socket, m_completionPort, (ULONG_PTR)(socket), 0);
                    return port == m_completionPort;
                }

                bool dispatch(default_socket_proactor::ptime expire)
                {
                    DWORD bytesTransferred = 0;
                    ULONG_PTR completionKey = 0;
                    LPOVERLAPPED osOverlapped = NULL;
                    DWORD millisecs = msecs_from_now(expire);
                    {
                        boost::mutex::scoped_lock lk(m_mutex);
                        if(!m_timerQueue.empty())
                            millisecs = std::min(millisecs, msecs_from_now(m_timerQueue.top().time()));
                    }

                    ::GetQueuedCompletionStatus(
                            m_completionPort,
                            &bytesTransferred,
                            &completionKey,
                            &osOverlapped,
                            millisecs);

                    bool timeout = ::GetLastError() != WAIT_TIMEOUT;
                    if (!timeout)
                    {
                        std::auto_ptr<overlapped> ol = std::auto_ptr<overlapped>(overlapped::from_overlapped(osOverlapped));
                        ol->complete(default_socket_impl::translate_error(-1), bytesTransferred);
                    }
                    else
                    {
                        timeout = (dispatch_timers() == 0);
                    }
                    return timeout;
                }
                
                unsigned int dispatch_timers()
                {
                    unsigned int expired = 0;
                    boost::mutex::scoped_lock lk(m_mutex);
                    while(!m_timerQueue.empty() && msecs_from_now(m_timerQueue.top().time()) == 0)
                    {
                        timer expiredTimer = m_timerQueue.top();
                        m_timerQueue.pop();
                        lk.unlock();
                        default_socket_proactor::ptime next = expiredTimer.fire();
                        ++expired;
                        if (next != default_socket_proactor::ptime(0))
                        {
                           boost::mutex::scoped_lock lk(m_mutex);
                           m_timerQueue.push(expiredTimer);
                        }
                        lk.lock();

                    }
                    return expired;
                }

                HANDLE                      m_completionPort;
                boost::mutex                m_mutex;
                std::priority_queue<timer>  m_timerQueue;
            };
            
            default_socket_proactor::default_socket_proactor() : m_impl(new impl)
            {
            }
            
            default_socket_proactor::~default_socket_proactor()
            {
                delete m_impl;
            }
            
            bool default_socket_proactor::attach(default_asynch_socket_impl& socket)
            {
                return m_impl->attach(socket.socket());
            }
            
            bool default_socket_proactor::set_timer(ptime fireTime, timer_callback_t callback)
            {
                boost::mutex::scoped_lock lk(m_impl->m_mutex);
                m_impl->m_timerQueue.push(timer(fireTime, callback));
                return true;
            }
            
            bool default_socket_proactor::dispatch(ptime timeout)
            {
                return m_impl->dispatch(timeout);
            }
            
        }// namespace
    }// namespaces
}// namespace

#ifdef _MSC_VER
#pragma warning (pop)
#endif
