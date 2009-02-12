//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/synchro/alt/alternating_component.hpp>

using namespace boost::alt;
using namespace boost;

namespace color {
    enum type {red, green};
}

//[TrafficLight
# if 0
class TrafficLight : public coroutine_obj<TrafficLight, void(void)> {
    typedef coroutine_obj<TrafficLight, void(void)> super_type;
    friend class super_type;
    static void body(coroutine_type::self& this_coroutine) {
        for(;;) {
            state_ = color::red;
            this_coroutine.yield();
            state_ = color::green;
            this_coroutine.yield();
        }
    }
    color::type state_;
public:
    TrafficLight() : state_(color::red) {}
};

class Controller {
    TrafficLight north_, south_;
    operator() body() {
        north_(); // north.state_==red
        south_();
        south_(); // south.state_==green
        for(;;) {
            // wait some time
            sleep(2);
            // switch the states
            south_();
            north_();
        }
    }
public:
    Controller(TrafficLight& north, TrafficLight& south);
};
#endif
typedef coro::coroutine<void()> TrafficLight;

void TrafficLightBody(TrafficLight::self& this_coroutine) {
    color::type state_=color::red;
       for(;;) {
           state_ = color::red;
           this_coroutine.yield();
           state_ = color::green;
           this_coroutine.yield();
       }
}

void Controller(TrafficLight north_, TrafficLight south_) {
       north_(); // north.state_==red
       south_();
       south_(); // south.state_==green
       for(;;) {
           // wait some time
           sleep(2);
           // switch the states
           south_();
           north_();
       }
}

int main()
{
    TrafficLight north(TrafficLightBody);
    TrafficLight south(TrafficLightBody);
    Controller controler(north, south); // Declaration of a singular component
    return 0;
}


class TrafficLight {
    color::type state_;
public:
    TrafficLight() : state_(color::red) {}
    void operator()() {
        for(;;) {
            state_ = color::red;
            alternating_component::suspend();
            state_ = color::green;
            alternating_component::suspend();
        }
    }
};

class Controller {
    alternating_component<TrafficLight> north, south;
public:
    Controller( alternating_component<TrafficLight> north, alternating_component<TrafficLight> south)
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


