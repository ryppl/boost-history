// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//[ applicator_example

#include <boost/dataflow/signals/component/applicator.hpp>
#include <boost/dataflow/signals/component/storage.hpp>
#include <boost/dataflow/signals/connection.hpp>

#include <iostream>

using namespace boost;

// A function that will be executed each time a signal passes.
template<typename T>
struct postincrement_and_log
{
    void operator()(T &t)
    {
        t++;
        std::cout << "Signal number " << t << " has passed." << std::endl;
    }
};

// We will now implement a logging counter component that can be used with
// any signal signature.  It will increment a counter and display a message
// with each passing signal.
template<typename Signature>
class logging_counter : public boost::signals::applicator<
        // the first parameter is the derived class
        logging_counter<Signature>,
        // the second parameter determines the type of conditional::member,
        // which we will use to store the number of signals passed.
        unsigned,
        // the third parameted specifies a function which will be applied to the
        // member with each passing signal.
        postincrement_and_log<unsigned>,
        // the fourth parameter determines the signal Signature the component
        // consumes and produces.
        Signature>
{
public:
    // Initializes the internal counter to 0.
    logging_counter()
    {   reset(); }
    
    // Sets the internal counter to 0.
    void reset()
    {   logging_counter::member = 0; }

    // Return the internal signal counter.
    unsigned count() const
    {   return logging_counter::member; }
};

int main()
{
    signals::storage<void ()> producer;
    logging_counter<void ()> counter;
        
    producer >>= counter;

    BOOST_ASSERT(counter.count() == 0);

    producer.send();
    producer.send();
    BOOST_ASSERT(counter.count() == 2);

    return 0;
}

//]