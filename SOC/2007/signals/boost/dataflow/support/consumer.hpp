// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_CONSUMER_HPP
#define BOOST_DATAFLOW_SUPPORT_CONSUMER_HPP

#include <boost/dataflow/detail/enable_if_defined.hpp>

#include <boost/type_traits/integral_constant.hpp>


namespace boost { namespace dataflow {

// trait giving the consumer category of a type.
template<typename T, typename Enable=void>
struct consumer_category_of;

template<typename T>
struct consumer_category_of<T,
typename detail::enable_if_defined<typename T::consumer_category>::type >
{
    typedef typename T::consumer_category type;
};

// trait giving the consumer category of a type.
template<typename T, typename Enable=void>
struct consumed_type_of;

template<typename T>
struct consumed_type_of<T,
typename detail::enable_if_defined<typename T::consumed_type>::type >
{
    typedef typename T::consumed_type type;
};

// trait determining whether a type is a consumer.
template<typename T, typename Enable=void>
struct is_consumer
    : public boost::false_type {};

template<typename T>
struct is_consumer<T,
        typename detail::enable_if_defined<typename consumer_category_of<T>::type>::type >
    : public boost::true_type {};

// trait determining whether a type is a consumer.
template<typename T, typename Enable=void>
struct is_single_type_consumer
    : public boost::false_type {};

template<typename T>
struct is_single_type_consumer<T,
        typename detail::enable_if_defined<detail::all_of<
            typename consumer_category_of<T>::type,
            typename consumed_type_of<T>::type>
        >::type >
    : public boost::true_type {};

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_CONSUMER_HPP
