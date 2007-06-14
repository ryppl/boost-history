// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/signal_network/storage.hpp>
#include <boost/signal_network/counter.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

// for access to connection operators >>= and |
using namespace boost::signal_network;
using namespace boost;

int test_main(int, char* [])
{
    {
        //[ test_disconnect_unfused
        signet::storage<void ()>::unfused banger;
        {
            signet::counter<void ()>::unfused counter;
            signet::storage<void (float)>::unfused floater;
            floater(2.5f);
            signet::storage<void (float)>::unfused collector(0.0f);
            
            banger
                | counter
                | (floater >>= collector);
            
            banger();
            BOOST_CHECK_EQUAL(counter.count(), 1);
            BOOST_CHECK_EQUAL(collector.at<0>(), 2.5f);
        } // counter, floater, and collector are now destroyed and disconnected with Boost.Signals
#ifdef SIGNAL_NETWORK_THREAD_SAFE
        // if Signal Network has detected thread safe signals, we need to
        // touch the signal to disconnect things that are destroyed:
        // unfortunatelly, this call hangs on MSVC!
        // banger();
#endif
        BOOST_CHECK_EQUAL(banger.default_signal().num_slots(), 0u); 
        
        signet::counter<void ()>::unfused counter;
        
        banger >>= counter;
        banger.disconnect_all_slots();
        
        banger();
        BOOST_CHECK_EQUAL(counter.count(), 0);
        //]
    }
    return 0;
} // int test_main(int, char* [])


