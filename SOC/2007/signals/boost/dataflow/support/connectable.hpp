// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_CONNECTABLE_HPP
#define BOOST_DATAFLOW_SUPPORT_CONNECTABLE_HPP

#include <boost/static_assert.hpp>


namespace boost { namespace dataflow {

namespace extension
{
    template<typename ProducerTag, typename ConsumerTag, typename Enable=void>
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
}

template<typename Producer, typename Consumer>
inline void connect(Producer &producer, Consumer &consumer)
{
    extension::connect_impl<
        typename producer_category_of<Producer>::type,
        typename consumer_category_of<Consumer>::type>
            ::template apply<
                typename get_proxied_producer_type<Producer>::type,
                Consumer
            >::call(get_proxied_producer(producer),consumer);
}

template<typename Producer, typename Consumer>
inline void connect(const Producer &producer, Consumer &consumer)
{
    extension::connect_impl<
        typename producer_category_of<Producer>::type,
        typename consumer_category_of<Consumer>::type>
            ::template apply<
                typename get_proxied_producer_type<Producer>::type,
                Consumer
            >::call(get_proxied_producer(producer),consumer);
}

template<typename Producer, typename Consumer>
inline void connect(Producer &producer, const Consumer &consumer)
{
    extension::connect_impl<
        typename producer_category_of<Producer>::type,
        typename consumer_category_of<Consumer>::type>
            ::template apply<
                typename get_proxied_producer_type<Producer>::type,
                Consumer
            >::call(get_proxied_producer(producer),consumer);
}

template<typename Producer, typename Consumer>
inline void connect(const Producer &producer, const Consumer &consumer)
{
    extension::connect_impl<
        typename producer_category_of<Producer>::type,
        typename consumer_category_of<Consumer>::type>
            ::template apply<
                typename get_proxied_producer_type<Producer>::type,
                Consumer
            >::call(get_proxied_producer(producer),consumer);
}

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_CONNECTABLE_HPP
