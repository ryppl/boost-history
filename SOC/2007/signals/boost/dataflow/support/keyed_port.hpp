// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_KEYED_PRODUCER_HPP
#define BOOST_DATAFLOW_SUPPORT_KEYED_PRODUCER_HPP

#include <boost/dataflow/support/producer.hpp>

#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_same.hpp>


namespace boost { namespace dataflow {

namespace concepts
{
    struct keyed_producer;
}

namespace extension
{
    template<typename Mechanism, typename KeyedProducerTag, typename ConsumerTag>
    struct get_keyed_producer_impl
    {        
        template<typename KeyedProducer, typename Consumer>
        struct apply
        {
            typedef void type;
            static void call(KeyedProducer &, Consumer &)
            {
                // Error: get_keyed_producer_impl has not been implemented
                // for KeyedProducerTag and ConsumerTag.
                BOOST_STATIC_ASSERT(sizeof(KeyedProducer)==0);
            }
        };
    };
}

template<typename Mechanism, typename T1, typename T2>
typename extension::get_keyed_producer_impl<
    Mechanism,
    typename producer_category_of<
        Mechanism, typename boost::remove_cv<T1>::type
    >::type,
    typename consumer_category_of<
        Mechanism, typename boost::remove_cv<T2>::type
    >::type
>::template apply<T1, T2>::type
get_keyed_producer(T1 &t1, T2 &t2)
{
    return extension::get_keyed_producer_impl<
        Mechanism,
        typename producer_category_of<
            Mechanism, typename boost::remove_cv<T1>::type
        >::type,
        typename consumer_category_of<
            Mechanism, typename boost::remove_cv<T2>::type
        >::type
    >::template apply<const T1, const T2>::call(t1, t2);
}

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_KEYED_PRODUCER_HPP
