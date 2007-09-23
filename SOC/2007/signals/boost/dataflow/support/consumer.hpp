// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_CONSUMER_HPP
#define BOOST_DATAFLOW_SUPPORT_CONSUMER_HPP

#include <boost/dataflow/detail/enable_if_defined.hpp>
#include <boost/dataflow/support/common.hpp>

#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_same.hpp>


namespace boost { namespace dataflow {

/// Trait giving the consumer category of a type.
template<typename Mechanism, typename T, typename Enable=void>
struct consumer_category_of
{
};

/// Allows intrusive specification of the consumer category.
template<typename Mechanism, typename T>
struct consumer_category_of<Mechanism, T,
    typename detail::enable_if_defined<
        typename T::template dataflow<Mechanism>::consumer_category>::type
    >
{
    typedef typename T::template dataflow<Mechanism>::consumer_category type;
};

/// Trait determining whether a type is a Consumer.
template<typename Mechanism, typename T, typename Enable=void>
struct is_consumer
    : public boost::false_type {};

template<typename Mechanism, typename T>
struct is_consumer<Mechanism, T,
        typename detail::enable_if_defined<
            typename consumer_category_of<Mechanism, T>::type
        >::type >
    : public boost::true_type {};

/// Trait giving the consumed type of a SingleTypeConsumer.
template<typename Mechanism, typename T, typename Enable=void>
struct consumed_type_of
{
};

/// Allows intrusive specification of the consumer category.
template<typename Mechanism, typename T>
struct consumed_type_of<Mechanism, T,
    typename detail::enable_if_defined<
        typename T::template dataflow<Mechanism>::consumed_type>::type
    >
{
    typedef typename T::template dataflow<Mechanism>::consumed_type type;
};

template<typename Mechanism, typename ConsumerTag>
struct consumer
{
    template<typename M, typename Enable=void>
    struct dataflow
    {
    };
    
    template<typename M>
    struct dataflow<M, typename boost::enable_if<is_same<M, Mechanism> >::type>
    {
        typedef ConsumerTag consumer_category;
    };
};

template<typename ConsumerTag>
struct consumer<all_mechanisms, ConsumerTag>
{
    template<typename M>
    struct dataflow
    {
        typedef ConsumerTag consumer_category;
    };
};

// Trait determining whether a type is a SingleTypeConsumer.
template<typename Mechanism, typename T, typename Enable=void>
struct is_single_type_consumer
    : public boost::false_type {};

template<typename Mechanism, typename T>
struct is_single_type_consumer<Mechanism, T,
        typename detail::enable_if_defined<detail::all_of<
            typename consumer_category_of<Mechanism, T>::type,
            typename consumed_type_of<Mechanism, T>::type
        > >::type >
    : public boost::true_type {};

template<typename Mechanism, typename ConsumerTag, typename ConsumedType>
struct single_type_consumer
{
    template<typename M, typename Enable=void>
    struct dataflow
    {
    };
    
    template<typename M>
    struct dataflow<M, typename boost::enable_if<is_same<M, Mechanism> >::type>
    {
        typedef ConsumerTag consumer_category;
        typedef ConsumedType consumed_type;
    };
};

template<typename ConsumerTag, typename ConsumedType>
struct single_type_consumer<all_mechanisms, ConsumerTag, ConsumedType>
{
    template<typename M>
    struct dataflow
    {
        typedef ConsumerTag consumer_category;
        typedef ConsumedType consumed_type;
    };
};

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_CONSUMER_HPP
