// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_PRODUCER_HPP
#define BOOST_DATAFLOW_SUPPORT_PRODUCER_HPP

#include <boost/dataflow/detail/enable_if_defined.hpp>
#include <boost/dataflow/support/common.hpp>

#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_same.hpp>


namespace boost { namespace dataflow {

/// Trait giving the producer category of a type.
template<typename Mechanism, typename T, typename Enable=void>
struct producer_category_of
{
};

/// Allows intrusive specification of the producer category.
template<typename Mechanism, typename T>
struct producer_category_of<Mechanism, T,
    typename detail::enable_if_defined<
        typename T::template dataflow<Mechanism>::producer_category>::type
    >
{
    typedef typename T::template dataflow<Mechanism>::producer_category type;
};

/// Trait determining whether a type is a Producer.
template<typename Mechanism, typename T, typename Enable=void>
struct is_producer
    : public boost::false_type {};

template<typename Mechanism, typename T>
struct is_producer<Mechanism, T,
        typename detail::enable_if_defined<
            typename producer_category_of<Mechanism, T>::type
        >::type >
    : public boost::true_type {};

/// Trait giving the produced type of a SingleTypeProducer.
template<typename Mechanism, typename T, typename Enable=void>
struct produced_type_of
{
};

/// Allows intrusive specification of the producer category.
template<typename Mechanism, typename T>
struct produced_type_of<Mechanism, T,
    typename detail::enable_if_defined<
        typename T::template dataflow<Mechanism>::produced_type>::type
    >
{
    typedef typename T::template dataflow<Mechanism>::produced_type type;
};

template<typename Mechanism, typename ProducerTag>
struct producer
{
    template<typename M, typename Enable=void>
    struct dataflow
    {
    };
    
    template<typename M>
    struct dataflow<M, typename boost::enable_if<is_same<M, Mechanism> >::type>
    {
        typedef ProducerTag producer_category;
    };
};

template<typename ProducerTag>
struct producer<all_mechanisms, ProducerTag>
{
    template<typename M>
    struct dataflow
    {
        typedef ProducerTag producer_category;
    };
};

// Trait determining whether a type is a SingleTypeProducer.
template<typename Mechanism, typename T, typename Enable=void>
struct is_single_type_producer
    : public boost::false_type {};

template<typename Mechanism, typename T>
struct is_single_type_producer<Mechanism, T,
        typename detail::enable_if_defined<detail::all_of<
            typename producer_category_of<Mechanism, T>::type,
            typename produced_type_of<Mechanism, T>::type
        > >::type >
    : public boost::true_type {};

template<typename Mechanism, typename ProducerTag, typename ProducedType>
struct single_type_producer
{
    template<typename M, typename Enable=void>
    struct dataflow
    {
    };
    
    template<typename M>
    struct dataflow<M, typename boost::enable_if<is_same<M, Mechanism> >::type>
    {
        typedef ProducerTag producer_category;
        typedef ProducedType produced_type;
    };
};

template<typename ProducerTag, typename ProducedType>
struct single_type_producer<all_mechanisms, ProducerTag, ProducedType>
{
    template<typename M>
    struct dataflow
    {
        typedef ProducerTag producer_category;
        typedef ProducedType produced_type;
    };
};

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_PRODUCER_HPP
