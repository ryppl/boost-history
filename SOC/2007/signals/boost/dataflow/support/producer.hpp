// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_PRODUCER_HPP
#define BOOST_DATAFLOW_SUPPORT_PRODUCER_HPP

#include <boost/dataflow/detail/enable_if_defined.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/static_assert.hpp>


namespace boost { namespace dataflow {

// trait giving the producer category of a type.
template<typename T, typename Enable=void>
struct producer_category_of
{
};

template<typename T>
struct producer_category_of<T,
    typename detail::enable_if_defined<typename T::producer_category>::type >
{
    typedef typename T::producer_category type;
};

template<typename T, typename Enable=void>
struct produced_type_of
{
};

// trait determining whether a type is a Producer.
template<typename T, typename Enable=void>
struct is_producer
    : public boost::false_type {};

template<typename T>
struct is_producer<T,
        typename detail::enable_if_defined<
            typename producer_category_of<T>::type
        >::type >
    : public boost::true_type {};

// trait determining whether a type is a SingleTypeProducer.
template<typename T, typename Enable=void>
struct is_single_type_producer
    : public boost::false_type {};

template<typename T>
struct is_single_type_producer<T,
        typename detail::enable_if_defined<detail::all_of<
            typename producer_category_of<T>::type,
            typename produced_type_of<T>::type
        > >::type >
    : public boost::true_type {};

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_PRODUCER_HPP
