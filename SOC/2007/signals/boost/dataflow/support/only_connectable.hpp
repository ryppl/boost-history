// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_ONLY_CONNECTABLE_HPP
#define BOOST_DATAFLOW_SUPPORT_ONLY_CONNECTABLE_HPP

#include <boost/dataflow/support/proxy_producer.hpp>
#include <boost/dataflow/support/proxy_consumer.hpp>
#include <boost/dataflow/support/keyed_producer.hpp>
#include <boost/dataflow/support/keyed_consumer.hpp>

#include <boost/static_assert.hpp>


namespace boost { namespace dataflow {

namespace extension
{
    template<typename Mechanism, typename ProducerTag, typename ConsumerTag, typename Enable=void>
    struct connect_only_impl
    {
        template<typename Producer, typename Consumer>
        struct apply
        {
            static void call(const Producer &, const Consumer &)
            {
                // Error: connect_only_impl has not been implemented for ProducerTag
                // and ConsumerTag.
                BOOST_STATIC_ASSERT(sizeof(Producer)==0);
            }
        };
    };
    
    template<typename Mechanism, typename ProducerTag, typename ConsumerTag>
    struct connect_only_impl<Mechanism, ProducerTag, ConsumerTag,
            typename enable_if<
            boost::is_same<typename ProducerTag::producer_concept, concepts::keyed_producer>
        >::type >
    {
        template<typename Producer, typename Consumer>
        struct apply
        {
            static void call(Producer &producer, Consumer &consumer)
            {
                connect_only(get_keyed_producer<Mechanism>(producer, consumer), consumer);
            }
        };
    };
    
    template<typename Mechanism, typename ProducerTag, typename ConsumerTag>
    struct connect_only_impl<Mechanism, ProducerTag, ConsumerTag,
            typename enable_if<
            boost::is_same<typename ConsumerTag::consumer_concept, concepts::keyed_consumer>
        >::type >
    {
        template<typename Producer, typename Consumer>
        struct apply
        {
            static void call(Producer &producer, Consumer &consumer)
            {
                connect_only(producer, get_keyed_consumer<Mechanism>(producer, consumer));
            }
        };
    };
}

template<typename Mechanism, typename Producer, typename Consumer>
inline void connect_only(Producer &producer, Consumer &consumer)
{
    typedef typename boost::remove_cv<Producer>::type ProducerNoCV;
    typedef typename boost::remove_cv<Consumer>::type ConsumerNoCV;
    
    extension::connect_only_impl<
        Mechanism,
        typename producer_category_of<Mechanism, ProducerNoCV>::type,
        typename consumer_category_of<Mechanism, ConsumerNoCV>::type>
            ::template apply<
                typename boost::remove_reference<
                    typename get_proxied_producer_result_type<Mechanism, Producer>::type
                >::type,
                typename boost::remove_reference<
                    typename get_proxied_consumer_result_type<Mechanism, Consumer>::type
                >::type
            >::call(get_proxied_producer<Mechanism>(producer),
                    get_proxied_consumer<Mechanism>(consumer));
}

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_ONLY_CONNECTABLE_HPP
