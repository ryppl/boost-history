//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include "boost/synchro/alt/alternating_component.hpp"

using namespace boost::alt;
using namespace boost;

//[TrafficLight
class  factorial {
    unsigned value[100];
    unsigned top_
    unsigned operator()(unsigned n) {
        T[0]=top_=0;
        for(;;) {
            if (top_<n) {
                for (i=top_+1; i<=n; ++i) {
                    value_[i]=value_[i-1]*i
                }
                top_ = n;
            }
            ++n;
            alternating_component::yield(value_[n-1]);
        }
    }
};

class Controller {
    alternating_component<TrafficLight> north, south;
public:
    Controller( alternating_component<TrafficLight> north, south)
    void operator()() {
        north.resume(); // north.state_==red
        south.resume();
        south.resume(); // south.state_==green
        for(;;) {
            // wait some time
            sleep(2);
            // switch the states
            south.resume();
            north.resume();
        }
    }
};

//]

int main()
{
    alternating_component<TrafficLight> north, south;
    alternating_component<Controller> controler(north, south); // Declaration of a singular component
    controler.start();
    return 0;
}


