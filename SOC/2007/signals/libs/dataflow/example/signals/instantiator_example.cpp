// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//[ instantiator_example

#include <boost/dataflow/signals/component/filter.hpp>
#include <boost/dataflow/signals/component/instantiator.hpp>
#include <boost/dataflow/signals/component/timed_generator.hpp>
#include <boost/dataflow/signals/connection.hpp>
#include <boost/thread/mutex.hpp>

#include <string>

using namespace boost;

// Ensures a component is processing only one signal at a time when using multiple threads.
// To do so, we need to instantiate a scoped lock over a mutex each time we
// are handling a signal.  We can inherit from instantiator to accomplish this.
template<typename Signature>
class my_mutex : public
    boost::signals::instantiator<
        // the first parameter is the derived class
        my_mutex<Signature>,
        // the second parameter determines the type of conditional::member,
        // which we will be our mutex.
        boost::mutex,
        // the third parameter determines the object which will be instantiated
        // with the member as the constructor argument.
        // In our case, that is the scoped_lock.
        boost::mutex::scoped_lock,
        // the fourth parameter determines the signal Signature the component
        // consumes and produces.
        Signature>
{
    // that's it.
};

// a consumer component, to occupy some time.
class printer : public boost::signals::consumer<printer>
{
public:
    void operator()(const std::string &message)
    {
        for (int j=0; j<3; j++)
            for (int i=0; i<10; i++)
                std::cout << message << i;            
        std::cout << std::endl;
    }
};

int main()
{
    // timed_generators send signals in their own thread
    signals::timed_generator<void ()> timer1;
    signals::timed_generator<void ()> timer2;
    
    // other components
    my_mutex<void ()> lock;
    signals::storage<void (const std::string &)> producer("#");
    printer consumer;
    
    // first, with the mutex:
    std::cout << "With mutex:" << std::endl;
    producer >>= consumer;
    timer1 >>= lock >>= producer.send_slot();
    timer2 >>= lock;

    // set each timer to send a signal 5 times, in 0s intervals (i.e., instantly)
    timer1.enable(0.0, 5);
    timer2.enable(0.0, 5);

    // wait until they are done.
    timer1.wait_until_completed();
    timer2.wait_until_completed();


    // now, without the mutex (reconnect timer1 and timer2 directly to producer)
    std::cout << "Without mutex:" << std::endl;
    disconnect_all(timer1);
    disconnect_all(timer2);
    
    timer1 >>= producer.send_slot();
    timer2 >>= producer.send_slot();
    
    // set each timer to send a signal 5 times, in 0s intervals (i.e., instantly)
    timer1.enable(0.0, 5);
    timer2.enable(0.0, 5);

    // wait until they are done.
    timer1.wait_until_completed();
    timer2.wait_until_completed();
    
    timer1.join();
    timer2.join();
    return 0;
}

//]