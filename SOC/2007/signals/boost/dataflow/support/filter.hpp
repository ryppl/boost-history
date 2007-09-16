// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_FILTER_HPP
#define BOOST_DATAFLOW_SUPPORT_FILTER_HPP

#include <boost/dataflow/support/producer.hpp>
#include <boost/dataflow/support/consumer.hpp>

namespace boost { namespace dataflow {

// trait giving the producer category of a type.
template<typename T, typename Enable=void>
struct filter_category_of
{
};

template<typename T>
struct filter_category_of<T,
    typename detail::enable_if_defined<typename T::filter_category>::type >
{
    typedef typename T::filter_category type;
};

template<typename T>
struct producer_category_of<T,
    typename detail::enable_if_defined<typename filter_category_of<T>::type>::type >
{
    typedef typename filter_category_of<T>::type type;
};

template<typename T>
struct consumer_category_of<T,
    typename detail::enable_if_defined<typename filter_category_of<T>::type>::type >
{
    typedef typename filter_category_of<T>::type type;
};

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_FILTER_HPP
