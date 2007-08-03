// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_PRODUCER_HPP
#define BOOST_DATAFLOW_SUPPORT_PRODUCER_HPP

#include <boost/dataflow/detail/enable_if_defined.hpp>

#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>


namespace boost { namespace dataflow {

// trait giving the producer category of a type.
template<typename T, typename Enable=void>
struct producer_category_of
{
    // Error: attempting to use T as a producer, but producer_category_of<T>
    // has not been specialized!
    BOOST_STATIC_ASSERT(sizeof(T)==0);
};

template<typename T>
struct producer_category_of<T,
typename detail::enable_if_defined<typename T::producer_category >::type >
{
    typedef typename T::producer_category type;
};

// trait determining whether a type is a producer.
template<typename T, typename Enable=void>
struct is_producer
    : public boost::false_type {};

template<typename T>
struct is_producer<T, 
        typename detail::enable_if_defined<typename producer_category_of<T>::type >::type >
    : public boost::true_type {};
    
template<typename T, typename Enable=void>
struct produced_type_of
{
    // Error: attempting to use T as a producer, but produced_type_of<T>
    // has not been specialized!
    BOOST_STATIC_ASSERT(sizeof(T)==0);
};

template<typename T>
struct produced_type_of<T,
typename detail::enable_if_defined<typename T::produced_type>::type >
{
    typedef typename T::produced_type type;
};

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_PRODUCER_HPP
