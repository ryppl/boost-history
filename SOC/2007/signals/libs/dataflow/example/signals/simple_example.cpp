// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//[ simple_example

#include <boost/dataflow/signals/component/storage.hpp>
#include <boost/dataflow/signals/component/timed_generator.hpp>
#include <boost/dataflow/signals/connection.hpp>
#include "simple_example_components.hpp"


using namespace boost;

int main(int, char* [])
{
    // For our data source, we will use timed_generator,
    // which creates its own thread and outputs it's stored value
    // at a specified time interval.  We'll store a value of 0 to be sent out.
    // The signature void(double) specifies that the signal carries a double,
    // and that there is no return value.
    signals::timed_generator<void (double)> input(0);
    
    // Data processor and output:
    processor proc;
    output out;
    
    // ---Connect the dataflow network ---------------------
    //
    //     ,---------.     ,---------.     ,---------.
    //     |  input  | --> |  proc   | --> |   out   |
    //     `---------'     `---------'     `---------'
    //
    // -----------------------------------------------------
    input >>= proc >>= out;

    // If you prefer, you can also do:
    // connect(input, proc);
    // connect(proc, out);

    // Tell the source to start producing data, every 0.5s:
    input.enable(0.5);
    
    // take a little nap :-)
    boost::xtime xt;
    boost::xtime_get(&xt, boost::TIME_UTC);
    xt.sec += 10;
    boost::thread::sleep(xt);

    input.join();
    return 0;
}

//]
