// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_PHOENIX_SUPPORT_HPP
#define BOOST_DATAFLOW_PHOENIX_SUPPORT_HPP

#include <boost/dataflow/support.hpp>

#include <boost/mpl/and.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_pod.hpp>

namespace boost { namespace dataflow {

struct phoenix_producer {};
struct phoenix_consumer {};

struct ptr_consumer : public phoenix_consumer {};
struct pod_producer : public phoenix_producer {};

template <typename T>
struct consumer_category_of<T *>
{
    typedef ptr_consumer type;
};

template <typename T>
struct consumer_category_of<T * const>
{
    typedef ptr_consumer type;
};

template<typename T>
struct producer_category_of<T, typename boost::enable_if<is_pod<T> >::type>
{
    typedef pod_producer type;
};

template<typename T>
struct produced_type_of<T, typename boost::enable_if<is_pod<T> >::type>
{
    typedef T type;
};

namespace extension
{
    // phoenix specific extensions
    namespace phoenix
    {
        template<typename ConsumerTag>
        struct set_address
        {
            template<typename Consumer>
            struct apply
            {
                template<typename T>
                static void call(const Consumer &consumer, T *ptr)
                {
                    consumer.ptr = ptr;
                }
            };
        };
        
        template<>
        struct set_address<boost::dataflow::ptr_consumer>
        {
            template<typename Consumer>
            struct apply
            {
                template<typename T>
                static void call(Consumer &consumer, T *ptr)
                {
                    consumer = ptr;
                }
            };
        };
        
        template<typename ProducerTag>
        struct get_address
        {
            template<typename Producer>
            struct apply
            {
                static const typename produced_type_of<Producer>::type *
                call(const Producer &producer)
                {
                    return &producer.value;
                }
            };
        };
        
        template<typename ProducerTag>
        struct get_value
        {
            template<typename Producer>
            struct apply
            {
                static const typename produced_type_of<Producer>::type &
                call(const Producer &producer)
                {
                    return producer.value;
                }
            };
        };
        
        template<>
        struct get_value<boost::dataflow::pod_producer>
        {
            template<typename Producer>
            struct apply
            {
                static const typename produced_type_of<Producer>::type &
                call(const Producer &producer)
                {
                    return producer;
                }
            };
        };
    }

    template<typename ProducerTag, typename ConsumerTag>
    struct connect_impl<ProducerTag, ConsumerTag,
            typename boost::enable_if<boost::mpl::and_<
                boost::is_base_of<phoenix_producer, ProducerTag>,
                boost::is_base_of<phoenix_consumer, ConsumerTag>
            > >::type >
    {
        template<typename Producer, typename Consumer>
        struct apply
        {
            static void call(const Producer &producer, const Consumer &consumer)
            {
                phoenix::set_address<ConsumerTag>::template apply<Consumer>
                    ::call(consumer,
                           phoenix::template get_address<ProducerTag>
                            ::template apply<Producer>::call(producer));
            }
            static void call(const Producer &producer, Consumer &consumer)
            {
                phoenix::set_address<ConsumerTag>::template apply<Consumer>
                    ::call(consumer,
                           phoenix::template get_address<ProducerTag>
                           ::template apply<Producer>::call(producer));
            }
        };
    };
}

} } // namespace boost::dataflow

namespace boost { namespace phoenix {
    
    using boost::dataflow::connect;
    
} } // namespace boost::phoenix

#endif // BOOST_DATAFLOW_SUPPORT_HPP
