// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/signal/component/storage.hpp>
#include <boost/dataflow/signal/component/counter.hpp>
#include <boost/dataflow/signal/connection.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

using namespace boost;
using namespace boost::dataflow::operators;

int test_main(int, char* [])
{
    {
        //[ test_disconnect_unfused
        signals::storage<void (), signals::unfused> banger;
        {
            signals::counter<void (), signals::unfused> counter;
            signals::storage<void (float), signals::unfused> floater;
            floater(2.5f);
            signals::storage<void (float), signals::unfused> collector(0.0f);
            
            banger
                | counter
                | (floater >>= collector).send_slot();
            
            banger();
            BOOST_CHECK_EQUAL(counter.count(), 1);
            BOOST_CHECK_EQUAL(collector.at<0>(), 2.5f);
        } // counter, floater, and collector are now destroyed and disconnected with Boost.Signals
#ifdef SIGNAL_NETWORK_THREAD_SAFE
        // if [DataflowSignals] has detected thread safe signals, we need to
        // touch the signal to disconnect things that are destroyed:
        // unfortunatelly, this call hangs on MSVC!
        // banger();
#endif
        BOOST_CHECK_EQUAL(banger.default_signal().num_slots(), 0u); 
        
        signals::counter<void (), signals::unfused> counter;
        
        banger >>= counter;
        banger.disconnect_all_slots();
        
        banger();
        BOOST_CHECK_EQUAL(counter.count(), 0);
        //]
    }
    return 0;
} // int test_main(int, char* [])


