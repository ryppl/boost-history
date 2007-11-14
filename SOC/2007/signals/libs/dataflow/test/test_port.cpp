// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "my_producer_consumer.hpp"

#include <boost/test/included/test_exec_monitor.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/type_traits/is_base_of.hpp>

// for different port registrations

//[ port_registration_example_prep
struct some_mechanism;

typedef df::port_traits<some_mechanism, df::ports::producer, df::concepts::port> producer_traits;
typedef df::port_traits<some_mechanism, df::ports::consumer, df::concepts::port> consumer_traits;

//]

//[ port_registration_example_intrusive

// Intrusive registration of a single port
struct intrusive_producer_port
{
    // intrusive_producer_port is a ProducerPort
    typedef producer_traits port_traits;
};

// Intrusive registration of multiple ports
struct intrusive_producer_consumer_port
{
    // intrusive_filter_port is a ProducerPort and a ConsumerPort
    typedef boost::mpl::vector<producer_traits, consumer_traits> port_traits;
};
//]

//[ port_registration_example_non_intrusive

struct non_intrusive_port
{
// Can't or don't want to modify the type
};

namespace boost { namespace dataflow {

// port_traits_of holds the PortTraits type of a Port, keyed by Mechanism
// and PortCategory
template<>
struct port_traits_of<some_mechanism, df::ports::producer, non_intrusive_port>
{
    typedef producer_traits type;
};

} } // namespace boost::dataflow

//]

//[ port_registration_example_non_intrusive_macro

// Non-intrusive registration via a macro:
struct non_intrusive_port2
{
// Can't or don't want to modify the type
};

DATAFLOW_PORT_TRAITS(non_intrusive_port2, producer_traits)

//]

//[ port_registration_example_non_intrusive_enable_if

// Non-intrusive registration via an enable_if expression
struct non_intrusive_port_base
{
};

namespace boost { namespace dataflow {

// port_traits_of holds the PortTraits type of a Port, keyed by Mechanism
// and PortCategory
template<typename T>
struct port_traits_of<
    some_mechanism,
    df::ports::producer,
    T,
    typename boost::enable_if<
        boost::is_base_of<non_intrusive_port_base, T>
    >::type>
{
    typedef producer_traits type;
};

} } // namespace boost::dataflow

struct non_intrusive_port_descendant : public non_intrusive_port_base
{};

//]

//[ port_registration_example_non_intrusive_enable_if_macro

// Non-intrusive registration via a macro and an enable_if expression
struct non_intrusive_port_base2
{
};

DATAFLOW_PORT_TRAITS_ENABLE_IF(
    T,
    boost::is_base_of<non_intrusive_port_base2 BOOST_PP_COMMA() T>,
    producer_traits)

struct non_intrusive_port_descendant2 : public non_intrusive_port_base2
{};

//]

int test_main(int, char* [])
{
    BOOST_CHECK(df::is_port_category<df::ports::producer>::value);
    BOOST_CHECK(df::is_port_category<df::ports::consumer>::value);
    
    BOOST_CHECK(!df::is_port_category<incomplete>::value);
    BOOST_CHECK(!df::is_port_category<empty>::value);
    
    BOOST_CHECK(df::is_port_traits<my_producer_traits>::value);
    BOOST_CHECK(df::is_port_traits<my_consumer_traits>::value);
    
    BOOST_CHECK(!df::is_port_traits<incomplete>::value);
    BOOST_CHECK(!df::is_port_traits<empty>::value);
    
    BOOST_CHECK(( df::is_port<my_mechanism, df::ports::producer, my_producer>::value ));
    BOOST_CHECK(( df::is_port<my_mechanism, df::ports::consumer, my_consumer>::value ));
    
    BOOST_CHECK(( !df::is_port<my_mechanism, df::ports::producer, incomplete>::value ));
    BOOST_CHECK(( !df::is_port<my_mechanism, df::ports::producer, empty>::value ));
    
    my_producer p;
    
    BOOST_CHECK_EQUAL(&p, (&df::get_port<my_mechanism, df::ports::producer>(p) ));
    BOOST_CHECK(( boost::is_same<df::get_port_result_type<my_mechanism, df::ports::producer, my_producer>::type, my_producer &>::value ));
    
    const volatile my_producer cp(p);
    
    BOOST_CHECK_EQUAL(&cp, (&df::get_port<my_mechanism, df::ports::producer>(cp)));
    BOOST_CHECK(( boost::is_same<df::get_port_result_type<my_mechanism, df::ports::producer, const volatile my_producer>::type, const volatile my_producer &>::value ));
    
    // check registrations
    //[ port_registration_check_example
    BOOST_CHECK(( df::is_port<some_mechanism, df::ports::producer, intrusive_producer_port>::value ));

    BOOST_CHECK(( df::is_port<some_mechanism, df::ports::producer, intrusive_producer_consumer_port>::value ));
    BOOST_CHECK(( df::is_port<some_mechanism, df::ports::consumer, intrusive_producer_consumer_port>::value ));
    
    BOOST_CHECK(( df::is_port<some_mechanism, df::ports::producer, non_intrusive_port>::value ));

    BOOST_CHECK(( df::is_port<some_mechanism, df::ports::producer, non_intrusive_port2>::value ));
    
    BOOST_CHECK(( df::is_port<some_mechanism, df::ports::producer, non_intrusive_port_descendant>::value ));

    BOOST_CHECK(( df::is_port<some_mechanism, df::ports::producer, non_intrusive_port_descendant2>::value ));
    //]
    return 0;
} // int test_main(int, char* [])
