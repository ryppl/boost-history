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
typedef df::port_traits<df::ports::producer> producer_traits;
typedef df::port_traits<df::ports::consumer> consumer_traits;

struct some_tag : public df::default_tag {};

typedef df::port_traits<df::ports::consumer, some_tag> other_consumer_traits;

//]

//[ port_registration_example_intrusive

// Intrusive registration of a single port
struct intrusive_producer_port
{
    // intrusive_producer_port is a ProducerPort
    typedef producer_traits dataflow_traits;
};

// Intrusive registration of a multiple ports
struct intrusive_producer_consumer_port
{
    // intrusive_producer_port is a ProducerPort for the default_tag,
    // and a ConsumerPort for some_tag
    typedef boost::mpl::vector<producer_traits, other_consumer_traits> dataflow_traits;
};
//]

//[ port_registration_example_non_intrusive

struct non_intrusive_port
{
// Can't or don't want to modify the type
};

namespace boost { namespace dataflow {

// register_traits holds the PortTraits type of a Port
template<>
struct register_traits<non_intrusive_port>
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

DATAFLOW_TRAITS(non_intrusive_port2, producer_traits)

//]

//[ port_registration_example_non_intrusive_enable_if

// Non-intrusive registration via an enable_if expression
struct non_intrusive_port_base
{
};

namespace boost { namespace dataflow {

// register_traits holds the PortTraits type of a Port
template<typename T>
struct register_traits<
    T,
    default_tag,
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

DATAFLOW_TRAITS_ENABLE_IF(
    T,
    boost::is_base_of<non_intrusive_port_base2 BOOST_PP_COMMA() T>,
    producer_traits)

struct non_intrusive_port_descendant2 : public non_intrusive_port_base2
{};

//]

struct whatever;

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


    BOOST_CHECK(( df::is_port<my_producer>::value ));
    BOOST_CHECK(( df::is_port<my_consumer>::value ));
    
    BOOST_CHECK(( !df::is_port<empty>::value ));
    
    my_producer p;
                
    // check registrations
    //[ port_registration_check_example
    BOOST_CHECK(( df::is_port<intrusive_producer_port>::value ));
    
    BOOST_CHECK(( df::is_port<intrusive_producer_consumer_port, some_tag>::value ));
    
    BOOST_CHECK(( df::is_port<non_intrusive_port>::value ));

    BOOST_CHECK(( df::is_port<non_intrusive_port2>::value ));
    
    BOOST_CHECK(( df::is_port<non_intrusive_port_descendant>::value ));

    BOOST_CHECK(( df::is_port<non_intrusive_port_descendant2>::value ));
    //]
    
    BOOST_CHECK(( df::has_default_port<my_producer, whatever, whatever>::value ));
    BOOST_CHECK_EQUAL(( &df::get_default_port<whatever, whatever, df::default_tag>(p)), &p );

    return 0;
} // int test_main(int, char* [])
