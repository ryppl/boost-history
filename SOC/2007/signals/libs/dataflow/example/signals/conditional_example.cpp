// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//[ conditional_example

#include <boost/dataflow/signals/component/conditional.hpp>
#include <boost/dataflow/signals/component/counter.hpp>
#include <boost/dataflow/signals/component/storage.hpp>
#include <boost/dataflow/signals/connection.hpp>

using namespace boost;

// just an identity function
template<typename T>
struct identity
{
    T operator ()(T t) const
    {
        return t;
    }
};

// We will implement a generic gate, which be used with any signal signature.
// To do so, we inherit from the conditional class.
template<typename Signature>
class my_gate
    : public boost::signals::conditional<
        // the first parameter is the derived class
        my_gate<Signature>,
        // the second parameter determines the type of conditional::member,
        // which we will use to store whether the gate is open or closed.
        bool,
        // the third parameted specifies a function which will be used on the
        // member to determine whether conditional should forward the signal
        // or not.  In our case, it is just identity.
        identity<bool>,
        // the fourth parameter determines the signal Signature the component
        // consumes and produces.
        Signature>
{
public:
    // Initialize the gate to be opened.
    my_gate(bool opened=true)
    {
        my_gate::member=opened;
    }
    // Opens the gate (signals will be forwarded).
    void open()
    {
        my_gate::member = true;
    }
    // Closes the gate (signals will not be forwarded).
    void close()
    {
        my_gate::member = false;
    }
};

int main()
{
    signals::storage<void ()> producer;
    signals::counter<void ()> consumer;
    my_gate<void ()> gate;
        
    producer >>= gate >>= consumer;

    producer.send();
    BOOST_ASSERT(consumer.count() == 1);

    gate.close();
    
    producer.send();
    BOOST_ASSERT(consumer.count() == 1);
    
    gate.open();
    
    producer.send();
    BOOST_ASSERT(consumer.count() == 2);
    return 0;
}

//]