// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//[ simple_example_components

#include <boost/dataflow/signals/component/filter.hpp>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <iostream>

using namespace boost;

// This will be our data processor.  The signature void(double) designates
// the output signal (we will be sending out a double).  The signals
// we can receive depend on how we overload operator().
class processor : public signals::filter<processor, void (double)>
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

// This will be our data output.  We just make a function object, and specify
// that it is a signal consumer by inheriting the signal::consumer class.
class output : public signals::consumer<output>
{
public:
    void operator()(double x)
    {
        std::cout << x << std::endl;
    }
};

//]
