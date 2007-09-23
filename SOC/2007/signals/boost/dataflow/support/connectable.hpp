// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_CONNECTABLE_HPP
#define BOOST_DATAFLOW_SUPPORT_CONNECTABLE_HPP

#include <boost/dataflow/support/proxy_producer.hpp>
#include <boost/dataflow/support/proxy_consumer.hpp>

#include <boost/static_assert.hpp>


namespace boost { namespace dataflow {

/*namespace concepts
{
    struct producer;
    struct single_type_producer;
    struct keyed_producer;
}*/

namespace extension
{
    template<typename Mechanism, typename ProducerTag, typename ConsumerTag, typename Enable=void>
    struct connect_impl
    {
        template<typename Producer, typename Consumer>
        struct apply
        {
            static void call(const Producer &, const Consumer &)
            {
                // Error: connect_impl has not been implemented for ProducerTag
                // and ConsumerTag.
                BOOST_STATIC_ASSERT(sizeof(Producer)==0);
            }
        };
    };
    
/*    template<typename Mechanism, typename ProducerTag, typename ConsumerTag,
        typename enable_if<
            boost::is_same<ProducerTag, typename concepts::keyed_producer>
        >::type >
    struct connect_impl
    {
        template<typename Producer, typename Consumer>
        struct apply
        {
            static void call(Producer &producer, Consumer &consumer)
            {
                connect(get_keyed_producer<Mechanism>(producer, consumer), consumer);
            }
        };
    };*/
}

template<typename Mechanism, typename Producer, typename Consumer>
inline void connect(Producer &producer, Consumer &consumer)
{
    extension::connect_impl<
        Mechanism,
        typename producer_category_of<Mechanism, Producer>::type,
        typename consumer_category_of<Mechanism, Consumer>::type>
            ::template apply<
                typename get_proxied_producer_type<Mechanism, Producer>::type,
                typename get_proxied_consumer_type<Mechanism, Consumer>::type
            >::call(get_proxied_producer<Mechanism>(producer),
                    get_proxied_consumer<Mechanism>(consumer));
}

template<typename Mechanism, typename Producer, typename Consumer>
inline void connect(const Producer &producer, Consumer &consumer)
{
    extension::connect_impl<
        Mechanism,
        typename producer_category_of<Mechanism, Producer>::type,
        typename consumer_category_of<Mechanism, Consumer>::type>
            ::template apply<
                typename get_proxied_producer_type<Mechanism, Producer>::type,
                typename get_proxied_consumer_type<Mechanism, Consumer>::type
            >::call(get_proxied_producer<Mechanism>(producer),
                    get_proxied_consumer<Mechanism>(consumer));
}

template<typename Mechanism, typename Producer, typename Consumer>
inline void connect(Producer &producer, const Consumer &consumer)
{
    extension::connect_impl<
        Mechanism,
        typename producer_category_of<Mechanism, Producer>::type,
        typename consumer_category_of<Mechanism, Consumer>::type>
            ::template apply<
                typename get_proxied_producer_type<Mechanism, Producer>::type,
                typename get_proxied_consumer_type<Mechanism, Consumer>::type
            >::call(get_proxied_producer<Mechanism>(producer),
                    get_proxied_consumer<Mechanism>(consumer));
}

template<typename Mechanism, typename Producer, typename Consumer>
inline void connect(const Producer &producer, const Consumer &consumer)
{
    extension::connect_impl<
        Mechanism,
        typename producer_category_of<Mechanism, Producer>::type,
        typename consumer_category_of<Mechanism, Consumer>::type>
            ::template apply<
                typename get_proxied_producer_type<Mechanism, Producer>::type,
                typename get_proxied_consumer_type<Mechanism, Consumer>::type
            >::call(get_proxied_producer<Mechanism>(producer),
                    get_proxied_consumer<Mechanism>(consumer));
}

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_CONNECTABLE_HPP
