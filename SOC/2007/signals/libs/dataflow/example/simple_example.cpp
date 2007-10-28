// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//[ simple_example

#include <boost/dataflow/signal/component/storage.hpp>
#include <boost/dataflow/signal/component/timed_generator.hpp>
#include <boost/dataflow/signal/connection.hpp>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <iostream>

using namespace boost;

// This will be our data processor.  The signature void(double) designates
// the output signal (we will be sending out an double).  The signals
// we can receive depend on how we overload operator().
class processor : public signals::filter<void (double)>
{
public:
    // Initialize the Gaussian noise generator.
    processor() : generator(mt, dist) {}
    
    // Receive void(double) signals, add some Gaussian noise, and send
    // out the modified value.
    void operator()(double x)
    {
        out(x + generator());
    }
private:
    mt19937 mt;
    normal_distribution<> dist;
    boost::variate_generator<mt19937&, boost::normal_distribution<> > generator;
};

// This will be our data output.  We just need to make a function object,
// and specify that it is a signals::call_consumer.
class output
{
public:
    typedef dataflow::signals::call_consumer port_traits;
    
    void operator()(double x)
    {
        std::cout << x << std::endl;
    }
};

int main(int, char* [])
{
    // For our data source, we will use timed_generator,
    // which creates its own thread and outputs it's stored value
    // at a specified time interval.  We'll store a value of 0 to be sent out.
    // The signature void(double) specifies that the signal carries a double,
    // and that there is no return value.
    signals::timed_generator<void (double), signals::unfused> input(0);
    
    // Data processor and output:
    processor proc;
    output out;
    
    // Now connect the network:
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