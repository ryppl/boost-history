/* example to demonstrate compile-in-place for the signals lib

Compile with
  $CXX -I path/to/boost_X_YY_Z test-signals.cpp

Notes:

$Id$
*/

#define BOOST_SIGNALS_COMPILE_IN_PLACE
#include <iostream>
#include <ostream>
#include <boost/signal.hpp>
#include <boost/signals/compile_in_place.cpp>

struct HelloWorld
{
    void operator()() const 
    {
        std::cout << "Hello, World!" << std::endl;
    }
};


int main()
{
    // Signal with no arguments and a void return value
    boost::signal<void ()> sig;

    // Connect a HelloWorld slot
    HelloWorld hello;
    sig.connect(hello);

    // Call all of the slots
    sig();
}

